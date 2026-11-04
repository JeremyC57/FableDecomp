// NThingManager::CEntityRuntimePersistence::CEntityScriptInfo::`scalar deleting destructor'(unsigned int flags) -- the compiler-generated deleting destructor: the implicit ~CEntityScriptInfo inlined (SaveData freed, then Script destroyed), then operator delete when bit 0 of flags is set; returns this
// Leaf: ??_GCEntityScriptInfo@CEntityRuntimePersistence@NThingManager@@QAEPAXI@Z
// Plan phase 3 (2026-11-04), CArray wave: the local CArray (an empty public std::vector<T>) and its STLport stand-ins replaced by the shared engine/CArray.h (Lionhead's array: a public STLport 4.6 vector<T>, 12 bytes, no fields of its own). STLport algorithm / construct helpers and placement new come from the header's model (<new>).
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0052AAD0, 48 bytes, thiscall, `ret 4`; unreferenced (no call or pointer to it in Fable.exe).
// Identity: NThingManager::CEntityRuntimePersistence::CEntityScriptInfo -- proven; evidence: FableWin.pdb names ??_GCEntityScriptInfo@CEntityRuntimePersistence@NThingManager@@QAEPAXI@Z (a class: V); the layout is Ego_r.pdb CEntityScriptInfo (CCharString Script +0, CArray<unsigned char> SaveData +4, long UncompressedSize, bool Valid), and the adjacent constructor 0x0052AB00 builds exactly that: CCharString() at +0, zeroes +4/+8/+0xC/+0x10 and the bool at +0x14
// Label: the manifest's "CParticleEffect::`scalar_deleting_destructor'" is propagated across byte-identical bodies.
// Callees: free 0x00BFEA14 (the inlined _Vector_base destructor: allocator::deallocate behind the null test), CCharString::~CCharString 0x0099EAE0, operator delete 0x00BFE9BC.
// Shape: no vtable store -- VC7.1 stores the vptr only in user-written destructors and CEntityScriptInfo's is implicit; members are
//   destroyed in reverse declaration order (the vector's trivially destructible elements need no loop: STLport's
//   __type_traits<T>::has_trivial_destructor selects the empty __destroy_aux).
//   The non-polymorphic deleting destructor is emitted where a delete expression needs it; retail keeps it although
//   nothing references it (no call or pointer in Fable.exe), so DeleteEntityScriptInfo is that delete expression, nothing more.
class CCharString {
public:
    CCharString();                                            // 0x0099E4B0
    ~CCharString();                                           // 0x0099EAE0
    void* PStringData;
};
extern "C" void __cdecl free(void* block);                   // 0x00BFEA14
#include "engine/CArray.h"
namespace NThingManager {
class CEntityRuntimePersistence {
public:
    class CEntityScriptInfo {                                 // 0x18 bytes
    public:
        CCharString           Script;                         // +0x00
        CArray<unsigned char> SaveData;                       // +0x04
        long                  UncompressedSize;               // +0x10
        bool                  Valid;                          // +0x14
    };
};
}
void DeleteEntityScriptInfo(NThingManager::CEntityRuntimePersistence::CEntityScriptInfo* p) { delete p; }