
#include <ripple/basics/tagged_integer.h>
#include <ripple/beast/unit_test.h>
#include <type_traits>
namespace ripple {
namespace test {
class tagged_integer_test
    : public beast::unit_test::suite
{
private:
    struct Tag1 { };
    struct Tag2 { };
    using TagUInt1 = tagged_integer <std::uint32_t, Tag1>;
    using TagUInt2 = tagged_integer <std::uint32_t, Tag2>;
    using TagUInt3 = tagged_integer <std::uint64_t, Tag1>;
    static_assert (std::is_constructible<TagUInt1, std::uint32_t>::value,
        "TagUInt1 should be constructible using a std::uint32_t");
    static_assert (!std::is_constructible<TagUInt1, std::uint64_t>::value,
        "TagUInt1 should not be constructible using a std::uint64_t");
    static_assert (std::is_constructible<TagUInt3, std::uint32_t>::value,
        "TagUInt3 should be constructible using a std::uint32_t");
    static_assert (std::is_constructible<TagUInt3, std::uint64_t>::value,
        "TagUInt3 should be constructible using a std::uint64_t");
    static_assert (!std::is_assignable<TagUInt1, std::uint32_t>::value,
        "TagUInt1 should not be assignable with a std::uint32_t");
    static_assert (!std::is_assignable<TagUInt1, std::uint64_t>::value,
        "TagUInt1 should not be assignable with a std::uint64_t");
    static_assert (!std::is_assignable<TagUInt3, std::uint32_t>::value,
        "TagUInt3 should not be assignable with a std::uint32_t");
    static_assert (!std::is_assignable<TagUInt3, std::uint64_t>::value,
        "TagUInt3 should not be assignable with a std::uint64_t");
    static_assert (std::is_assignable<TagUInt1, TagUInt1>::value,
        "TagUInt1 should be assignable with a TagUInt1");
    static_assert (!std::is_assignable<TagUInt1, TagUInt2>::value,
        "TagUInt1 should not be assignable with a TagUInt2");
    static_assert (std::is_assignable<TagUInt3, TagUInt3>::value,
        "TagUInt3 should be assignable with a TagUInt1");
    static_assert (!std::is_assignable<TagUInt1, TagUInt3>::value,
        "TagUInt1 should not be assignable with a TagUInt3");
    static_assert (!std::is_assignable<TagUInt3, TagUInt1>::value,
        "TagUInt3 should not be assignable with a TagUInt1");
    static_assert (!std::is_convertible<std::uint32_t, TagUInt1>::value,
        "std::uint32_t should not be convertible to a TagUInt1");
    static_assert (!std::is_convertible<std::uint32_t, TagUInt3>::value,
        "std::uint32_t should not be convertible to a TagUInt3");
    static_assert (!std::is_convertible<std::uint64_t, TagUInt3>::value,
        "std::uint64_t should not be convertible to a TagUInt3");
    static_assert (!std::is_convertible<std::uint64_t, TagUInt2>::value,
        "std::uint64_t should not be convertible to a TagUInt2");
    static_assert (!std::is_convertible<TagUInt1, TagUInt2>::value,
        "TagUInt1 should not be convertible to TagUInt2");
    static_assert (!std::is_convertible<TagUInt1, TagUInt3>::value,
        "TagUInt1 should not be convertible to TagUInt3");
    static_assert (!std::is_convertible<TagUInt2, TagUInt3>::value,
        "TagUInt2 should not be convertible to a TagUInt3");
public:
    void run () override
    {
        using TagInt = tagged_integer<std::int32_t, Tag1>;
        {
            testcase ("Comparison Operators");
            TagInt const zero(0);
            TagInt const one(1);
            BEAST_EXPECT(one == one);
            BEAST_EXPECT(!(one == zero));
            BEAST_EXPECT(one != zero);
            BEAST_EXPECT(!(one != one));
            BEAST_EXPECT(zero < one);
            BEAST_EXPECT(!(one < zero));
            BEAST_EXPECT(one > zero);
            BEAST_EXPECT(!(zero > one));
            BEAST_EXPECT(one >= one);
            BEAST_EXPECT(one >= zero);
            BEAST_EXPECT(!(zero >= one));
            BEAST_EXPECT(zero <= one);
            BEAST_EXPECT(zero <= zero);
            BEAST_EXPECT(!(one <= zero));
        }
        {
            testcase ("Increment/Decrement Operators");
            TagInt const zero(0);
            TagInt const one(1);
            TagInt a{0};
            ++a;
            BEAST_EXPECT(a == one);
            --a;
            BEAST_EXPECT(a == zero);
            a++;
            BEAST_EXPECT(a == one);
            a--;
            BEAST_EXPECT(a == zero);
        }
        {
            testcase ("Arithmetic Operators");
            TagInt a{-2};
            BEAST_EXPECT(+a == TagInt{-2});
            BEAST_EXPECT(-a == TagInt{2});
            BEAST_EXPECT(TagInt{-3} + TagInt{4} == TagInt{1});
            BEAST_EXPECT(TagInt{-3} - TagInt{4} == TagInt{-7});
            BEAST_EXPECT(TagInt{-3} * TagInt{4} == TagInt{-12});
            BEAST_EXPECT(TagInt{8}/TagInt{4} == TagInt{2});
            BEAST_EXPECT(TagInt{7} %TagInt{4} == TagInt{3});
            BEAST_EXPECT(~TagInt{8}  == TagInt{~TagInt::value_type{8}});
            BEAST_EXPECT((TagInt{6} & TagInt{3}) == TagInt{2});
            BEAST_EXPECT((TagInt{6} | TagInt{3}) == TagInt{7});
            BEAST_EXPECT((TagInt{6} ^ TagInt{3}) == TagInt{5});
            BEAST_EXPECT((TagInt{4} << TagInt{2}) == TagInt{16});
            BEAST_EXPECT((TagInt{16} >> TagInt{2}) == TagInt{4});
        }
        {
            testcase ("Assignment Operators");
            TagInt a{-2};
            TagInt b{0};
            b = a;
            BEAST_EXPECT(b == TagInt{-2});
            a = TagInt{-3};
            a += TagInt{4};
            BEAST_EXPECT(a == TagInt{1});
            a = TagInt{-3};
            a -= TagInt{4};
            BEAST_EXPECT(a == TagInt{-7});
            a = TagInt{-3};
            a *= TagInt{4};
            BEAST_EXPECT(a == TagInt{-12});
            a = TagInt{8};
            a /= TagInt{4};
            BEAST_EXPECT(a == TagInt{2});
            a = TagInt{7};
            a %= TagInt{4};
            BEAST_EXPECT(a == TagInt{3});
            a = TagInt{6};
            a /= TagInt{3};
            BEAST_EXPECT(a == TagInt{2});
            a = TagInt{6};
            a |= TagInt{3};
            BEAST_EXPECT(a == TagInt{7});
            a = TagInt{6};
            a ^= TagInt{3};
            BEAST_EXPECT(a == TagInt{5});
            a = TagInt{4};
            a <<= TagInt{2};
            BEAST_EXPECT(a == TagInt{16});
            a = TagInt{16};
            a >>= TagInt{2};
            BEAST_EXPECT(a == TagInt{4});
        }
    }
};
BEAST_DEFINE_TESTSUITE(tagged_integer,ripple_basics,ripple);
} 
} 