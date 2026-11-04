// CDefinitionManager::CDefClassInfo::~CDefClassInfo() -- the compiler-generated destructor: free the GlobalIndices block, then destroy Name
// Leaf: ??1CDefClassInfo@CDefinitionManager@@QAE@XZ
// Plan phase 3 (2026-11-04), CArray wave: the local CArray (an empty public std::vector<T>) and its STLport stand-ins replaced by the shared engine/CArray.h (Lionhead's array: a public STLport 4.6 vector<T>, 12 bytes, no fields of its own). STLport algorithm / construct helpers and placement new come from the header's model (<new>).
// Retail: Fable.exe (TLC, VC7.1) function @ 0x009B5D50, 27 bytes, thiscall, tail `jmp ~CCharString`.
// Identity (proven): unreferenced in retail (inlined at its uses; the linker kept the COMDAT). FableWin.pdb has the pair
//   ??0CDefClassInfo@CDefinitionManager@@QAE@XZ (0x0305A7B0) then ??1CDefClassInfo@CDefinitionManager@@QAE@XZ (0x0305A820)
//   back to back; retail has the same pair: the landed ctor 0x009B5D30 then this row. Ego_r.pdb CDefClassInfo is
//   {CCharString Name +0, AllocFunc +4, CArray<unsigned long> GlobalIndices +8}: exactly the members this body destroys
//   (vector start at +8, CCharString at +0). Not polymorphic (no vtable). The catalog label "CTCInventoryItem::OnDie" is
//   propagated.
// Callees: free 0x00BFEA14 (~_Vector_base's deallocate), CCharString::~CCharString 0x0099EAE0.
// Shape: members in reverse order: GlobalIndices (CArray over the STLport vector: trivial _Destroy, null-tested free), then
//   Name's out-of-line destructor as the tail jump. DestroyOne (a destruction by name, ungraded) makes VC7.1 emit this
//   out-of-line implicit `??1`.
extern "C" void __cdecl free(void* block);                      // 0x00BFEA14
#include "engine/CArray.h"

class CCharStringData;
class CCharString {
public:
    CCharString();                                          // 0x0099E4B0
    ~CCharString();                                         // 0x0099EAE0
    CCharStringData* PStringData;                           // +0x00
};
class CDefClassBase;
class CDefinitionManager {
public:
    class CDefClassInfo {                                   // Ego_r.pdb, 0x14 bytes in retail
    public:
        CDefClassInfo() { }                                 // 0x009B5D30
        CCharString           Name;                         // +0x00
        CDefClassBase* (__cdecl* AllocFunc)();              // +0x04
        CArray<unsigned long> GlobalIndices;                // +0x08
    };
};

// Emission helper: destroying the object by name is what makes VC7.1 emit the out-of-line implicit `??1` (chunk 656/657).
void DestroyOne(CDefinitionManager::CDefClassInfo* info)
{
    info->CDefinitionManager::CDefClassInfo::~CDefClassInfo();
}