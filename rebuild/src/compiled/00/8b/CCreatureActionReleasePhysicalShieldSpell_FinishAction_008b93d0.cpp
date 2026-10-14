// CCreatureAction_ReleasePhysicalShieldSpell::FinishAction() -- if the world hands out a scripted camera manager, release its scripted control, then run the combat-action base FinishAction
// Retail: Fable.exe (TLC, VC7.1) function @ 0x008B93D0, length 45 bytes.
// Callees: CCreatureActionBase::GetWorld @ 0x00692940 (thiscall; propagated STL label in the manifest), the camera interface's virtual slot 9 (`call [eax+24h]`, one out-pointer),
// CGameCameraManager::ReleaseScriptedControl @ 0x00698950, CCombatActionBase::FinishAction @ 0x00859A20 (non-virtual, landed naming).
struct CGameCameraManager;
struct ICameraOwner {                       // the world's camera interface at +0x34: slot 9 hands out the scripted-camera owner
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04(); virtual void v05(); virtual void v06(); virtual void v07(); virtual void v08();
    virtual bool GetScriptedCameraManager(CGameCameraManager** ppManager);   // slot 9
};
struct CWorld {
    unsigned char _pad[0x34];
    ICameraOwner* m_pCamera;                // +0x34
    __forceinline ICameraOwner* GetCamera() { return m_pCamera; }   // accessor: the pointer is fetched into ecx before the out-pointer push
};
struct CGameCameraManager {
    void ReleaseScriptedControl();          // 0x00698950
};

struct CCombatActionBase {
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03();
    virtual void FinishAction();            // slot 4: 0x00859A20
    CWorld* GetWorld();                     // 0x00692940
};
struct CCreatureAction_ReleasePhysicalShieldSpell : CCombatActionBase {
    virtual void FinishAction();
};

void CCreatureAction_ReleasePhysicalShieldSpell::FinishAction()
{
    CGameCameraManager* pManager;
    if (GetWorld()->GetCamera()->GetScriptedCameraManager(&pManager))
        pManager->ReleaseScriptedControl();
    CCombatActionBase::FinishAction();
}