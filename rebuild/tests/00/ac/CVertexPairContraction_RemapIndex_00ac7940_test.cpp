// CVertexPairContraction::RemapIndex(unsigned long from, unsigned long to) -- replace vertex index `from` by `to` in both indices of every sub-contraction
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00AC7940, length 75 bytes (padded to 80), `ret 8`. No callees.
// Layout (Ego_r.pdb): QuadricOptimiserInternals::CVertexPairContraction, 44 bytes, no vptr = {bool Exists +0, bool Unsplit +1,
// CArray<CSubContraction> SubContractions +4 (16 bytes; its start/finish words at +4/+8), float Weight2 +0x14, float Cost
// +0x18, C3DVector ResultingPosition +0x1c, map iterator ListIterator +0x28}; CSubContraction = {ulong Index1, ulong Index2}.
// The engine_api signature is `void __fastcall RemapIndex(CVertexPairContraction*, ulong, ulong)`.
// Shape: the size `finish - start` over 8-byte elements recomputed from memory every iteration and compared UNSIGNED
// (`jbe`/`jb`); the per-element work is an inline CSubContraction member taking `this` = &SubContractions[i]: the start is
// reloaded at the loop top, Index1 is loaded into ebx before the element address `lea eax,[eax+edx*8]` is formed once,
// Index2 compared from memory through it. A `SubContraction& sub = ...` reference in the loop body instead folds the first
// field to `cmp [eax+edx*8],esi` and lea-s the second, and CSEs the start pointer between the size and the element address.
// CSubContraction::RemapIndex is an inline stand-in (no out-of-line copy in retail; name not in the PDB).
struct CSubContraction {                            // QuadricOptimiserInternals::CSubContraction, 8 bytes
    unsigned long Index1;                           // +0x00
    unsigned long Index2;                           // +0x04
    void RemapIndex(unsigned long from, unsigned long to)
    {
        if (Index1 == from)
            Index1 = to;
        if (Index2 == from)
            Index2 = to;
    }
};
struct CSubContractionArray {                       // CArray<CSubContraction>: the vector words this function reads
    CSubContraction* _M_start;                      // +0x00
    CSubContraction* _M_finish;                     // +0x04
    CSubContraction* _M_end_of_storage;             // +0x08
    unsigned long size() const { return (unsigned long)(_M_finish - _M_start); }
    CSubContraction& operator[](unsigned long n) { return *(_M_start + n); }
};
struct CVertexPairContraction {                     // QuadricOptimiserInternals::CVertexPairContraction (Ego_r.pdb)
    bool                 Exists;                    // +0x00
    bool                 Unsplit;                   // +0x01
    CSubContractionArray SubContractions;           // +0x04
    void RemapIndex(unsigned long from, unsigned long to);
};

void CVertexPairContraction::RemapIndex(unsigned long from, unsigned long to)
{
    for (unsigned long i = 0; i < SubContractions.size(); ++i)
        SubContractions[i].RemapIndex(from, to);
}
// ---- harness ----
#include <cstdio>
static int fails = 0;
#define CHECK(c) do { if (!(c)) { ++fails; printf("FAIL %d: %s\n", __LINE__, #c); } } while (0)
int main()
{
    CSubContraction subs[4] = { {1, 2}, {3, 1}, {1, 1}, {4, 5} };
    CVertexPairContraction c;
    c.Exists = true; c.Unsplit = false;
    c.SubContractions._M_start = subs;
    c.SubContractions._M_finish = subs + 4;
    c.SubContractions._M_end_of_storage = subs + 4;
    c.RemapIndex(1, 9);
    CHECK(subs[0].Index1 == 9 && subs[0].Index2 == 2);
    CHECK(subs[1].Index1 == 3 && subs[1].Index2 == 9);
    CHECK(subs[2].Index1 == 9 && subs[2].Index2 == 9);
    CHECK(subs[3].Index1 == 4 && subs[3].Index2 == 5);
    c.SubContractions._M_finish = subs;               // empty: nothing touched
    c.RemapIndex(4, 0);
    CHECK(subs[3].Index1 == 4);
    if (fails == 0) printf("00ac7940_TEST PASS\n");
    return fails;
}