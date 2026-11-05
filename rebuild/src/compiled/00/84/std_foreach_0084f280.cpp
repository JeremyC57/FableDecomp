// std::for_each(const CCountedPointer<NShape::CShape>* first, const CCountedPointer<NShape::CShape>* last, NShape::CCallShapeSlot1 f) -- calls virtual slot 1 on every shape through a by-value counted-pointer copy and returns the empty functor
// Leaf: for_each
// Plan phase 5 (2026-11-05), CCountedPointer wave: the local CCountedPointer / count-block stand-ins replaced by the shared engine/CCountedPointer.h (Ego_r.pdb: CCountedPointer<T> Data +0x0 / Info +0x4; CCPPointerInfo RefCount +0x0 / DeleteFunc +0x4 / Data +0x8). Also: the local CCountedPointer members m_pObject->Data, m_pInfo->Info are the header CCountedPointer's (same offsets).
// Retail: Fable.exe (TLC, VC7.1) @ 0x0084F280, 72 bytes, `ret 4`. Caller: CFirstPerson::OnDie 0x0084E097. Callees: the shape's
// virtual slot 1 (+0x04), the counted pointer's release function (info +0x04, called with info +0x08) and operator delete @ 0x00BFE9BC.
// The catalog calls the functor CIsShapeTrack; the shape is a for_each that drops the slot-1 result (a predicate used with for_each).
// Uses the SGI for_each with first in ecx and last in edx (no hidden pointer: an empty POD functor comes back in al).
// Shape: operator() takes CCountedPointer BY VALUE: the inline copy ctor (`test esi,esi ; je ; inc [esi]`) and dtor
// (`dec [esi] ; jne`, release + delete) wrap the virtual call.
void __cdecl operator delete(void* p);           // 0x00BFE9BC
struct SCountedPointerInfo {
    long m_count;                                 // +0x00
    void (__fastcall *m_pfnRelease)(void* owner); // +0x04
    void* m_pOwner;                               // +0x08
};
#include "engine/CCountedPointer.h"
namespace std {
template <class InputIter, class Function>
Function __fastcall for_each(InputIter first, InputIter last, Function f)
{
    for ( ; first != last; ++first)
        f(*first);
    return f;
}
}
namespace NShape {
class CShape {
public:
    virtual ~CShape();
    virtual bool IsTrack() const;                 // slot 1 (+0x04)
};
struct CCallShapeSlot1 {
    bool operator()(CCountedPointer<CShape> shape) const { return shape->IsTrack(); }
};
}
template NShape::CCallShapeSlot1 __fastcall std::for_each(const CCountedPointer<NShape::CShape>*, const CCountedPointer<NShape::CShape>*, NShape::CCallShapeSlot1);