// CDualArrayRecord::~CDualArrayRecord() -- free the second array's buffer, then the first's
// Leaf: ??1CDualArrayRecord
// Plan phase 3 (2026-11-04), CArray wave: the local CArray (an empty public std::vector<T>) and its STLport stand-ins replaced by the shared engine/CArray.h (Lionhead's array: a public STLport 4.6 vector<T>, 12 bytes, no fields of its own). STLport algorithm / construct helpers and placement new come from the header's model (<new>).
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0090BA20, length 36 bytes (+ int3 padding).
// Callee: free @ 0x00BFEA14 (twice).
// Identity: the manifest label "CSystemRegistry::~CSystemRegistry" is propagated over byte-identical
// bodies and is wrong (CSystemRegistry owns registry handles, not heap arrays). This copy sits
// next to the CLandscapeBackgroundLoadTask deleting destructor (0x0090BA50) and has no direct caller in the image, so the owning class is not
// established: it is modelled as a descriptive 0x18-byte record of two retail CArrays -- the same body
// as CIDrawEnvironmentTextureSet::~CIDrawEnvironmentTextureSet (0x006B5B00).
//
// Shape: two member objects with the inline CArray destructor (`if (first) free(first)`), destroyed in
// reverse declaration order (+0x0c, then +0x00 -- the last one through `mov esi,[esi]`). The retail
// CArray is three pointers (0x0c bytes).
extern "C" void __cdecl free(void* block);       // 0x00BFEA14

#include "engine/CArray.h"

class CDualArrayRecord {
public:
    ~CDualArrayRecord();
    CArray<long> m_first;                        // +0x00
    CArray<long> m_second;                       // +0x0c
};

CDualArrayRecord::~CDualArrayRecord()
{
}