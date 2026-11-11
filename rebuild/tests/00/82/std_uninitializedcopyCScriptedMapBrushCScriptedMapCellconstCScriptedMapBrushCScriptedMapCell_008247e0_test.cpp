#include <cstdio>
#include <new>

struct __false_type { };
struct SRecord20 {
    long m_0, m_4, m_8, m_c, m_10;
};

typedef SRecord20 T_;

static T_* UninitCopy(const T_* first, const T_* last, T_* result, const __false_type&)
{
    for (; first != last; ++first, ++result)
        new (result) T_(*first);
    return result;
}

int main()
{
    bool ok = true;
    T_* src = (T_*)new double[(3 * sizeof(T_)) / sizeof(double) + 1];
    for (int i = 0; i < 3; ++i) {
        T_* s = new (&src[i]) T_();
        s->m_0 = 100 * i + 1;
        s->m_4 = 100 * i + 2;
        s->m_8 = 100 * i + 3;
        s->m_c = 100 * i + 4;
        s->m_10 = 100 * i + 5;
    }
    static double storage[64];
    T_* dst = (T_*)storage;
    __false_type tag;
    T_* r = UninitCopy(src, src + 3, dst, tag);
    if (r != dst + 3) ok = false;
    for (int i = 0; i < 3; ++i) {
        const T_* s = &src[i];
        const T_* d = &dst[i];
        if (d->m_0 != s->m_0) ok = false;
        if (d->m_4 != s->m_4) ok = false;
        if (d->m_8 != s->m_8) ok = false;
        if (d->m_c != s->m_c) ok = false;
        if (d->m_10 != s->m_10) ok = false;
    }
    if (UninitCopy(src, src, dst + 1, tag) != dst + 1) ok = false;
    if (!ok) { std::printf("008247e0_TEST FAIL\n"); return 1; }
    std::printf("008247e0_TEST PASS\n");
    return 0;
}