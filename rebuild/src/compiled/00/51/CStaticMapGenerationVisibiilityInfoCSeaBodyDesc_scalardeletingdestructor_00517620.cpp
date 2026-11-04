// CStaticMapGenerationVisibiilityInfo::CSeaBodyDesc::`scalar deleting destructor'(unsigned int flags) -- the compiler-generated deleting destructor: the implicit ~CSeaBodyDesc inlined (Maps freed, then Identifier destroyed), then operator delete when bit 0 of flags is set; returns this
// Leaf: ??_GCSeaBodyDesc@CStaticMapGenerationVisibiilityInfo@@QAEPAXI@Z
// Plan phase 3 (2026-11-04), CArray wave: the local CArray (an empty public std::vector<T>) and its STLport stand-ins replaced by the shared engine/CArray.h (Lionhead's array: a public STLport 4.6 vector<T>, 12 bytes, no fields of its own). STLport algorithm / construct helpers and placement new come from the header's model (<new>).
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00517620, 48 bytes, thiscall, `ret 4`; unreferenced (no call or pointer to it in Fable.exe).
// Identity: CStaticMapGenerationVisibiilityInfo::CSeaBodyDesc -- proven; evidence: FableWin.pdb names ??_GCSeaBodyDesc@CStaticMapGenerationVisibiilityInfo@@QAEPAXI@Z (a struct: U in its CArray<> names); the layout is Ego_r.pdb CSeaBodyDesc (CCharString Identifier +0, CArray<long> Maps +4) and the same TU holds the CSeaBodyDesc vector helpers (copy_backward over 20-byte CCharString+CArray elements at 0x00517430)
// Label: the manifest's "CParticleEffect::`scalar_deleting_destructor'" is propagated across byte-identical bodies.
// Callees: free 0x00BFEA14 (the inlined _Vector_base destructor: allocator::deallocate behind the null test), CCharString::~CCharString 0x0099EAE0, operator delete 0x00BFE9BC.
// Shape: no vtable store -- VC7.1 stores the vptr only in user-written destructors and CSeaBodyDesc's is implicit; members are
//   destroyed in reverse declaration order (the vector's trivially destructible elements need no loop: STLport's
//   __type_traits<T>::has_trivial_destructor selects the empty __destroy_aux).
//   The non-polymorphic deleting destructor is emitted where a delete expression needs it; retail keeps it although
//   nothing references it (no call or pointer in Fable.exe), so DeleteSeaBodyDesc is that delete expression, nothing more.
class CCharString {
public:
    CCharString();                                            // 0x0099E4B0
    ~CCharString();                                           // 0x0099EAE0
    void* PStringData;
};
extern "C" void __cdecl free(void* block);                   // 0x00BFEA14
#include "engine/CArray.h"
class CStaticMapGenerationVisibiilityInfo {
public:
    struct CSeaBodyDesc {                                     // 16 bytes
        CCharString  Identifier;                              // +0x00
        CArray<long> Maps;                                    // +0x04
    };
};
void DeleteSeaBodyDesc(CStaticMapGenerationVisibiilityInfo::CSeaBodyDesc* p) { delete p; }