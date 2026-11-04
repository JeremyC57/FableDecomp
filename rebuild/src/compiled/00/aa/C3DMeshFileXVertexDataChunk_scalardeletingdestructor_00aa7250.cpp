// C3DMeshFileXVertexDataChunk::`scalar deleting destructor'(unsigned int flags) -- the compiler-generated deleting destructor: the implicit ~C3DMeshFileXVertexDataChunk inlined (VertexData freed, then ~CChunkedFileChunk), then operator delete when bit 0 of flags is set; returns this
// Leaf: ??_GC3DMeshFileXVertexDataChunk@@UAEPAXI@Z
// Plan phase 3 (2026-11-04), CArray wave: the local CArray (an empty public std::vector<T>) and its STLport stand-ins replaced by the shared engine/CArray.h (Lionhead's array: a public STLport 4.6 vector<T>, 12 bytes, no fields of its own). STLport algorithm / construct helpers and placement new come from the header's model (<new>).
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00AA7250, 48 bytes, thiscall, `ret 4`; reached only through vtable slot 0.
// Identity: C3DMeshFileXVertexDataChunk -- proven; evidence: vtable 0x0129E33C slot 0 (RTTI .?AVC3DMeshFileXVertexDataChunk@@ : CChunkedFileChunk : CBaseClassNonCopyable : CBaseClass); FableWin.pdb names ??_GC3DMeshFileXVertexDataChunk@@UAEPAXI@Z; the freed vector at +0x40 is Ego_r.pdb VertexData (CArray<char>, after FormatID, VertexCount, VertexSize, InitFlags and the 0x20-byte VBCParams), after CChunkedFileChunk's 0x10 bytes; 0x00AA75C0 stores the CChunkedFileChunk vtable 0x0129E474 (RTTI .?AVCChunkedFileChunk@@)
// Label: the manifest's "CParticleEffect::`scalar_deleting_destructor'" is propagated across byte-identical bodies.
// Callees: free 0x00BFEA14 (the inlined _Vector_base destructor: allocator::deallocate behind the null test), CChunkedFileChunk::~CChunkedFileChunk 0x00AA75C0, operator delete 0x00BFE9BC.
// Shape: no vtable store -- VC7.1 stores the vptr only in user-written destructors and C3DMeshFileXVertexDataChunk's is implicit; members are
//   destroyed in reverse declaration order (the vector's trivially destructible elements need no loop: STLport's
//   __type_traits<T>::has_trivial_destructor selects the empty __destroy_aux).
//   The deleting destructor is emitted with the vtable, i.e. in a TU that constructs the class: MakeOne is that
//   construction, nothing more. KillOne's qualified explicit destructor call makes VC7.1 define the implicit destructor
//   before it generates the vtable's deleting destructor, which then inlines it (without it the deleting destructor
//   calls an out-of-line ??1 whose body holds the inline free: 30 bytes against 48).
#include "engine/CBaseClass.h"                         // CBaseClass() 0x0099A2D0, ~CBaseClass 0x0099A2E0
class CBaseClassNonCopyable : public CBaseClass {             // vtable 0x0129A7C4
protected:
    CBaseClassNonCopyable();                                  // 0x0099A2F0 (FableWin.pdb ??0CBaseClassNonCopyable@@IAE@XZ)
    virtual ~CBaseClassNonCopyable();                         // 0x0099A300 (FableWin.pdb ??1CBaseClassNonCopyable@@MAE@XZ)
};
class CChunkedFileSuperChunk;
class CChunkedFileChunk : public CBaseClassNonCopyable {      // vtable 0x0129E474, 0x10 bytes
protected:
    CChunkedFileChunk();
public:
    virtual ~CChunkedFileChunk();                             // 0x00AA75C0 (stores 0x0129E474, tail jmp ~CBaseClassNonCopyable)
    char ChunkType[5];                                        // +0x04
    CChunkedFileSuperChunk* Parent;                           // +0x0c
};
extern "C" void __cdecl free(void* block);                   // 0x00BFEA14
#include "engine/CArray.h"
class CVertexCompressionParams {                          // 0x20 bytes
public:
    float PosScaleX, PosScaleY, PosScaleZ, PosScaleW;         // +0x00
    float PosBiasX, PosBiasY, PosBiasZ, PosBiasW;             // +0x10
};
class C3DMeshFileXVertexDataChunk : public CChunkedFileChunk { // 0x4c bytes
public:
    unsigned long            FormatID;                        // +0x10
    unsigned long            VertexCount;                     // +0x14
    unsigned long            VertexSize;                      // +0x18
    unsigned long            InitFlags;                       // +0x1c
    CVertexCompressionParams VBCParams;                       // +0x20
    CArray<char>             VertexData;                      // +0x40
};
void* MakeOne() { return new C3DMeshFileXVertexDataChunk; }
void KillOne(C3DMeshFileXVertexDataChunk* p) { p->C3DMeshFileXVertexDataChunk::~C3DMeshFileXVertexDataChunk(); }