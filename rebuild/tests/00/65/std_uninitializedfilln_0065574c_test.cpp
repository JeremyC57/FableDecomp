// behaviour test for 0x0065574C std::uninitialized_fill_n<CIntelligentPointer<CThing>*, unsigned int, CIntelligentPointer<CThing> >
// (model: the STLport dispatch forwards first / n / x unchanged to the non-POD __uninitialized_fill_n with a __false_type tag and
// returns its result; the fill copy-constructs n pointers to x's item into raw storage)
#include <cstdio>
#include <new>
namespace model {
struct false_type { false_type() { } };
struct true_type { true_type() { } };
static int g_falseCalls = 0, g_trueCalls = 0, g_copies = 0;
struct P { void* item; P() : item(0) { } P(const P& o) : item(0) { item = o.item; ++g_copies; } };
template <class I, class N, class T> I fill_n_impl(I first, N n, const T& x, const false_type&)
{
    ++g_falseCalls;
    I cur = first;
    for (; n > 0; --n, ++cur)
        new (cur) T(x);
    return cur;
}
template <class I, class N, class T> I fill_n_impl(I first, N, const T&, const true_type&) { ++g_trueCalls; return first; }
template <class I, class N, class T> I uninitialized_fill_n(I first, N n, const T& x) { return fill_n_impl(first, n, x, false_type()); }
}
int main()
{
    static double raw[8];
    int obj = 0;
    model::P x; x.item = &obj;
    model::P* first = (model::P*)raw;
    model::P* end = model::uninitialized_fill_n(first, 3u, x);
    int ok = end == first + 3 && first[0].item == &obj && first[2].item == &obj && model::g_copies == 3
          && model::g_falseCalls == 1 && model::g_trueCalls == 0;
    model::P* none = model::uninitialized_fill_n(first, 0u, x);
    ok = ok && none == first && model::g_falseCalls == 2 && model::g_copies == 3;
    if (!ok) { std::printf("UMOVE_0065574c_FAIL\n"); return 1; }
    std::printf("UMOVE_0065574c_OK\n");
    return 0;
}
