// CEngineInternalPrimitiveChanger::~CEngineInternalPrimitiveChanger() -- release the mesh effect, destroy every child primitive handle and free the handle array, then fall back to the counted-pointee base (its vtable restored)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00BC5F50, length 80 bytes.
// Leaf: ??1CEngineInternalPrimitiveChanger
// Label correction: the catalog "_Dest_val<..., std::pair<EAnimComponent, CIVCountedPointer<CAnimComponentBase>>>" is
// propagated. The only caller is 0x00BC5F33 inside the deleting destructor 0x00BC5F30, which is slot 0 of vtable
// 0x012A662C = CEngineInternalPrimitiveChanger (rtti-vtables), so this is that class's destructor.
// Layout: Ego_r.pdb CEngineInternalPrimitiveChanger (104 bytes): Primitives (CArray<CEnginePrimitiveHandle>) +0x48,
// MeshEffect (CIVCountedPointer<CEngineMeshEffect>) +0x58, ActiveIndex, PreloadAll, PrimitiveFlags; retail vectors are
// 12 bytes, so MeshEffect sits at +0x54 (the landed ClearAllChildPrimitives 0x00BC5C00 reads the vector at +0x48).
// CEnginePrimitiveHandle (PDB 8 bytes: vptr + CIVCountedPointer Data, vtable 0x0122F598) is destroyed through its
// virtual destructor (`push 0 ; call [vtbl]`). The base chain ends in CIVCountedPointeeBase (vtable 0x012354D0, an
// inline `virtual ~CIVCountedPointeeBase() {}`). engine/CEngineInternalPrimitiveBase.h is a data-only header without
// the polymorphic base, so the 0x48-byte base is modelled locally over a polymorphic CIVCountedPointeeBase.
// Callees: free @ 0x00BFEA14 (the handle array), the mesh effect's virtual slot 1 at zero references.
// Shape: no vtable store of the changer or the primitive base (`__declspec(novtable)` classes whose destructors are
// empty, GOTCHAS 24o); members die in reverse order: the counted pointer (release only when set, then nulled), the
// STLport vector (by-value `_Destroy(start, finish)` with the end in ebx, then the null-tested free); last the base's
// inline destructor stores the CIVCountedPointeeBase vtable.
extern "C" void __cdecl free(void* p);      // 0x00BFEA14
class CIVCountedPointeeBase { public:              // vtable 0x012354D0
    virtual ~CIVCountedPointeeBase() { }    // slot 0
    virtual void OnLastReference();         // slot 1 (+0x04): called when IVRefCount drops to zero
    unsigned long IVRefCount;               // +0x04
    void Release()
    {
        if (--IVRefCount == 0)
            OnLastReference();
    }
};
template <class T> class CIVCountedPointer { public:
    T* m_p;
    ~CIVCountedPointer()
    {
        if (m_p) {
            m_p->Release();
            m_p = 0;
        }
    }
};
class CEnginePersistentPrimitiveBase : public CIVCountedPointeeBase { public: };
class CEngineMeshEffect : public CIVCountedPointeeBase { public: };
class CEnginePrimitiveHandle { public:             // vtable 0x0122F598
    virtual ~CEnginePrimitiveHandle();      // slot 0
    CIVCountedPointer<CEnginePersistentPrimitiveBase> Data;   // +0x04
};
namespace std {
template <class T> inline void _Destroy(T* first, T* last)
{
    for (; first != last; ++first)
        first->~T();
}
template <class T> struct vector {          // retail STLport vector (12 bytes)
    T* _M_start;
    T* _M_finish;
    T* _M_end_of_storage;
    ~vector()
    {
        _Destroy(_M_start, _M_finish);
        if (_M_start)
            free(_M_start);
    }
};
}
template <class T> class CArray : public std::vector<T> { public: };
struct __declspec(novtable) CEngineInternalPrimitiveBase : CIVCountedPointeeBase {   // vtable 0x012A3D7C, 0x48 bytes
    unsigned char _body[0x48 - 0x08];       // EnginePrimitiveFlags .. RefLayerMask (engine/CEngineInternalPrimitiveBase.h)
};
struct __declspec(novtable) CEngineInternalPrimitiveChanger : CEngineInternalPrimitiveBase {   // vtable 0x012A662C
    CArray<CEnginePrimitiveHandle>        Primitives;       // +0x48
    CIVCountedPointer<CEngineMeshEffect>  MeshEffect;       // +0x54
    long                                  ActiveIndex;      // +0x58
    bool                                  PreloadAll;       // +0x5c
    unsigned long                         PrimitiveFlags;   // +0x60
    ~CEngineInternalPrimitiveChanger();
};

CEngineInternalPrimitiveChanger::~CEngineInternalPrimitiveChanger()
{
}