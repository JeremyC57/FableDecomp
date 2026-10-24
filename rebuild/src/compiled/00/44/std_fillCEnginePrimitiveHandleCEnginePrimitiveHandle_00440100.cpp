// std::fill<CEnginePrimitiveHandle*, CEnginePrimitiveHandle>(CEnginePrimitiveHandle*, CEnginePrimitiveHandle*, const CEnginePrimitiveHandle&) -- assign one primitive handle to every element of a range
// Leaf: fill
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00440100, length 128 bytes (113 + int3 padding), `ret 4` (__fastcall: first in ecx,
// last in edx, &value on the stack), void.
// Name: STLport 4.x `fill(first, last, val)` -- `for (; first != last; ++first) *first = val;`; single caller 0x00442434. The
// catalog's "_Move<CEnginePrimitiveHandle*,CEnginePrimitiveHandle*>" is a label (three different algorithms carry it).
// Callees: the primitive's virtual slots 3 (+0x0C, OnLastHandleReleased(handle) when the target holds the last handle) and 1
// (+0x04, Destroy on the last reference) -- all through CEnginePrimitiveHandle::operator=, inlined.
// Element: CEnginePrimitiveHandle (Ego_r.pdb, 8 bytes: vptr + CIVCountedPointer<CEnginePersistentPrimitiveBase> Data +4) with the
// counted-pointer model of NParticleEngine::CPSCLight::Kill 0x00B19C20 and the pointee's NotifyIfLastHandle of the landed handle
// dtor 0x0041C500. Clear() is `if (Data.Object) Data.Object->NotifyIfLastHandle(this); Data = 0;` -- the pointee member keeps the
// pointer in ecx (`mov ecx,[esi+4] ; cmp ecx,edi ; je ; cmp [ecx+4],1`); the null-safe `GetRefCount() == 1` and `IsUnique()`
// forms load it into eax and add a `mov ecx,eax` before the call (2 bytes long, graded); `Data = 0` = the counted release plus
// the second, unconditional zero store. operator= is `Clear(); Data = other.Data;` -- the
// counted-pointer copy assignment re-tests the (known-null) target against the source and re-reads the source before the store
// and add-ref. Fable builds its templates __fastcall (explicit here: the lane default is __cdecl).
// Shape: /O2; `edi` holds zero for the whole loop, the value's address in ebx.
// Model: local -- engine/CEngineInternalPrimitiveBase.h exists, but the generated header is data-only (pack(1), no member
// functions, no virtuals: the vptr and counted pointers are flattened into raw words); the handle assignment calls the pointee's
// virtual slots 1 and 3, so the pointee is local (the landed 0x0041C500 / 0x00B19C20 model).
struct CBaseClass {
    virtual void v00();
    ~CBaseClass();                                       // 0x0099A2E0
};
class CEnginePrimitiveHandle;
struct CEngineInternalPrimitiveBase {                    // the referenced primitive: handle count at +4
    virtual void v00();
    virtual void Destroy();                              // slot 1 (+0x04)
    virtual void v02();
    virtual void OnLastHandleReleased(CEnginePrimitiveHandle* handle);   // slot 3 (+0x0C)
    long m_handleCount;                                  // +0x04
    void NotifyIfLastHandle(CEnginePrimitiveHandle* handle) { if (m_handleCount == 1) OnLastHandleReleased(handle); }
};
struct CIVCountedPointer {                               // CIVCountedPointer<CEnginePersistentPrimitiveBase> (landed 0x00B19C20 model)
    CEngineInternalPrimitiveBase* Object;
    __forceinline void Release()
    {
        CEngineInternalPrimitiveBase* object = Object;
        if (object) {
            if (--object->m_handleCount == 0)
                object->Destroy();
            Object = 0;
        }
    }
    __forceinline CIVCountedPointer& operator=(CEngineInternalPrimitiveBase* object)
    {
        Release();
        Object = object;
        if (object)
            ++object->m_handleCount;
        return *this;
    }
    __forceinline CIVCountedPointer& operator=(const CIVCountedPointer& other)
    {
        if (Object != other.Object) {
            Release();
            Object = other.Object;
            if (Object)
                ++Object->m_handleCount;
        }
        return *this;
    }
};
class CEnginePrimitiveHandle : public CBaseClass {       // vtable 0x0122F598, 8 bytes
public:
    CIVCountedPointer Data;                              // +0x04
    virtual void v00();
    __forceinline void Clear()
    {
        if (Data.Object)
            Data.Object->NotifyIfLastHandle(this);
        Data = 0;
    }
    __forceinline CEnginePrimitiveHandle& operator=(const CEnginePrimitiveHandle& other)
    {
        Clear();
        Data = other.Data;
        return *this;
    }
};
namespace std {
template <class ForwardIter, class T>
void __fastcall fill(ForwardIter first, ForwardIter last, const T& val)
{
    for (; first != last; ++first)
        *first = val;
}
template void __fastcall fill<CEnginePrimitiveHandle*, CEnginePrimitiveHandle>(CEnginePrimitiveHandle*, CEnginePrimitiveHandle*,
                                                                              const CEnginePrimitiveHandle&);
}