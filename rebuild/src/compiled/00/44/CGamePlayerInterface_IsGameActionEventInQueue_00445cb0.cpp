// CGamePlayerInterface::IsGameActionEventInQueue(EGameAction action, EInputButtonEventType type, long player, float& value) const -- whether any input bound to the action (the Controls map's equal range) has a queued event; the special-move (0x36), quick-access (0x37) and context-sensitive (0x38) actions also check their button lists
// Leaf: IsGameActionEventInQueue
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00445CB0, length 368 bytes (`ret 10h`, bool in al).
// Callees: CVectorMap<EGameAction, CCountedPointer<CInputTypeBase const>>::UpperBound @ 0x00447CE0 and LowerBound @ 0x00447C80
//   (manifest owner labels propagated), CInputTypeBase virtual slot 5 (+0x14) IsEventOfThisTypeInQueue(EInputButtonEventType, long, float&).
// Layout: engine/CGamePlayerInterface.h names Controls +0x7A8, AssignableSpecialMoveButtons, QuickAccessItemButtons and
//   ContextSensitiveItemButtons, but the header carries 16-byte PDB containers: retail Controls is a 16-byte CVectorMap
//   (three pointers + comparator/dirty bytes) and the button vectors are 12-byte STLport vectors at +0x7B8 / +0x7C4 /
//   +0x7D0 (the instruction offsets), so a local model is used.
// Shape: the inline equal_range builds its pair right to left (UpperBound first, kept in the frame word); each button list
//   is walked by an unsigned index with size() recomputed per pass; every list returns false on its own.
enum EInputButtonEventType { INPUT_BUTTON_EVENT_NONE };
enum EGameAction { GAME_ACTION_SPECIAL_MOVE_BUTTON = 0x36, GAME_ACTION_QUICK_ACCESS_ITEM = 0x37, GAME_ACTION_CONTEXT_SENSITIVE_ITEM = 0x38 };
class CInputTypeBase { public:
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04();
    virtual bool IsEventOfThisTypeInQueue(EInputButtonEventType type, long player, float& value) const;   // slot 5 (+0x14)
};
struct CInputPointer {                                  // CCountedPointer<CInputTypeBase const>
    const CInputTypeBase* Data;
    void*                 Info;
};
struct SControlEntry {                                  // pair<EGameAction, CCountedPointer<CInputTypeBase const>>
    EGameAction   first;
    CInputPointer second;
};
struct SControlRange { const SControlEntry* first; const SControlEntry* second; SControlRange(const SControlEntry* a, const SControlEntry* b) : first(a), second(b) {} };
struct CControlMap {                                    // CVectorMap<EGameAction, CCountedPointer<...>, CKeyPairCompareLess<...>>
    SControlEntry* _M_start;
    SControlEntry* _M_finish;
    SControlEntry* _M_end_of_storage;
    unsigned char  Compare;
    bool           Dirty;
    unsigned char  _pad[2];
    const SControlEntry* LowerBound(const EGameAction& key) const;   // 0x00447C80
    const SControlEntry* UpperBound(const EGameAction& key) const;   // 0x00447CE0
    SControlRange EqualRange(const EGameAction& key) const { return SControlRange(LowerBound(key), UpperBound(key)); }
};
struct CInputPointerVector {
    CInputPointer* _M_start;
    CInputPointer* _M_finish;
    CInputPointer* _M_end_of_storage;
    unsigned long size() const { return _M_finish - _M_start; }
    const CInputPointer& operator[](unsigned long i) const { return _M_start[i]; }
};
class CGamePlayerInterface { public:
    unsigned char       _pad000[0x7a8];
    CControlMap         Controls;                       // +0x7A8
    CInputPointerVector AssignableSpecialMoveButtons;   // +0x7B8
    CInputPointerVector QuickAccessItemButtons;         // +0x7C4
    CInputPointerVector ContextSensitiveItemButtons;    // +0x7D0
    bool IsGameActionEventInQueue(EGameAction action, EInputButtonEventType type, long player, float& value) const;
};

bool CGamePlayerInterface::IsGameActionEventInQueue(EGameAction action, EInputButtonEventType type, long player, float& value) const
{
    SControlRange range = Controls.EqualRange(action);
    for (; range.first != range.second; ++range.first) {
        if (range.first->second.Data->IsEventOfThisTypeInQueue(type, player, value))
            return true;
    }
    if (action == GAME_ACTION_QUICK_ACCESS_ITEM) {
        for (unsigned long i = 0; i < QuickAccessItemButtons.size(); ++i)
            if (QuickAccessItemButtons[i].Data->IsEventOfThisTypeInQueue(type, player, value))
                return true;
    } else if (action == GAME_ACTION_CONTEXT_SENSITIVE_ITEM) {
        for (unsigned long i = 0; i < ContextSensitiveItemButtons.size(); ++i)
            if (ContextSensitiveItemButtons[i].Data->IsEventOfThisTypeInQueue(type, player, value))
                return true;
    } else if (action == GAME_ACTION_SPECIAL_MOVE_BUTTON) {
        for (unsigned long i = 0; i < AssignableSpecialMoveButtons.size(); ++i)
            if (AssignableSpecialMoveButtons[i].Data->IsEventOfThisTypeInQueue(type, player, value))
                return true;
    }
    return false;
}