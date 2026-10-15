// CTCWallMountEffects::OnTakenOffMount() -- a teleport mount loses its teleporter TC; an effect mount that was not persisted before goes back to unpersisted
// Retail: Fable.exe (TLC, VC7.1) function @ 0x007DF440, length 54 bytes.
// Callees: CThing::RemoveTC(long type) @ 0x004C9840 (looks the type up in the thing's TC map at +0x44; the manifest
// label "CBaseObject::OnPropertyChanged" is propagated), CThing::SetAsGameStatePersisted(bool) @ 0x004C7880.
// 0xDD is the TC type of CTCTeleporter (the TC OnPutOnMount 0x007DF3E0 adds by name).
// Layout (Ego_r.pdb): CTCWallMountEffects PDef +0x0c, ObjectWasGamePersistant +0x20; CWallMountEffectsDef Effect +0x28.
// Shape: undoes OnPutOnMount; Effect is re-read through PDef for the second test, the `&&` gives two `je` to the
// shared epilogue.
struct CThing {
    void RemoveTC(long type);                               // 0x004C9840
    void SetAsGameStatePersisted(bool persisted);           // 0x004C7880
};

enum EWallMountEffects { WALL_MOUNT_EFFECT_NONE = 0, WALL_MOUNT_EFFECT_TELEPORT = 1 };

enum { TC_TELEPORTER = 0xDD };

struct CWallMountEffectsDef {
    unsigned char     _pad_00[0x28];
    EWallMountEffects Effect;                               // +0x28
};

struct CTCWallMountEffects {
    void*                       __vftable;                  // +0x00
    CThing*                     ParentThing;                // +0x04
    bool                        ToBeRemoved;                // +0x08
    bool                        IsInGlobalUpdateTCs;        // +0x09
    const CWallMountEffectsDef* PDef;                       // +0x0c CDefPointer<CWallMountEffectsDef const>
    long                        WaitingForConfirmation;     // +0x10
    unsigned char               CreaturePerformingAction[8];// +0x14 CIntelligentPointer<CThingCreatureBase>
    long                        FrameEffectLastUsed;        // +0x1c
    bool                        ObjectWasGamePersistant;    // +0x20
    void OnTakenOffMount();
};

void CTCWallMountEffects::OnTakenOffMount()
{
    if (PDef->Effect == WALL_MOUNT_EFFECT_TELEPORT)
        ParentThing->RemoveTC(TC_TELEPORTER);
    if (PDef->Effect != WALL_MOUNT_EFFECT_NONE && !ObjectWasGamePersistant)
        ParentThing->SetAsGameStatePersisted(false);
}