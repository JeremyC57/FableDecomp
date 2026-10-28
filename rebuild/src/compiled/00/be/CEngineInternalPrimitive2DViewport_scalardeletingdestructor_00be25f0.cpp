// CEngineInternalPrimitive2DViewport::`scalar deleting destructor'(unsigned int flags) -- the compiler-generated destructor (Primitives destroyed, then the inline CIVCountedPointeeBase destructor's vtable store) and the flag-tested operator delete
// Leaf: ??_GCEngineInternalPrimitive2DViewport@@UAEPAXI@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00BE25F0, 42 bytes (+ int3 padding), thiscall `ret 4`, returns this.
// Identity (proven): CEngineInternalPrimitive2DViewport::`scalar deleting destructor'; evidence: it is slot 0 of vtable 0x012A7F14 (RTTI .?AVCEngineInternalPrimitive2DViewport@@,
//   bases .?AVCEngineInternalPrimitiveBase@@(+0) .?AVCEnginePersistentPrimitiveBase@@(+0) .?AVCIVCountedPointeeBase@@(+0)), it destroys Primitives at +0x94 (Ego_r.pdb CArray<CEnginePrimitiveHandle> Primitives +0x94) and stores the
//   CIVCountedPointeeBase vtable 0x012354D0 (RTTI .?AVCIVCountedPointeeBase@@).
// Label: the manifest's "`scalar_deleting_destructor'" is propagated (family H085: six deleting destructors of classes whose only non-trivial
//   member is one object at +8 / +0x94; retail has no identical-code folding, so each address is its own class's ??_G).
// Callees: CArray<CEnginePrimitiveHandle>::~CArray 0x00AF7210 on Primitives (each handle's virtual destructor, then the block), operator delete 0x00BFE9BC.
// Shape: CEngineInternalPrimitive2DViewport has no user-declared destructor (no own vtable store); CIVCountedPointeeBase's `virtual ~CIVCountedPointeeBase() { }` is inline
//   and stores its vtable; the vtable (and so this ??_G) is emitted by the construction helper below. Local classes, Review at landing.
void __cdecl operator delete(void* block);         // 0x00BFE9BC
namespace std {
template <class _Tp> class allocator { };
template <class _Tp, class _Alloc = allocator<_Tp> > class vector {   // retail: 12 bytes
public:
    _Tp* _M_start;
    _Tp* _M_finish;
    _Tp* _M_end_of_storage;
    vector();
    ~vector();
};
}
// 0x00AF7210 is the engine library's CArray<CEnginePrimitiveHandle> destructor (Ego_r.pdb: the engine members are
//   CArray<CEnginePrimitiveHandle>, and ??1?$CArray@VCEnginePrimitiveHandle@@@@QAE@XZ is a PDB symbol); the game's
//   byte-identical std::vector<CEnginePrimitiveHandle>::~vector is 0x00440C40 (CTCInventoryExperience members).
//   Two retail addresses cannot share one COMDAT name, so the engine copy is the CArray destructor.
template <class T> class CArray : public std::vector<T> {        // retail CArray: an STLport vector (12 bytes)
public:
    ~CArray();                                              // 0x00AF7210
};
class CEnginePrimitiveHandle;
class CIVCountedPointeeBase {                               // vtable 0x012354D0
public:
    virtual ~CIVCountedPointeeBase() { }
    long RefCount;                                          // +0x04
};
class CEnginePersistentPrimitiveBase : public CIVCountedPointeeBase {
public:
    unsigned char PersistentData[0x40];                     // +0x08 .. +0x47 (trivially destructible)
};
class CEngineInternalPrimitiveBase : public CEnginePersistentPrimitiveBase {
};
class CEngineInternalPrimitive2DViewport : public CEngineInternalPrimitiveBase {
public:
    unsigned char ViewportData[0x94 - 0x48];                // +0x48 Viewport .. +0x90 Layer (trivially destructible)
    CArray<CEnginePrimitiveHandle> Primitives;              // +0x94
};

// VC7.1 emits a class's vtable (and the deleting destructor it references) in every translation
// unit that constructs the object; this helper is that construction, nothing more.
void* MakeOne_00be25f0()
{
    return new CEngineInternalPrimitive2DViewport;
}