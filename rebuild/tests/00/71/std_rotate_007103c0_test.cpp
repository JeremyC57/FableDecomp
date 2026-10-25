// Behaviour test for 0x007103C0 std::__rotate (random access, gcd cycle-leader) over list<CAnimSet>::const_iterator:
// every (length, middle) split up to 13 elements matches a reference rotation and returns first + (last - middle)
// (last when middle == first); the k == l case is the swap_ranges branch
#include <cstdio>

namespace model {

struct CAnimSet { int Priority; int Tag; };
struct ConstIterator { const CAnimSet* Node; };

ConstIterator* swap_ranges(ConstIterator* first1, ConstIterator* last1, ConstIterator* first2)
{
    for (; first1 != last1; ++first1, ++first2) { ConstIterator t = *first1; *first1 = *first2; *first2 = t; }
    return first2;
}
int gcd(int m, int n) { while (n != 0) { int t = m % n; m = n; n = t; } return m; }

ConstIterator* rotate(ConstIterator* first, ConstIterator* middle, ConstIterator* last)
{
    int n = (int)(last - first);
    int k = (int)(middle - first);
    int l = n - k;
    ConstIterator* result = first + (last - middle);
    if (k == 0)
        return last;
    if (k == l) {
        swap_ranges(first, middle, middle);
        return result;
    }
    int d = gcd(n, k);
    for (int i = 0; i < d; i++) {
        ConstIterator tmp = *first;
        ConstIterator* p = first;
        if (k < l) {
            for (int j = 0; j < l / d; j++) {
                if (p > first + l) { *p = *(p - l); p -= l; }
                *p = *(p + k);
                p += k;
            }
        }
        else {
            for (int j = 0; j < k / d - 1; j++) {
                if (p < last - k) { *p = *(p + k); p += k; }
                *p = *(p - l);
                p -= l;
            }
        }
        *p = tmp;
        ++first;
    }
    return result;
}

}

int main()
{
    using namespace model;
    CAnimSet sets[13];
    for (int i = 0; i < 13; ++i) { sets[i].Priority = i * 3; sets[i].Tag = i; }
    int bad = 0;
    for (int n = 0; n <= 13; ++n)
        for (int k = 0; k <= n; ++k) {
            ConstIterator v[13];
            for (int i = 0; i < n; ++i) v[i].Node = &sets[i];
            ConstIterator* r = rotate(v, v + k, v + n);
            if (r != (k == 0 ? v + n : v + (n - k))) bad = 1;
            for (int i = 0; i < n; ++i)
                if (v[i].Node != &sets[(i + k) % n]) bad = 2;
        }
    if (bad) { std::printf("007103c0_TEST FAIL\n"); return 1; }
    std::printf("007103c0_TEST PASS\n");
    return 0;
}