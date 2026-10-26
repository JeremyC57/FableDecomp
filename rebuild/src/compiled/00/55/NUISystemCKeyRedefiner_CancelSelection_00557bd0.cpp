// NUISystem::CKeyRedefiner::CancelSelection() -- when this redefiner is the selected one (0x013B8AC8) clear the selection, refresh the key text (0x00557A10), set the clickable base's state 0x21 (slot 4) and mode 0x19 (slot 3), and remove this control as the UI manager's exclusive observer (CManager::GetInstance 0x0041E5F2, CObservable slot 4 RemoveExclusiveObserver)
// Leaf: ?CancelSelection@CKeyRedefiner@NUISystem@@QAEXXZ
// Flags: /O2 /Oy /W3
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00557BD0, length 64 bytes.
// Callees: UpdateKeyText @ 0x00557A10, NUISystem::CManager::GetInstance @ 0x0041E5F2 (`__fastcall`, no arguments; the
// landed source NUISystemCManager_GetInstance_0041e5f2.cpp -- its instance stores vtable 0x01230134 = RTTI NUISystem::CManager,
// a CObservable), virtual slots of the control base at +4 and of the manager.
// Label corrections (chunk 345 report): the earlier landing called 0x0041E5F2 "CFrontEndManager::GetInstance" and the
// manager's slot 4 "SetFocusControl"; vtable 0x01230134 is NUISystem::CManager : CObservable, whose slot 2 is AddObserver
// 0x0055CA40, slot 3 AddExclusiveObserver 0x0055C930 and slot 4 RemoveExclusiveObserver 0x0055C940 (Ego_r.pdb
// ?RemoveExclusiveObserver@CObservable@NUISystem@@UAEXPAVCObserver@2@@Z; the same naming as the landed sibling
// NUISystemCKeyRedefiner_SetAction_005586a0.cpp). Bytes unchanged.
// Shape: `this` in esi compared against the global, then esi advanced to the control base (+4) for the three calls, `ret`.
namespace NUISystem {
class CObserver;
struct CClickableBase_00557bd0 {                 // the second base (vptr at +4): the clickable / observer side
    virtual void v000();
    virtual void v001();
    virtual void v002();
    virtual void SetMode(long mode);                // slot 3 (+0x0c)
    virtual void SetState(long state);              // slot 4 (+0x10)
};
struct CObservable {                              // NUISystem::CObservable vtable layout
    virtual void v000();
    virtual void v001();
    virtual void AddObserver(CObserver* observer);                 // slot 2 (0x0055CA40 on CManager)
    virtual void AddExclusiveObserver(CObserver* observer);        // slot 3 (0x0055C930)
    virtual void RemoveExclusiveObserver(CObserver* observer);     // slot 4 (0x0055C940)
};
class CManager : public CObservable { public:                  // vtable 0x01230134
    static CManager* __fastcall GetInstance();    // 0x0041E5F2
};
struct CKeyRedefinerRoot_00557bd0 {               // the first base (vptr at +0)
    virtual void v00();
};
class CKeyRedefiner : public CKeyRedefinerRoot_00557bd0, public CClickableBase_00557bd0 { public:
    void UpdateKeyText();                          // 0x00557A10
    void CancelSelection();
};
extern CKeyRedefiner* g_pSelectedKeyRedefiner;     // 0x013B8AC8

void CKeyRedefiner::CancelSelection()
{
    if (g_pSelectedKeyRedefiner == this) {
        g_pSelectedKeyRedefiner = 0;
        UpdateKeyText();
        SetState(0x21);
        SetMode(0x19);
        CManager::GetInstance()->RemoveExclusiveObserver((CObserver*)(CClickableBase_00557bd0*)this);
    }
}
}
