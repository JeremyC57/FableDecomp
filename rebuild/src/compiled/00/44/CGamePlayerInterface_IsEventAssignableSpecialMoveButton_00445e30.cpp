// CGamePlayerInterface::IsEventAssignableSpecialMoveButton(const CInputEvent& event) const -- does any assignable special-move button input recognise the event
// Leaf: ?IsEventAssignableSpecialMoveButton@CGamePlayerInterface@@QBE_NABVCInputEvent@@@Z
// Plan phase 5 (2026-11-05), CCountedPointer wave: the local CCountedPointer / count-block stand-ins replaced by the shared engine/CCountedPointer.h (Ego_r.pdb: CCountedPointer<T> Data +0x0 / Info +0x4; CCPPointerInfo RefCount +0x0 / DeleteFunc +0x4 / Data +0x8).
// Retail: Fable.exe @ 0x00445E30, 87 bytes, `ret 4`; reached by a tail jmp from 0x00687C73 (the CInputProcessGameBase
// forwarder); Ego_r.pdb ?IsEventAssignableSpecialMoveButton@CGamePlayerInterface@@QBE_NABVCInputEvent@@@Z.
// Body: for (unsigned i = 0; i < AssignableSpecialMoveButtons.size(); ++i) if (AssignableSpecialMoveButtons[i]->
// IsEventOfThisType(event)) (virtual slot 3, +0xc) return true; return false (size re-read every pass).
// Layout: retail CGamePlayerInterface -- AssignableSpecialMoveButtons (12-byte STLport vector<CCountedPointer<const
// CInputTypeBase> >) +0x7b8 (Ego_r.pdb +0x7bc; its vectors are 16 bytes), as the matched 0x00445E90 row.
// Shape: /O2 (16-aligned entry, `mov edi,edi` loop-head pad).
class CInputEvent;
class CInputTypeBase {
public:
    virtual ~CInputTypeBase();
    virtual void _slot01() const;
    virtual void _slot02() const;
    virtual bool IsEventOfThisType(const CInputEvent& event) const;   // slot 3 (+0xc)
};
#include "engine/CCountedPointer.h"
namespace std {
template <class T> class vector {
public:
    T* _M_start;                                         // +0x00
    T* _M_finish;                                        // +0x04
    T* _M_end_of_storage;                                // +0x08
    unsigned int size() const { return unsigned int(_M_finish - _M_start); }
    const T& operator[](unsigned int n) const { return _M_start[n]; }
};
}
class CGamePlayerInterface {
public:
    unsigned char _pad0000[0x7b8];
    std::vector<CCountedPointer<const CInputTypeBase> > AssignableSpecialMoveButtons;   // +0x7b8
    bool IsEventAssignableSpecialMoveButton(const CInputEvent& event) const;
};
bool CGamePlayerInterface::IsEventAssignableSpecialMoveButton(const CInputEvent& event) const
{
    for (unsigned long i = 0; i < AssignableSpecialMoveButtons.size(); ++i)
    {
        if (AssignableSpecialMoveButtons[i]->IsEventOfThisType(event))
            return true;
    }
    return false;
}