// Behaviour test for 0x005E15E0 std::vector<const std::pair<unsigned long, CAnimationEntry>*>::reserve, linked against the row
// object (the shared header declares reserve only here). Checks retail's body: nothing happens unless capacity() < n (equal and
// smaller requests keep the block); a non-empty vector gets a new block of exactly n slots with the elements copied (memmove) and
// size kept; an empty (null) vector allocates n slots without a copy; reserve(0) on a null vector stays null.
#define FABLE_RETAIL_STLP_VECTOR
#define FABLE_RETAIL_STLP_VECTOR_C
#include <stdio.h>
#include "stlport/fable_containers.h"
class CAnimationEntry;
typedef const std::pair<unsigned long, CAnimationEntry>* TAnimIt;

static unsigned long g_pairs[10 * 5];   // ten 0x14-byte map pairs; the vector holds their addresses (never dereferenced)
static TAnimIt P(int i) { return (TAnimIt)&g_pairs[i * 5]; }
static int g_fail;
static void Check(int ok, const char* what) { if (!ok) { printf("FAIL: %s\n", what); g_fail = 1; } }

int main()
{
    {
        std::vector<TAnimIt> v;
        v.reserve(0);
        Check(v.begin() == 0 && v.capacity() == 0, "reserve(0) on a null vector: no allocation");
        v.reserve(5);
        Check(v.begin() != 0 && v.capacity() == 5 && v.size() == 0, "null vector: exactly n slots, size 0");
        for (int i = 0; i < 3; ++i) v.push_back(P(i));
        TAnimIt* block = v.begin();
        v.reserve(2);
        Check(v.begin() == block && v.capacity() == 5, "smaller request keeps the block");
        v.reserve(5);
        Check(v.begin() == block && v.capacity() == 5, "equal request keeps the block (capacity() < n)");
        v.reserve(6);
        Check(v.begin() != block && v.capacity() == 6 && v.size() == 3, "growth: new block of exactly n, size kept");
        Check(v[0] == P(0) && v[1] == P(1) && v[2] == P(2), "growth copies the elements in order");
        v.push_back(P(3)); v.push_back(P(4)); v.push_back(P(5));
        Check(v.capacity() == 6 && v.size() == 6, "the reserved slots are used before any regrowth");
        v.reserve(40);
        int same = v.size() == 6 && v.capacity() == 40;
        for (int i = 0; i < 6 && same; ++i) same = v[i] == P(i);
        Check(same, "large growth keeps all six elements");
    }
    printf(g_fail ? "005e15e0_TEST FAIL\n" : "005e15e0_TEST PASS\n");
    return g_fail;
}
