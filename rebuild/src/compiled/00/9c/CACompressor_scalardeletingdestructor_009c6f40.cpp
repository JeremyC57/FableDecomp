// CACompressor::`scalar deleting destructor'(unsigned int flags) -- the compiler-generated deleting destructor: the implicit ~CACompressor inlined (OwnWorkspace freed, then ~CBaseClass), then operator delete when bit 0 of flags is set; returns this
// Leaf: ??_GCACompressor@@UAEPAXI@Z
// Plan phase 3 (2026-11-04), CArray wave: the local CArray (an empty public std::vector<T>) and its STLport stand-ins replaced by the shared engine/CArray.h (Lionhead's array: a public STLport 4.6 vector<T>, 12 bytes, no fields of its own). STLport algorithm / construct helpers and placement new come from the header's model (<new>).
// Retail: Fable.exe (TLC, VC7.1) function @ 0x009C6F40, 48 bytes, thiscall, `ret 4`; reached only through vtable slot 0.
// Identity: CACompressor -- proven; evidence: vtable 0x0129B424 slot 0 (RTTI .?AVCACompressor@@ : CBaseClass); FableWin.pdb names ??_GCACompressor@@UAEPAXI@Z; the freed vector at +0x10 is Ego_r.pdb OwnWorkspace (std::vector<unsigned char>) after Workspace, WorkspaceSize, CompressionOn; 0x0099A2E0 is CBaseClass::~CBaseClass (CBaseClass's own deleting destructor 0x004371F0 calls it)
// Label: the manifest's "CParticleEffect::`scalar_deleting_destructor'" is propagated across byte-identical bodies.
// Callees: free 0x00BFEA14 (the inlined _Vector_base destructor: allocator::deallocate behind the null test), CBaseClass::~CBaseClass 0x0099A2E0, operator delete 0x00BFE9BC.
// Shape: no vtable store -- VC7.1 stores the vptr only in user-written destructors and CACompressor's is implicit; members are
//   destroyed in reverse declaration order (the vector's trivially destructible elements need no loop: STLport's
//   __type_traits<T>::has_trivial_destructor selects the empty __destroy_aux).
//   The deleting destructor is emitted with the vtable, i.e. in a TU that constructs the class: MakeOne is that
//   construction, nothing more. KillOne's qualified explicit destructor call makes VC7.1 define the implicit destructor
//   before it generates the vtable's deleting destructor, which then inlines it (without it the deleting destructor
//   calls an out-of-line ??1 whose body holds the inline free: 30 bytes against 48).
#include "engine/CBaseClass.h"                         // CBaseClass() 0x0099A2D0, ~CBaseClass 0x0099A2E0
extern "C" void __cdecl free(void* block);                   // 0x00BFEA14
#include "engine/CArray.h"
class CACompressor : public CBaseClass {                  // 0x1c bytes
public:
    void*                      Workspace;                     // +0x04
    unsigned long              WorkspaceSize;                 // +0x08
    bool                       CompressionOn;                 // +0x0c
    std::vector<unsigned char> OwnWorkspace;                  // +0x10
};
void* MakeOne() { return new CACompressor; }
void KillOne(CACompressor* p) { p->CACompressor::~CACompressor(); }