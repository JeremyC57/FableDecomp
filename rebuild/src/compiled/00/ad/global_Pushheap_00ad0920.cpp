// _global::_Push_heap(QuadricOptimiserInternals::COptimisedVertex::CGroup*, long, long, QuadricOptimiserInternals::COptimisedVertex::CGroup, bool (__fastcall*)(const CGroup&, const CGroup&)) -- heap sift-up of bone-weight groups under a function-pointer predicate
// Leaf: _Push_heap
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00AD0920, 104 bytes, `ret 0x10`.  No direct callees.
// Element: QuadricOptimiserInternals::COptimisedVertex::CGroup (PDB, 8 bytes): BoneIndex +0 (ulong),
// BoneWeight +4 (float); the heap is built under COptimisedVertex::CopyTo (0x00ACC490) / the
// COptimisedMesh TU, so the manifest label is right.  The landed sibling _Adjust_heap @ 0x00AD1DD0
// models the same 8-byte layout as CSpeechAnalyser::CMatchProbability (identical shape).
// Link-time __fastcall (/Gr algorithm TU): first in ecx, hole in edx, then top, the 8-byte group by
// value and the predicate (4 + 8 + 4 = 0x10).  The predicate is a plain __fastcall function pointer:
// `mov ecx,&parent ; lea edx,&value ; call [esp+20h]`.  Moves are two dword copies (implicit
// assignment: the float travels through an integer register).
// Shape: SGI/STLport `__push_heap` (`parent = (hole - 1) / 2; while (hole > top && comp(first[parent], value))`).
namespace QuadricOptimiserInternals {
struct COptimisedVertex {
    struct CGroup {
        unsigned long BoneIndex;               // +0x0
        float BoneWeight;                      // +0x4
    };
};
}
typedef QuadricOptimiserInternals::COptimisedVertex::CGroup CGroup;
typedef bool (__fastcall* TGroupCompare)(const CGroup& a, const CGroup& b);

void __fastcall _Push_heap(CGroup* first, long hole, long top, CGroup value, TGroupCompare comp)
{
    long parent = (hole - 1) / 2;
    while (hole > top && comp(first[parent], value)) {
        first[hole] = first[parent];
        hole = parent;
        parent = (hole - 1) / 2;
    }
    first[hole] = value;
}