// QuadricOptimiserInternals::CVertexPairContraction::CleanUp() -- removes duplicate sub-contractions: for each pair, every later pair with the same Index1/Index2 is erased from SubContractions
// Leaf: CleanUp
// Plan phase 3 (2026-11-04), CArray wave: the local CArray (an empty public std::vector<T>) and its STLport stand-ins replaced by the shared engine/CArray.h (Lionhead's array: a public STLport 4.6 vector<T>, 12 bytes, no fields of its own). STLport algorithm / construct helpers and placement new come from the header's model (<new>).
// Retail: Fable.exe (TLC, VC7.1) @ 0x00AC7B00, 188 bytes, plain `ret`.
//   PDB name ?CleanUp@CVertexPairContraction@QuadricOptimiserInternals@@QAEXXZ.
// Callees: none (STLport vector::erase and the SGI random-access copy loop are inline).
// Layout (retail; landed COptimisedPrimitive::PerformContraction 0x00ACD840 model): Exists +0, Unsplit +1,
//   SubContractions CArray<CSubContraction> +0x04 (first +4, last +8, end +0xc), 8-byte {Index1, Index2} elements.
// Shape: two index loops re-reading size() every pass; `a` is a reference to element i taken once per outer pass;
//   erase(begin() + j) copies the tail down element by element (STLport __copy_ptrs with __false_type: SGI
//   `for (n = last - first; n > 0; --n)`), decrements finish, and the loop steps j back.
#include "engine/CArray.h"
namespace QuadricOptimiserInternals {
class CSubContraction { public:
    unsigned long Index1;                          // +0x00
    unsigned long Index2;                          // +0x04
};
class CVertexPairContraction { public:
    bool                    Exists;                // +0x00
    bool                    Unsplit;               // +0x01
    CArray<CSubContraction> SubContractions;       // +0x04
    float                   Weight2;               // +0x10
    float                   Cost;                  // +0x14
    float                   ResultingPosition[3];  // +0x18
    void*                   ListIterator;          // +0x24
    void CleanUp();
};
void CVertexPairContraction::CleanUp()
{
    for (unsigned long i = 0; i < SubContractions.size(); ++i) {
        CSubContraction& a = SubContractions[i];
        for (unsigned long j = i + 1; j < SubContractions.size(); ++j) {
            CSubContraction& b = SubContractions[j];
            if (a.Index1 == b.Index1 && a.Index2 == b.Index2) {
                SubContractions.erase(SubContractions.begin() + j);
                --j;
            }
        }
    }
}
}