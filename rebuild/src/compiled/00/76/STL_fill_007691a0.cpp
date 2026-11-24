// std::fill(CDefPointer<const CThingBaseDef>* first, CDefPointer<const CThingBaseDef>* last, const CDefPointer<const CThingBaseDef>& value) -- assign one counted def handle over [first, last)
// Leaf: ??$fill@PAV?$CDefPointer@$$CBVCThingBaseDef@@@@V1@@std@@YIXPAV?$CDefPointer@$$CBVCThingBaseDef@@@@0ABV1@@Z
// Name fix (NF382): was the fill<CIVCountedPointer<CLocalDetailPlacementGrid>*> instance (FableWin: Engine_Local_Detail_Generator.obj,
//   not this caller). The only call site 0x0076BA28 is in the landed vector<CDefPointer<const CThingBaseDef> >::_M_fill_insert
//   0x0076B950 (reached from that vector's resize 0x0076BF80), and FableWin vector<CDefPointer<const CThingBaseDef> >::_Insert_n
//   calls fill<CDefPointer<const CThingBaseDef>*> (tc_object_augmentations.obj). Same bytes: the handle is one word on a counted
//   pointee (count at +4, slot-1 release), exactly the assignment below.
// Retail: Fable.exe (TLC, VC7.1) function @ 0x007691A0, 66 bytes, `ret 4`.
// Caller: 0x0076BA28 (vector<CDefPointer<const CThingBaseDef> > fill-insert path). No callees: the pointee's
//   "delete me" virtual (slot 1, `call [eax+4]` with the pointee in ecx) is the only call.
// Shape: STLport `fill(first, last, val)` from an /Gr algorithm TU (__fastcall: first in ecx, last in
//   edx, the value reference on the stack). The element assignment is the inlined
//   CDefPointer<T>::operator=: when the pointees differ, release the old one (`dec [ecx+4]`,
//   slot-1 virtual at zero, then the unconditional `mov [esi],0` inside the non-null branch), adopt the
//   new pointee and bump its IVRefCount. The `nop` before the loop head is VC7.1's loop alignment.
// Layout: CDefPointeeBase (vtable +0, count +4); CDefPointer<T> is the one-word { CDefPointeeBase* Object; } of Ego_r.pdb.
class CDefPointeeBase { public:                   // counted def object (engine/CDefPointer.h layout)
    virtual void Destroy(int flags);              // slot 0
    virtual void Delete();                        // slot 1
    unsigned long RefCount;                       // +0x04
};

class CThingBaseDef;

template <class T> class CDefPointer {
public:
    CDefPointeeBase* Object;                      // +0x00 (Ego_r.pdb)
    void Release()
    {
        CDefPointeeBase* object = Object;
        if (object) {
            if (--object->RefCount == 0)
                object->Delete();
            Object = 0;
        }
    }
    CDefPointer& operator=(const CDefPointer& other)
    {
        if (Object != other.Object) {
            Release();
            Object = other.Object;
            if (Object)
                ++Object->RefCount;
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

template void __fastcall fill<CDefPointer<const CThingBaseDef>*, CDefPointer<const CThingBaseDef> >(
    CDefPointer<const CThingBaseDef>*, CDefPointer<const CThingBaseDef>*,
    const CDefPointer<const CThingBaseDef>&);

}