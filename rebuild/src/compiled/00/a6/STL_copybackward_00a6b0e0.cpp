// _STL::__copy_backward(CIVCountedPointer<CLocalDetailPlacementGrid>* first, CIVCountedPointer<CLocalDetailPlacementGrid>* last, CIVCountedPointer<CLocalDetailPlacementGrid>* result, const random_access_iterator_tag&, int*) -- copy [first, last) backwards onto the range ending at result, counted-pointer assignment inlined
// Leaf: ??$__copy_backward@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A6B0E0, 88 bytes, `ret 0Ch` (link-time __fastcall: first in ecx, last
//   in edx; result, the tag reference and the null distance pointer on the stack). No direct callees (the pointee's
//   slot-1 virtual release).
// Callers: 0x00A6B39C and 0x00A6BD49 -- the vector<CIVCountedPointer<CLocalDetailPlacementGrid> > insert paths of the
//   CLocalDetailPlacementGrid TU (0x00A6BCD0 "Vector_InsertN_DetailPlacementGrid", next to the catalogued
//   _Fill<CIVCountedPointer<CLocalDetailPlacementGrid>*> 0x00A6B000 and the landed _Uninit_fill_n 0x00A6B340).
// Label: the manifest's `_Copy_backward<CDefPointer<CThingBaseDef const>*,...>` is propagated from the byte-identical
//   landed 0x00769210 (CDefPointer<const CThingBaseDef>, same one-word counted pointer over a vptr + count-at-+4
//   pointee); this instance's callers sort/insert the detail-placement-grid pointers.
// Shape: STLport `__copy_backward(first, last, result, random_access_iterator_tag, _Distance*)`: `for (n = last -
//   first; n > 0; --n) *--result = *--last; return result;` -- the count in ebx, an empty range returns result
//   straight from its slot. The element assignment is the inlined CIVCountedPointer::operator= (self test; old pointee
//   released through its slot-1 virtual at count zero and nulled inside the non-null branch; new pointee adopted and
//   add-ref'd). The pointee is modelled locally (engine/CIVCountedPointeeBase.h carries the vptr as a data member, so
//   the slot-1 call cannot be spelled through it); its layout (vptr +0, IVRefCount +4) is the header's.
class CIVCountedPointeeBase {
public:
    virtual void Destroy(int flags);              // slot 0
    virtual void Delete();                        // slot 1 (+0x04): destroy at count zero
    unsigned long IVRefCount;                     // +0x04
    void RemoveRef()
    {
        if (--IVRefCount == 0)
            Delete();
    }
};

class CLocalDetailPlacementGrid : public CIVCountedPointeeBase {
};

template <class T> class CIVCountedPointer {
public:
    T* Object;                                    // +0x00
    CIVCountedPointer& operator=(const CIVCountedPointer& other)
    {
        if (Object != other.Object) {
            if (Object) {
                Object->RemoveRef();
                Object = 0;
            }
            Object = other.Object;
            if (Object)
                ++Object->IVRefCount;
        }
        return *this;
    }
};

namespace _STL {

class random_access_iterator_tag { public: random_access_iterator_tag() { } };

template <class _RandomAccessIter, class _BidirectionalIter, class _Distance>
_BidirectionalIter __fastcall __copy_backward(_RandomAccessIter __first, _RandomAccessIter __last,
                                              _BidirectionalIter __result, const random_access_iterator_tag&, _Distance*)
{
    for (_Distance __n = __last - __first; __n > 0; --__n)
        *--__result = *--__last;
    return __result;
}

typedef CIVCountedPointer<CLocalDetailPlacementGrid> TGridPtr;
template TGridPtr* __fastcall __copy_backward<TGridPtr*, TGridPtr*, int>(TGridPtr*, TGridPtr*, TGridPtr*,
                                                                         const random_access_iterator_tag&, int*);

}