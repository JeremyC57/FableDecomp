// CGamePlayerInterface::IsOtherQuickAccessItemButtonHeld(long index, long player) const -- held-in-queue answer of the first quick-access button other than index
// Leaf: ?IsOtherQuickAccessItemButtonHeld@CGamePlayerInterface@@QBE_NJJ@Z
// Retail: Fable.exe @ 0x00446060, 94 bytes, `ret 8`; Ego_r.pdb ?IsOtherQuickAccessItemButtonHeld@CGamePlayerInterface@@QBE_NJJ@Z.
// Body: for i over QuickAccessItemButtons (+0x7c4, size hoisted after the empty test): the first i != index returns
// QuickAccessItemButtons[i]->IsEventOfThisTypeInQueue(2, player, value) (virtual slot 5, +0x14; `value` a 0.0f float
// packed into the dead `index` slot) -- the loop never looks past that first other button; none -> false.
// Layout: retail CGamePlayerInterface QuickAccessItemButtons +0x7c4, as IsEventQuickAccessItemButton 0x00446000.
// Shape: /O2 (16-aligned entry). The button pointer is bound before `value` is declared: declaring value first schedules
// its zero store ahead of the argument pushes (retail stores it after pushing &value and player).
enum EXboxControllerButton { XBOX_CONTROLLER_BUTTON_NONE };
enum EMouseButtonControl { MOUSE_BUTTON_CONTROL_NONE };
enum EInputButtonEventType { INPUT_BUTTON_EVENT_TYPE_HELD = 2 };   // value as pushed (name not confirmed)
class CInputEvent;
class CInputTypeBase {
public:
    virtual ~CInputTypeBase();
    virtual void _slot01() const;                        // GetType
    virtual void _slot02() const;                        // GetControlDirection
    virtual bool IsEventOfThisType(const CInputEvent& event) const;                    // slot 3 (+0xc)
    virtual void _slot04() const;
    virtual bool IsEventOfThisTypeInQueue(EInputButtonEventType type, long player, float& value) const;   // slot 5 (+0x14)
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
    bool IsOtherQuickAccessItemButtonHeld(long index, long player) const;
};
bool CGamePlayerInterface::IsOtherQuickAccessItemButtonHeld(long index, long player) const
{
    for (unsigned long i = 0; i < QuickAccessItemButtons.size(); ++i)
    {
        if (i != index)
        {
            const CInputTypeBase* button = QuickAccessItemButtons[i].Data;
            float value = 0.0f;
            return button->IsEventOfThisTypeInQueue(INPUT_BUTTON_EVENT_TYPE_HELD, player, value);
        }
    }
    return false;
}