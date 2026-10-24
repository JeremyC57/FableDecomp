// Behaviour test for 0x00AD7DC0 __destroy_aux over COptimisedSubMesh (bone names destroyed, both vectors freed).
#include <cstdio>
#include <cstdlib>

static int g_stringsDestroyed = 0, g_freed = 0, g_rangeDestroys = 0, g_pointersInRanges = 0;
struct CCharString { void* PStringData; ~CCharString() { ++g_stringsDestroyed; } };
struct SBone { CCharString BoneName; long Parent; bool Remapped; long NewIndex; };
struct SCounted { void* Data; void* Info; };
template <class T> struct SVector { T* Start; T* Finish; T* End; };
struct SSubMesh { SVector<SCounted> Primitives; SVector<SBone> Bones; };

static void Free(void* p) { ++g_freed; std::free(p); }
// stand-in for the out-of-line counted-pointer range destroy 0x00AD37E0
static void DestroyCountedRange(SCounted* first, SCounted* last) { ++g_rangeDestroys; g_pointersInRanges += (int)(last - first); }

// reconstructed body
static void DestroyAux(SSubMesh* first, SSubMesh* last)
{
    for (; first != last; ++first) {
        for (SBone* b = first->Bones.Start; b != first->Bones.Finish; ++b) b->BoneName.~CCharString();
        if (first->Bones.Start != 0) Free(first->Bones.Start);
        DestroyCountedRange(first->Primitives.Start, first->Primitives.Finish);
        if (first->Primitives.Start != 0) Free(first->Primitives.Start);
    }
}

int main()
{
    SSubMesh m[2];
    m[0].Bones.Start = (SBone*)std::malloc(3 * sizeof(SBone)); m[0].Bones.Finish = m[0].Bones.Start + 3; m[0].Bones.End = m[0].Bones.Finish;
    m[0].Primitives.Start = (SCounted*)std::malloc(2 * sizeof(SCounted)); m[0].Primitives.Finish = m[0].Primitives.Start + 2; m[0].Primitives.End = m[0].Primitives.Finish;
    m[1].Bones.Start = m[1].Bones.Finish = m[1].Bones.End = 0;
    m[1].Primitives.Start = m[1].Primitives.Finish = m[1].Primitives.End = 0;
    DestroyAux(m, m + 2);
    if (g_stringsDestroyed != 3 || g_freed != 2 || g_rangeDestroys != 2 || g_pointersInRanges != 2) { std::printf("00ad7dc0_TEST FAIL\n"); return 1; }
    DestroyAux(m, m);
    if (g_rangeDestroys != 2) { std::printf("00ad7dc0_TEST FAIL\n"); return 1; }
    std::printf("00ad7dc0_TEST PASS\n");
    return 0;
}