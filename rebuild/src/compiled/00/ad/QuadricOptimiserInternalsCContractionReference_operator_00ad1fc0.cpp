// QuadricOptimiserInternals::CContractionReference::operator=(const CContractionReference& other) -- assign a contraction reference: re-point the counted CVertexPairContraction pointer (release the old block, add a reference to the new one) when the blocks differ, then copy the sub-index
// Leaf: ??4CContractionReference
// Plan phase 5 (2026-11-05), CCountedPointer wave: the local CCountedPointer / count-block stand-ins replaced by the shared engine/CCountedPointer.h (Ego_r.pdb: CCountedPointer<T> Data +0x0 / Info +0x4; CCPPointerInfo RefCount +0x0 / DeleteFunc +0x4 / Data +0x8).
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00AD1FC0, length 80 bytes, `ret 4`. Callers: the quadric optimiser's tree
// deep copy (0x00ACA510) and its vertex-pair contraction (0x00ACCE49). The catalog's CCountedPointer<CDiskFileWin32>::operator=
// label is propagated: the 12-byte {Data, Info, SubIndex} layout is the CContractionReference of the landed quadric
// optimiser map 0x00AD52A0 (CCountedPointer<CVertexPairContraction> Contraction + unsigned SubIndex).
// Callees: the block's DeleteFunc (CCPPointerInfo +4, called with the data in ecx), operator delete 0x00BFE9BC.
// Shape: the source pointer/info words are read into locals first (ebp / edi) and the data store sits inside the
// different-block arm (retail skips it when the blocks match); the release is `--Info->RefCount` with the member re-read
// for the zero test (GOTCHAS 24h), then DeleteFunc(Info->Data) and `delete Info` re-reading Info each time.
// Layout: Ego_r.pdb CCPPointerInfo {RefCount +0, DeleteFunc +4, Data +8}.
void __cdecl operator delete(void* block);                  // 0x00BFE9BC

#include "engine/CCountedPointer.h"


namespace QuadricOptimiserInternals {

class CVertexPairContraction;

class CContractionReference { public:                               // 12 bytes
    CCountedPointer<CVertexPairContraction> Contraction;     // +0x00
    unsigned long                           SubIndex;        // +0x08
    CContractionReference& operator=(const CContractionReference& other);
};

CContractionReference& CContractionReference::operator=(const CContractionReference& other)
{
    Contraction = other.Contraction;
    SubIndex = other.SubIndex;
    return *this;
}

}