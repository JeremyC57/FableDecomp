// CGamePlayerInterface::IsQuickAccessEventInQueue(long index, EInputButtonEventType button_event_type, long player) -- is the indexed QuickAccessItemButtons input waiting in the queue
// Leaf: ?IsQuickAccessEventInQueue@CGamePlayerInterface@@QBE_NJW4EInputButtonEventType@@J@Z
// Retail: Fable.exe @ 0x004460C0, 72 bytes, `ret 0Ch`; Ego_r.pdb ?IsQuickAccessEventInQueue@CGamePlayerInterface@@QBE_NJW4EInputButtonEventType@@J@Z;
// catalog label IsAssignableSpecialMoveButtonInQueue is wrong (it reads QuickAccessItemButtons).
// Body: index in range (`jl` on the sign, then unsigned `jae` against size()) -> a zeroed float local (stored in the dead
// index slot) and QuickAccessItemButtons[index].Data->IsEventOfThisTypeInQueue(button_event_type, player, value) (virtual slot 5,
// +0x14); else false.
// Layout: retail CGamePlayerInterface -- QuickAccessItemButtons (12-byte STLport vector<CCountedPointer<const CInputTypeBase>>) +0x7c4
// (Ego_r.pdb +0x7cc; its vectors are 16 bytes); CCountedPointer {Data, Info} 8 bytes. Modelled locally: only that member is read.
// Shape: /O2 (16-aligned entry); the element pointer bound to a local before the float, so the zero store follows the pushes.
enum EInputButtonEventType { INPUT_BUTTON_EVENT_TYPE_NONE };
class CInputTypeBase {
public:
    virtual ~CInputTypeBase();
    virtual void _slot01() const; virtual void _slot02() const; virtual void _slot03() const; virtual void _slot04() const;
    virtual bool IsEventOfThisTypeInQueue(EInputButtonEventType button_event_type, long player, float& value) const;   // +0x14
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
    unsigned char _pad0000[0x7c4];
    std::vector<CCountedPointer<const CInputTypeBase> > QuickAccessItemButtons;   // +0x7c4
    bool IsQuickAccessEventInQueue(long index, EInputButtonEventType button_event_type, long player) const;
};
bool CGamePlayerInterface::IsQuickAccessEventInQueue(long index, EInputButtonEventType button_event_type, long player) const
{
    if (index >= 0 && (unsigned long)index < QuickAccessItemButtons.size())
    {
        const CInputTypeBase* input = QuickAccessItemButtons[index].Data;
        float value = 0.0f;
        return input->IsEventOfThisTypeInQueue(button_event_type, player, value);
    }
    return false;
}