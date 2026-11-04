// CIDrawEnvironmentTextureSet::~CIDrawEnvironmentTextureSet() -- free the blend array, then the texture array
// Leaf: ??1CIDrawEnvironmentTextureSet
// Plan phase 3 (2026-11-04), CArray wave: the local CArray (an empty public std::vector<T>) and its STLport stand-ins replaced by the shared engine/CArray.h (Lionhead's array: a public STLport 4.6 vector<T>, 12 bytes, no fields of its own). STLport algorithm / construct helpers and placement new come from the header's model (<new>).
// Retail: Fable.exe (TLC, VC7.1) function @ 0x006B5B00, length 36 bytes.
// Callee: free @ 0x00BFEA14 (twice).
// Identity: the manifest label "CSystemRegistry::~CSystemRegistry" is propagated over byte-identical
// bodies. This one is the element destructor that 0x006B6390 hands to `eh vector destructor iterator`
// (0x00401400: element size 0x18, 3 elements, 6 rows of 0x1F4 bytes) inside the environment-theme code
// (0x006B63C0 is CEnvironmentThemeDef::operator=). The PDB class with that layout is
// CIDrawEnvironmentTextureSet { CArray<long> Textures; CArray<float> Blends; } -- the PDB's 16-byte
// CArray puts Blends at +0x10; the retail CArray is three pointers, so Blends sits at +0x0c and the
// record is 0x18 bytes, matching the iterator's element size.
//
// Shape: two member objects with the inline CArray destructor (`if (first) free(first)`), destroyed in
// reverse declaration order (+0x0c, then +0x00 -- the last one through `mov esi,[esi]`).
extern "C" void __cdecl free(void* block);       // 0x00BFEA14

#include "engine/CArray.h"

struct CIDrawEnvironmentTextureSet {
public:
    ~CIDrawEnvironmentTextureSet();
    CArray<long>  Textures;                      // +0x00
    CArray<float> Blends;                        // +0x0c
};

CIDrawEnvironmentTextureSet::~CIDrawEnvironmentTextureSet()
{
}