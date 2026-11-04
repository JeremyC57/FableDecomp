// CDefinitionManager::CDefClassInfo::CDefClassInfo() -- the record's default constructor: an empty name, then the empty array
// Leaf: ??0CDefClassInfo@CDefinitionManager@@QAE@XZ
// Plan phase 3 (2026-11-04), CArray wave: the local CArray (an empty public std::vector<T>) and its STLport stand-ins replaced by the shared engine/CArray.h (Lionhead's array: a public STLport 4.6 vector<T>, 12 bytes, no fields of its own). STLport algorithm / construct helpers and placement new come from the header's model (<new>).
// Retail: Fable.exe (TLC, VC7.1) @ 0x009B5D30, 23 bytes, thiscall, plain `ret`.
// Identity (probable): unreferenced (inlined at its uses); Ego_r.pdb CDefinitionManager::CDefClassInfo is {CCharString Name, AllocFunc, CArray<unsigned long> GlobalIndices} -- the only Ego_r.pdb record with a CCharString at +0, one scalar at +4 and an array at +8 -- and FableWin.pdb has ??0CDefClassInfo@CDefinitionManager@@QAE@XZ; it sits among the definition manager's instantiations (CArray<unsigned char>::DeallocateUnusedMemory 0x009B5DB0, sort_heap<pair<CCharString, unsigned long> >).
// Callees: CCharString::CCharString() 0x0099E4B0.
// Shape (family M109, representative 0x00515620): CCharString's default constructor out of line, then the CArray (an STLport
//   vector, the landed `CArray<T> : public std::vector<T>` model) zeroes its three pointers at +0x8 (one `xor eax,eax`).
class CCharString {
public:
    CCharString();                                        // 0x0099E4B0
    void* PStringData;
};
#include "engine/CArray.h"
class CDefClassBase;
class CDefinitionManager {
public:
    class CDefClassInfo {                                 // Ego_r.pdb, 24 bytes (Dinkumware CArray)
    public:
        CDefClassInfo() { }
        CCharString Name;                                 // +0x0
        CDefClassBase* (__cdecl* AllocFunc)();            // +0x4 (left uninitialised)
        CArray<unsigned long> GlobalIndices;              // +0x8
    };
};
// the implicit-in-retail constructor, forced out of line for this object (not part of the retail body)
CDefinitionManager::CDefClassInfo* ConstructOne(void* memory);
CDefinitionManager::CDefClassInfo* ConstructOne(void* memory) { return new (memory) CDefinitionManager::CDefClassInfo; }