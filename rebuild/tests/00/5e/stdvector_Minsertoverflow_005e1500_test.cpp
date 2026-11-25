// Behaviour test for 0x005E1500 std::vector<const std::pair<unsigned long, CAnimationEntry>*>::_M_insert_overflow (the POD
// __true_type overload), linked against the row object (the shared header's overflow body is declaration-only here, so every
// growth path below calls the row). Checks retail's body: new capacity = size + max(size, fillLength); the head [start, position)
// is copied, fillLength copies of x follow, the tail [position, finish) is copied only when !atEnd (push_back passes atEnd);
// the vector adopts the new block (start / finish / end of storage).
#define FABLE_RETAIL_STLP_VECTOR
#define FABLE_RETAIL_STLP_VECTOR_C
#define FABLE_RETAIL_STLP_VECTOR_OVERFLOW_DECL_ONLY
#include <stdio.h>
#include "stlport/fable_containers.h"
class CAnimationEntry;
typedef const std::pair<unsigned long, CAnimationEntry>* TAnimIt;

static unsigned long g_pairs[20 * 5];   // twenty 0x14-byte map pairs; the vector holds their addresses (never dereferenced)
static TAnimIt P(int i) { return (TAnimIt)&g_pairs[i * 5]; }
static int g_fail;
static void Check(int ok, const char* what) { if (!ok) { printf("FAIL: %s\n", what); g_fail = 1; } }
static int Is(const std::vector<TAnimIt>& v, const int* want, int n)
{
    if ((int)v.size() != n) return 0;
    for (int i = 0; i < n; ++i) if (v[i] != P(want[i])) return 0;
    return 1;
}

int main()
{
    std::vector<TAnimIt> v;
    v.push_back(P(0));                                         // overflow(finish=0, x, 1, atEnd): 0 + max(0, 1)
    Check(v.capacity() == 1 && v.size() == 1 && v[0] == P(0), "first push_back: capacity 1");
    TAnimIt* block = v.begin();
    v.push_back(P(1));                                         // 1 + max(1, 1)
    Check(v.capacity() == 2 && v.begin() != block, "second push_back: capacity 2, new block");
    v.push_back(P(2));                                         // 2 + max(2, 1)
    Check(v.capacity() == 4, "third push_back: capacity 4");
    v.push_back(P(3));                                         // fits: no overflow
    { static const int w[4] = { 0, 1, 2, 3 }; Check(Is(v, w, 4) && v.capacity() == 4, "append-at-end keeps order"); }
    // middle insert of one element when full: head [0,1), x, tail [1,4)
    v.insert(v.begin() + 1, P(9));
    { static const int w[5] = { 0, 9, 1, 2, 3 }; Check(Is(v, w, 5) && v.capacity() == 8, "middle insert: head, x, tail; capacity 8"); }
    v.push_back(P(4)); v.push_back(P(5)); v.push_back(P(6));   // fill to 8 without overflow
    Check(v.size() == 8 && v.capacity() == 8, "filled to capacity");
    // fill insert of 3 copies in the middle when full: 8 + max(8, 3) = 16
    v._M_fill_insert(v.begin() + 2, 3, P(7));
    { static const int w[11] = { 0, 9, 7, 7, 7, 1, 2, 3, 4, 5, 6 }; Check(Is(v, w, 11) && v.capacity() == 16, "fill insert: n copies between head and tail"); }
    // fillLength larger than size: 1 + max(1, 5) = 6, appended at the end (no tail)
    std::vector<TAnimIt> u;
    u.push_back(P(10));
    u._M_fill_insert(u.end(), 5, P(11));
    { static const int w[6] = { 10, 11, 11, 11, 11, 11 }; Check(Is(u, w, 6) && u.capacity() == 6, "fill larger than size: capacity size + n"); }
    // insert at the front when full: empty head, x, whole tail
    std::vector<TAnimIt> f;
    f.push_back(P(12)); f.push_back(P(13));
    f.insert(f.begin(), P(14));
    { static const int w[3] = { 14, 12, 13 }; Check(Is(f, w, 3) && f.capacity() == 4, "front insert: x then the whole tail"); }
    printf(g_fail ? "005e1500_TEST FAIL\n" : "005e1500_TEST PASS\n");
    return g_fail;
}
