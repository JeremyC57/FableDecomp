#include <cstdio>
#include <new>

struct __false_type { };
struct SRecord12 {
    long m_a, m_b, m_c;
    SRecord12() : m_a(0), m_b(0), m_c(0) { }
    SRecord12(const SRecord12& o) : m_a(o.m_a), m_b(o.m_b), m_c(o.m_c) { }
};
struct SRecord56 {
    long      m_0, m_4, m_8, m_c, m_10, m_14;
    SRecord12 m_record;
    bool      m_flag24;
    bool      m_flag25;
    long      m_28, m_2c, m_30, m_34;
    SRecord56() : m_0(0), m_4(0), m_8(0), m_c(0), m_10(0), m_14(0), m_flag24(false), m_flag25(false), m_28(0), m_2c(0), m_30(0), m_34(0) { }
    SRecord56(const SRecord56& o)
        : m_0(o.m_0), m_4(o.m_4), m_8(o.m_8), m_c(o.m_c), m_10(o.m_10), m_14(o.m_14), m_record(o.m_record),
          m_flag24(o.m_flag24), m_flag25(o.m_flag25), m_28(o.m_28), m_2c(o.m_2c), m_30(o.m_30), m_34(o.m_34) { }
};

typedef SRecord56 T_;

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
        s->m_14 = 100 * i + 6;
        s->m_record.m_a = 100 * i + 7;
        s->m_record.m_b = 100 * i + 8;
        s->m_record.m_c = 100 * i + 9;
        s->m_28 = 100 * i + 10;
        s->m_2c = 100 * i + 11;
        s->m_30 = 100 * i + 12;
        s->m_34 = 100 * i + 13;
        s->m_flag24 = (i != 1); s->m_flag25 = (i == 2);
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
        if (d->m_14 != s->m_14) ok = false;
        if (d->m_record.m_a != s->m_record.m_a) ok = false;
        if (d->m_record.m_b != s->m_record.m_b) ok = false;
        if (d->m_record.m_c != s->m_record.m_c) ok = false;
        if (d->m_28 != s->m_28) ok = false;
        if (d->m_2c != s->m_2c) ok = false;
        if (d->m_30 != s->m_30) ok = false;
        if (d->m_34 != s->m_34) ok = false;
        if (d->m_flag24 != (i != 1) || d->m_flag25 != (i == 2)) ok = false;
    }
    if (UninitCopy(src, src, dst + 1, tag) != dst + 1) ok = false;
    if (!ok) { std::printf("00b28ed0_TEST FAIL\n"); return 1; }
    std::printf("00b28ed0_TEST PASS\n");
    return 0;
}