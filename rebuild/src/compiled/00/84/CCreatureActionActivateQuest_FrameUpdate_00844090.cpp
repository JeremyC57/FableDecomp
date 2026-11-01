// CCreatureAction_ActivateQuest::FrameUpdate() -- finish the action once the named quest is no longer registered
// Naming wave 18: the global stand-in g_pQuestManager (0x013B89FC) is the private static CCountedPointer<CQuestManager>
//   CTBaseSingletonNoDefaultAlloc<CQuestManager>::PClass, read through the inline static accessor Get() (name-only: the bytes are unchanged).
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00844090, length 35 bytes. Vtable slot 56 of CCreatureAction_ActivateQuest (0x012752C4).
// Callees: the quest manager singleton @ 0x013B89FC, its FindByName(const CCharString&) @ 0x004AF610 (manifest
// "CQuest_Array_FindByName": walks the quest list at +0x38 comparing the name data pointer; bool result), virtual
// FinishAction (slot 4, +0x10) tail-jumped.
// ScriptName is the CCharString at +0xa8 (engine header PDB +0xb4: retail CCreatureActionBase is 0xa8 bytes, 0xc less).
class CCharString { public:                        // one pointer (fable_string.h)
    char* m_pData;
};

class CCPPointerInfo;
template <class T> class CCountedPointer { public:                    // engine/CCountedPointer.h shape: Data +0, Info +4
    T* Data;
    CCPPointerInfo* Info;
    T& operator*() const { return *Data; }
};
template <class T> class CTBaseSingletonNoDefaultAlloc {              // the CRTP singleton base of CQuestManager
    static CCountedPointer<T> PClass;                                // 0x013B89FC ?PClass@?$CTBaseSingletonNoDefaultAlloc@VCQuestManager@@@@0V?$CCountedPointer@VCQuestManager@@@@A (private static)
public:
    static T& __fastcall Get() { return *PClass; }                   // Ego_r / FableWin ?Get@?$CTBaseSingletonNoDefaultAlloc@VCQuestManager@@@@SIAAVCQuestManager@@XZ (inline)
};
class CQuestManager { public:              // engine/CQuestManager.h exists but is not used: only this call is modelled
public:
    bool IsQuestActive(const CCharString&) const;   // 0x004AF610
};


// engine/CCreatureAction_ActivateQuest.h exists but is not used: its PDB layout puts ScriptName at +0xb4 (retail +0xa8,
// the base's three containers are 4 bytes wider there) and carries no vtable; the retail `this` is modelled locally.
class CCreatureAction_ActivateQuest { public:
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03();
    virtual void FinishAction();            // slot 4 (+0x10)
    unsigned char _pad04[0xa8 - 4];         // +0x04 CCreatureActionBase body
    CCharString ScriptName;                 // +0xa8
    unsigned char LoadResources;            // +0xac
    void FrameUpdate();
};

void CCreatureAction_ActivateQuest::FrameUpdate()
{
    if (!CTBaseSingletonNoDefaultAlloc<CQuestManager>::Get().IsQuestActive(ScriptName))
        FinishAction();
}