// CDefPointer<const CUnprovenDef_006551ff>::operator=(const CDefPointer& other) -- counted def pointer assignment: when the pointees differ, release the old def (Destroy at zero) and clear, then take other's def and add a reference; return *this
// Leaf: ??4?$CDefPointer@$$CBVCUnprovenDef_006551ff@@@@QAEAAV0@ABV0@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x006551ff, 49 bytes, thiscall, `ret 4`, returns this; size-optimised TU (unaligned address).
// Identity (descriptive): no call, jump or data reference reaches 0x006551ff (an unreferenced COMDAT the retail link kept), so
//   the def type is unprovable: CUnprovenDef_006551ff. The shape is Ego_r.pdb CDefPointer<T>'s assignment (Object +0, the
//   pointee's count at +4, Destroy slot 1); the manifest label CEngineInternalPrimitiveMeshBase::GetMeshEffect is propagated.
// Callees: the old def's Destroy() (vtable slot 1).
// Shape (family L015): `#pragma optimize("s", on)`; inline Release()/AddRef() on the pointee (`dec [ecx+4] ; jne`,
//   `inc [eax+4]`), `Object = 0` as `and [esi],0`.
#pragma optimize("s", on)
class CDefPointeeBase {                                    // vptr; reference count at +4
public:
    virtual ~CDefPointeeBase();
    virtual void Destroy() const;                         // slot 1
    mutable long RefCount;                                // +0x04
    __forceinline void AddRef() const { ++RefCount; }
    __forceinline void Release() const
    {
        if (--RefCount == 0)
            Destroy();
    }
};
class CUnprovenDef_006551ff;                                   // descriptive: the def type is unproven
template <class T> class CDefPointer {                     // Ego_r.pdb, 4 bytes
public:
    const CDefPointeeBase* Object;                        // +0x00
    __forceinline void Clear()
    {
        if (Object) {
            Object->Release();
            Object = 0;
        }
    }
    CDefPointer& operator=(const CDefPointer& other)
    {
        if (Object != other.Object) {
            Clear();
            Object = other.Object;
            if (Object)
                Object->AddRef();
        }
        return *this;
    }
};
template class CDefPointer<const CUnprovenDef_006551ff>;