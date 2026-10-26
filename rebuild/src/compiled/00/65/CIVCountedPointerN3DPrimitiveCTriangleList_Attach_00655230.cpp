// CIVCountedPointer<N3DPrimitive::CTriangleList>::Attach(N3DPrimitive::CTriangleList* p) -- releases the held object, then takes p without adding a reference
// Leaf: Attach@?$CIVCountedPointer
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00655230, 32 bytes, `ret 4`; no code references (an unreferenced
// instantiation). Byte-identical to the landed CIVCountedPointer<T>::Attach 0x0042FC66 and to 0x00CD23DA; this model is the
// landed one. The catalog label std::_Dest_val<allocator<CIVCountedPointer<...>>> is propagated: _Dest_val takes the
// allocator and pointer as arguments, this is a thiscall on the pointer object itself (the pointee type is not
// recoverable from the bytes; CTriangleList is the label's). Body: `if (Object) { Object->Release(); Object = 0; }`
// (IVRefCount +4, slot 1 on zero) then `Object = p` with no add-ref. The name Attach is behavioural (no Ego_r.pdb match).
// Opt: size TU (`and dword ptr [esi],0`) -- `#pragma optimize("s", on)`.
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