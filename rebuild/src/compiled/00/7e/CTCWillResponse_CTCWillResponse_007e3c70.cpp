// Grade: RELOCATION_MATCH (Fable landing 2026-10-19 after the Codex review hold: test exercises a hand-written stand-in, not the candidate)
// CTCWillResponse::CTCWillResponse(CThing*) -- construct the TC, fetch the thing's CWillResponseDef sub-definition and
//   copy its response flags into the TC's bitfield (and the slow-time flag into the thing); without a def all
//   responses except IsNotBloodyWillable default to true
// Retail: Fable.exe (TLC, VC7.1) @ 0x007E3C70, 0xe0 bytes. Callees: CTCBase::CTCBase(CThing*) 0x00686800 (manifest label
//   is a propagated CEngineSubPrimitive2DClockSprite twin), CDefClassBase::GetSubDef<T>(CDefPointer<const T>*) 0x007E47D0
//   (folded instance; manifest names the CAICreatureWillPowerIndicatorDef instance). Vtable 0x01270534 = CTCWillResponse.
// Layout: PDB (Ego_r) names: CTCWillResponse PDef +0x0c, flag bits +0x10 (PDB field-list order); CWillResponseDef bools
//   +0x25..+0x2c (offsets equal the PDB); CThing PBaseDef at retail +0x70 and IsAffectedByBulletTimeOverride bit 5 of
//   retail +0x93 (PDB +0x80 / +0xa3: the TLC CThing is 0x10 bytes shorter before them). Local model: engine/CTCBase.h is
//   a flat generated layout without the constructor, and no CThing/CWillResponseDef header exists.
template<class T> struct CDefPointer;
struct CDefClassBase {
    template<class T> bool GetSubDef(CDefPointer<const T>* out) const;   // 0x007E47D0
};
template<class T> struct CDefPointer { T* Object; CDefPointer() : Object(0) {} };
struct CThingBaseDef : CDefClassBase {};
struct CThing {
    unsigned char _pad00[0x70];
    CDefPointer<const CThingBaseDef> PBaseDef;          // +0x70
    unsigned char _pad74[0x1f];
    unsigned char NotPausedDuringCutscenes : 1;         // +0x93 bit 0
    unsigned char DeathPersistent : 1;
    unsigned char KillOnLevelUnload : 1;
    unsigned char ActivatedScript : 1;
    unsigned char LevelIsUnloading : 1;
    unsigned char IsAffectedByBulletTimeOverride : 1;   // +0x93 bit 5
    unsigned char AddingTCsPermitted : 1;
};
struct CTCBase {
    virtual void VFunc00();
    CThing* ParentThing;                                // +0x04
    bool ToBeRemoved;                                   // +0x08
    bool IsInGlobalUpdateTCs;                           // +0x09
    CTCBase(CThing* thing);                             // 0x00686800
};
struct CWillResponseDef {
    unsigned char _base[0x25];
    bool IsForcePushable;                               // +0x25
    bool IsLifeDrainable;                               // +0x26
    bool IsLightningable;                               // +0x27
    bool IsNotBloodyWillable;                           // +0x28
    bool IsAffectedBySlowTime;                          // +0x29
    bool IsAbleToBeLifted;                              // +0x2a
    bool IsEpicPowerExplodable;                         // +0x2b
    bool IsEpicSpellable;                               // +0x2c
};
struct CTCWillResponse : CTCBase {
    CDefPointer<const CWillResponseDef> PDef;           // +0x0c
    bool IsForcePushable : 1;                           // +0x10 bit 0
    bool IsLifeDrainable : 1;
    bool IsLightningable : 1;
    bool IsNotBloodyWillable : 1;
    bool IsAbleToBeLifted : 1;
    bool IsEpicPowerExplodable : 1;
    bool IsEpicSpellable : 1;                           // +0x10 bit 6
    virtual void VFunc00();
    CTCWillResponse(CThing* thing);
};
CTCWillResponse::CTCWillResponse(CThing* thing) : CTCBase(thing), PDef() {
    if (ParentThing->PBaseDef.Object->GetSubDef(&PDef)) {
        const CWillResponseDef* d = PDef.Object;
        IsForcePushable = d->IsForcePushable;
        IsLifeDrainable = d->IsLifeDrainable;
        IsLightningable = d->IsLightningable;
        IsNotBloodyWillable = d->IsNotBloodyWillable;
        IsAbleToBeLifted = d->IsAbleToBeLifted;
        IsEpicPowerExplodable = d->IsEpicPowerExplodable;
        IsEpicSpellable = d->IsEpicSpellable;
        ParentThing->IsAffectedByBulletTimeOverride = d->IsAffectedBySlowTime;
    } else {
        IsForcePushable = true;
        IsLifeDrainable = true;
        IsLightningable = true;
        IsNotBloodyWillable = false;
        IsAbleToBeLifted = true;
        IsEpicPowerExplodable = true;
        IsEpicSpellable = true;
    }
}