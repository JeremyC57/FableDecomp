#include <cstdio>
#include <new>

struct __false_type { };
struct SRecord12 {
    long m_a, m_b, m_c;
    SRecord12() : m_a(0), m_b(0), m_c(0) { }
    SRecord12(const SRecord12& o) : m_a(o.m_a), m_b(o.m_b), m_c(o.m_c) { }
};
struct SRecord32 {
    long      m_0, m_4, m_8;
    SRecord12 m_record;
    long      m_18, m_1c;
    SRecord32() : m_0(0), m_4(0), m_8(0), m_18(0), m_1c(0) { }
    SRecord32(const SRecord32& o) : m_0(o.m_0), m_4(o.m_4), m_8(o.m_8), m_record(o.m_record), m_18(o.m_18), m_1c(o.m_1c) { }
};

typedef SRecord32 T_;

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
        s->m_record.m_a = 100 * i + 4;
        s->m_record.m_b = 100 * i + 5;
        s->m_record.m_c = 100 * i + 6;
        s->m_18 = 100 * i + 7;
        s->m_1c = 100 * i + 8;
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
        if (d->m_record.m_a != s->m_record.m_a) ok = false;
        if (d->m_record.m_b != s->m_record.m_b) ok = false;
        if (d->m_record.m_c != s->m_record.m_c) ok = false;
        if (d->m_18 != s->m_18) ok = false;
        if (d->m_1c != s->m_1c) ok = false;
    }
    if (UninitCopy(src, src, dst + 1, tag) != dst + 1) ok = false;
    if (!ok) { std::printf("00a95a10_TEST FAIL\n"); return 1; }
    std::printf("00a95a10_TEST PASS\n");
    return 0;
}