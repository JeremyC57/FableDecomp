// CGamePlayerInterface::IsEventQuickAccessItemButton(const CInputEvent& event, long& index) const -- is the event one of the quick-access item buttons? index = which
// Leaf: ?IsEventQuickAccessItemButton@CGamePlayerInterface@@QBE_NABVCInputEvent@@AAJ@Z
// Retail: Fable.exe @ 0x00446000, 93 bytes, `ret 8`; Ego_r.pdb ?IsEventQuickAccessItemButton@CGamePlayerInterface@@QBE_NABVCInputEvent@@AAJ@Z.
// Catalog label IsEventContextSensitiveItemButton is wrong: this body walks the vector at +0x7c4 (QuickAccessItemButtons,
// the one after AssignableSpecialMoveButtons +0x7b8); the +0x7d0 twin (ContextSensitiveItemButtons) is 0x00446110, and the
// Ego_r.pdb order (QuickAccess 0x00442F70 before ContextSensitive 0x00443090) agrees.
// Body: for (unsigned i = 0; i < QuickAccessItemButtons.size(); ++i) if (QuickAccessItemButtons[i]->IsEventOfThisType(event))
// (virtual slot 3, +0xc) { index = i; return true; } return false (size re-read every pass).
// Layout: retail CGamePlayerInterface -- AssignableSpecialMoveButtons +0x7b8, QuickAccessItemButtons +0x7c4 (12-byte STLport
// vectors of CCountedPointer<const CInputTypeBase>; Ego_r.pdb +0x7bc / +0x7cc with 16-byte vectors), as 0x00445FA0.
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
    std::vector<CCountedPointer<const CInputTypeBase> > QuickAccessItemButtons;         // +0x7c4
    bool IsEventQuickAccessItemButton(const CInputEvent& event, long& index) const;
};
bool CGamePlayerInterface::IsEventQuickAccessItemButton(const CInputEvent& event, long& index) const
{
    for (unsigned long i = 0; i < QuickAccessItemButtons.size(); ++i)
    {
        if (QuickAccessItemButtons[i]->IsEventOfThisType(event))
        {
            index = i;
            return true;
        }
    }
    return false;
}