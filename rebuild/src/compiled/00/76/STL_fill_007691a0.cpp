// std::fill(CIVCountedPointer<CLocalDetailPlacementGrid>* first, CIVCountedPointer<CLocalDetailPlacementGrid>* last, const CIVCountedPointer<CLocalDetailPlacementGrid>& value) -- assign one intrusive counted pointer over [first, last)
// Leaf: ??$fill@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x007691A0, 66 bytes, `ret 4`.
// Caller: 0x0076BA28 (vector<CIVCountedPointer<..> > fill-assign path). No callees: the pointee's
//   "delete me" virtual (slot 1, `call [eax+4]` with the pointee in ecx) is the only call.
// Shape: STLport `fill(first, last, val)` from an /Gr algorithm TU (__fastcall: first in ecx, last in
//   edx, the value reference on the stack). The element assignment is the inlined
//   CIVCountedPointer<T>::operator=: when the pointees differ, release the old one (`dec [ecx+4]`,
//   slot-1 virtual at zero, then the unconditional `mov [esi],0` inside the non-null branch), adopt the
//   new pointee and bump its IVRefCount. The `nop` before the loop head is VC7.1's loop alignment.
// Layout: CIVCountedPointeeBase as in the landed 0x0083CE60 / 0x0084B690 (vtable +0, IVRefCount +4);
//   CIVCountedPointer<T> is the one-word { T* Object; } of Ego_r.pdb.
struct CIVCountedPointeeBase {
    virtual void Destroy(int flags);              // slot 0
    virtual void Delete();                        // slot 1
    unsigned long IVRefCount;                     // +0x04
};

class CLocalDetailPlacementGrid : public CIVCountedPointeeBase {
};

template <class T> class CIVCountedPointer {
public:
    T* Object;                                    // +0x00
    void Release()
    {
        T* object = Object;
        if (object) {
            if (--object->IVRefCount == 0)
                object->Delete();
            Object = 0;
        }
    }
    CIVCountedPointer& operator=(const CIVCountedPointer& other)
    {
        if (Object != other.Object) {
            Release();
            Object = other.Object;
            if (Object)
                ++Object->IVRefCount;
        }
        return *this;
    }
};

namespace std {

template <class _ForwardIter, class _Tp>
void __fastcall fill(_ForwardIter __first, _ForwardIter __last, const _Tp& __val)
{
    for ( ; __first != __last; ++__first)
        *__first = __val;
}

template void __fastcall fill<CIVCountedPointer<CLocalDetailPlacementGrid>*, CIVCountedPointer<CLocalDetailPlacementGrid> >(
    CIVCountedPointer<CLocalDetailPlacementGrid>*, CIVCountedPointer<CLocalDetailPlacementGrid>*,
    const CIVCountedPointer<CLocalDetailPlacementGrid>&);

}