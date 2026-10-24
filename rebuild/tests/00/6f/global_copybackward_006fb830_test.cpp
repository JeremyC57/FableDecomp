// Behaviour test for 006fb830: __copy_backward over pair<unsigned long, CSmallVector<NAnimationEvents::CEvent,32> > --
// an overlapping shift right assigns from the back (the small vector's operator= skips self-assignment and otherwise
// deep-copies through Copy) and returns the new start.
#include <cstdio>

static int g_copies = 0;
namespace NAnimationEvents {
struct CEvent { long a; long b; };
}
template <class T, int N> class CSmallVector {
public:
    T*  Start;
    int Size;
    void Copy(const CSmallVector& other) { Start = other.Start; Size = other.Size; ++g_copies; }   // retail reallocates
    CSmallVector& operator=(const CSmallVector& other)
    {
        if (&other != this)
            Copy(other);
        return *this;
    }
};
struct PairULongEventVector {
    unsigned long                              first;
    CSmallVector<NAnimationEvents::CEvent, 32> second;
};
struct random_access_iterator_tag { };

PairULongEventVector* __copy_backward(PairULongEventVector* first, PairULongEventVector* last,
                                       PairULongEventVector* result, const random_access_iterator_tag&, long*)
{
    for (long n = long(last - first); n > 0; --n)
        *--result = *--last;
    return result;
}

static NAnimationEvents::CEvent g_events[5];
int main()
{
    PairULongEventVector v[5];
    for (int i = 0; i < 5; ++i) {
        v[i].first = 10 * i;
        v[i].second.Start = &g_events[i];
        v[i].second.Size = i;
    }
    random_access_iterator_tag tag;
    PairULongEventVector* r = __copy_backward(v + 1, v + 4, v + 5, tag, (long*)0);   // [1,4) -> [2,5)
    if (r != v + 2 || g_copies != 3) { std::printf("006fb830_TEST FAIL\n"); return 1; }
    static const unsigned long want[5] = { 0, 10, 10, 20, 30 };
    for (int i = 0; i < 5; ++i) {
        int src = (i < 2) ? i : i - 1;
        if (v[i].first != want[i] || v[i].second.Start != &g_events[src] || v[i].second.Size != src) {
            std::printf("006fb830_TEST FAIL\n");
            return 1;
        }
    }
    g_copies = 0;
    __copy_backward(v, v + 2, v + 2, tag, (long*)0);                                  // in place: every operator= is a self-assignment
    if (g_copies != 0 || v[0].first != 0 || v[1].first != 10) { std::printf("006fb830_TEST FAIL\n"); return 1; }
    std::printf("006fb830_TEST PASS\n");
    return 0;
}