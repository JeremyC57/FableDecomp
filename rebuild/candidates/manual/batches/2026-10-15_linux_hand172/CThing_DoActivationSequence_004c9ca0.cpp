// CThing::DoActivationSequence() -- create and activate the thing, or kill it (not immediately) when OnCreate fails
// Leaf: ?DoActivationSequence@CThing
// Retail: Fable.exe (TLC, VC7.1) @ 0x004C9CA0, 121 bytes (+7 int3). __thiscall, returns bool.
// Callees: CThing virtual slots 8 OnCreate (+0x20), 9 InitialActivate (+0x24), 10 OnInitialActivate (+0x28); the inlined
//          CThing::Kill(false) 0x004C9B80 body: slot 12 OnKill (+0x30), UpdateThingScanInclusion() 0x004C8C00, thing manager
//          (0x013B8A1C) 0x005202B0 and RemoveFromRenderList 0x0051E000.
// Shape: `if (OnCreate()) { InitialActivate(); OnInitialActivate(); return true; } Kill(false); return false;` with Kill
//        defined inline in the TU (the `immediately` arms fold away).
// Layout: as Kill 0x004C9B80.
class CThing;
struct CThingManager {
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
    void Kill(bool immediately)
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
    bool DoActivationSequence();
};
bool CThing::DoActivationSequence()
{
    if (OnCreate()) {
        InitialActivate();
        OnInitialActivate();
        return true;
    }
    Kill(false);
    return false;
}
