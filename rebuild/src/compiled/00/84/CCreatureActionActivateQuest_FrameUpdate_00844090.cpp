// CCreatureAction_ActivateQuest::FrameUpdate() -- finish the action once the named quest is no longer registered
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00844090, length 35 bytes. Vtable slot 56 of CCreatureAction_ActivateQuest (0x012752C4).
// Callees: the quest manager singleton @ 0x013B89FC, its FindByName(const CCharString&) @ 0x004AF610 (manifest
// "CQuest_Array_FindByName": walks the quest list at +0x38 comparing the name data pointer; bool result), virtual
// FinishAction (slot 4, +0x10) tail-jumped.
// ScriptName is the CCharString at +0xa8 (engine header PDB +0xb4: retail CCreatureActionBase is 0xa8 bytes, 0xc less).
struct CCharString {                        // one pointer (fable_string.h)
    char* m_pData;
};

struct CQuestManager_Calls {              // engine/CQuestManager.h exists but is not used: only this call is modelled
    bool FindByName(const CCharString& name);   // 0x004AF610
};

extern CQuestManager_Calls* g_pQuestManager;   // 0x013B89FC

// engine/CCreatureAction_ActivateQuest.h exists but is not used: its PDB layout puts ScriptName at +0xb4 (retail +0xa8,
// the base's three containers are 4 bytes wider there) and carries no vtable; the retail `this` is modelled locally.
struct CCreatureAction_ActivateQuest {
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03();
    virtual void FinishAction();            // slot 4 (+0x10)
    unsigned char _pad04[0xa8 - 4];         // +0x04 CCreatureActionBase body
    CCharString ScriptName;                 // +0xa8
    unsigned char LoadResources;            // +0xac
    void FrameUpdate();
};

void CCreatureAction_ActivateQuest::FrameUpdate()
{
    if (!g_pQuestManager->FindByName(ScriptName))
        FinishAction();
}