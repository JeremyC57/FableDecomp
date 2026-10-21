// CGamePlayerInterface::GetAssignableSpecialMoveIndexForMouseButton(EMouseButtonControl button) const -- index of the first assignable special-move button input for which IsInputTypeWithMouseButton holds, else -1
// Leaf: ?GetAssignableSpecialMoveIndexForMouseButton@CGamePlayerInterface@@QBEJW4EMouseButtonControl@@@Z
// Retail: Fable.exe @ 0x00445F40, 88 bytes, `ret 4`; reached by a tail jmp from 0x00657215; Ego_r.pdb
// ?GetAssignableSpecialMoveIndexForMouseButton@CGamePlayerInterface@@QBEJW4EMouseButtonControl@@@Z; catalog label (…ForButton) is wrong: the slot-7 test is IsInputTypeWithMouseButton (CInputTypeMouseButtonEvent vtable 0x012318C8: slot 6 0x00447A80 is the shared `return false`, slot 7 0x00447A90 its mouse-button test).
// Body: for (unsigned i = 0; i < AssignableSpecialMoveButtons.size(); ++i) if (AssignableSpecialMoveButtons[i]->IsInputTypeWithMouseButton(button))
// (virtual slot 7, +0x1c) return i; return -1 (size re-read every pass). One of three byte siblings 0x00445EE0 / 0x00445F40 /
// 0x00445FA0 that differ only in the virtual slot (6 / 7 / 3).
// Layout: retail CGamePlayerInterface -- AssignableSpecialMoveButtons (12-byte STLport vector<CCountedPointer<const
// CInputTypeBase> >) +0x7b8 (Ego_r.pdb +0x7bc; its vectors are 16 bytes), as the matched 0x00445E90 / 0x00445E30 rows.
// CInputTypeBase slots from the PDB virtual names and the retail vtables.
// Shape: /O2 (16-aligned entry, `mov edi,edi` loop-head pad).
enum EXboxControllerButton { XBOX_CONTROLLER_BUTTON_NONE };
enum EMouseButtonControl { MOUSE_BUTTON_CONTROL_NONE };
class CInputEvent;
class CInputTypeBase {
public:
    virtual ~CInputTypeBase();
    virtual void _slot01() const;                        // GetType
    virtual void _slot02() const;                        // GetControlDirection
    virtual bool IsEventOfThisType(const CInputEvent& event) const;                    // slot 3 (+0xc)
    virtual void _slot04() const;
    virtual void _slot05() const;                        // IsEventOfThisTypeInQueue
    virtual bool IsInputTypeWithButton(EXboxControllerButton button) const;            // slot 6 (+0x18)
    virtual bool IsInputTypeWithMouseButton(EMouseButtonControl button) const;         // slot 7 (+0x1c)
};
template <class T> class CCountedPointer {
public:
    T*    Data;                                          // +0x00
    void* Info;                                          // +0x04
    T* operator->() const { return Data; }
};
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
    long GetAssignableSpecialMoveIndexForMouseButton(EMouseButtonControl button) const;
};
long CGamePlayerInterface::GetAssignableSpecialMoveIndexForMouseButton(EMouseButtonControl button) const
{
    for (unsigned long i = 0; i < AssignableSpecialMoveButtons.size(); ++i)
    {
        if (AssignableSpecialMoveButtons[i]->IsInputTypeWithMouseButton(button))
            return i;
    }
    return -1;
}