// Behaviour test for std::__uninitialized_fill_n<QuadricOptimiserInternals::COptimisedEdge*> @ 0x00ACFFE0
#include <cstdio>
#include <new>

struct COptimisedEdge { unsigned int FaceA, FaceB, UniqueVertex1, UniqueVertex2; };

static COptimisedEdge* uninitialized_fill_n(COptimisedEdge* first, unsigned int n, const COptimisedEdge& x)
{
    COptimisedEdge* cur = first;
    for ( ; n > 0; --n, ++cur)
        new (cur) COptimisedEdge(x);
    return cur;
}

int main()
{
    COptimisedEdge storage[6];
    for (int i = 0; i < 6; ++i) { storage[i].FaceA = storage[i].FaceB = storage[i].UniqueVertex1 = storage[i].UniqueVertex2 = 0xDEAD; }
    COptimisedEdge x = { 1, 2, 3, 4 };
    COptimisedEdge* end = uninitialized_fill_n(storage, 4, x);
    int ok = end == storage + 4 && sizeof(COptimisedEdge) == 16u;
    for (int i = 0; i < 4 && ok; ++i)
        if (storage[i].FaceA != 1 || storage[i].FaceB != 2 || storage[i].UniqueVertex1 != 3 || storage[i].UniqueVertex2 != 4) ok = 0;
    if (storage[4].FaceA != 0xDEAD) ok = 0;
    if (uninitialized_fill_n(storage, 0, x) != storage) ok = 0;
    if (!ok) { std::printf("00acffe0_TEST FAIL\n"); return 1; }
    std::printf("00acffe0_TEST PASS\n");
    return 0;
}