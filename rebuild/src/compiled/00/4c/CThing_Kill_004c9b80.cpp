// CThing::Kill(bool immediately) -- start the thing's death: mark it dying (and death-persistent when immediate), run the kill hooks, mark it dead and drop it from scans and the render list
// Leaf: ?Kill@CThing
// Retail: Fable.exe (TLC, VC7.1) @ 0x004C9B80, 144 bytes. __fastcall, `ret 4`. Inlined (with false) into
//         CThing::DoActivationSequence 0x004C9CA0.
// Callees: CThing virtual slots 13 (+0x34, an immediate-kill permission query), 11 (+0x2C, the immediate-kill hook) and 12
//          (+0x30, CThing::OnKill 0x004C7CF0); CThing::UpdateThingScanInclusion() 0x004C8C00; on the global thing manager
//          0x013B8A1C: 0x005202B0 (descriptive OnThingKilled) and RemoveFromRenderList(CThing*) 0x0051E000.
// Shape: early out when Dead (+0x91 bit 0), Dying (+0x92 bit 2) or KillLocked (+0x91 bit 6); the flag sets are bitfield
//        assignments (`or cl,4` etc.); the `immediately` byte lives in bl.
// Layout: retail CThing = Ego_r.pdb - 0x10: flag bytes +0x91..+0x93 with the PDB bit order (+0x93 bit 1 = DeathPersistent).
class CThing;
class CThingManager { public:
    void OnThingKilled(CThing* thing);           // 0x005202B0 (catalog "CTCCameraPointScriptedSpline::EditCyclePreviousKeyCamera"
                                                 //   is a byte-twin label; descriptive name)
    void RemoveFromRenderList(CThing* thing);    // 0x0051E000
};
extern CThingManager* g_pThingManager;           // 0x013B8A1C
class CThing {
public:
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04(); virtual void v05();
    virtual void v06(); virtual void v07();
    virtual bool OnCreate();                     // slot 8 (+0x20) 0x004C9560
    virtual void InitialActivate();              // slot 9 (+0x24) 0x004C9690
    virtual void OnInitialActivate();            // slot 10 (+0x28) 0x004C97B0
    virtual void OnKillImmediately();            // slot 11 (+0x2c) 0x004C7D50 (descriptive)
    virtual void OnKill();                       // slot 12 (+0x30) 0x004C7CF0
    virtual bool CanBeKilledImmediately();       // slot 13 (+0x34) 0x004CD1B0 (descriptive)
    unsigned char _pad04[0x91 - 0x04];
    unsigned char Dead : 1;                      // +0x91
    unsigned char CreatedByOwnerThingCreation : 1;
    unsigned char IncludeInThingScans : 1;
    unsigned char Activated : 1;
    unsigned char GloballyLoaded : 1;
    unsigned char InLimbo : 1;
    unsigned char KillLocked : 1;
    unsigned char CreatedBySerialisation : 1;
    unsigned char DeletedOnOwnerLevelOutOfScope : 1;   // +0x92
    unsigned char Drawable : 1;
    unsigned char Dying : 1;
    unsigned char ForceFrameUpdate : 1;
    unsigned char LevelPersistent : 1;
    unsigned char GameStatePersisted : 1;
    unsigned char PersistOnUnloadWhenInLimbo : 1;
    unsigned char PausedDuringCutscenes : 1;
    unsigned char NotPausedDuringCutscenes : 1;  // +0x93
    unsigned char DeathPersistent : 1;
    unsigned char KillOnLevelUnload : 1;
    unsigned char _bits93 : 5;
    void UpdateThingScanInclusion();             // 0x004C8C00
    void Kill(bool immediately);
    bool DoActivationSequence();
};
void CThing::Kill(bool immediately)
{
    if (Dead || Dying || KillLocked)
        return;
    if (immediately && !CanBeKilledImmediately())
        return;
    Dying = true;
    if (immediately) {
        DeathPersistent = true;
        OnKillImmediately();
    }
    OnKill();
    Dead = true;
    UpdateThingScanInclusion();
    g_pThingManager->OnThingKilled(this);
    g_pThingManager->RemoveFromRenderList(this);
}