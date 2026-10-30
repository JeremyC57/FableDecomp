// std::__copy_backward<CEnginePrimitiveHandle*, CEnginePrimitiveHandle*, int>(first, last, result, const random_access_iterator_tag&, int*) -- assign a range of primitive handles backward onto the range ending at result, return its start
// Leaf: __copy_backward
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00440390, length 137 bytes, `ret 0xc` (__fastcall: first in ecx, last in edx; result,
// the tag reference and the distance pointer on the stack), returns result - n.
// Name: STLport 4.x `__copy_backward(first, last, result, random_access_iterator_tag, _Distance*)` -- `for (n = last - first;
// n > 0; --n) *--result = *--last; return result;`; callers 0x00440ABC and 0x004423D5 (vector<CEnginePrimitiveHandle> insert).
// The catalog's "_Move<...>" is a label.
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
// Shape: /O2; the source walks as `edi = &last->Data` (`lea edi,[edx+4]`), both pointers pre-decremented by 8 each pass (the target
// handle's Data read at [esi-4] before `sub esi,8`), the 6-byte `lea ebx,[ebx]` loop-alignment pad, zero in ebx.
// Model: local -- engine/CEngineInternalPrimitiveBase.h exists, but the generated header is data-only (pack(1), no member
// functions, no virtuals: the vptr and counted pointers are flattened into raw words); the handle assignment calls the pointee's
// virtual slots 1 and 3, so the pointee is local (the landed 0x0041C500 / 0x00B19C20 model).
#include "engine/CBaseClass.h"                         // CBaseClass() 0x0099A2D0, ~CBaseClass 0x0099A2E0 (slot 0), implicit copy
class CEnginePrimitiveHandle;
class CEngineInternalPrimitiveBase { public:                    // the referenced primitive: handle count at +4
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
struct random_access_iterator_tag {};
template <class RandomAccessIter, class BidirectionalIter, class Distance>
BidirectionalIter __fastcall __copy_backward(RandomAccessIter first, RandomAccessIter last, BidirectionalIter result,
                                             const random_access_iterator_tag&, Distance*)
{
    for (Distance n = last - first; n > 0; --n)
        *--result = *--last;
    return result;
}
template CEnginePrimitiveHandle* __fastcall __copy_backward<CEnginePrimitiveHandle*, CEnginePrimitiveHandle*, int>(
    CEnginePrimitiveHandle*, CEnginePrimitiveHandle*, CEnginePrimitiveHandle*, const random_access_iterator_tag&, int*);
}