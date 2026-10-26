// CIVCountedPointer<T>::Attach(T* p) -- releases the held object, then takes p without adding a reference
// Leaf: Attach@?$CIVCountedPointer
// Retail: Fable.exe @ 0x0042FC66, 32 bytes, `ret 4`; no code references (unreferenced instantiation). Catalog label
// std::_Dest_val<allocator<CIVCountedPointer<N3DPrimitive::CTriangleList>>, ...> is wrong: _Dest_val takes the allocator and
// pointer as arguments, this is a thiscall on the pointer object itself. Body: `if (Object) { Object->Release(); Object = 0; }`
// (IVRefCount +4, slot 1 on zero) then `Object = p` with no add-ref. The name Attach is behavioural (no Ego_r.pdb match).
// Opt: size TU (`and dword ptr [esi],0`).
#pragma optimize("s", on)
class CIVCountedPointeeBase { public:
    virtual ~CIVCountedPointeeBase();
    virtual void DeleteThis();                           // slot 1 (+4)
    unsigned long IVRefCount;                            // +0x04
    __forceinline void Release() { if (--IVRefCount == 0) DeleteThis(); }
};
namespace N3DPrimitive { class CTriangleList : public CIVCountedPointeeBase { }; }
template <class T> class CIVCountedPointer {
public:
    T* Object;
    void Attach(T* p)
    {
        if (Object) {
            Object->Release();
            Object = 0;
        }
        Object = p;
    }
};
template class CIVCountedPointer<N3DPrimitive::CTriangleList>;