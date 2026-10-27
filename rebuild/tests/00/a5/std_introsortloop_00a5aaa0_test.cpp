// Behaviour test for 0x00A5AAA0 std::__introsort_loop<float*, float, int, less<float> >
#include <cstdio>

static int g_partialCalls = 0;

static bool lessf(const float& a, const float& b) { return a < b; }

static const float& median3(const float& a, const float& b, const float& c)
{
    if (lessf(a, b))
        if (lessf(b, c)) return b; else if (lessf(a, c)) return c; else return a;
    else if (lessf(a, c)) return a;
    else if (lessf(b, c)) return c;
    else return b;
}

static float* partition(float* first, float* last, float pivot)
{
    for (;;) {
        while (lessf(*first, pivot)) ++first;
        --last;
        while (lessf(pivot, *last)) --last;
        if (!(first < last)) return first;
        float t = *first; *first = *last; *last = t;
        ++first;
    }
}

static void partial_sort_all(float* first, float* last)   // stands in for 0x00A5A8A0 with middle == last
{
    ++g_partialCalls;
    for (float* i = first + 1; i < last; ++i)
        for (float* j = i; j > first && *j < *(j - 1); --j) { float t = *j; *j = *(j - 1); *(j - 1) = t; }
}

static void introsort_loop(float* first, float* last, int depth)
{
    while (last - first > 16) {
        if (depth == 0) { partial_sort_all(first, last); return; }
        --depth;
        float* cut = partition(first, last, float(median3(*first, *(first + (last - first) / 2), *(last - 1))));
        introsort_loop(cut, last, depth);
        last = cut;
    }
}

int main()
{
    float a[200];
    unsigned int seed = 12345;
    for (int i = 0; i < 200; ++i) { seed = seed * 1103515245u + 12345u; a[i] = (float)((seed >> 8) % 1000); }
    float sum0 = 0; for (int i = 0; i < 200; ++i) sum0 += a[i];
    introsort_loop(a, a + 200, 16);
    // after the loop every element of a block of at most 16 is >= everything before that block
    float* f = a; float* l = a + 200;
    for (int i = 0; i < 200; ++i)
        for (int j = i + 17; j < 200; ++j)
            if (a[j] < a[i]) { std::printf("00a5aaa0_TEST FAIL\n"); return 1; }
    float sum1 = 0; for (int i = 0; i < 200; ++i) sum1 += a[i];
    if (sum0 != sum1 || g_partialCalls != 0) { std::printf("00a5aaa0_TEST FAIL\n"); return 1; }
    // depth exhausted: the whole range goes to partial_sort
    introsort_loop(f, l, 0);
    for (int i = 1; i < 200; ++i) if (a[i] < a[i - 1]) { std::printf("00a5aaa0_TEST FAIL\n"); return 1; }
    if (g_partialCalls != 1) { std::printf("00a5aaa0_TEST FAIL\n"); return 1; }
    std::printf("00a5aaa0_TEST PASS\n");
    return 0;
}