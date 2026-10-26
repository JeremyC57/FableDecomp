// NCreatureMode::CModeSleepBed::FrameUpdate() -- the CModeBase frame update (0x0073AC50), then, while the mode holds a pending action (the counted pointer at +0x2c), the thing (+4) takes it when it is free to (IsFreeToPerformAction @ 0x00662210 with the raw action, then SetCurrentAction @ 0x00664670 with the counted pointer)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x008D6D40, length 45 bytes.
// Callees: CModeBase::FrameUpdate @ 0x0073AC50, CThingCreatureBase::IsFreeToPerformAction @ 0x00662210, 0x00664670 (SetCurrentAction(const CCountedPointer&); the label is propagated). Vtable slot 3 of NCreatureMode::CModeSleepBed (rtti-vtables.tsv; the chunk label CModeSitBase::FrameUpdate is the base). Shape: `this` in esi then replaced by the thing inside the branch, the pointer member's address in edi, `ret`.
namespace NCreatureMode {
struct CCreatureActionBase;
struct CCountedActionPointer {                     // CCountedPointer<CCreatureActionBase>: Data, Info
    CCreatureActionBase* m_pData;
    void*                m_pInfo;
};
struct CThingCreatureBase {
    bool IsFreeToPerformAction(CCreatureActionBase* action);      // 0x00662210
    void SetCurrentAction(const CCountedActionPointer& action);   // 0x00664670
};

struct CModeBase {                                 // CTCCreatureModeManager::CModeBase, vtable 0x012673CC
    virtual ~CModeBase();
    virtual void v01();
    virtual void v02();
    virtual void FrameUpdate();                    // 0x0073AC50
    CThingCreatureBase* m_pThing;                  // +0x04
};

class CModeSleepBed : public CModeBase { public:
    unsigned char         _pad08[0x24];            // +0x08 .. +0x2c
    CCountedActionPointer m_pendingAction;         // +0x2c
    virtual void FrameUpdate();
};

void CModeSleepBed::FrameUpdate()
{
    CModeBase::FrameUpdate();
    if (m_pendingAction.m_pData) {
        CThingCreatureBase* thing = m_pThing;
        if (thing->IsFreeToPerformAction(m_pendingAction.m_pData))
            thing->SetCurrentAction(m_pendingAction);
    }
}
}