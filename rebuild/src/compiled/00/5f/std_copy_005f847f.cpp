// std::__copy(CDefPointer<const CHeroAbilityDef>* first, CDefPointer<const CHeroAbilityDef>* last, CDefPointer<const CHeroAbilityDef>* result, const random_access_iterator_tag&, int*) -- STLport counted copy-assignment of def pointers (release the old def, add-ref the new); returns the advanced result
// Leaf: ??$__copy@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x005F847F, 71 bytes, `ret 0Ch` (/Gr TU: first in ecx, last in edx; result,
//   the const-ref iterator tag and the null Distance* on the stack).
// Callers: the `__copy_ptrs(.., __false_type)` forwarder 0x005F8C0A and vector<CDefPointer<const CHeroAbilityDef> >::
//   operator= 0x005F9F5A (both copy loops), CTCInventoryAbilities TU.
// Label correction: the catalog `_Move<CDefPointer<CHeroAbilityDef const>*,...>` is a propagated Dinkumware spelling; the
//   body is STLport `__copy(first, last, result, random_access_iterator_tag, _Distance*)` over CDefPointer elements.
// Shape: size TU (`and dword ptr [esi],0` zero store, `#pragma optimize("s", on)`); CDefPointer::operator= is inlined
//   (`__forceinline`, a size TU otherwise calls it): compare, release the old def through its vtable slot 1 when the
//   count drops to zero (`dec [ecx+4] ; jne ; call [eax+4]`), clear, copy, add-ref (`inc dword ptr [eax+4]`).
// Layout: CDefPointer = one pointer to a CDefPointeeBase (vptr +0, reference count +4), as the landed pair copy
//   constructor 0x005F8578 of this TU.
#pragma optimize("s", on)

struct CDefPointeeBase {                    // counted def object: vptr, reference count at +4
    virtual void v00();
    virtual void Destroy() const;           // slot 1
    mutable long m_refCount;                // +0x04 (mutable: const defs are reference counted too)
    void Release() const { if (--m_refCount == 0) Destroy(); }
};

struct CHeroAbilityDef : CDefPointeeBase { };

template <class T> struct CDefPointer {
    T* Object;                              // +0x00
    __forceinline CDefPointer& operator=(const CDefPointer& other)
    {
        if (Object != other.Object) {
            if (Object) {
                Object->Release();
                Object = 0;
            }
            Object = other.Object;
            if (Object)
                ++Object->m_refCount;
        }
        return *this;
    }
};

namespace std {
struct random_access_iterator_tag { random_access_iterator_tag() { } };

template <class RandomAccessIter, class OutputIter, class Distance>
OutputIter __fastcall __copy(RandomAccessIter first, RandomAccessIter last, OutputIter result,
                             const random_access_iterator_tag&, Distance*)
{
    for (Distance n = last - first; n > 0; --n) {
        *result = *first;
        ++first;
        ++result;
    }
    return result;
}

typedef CDefPointer<const CHeroAbilityDef> TAbilityDefPointer;
template TAbilityDefPointer* __fastcall __copy<TAbilityDefPointer*, TAbilityDefPointer*, int>(
    TAbilityDefPointer*, TAbilityDefPointer*, TAbilityDefPointer*, const random_access_iterator_tag&, int*);
}