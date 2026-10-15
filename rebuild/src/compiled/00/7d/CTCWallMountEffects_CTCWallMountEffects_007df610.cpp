// CTCWallMountEffects::CTCWallMountEffects(CThing*) -- CTCBase(thing), own vtable, null def / creature pointers, effect never used, then resolve the owner def's wall-mount sub-def
// Retail: Fable.exe (TLC, VC7.1) function @ 0x007DF610, length 85 bytes (+ int3 padding).
// Callees: CTCBase::CTCBase(CThing*) @ 0x00686800 (manifest "CEngineSubPrimitive2DClockSprite" is propagated),
// CBaseIntelligentPointer::CBaseIntelligentPointer @ 0x00A01B10, CDefClassBase::GetSubDef<CWallMountEffectsDef>
// @ 0x007DF9E0 (manifest "GetSubDef<CAICreatureWillPowerIndicatorDef>" is the propagated template name).
// Vtables: CTCWallMountEffects 0x0126FC84, CIntelligentPointer<CThingCreatureBase> 0x0123E7D8.
// Layout (Ego_r.pdb CTCWallMountEffects, 36 bytes): CTCBase 0xc, PDef +0x0c (CDefPointer), WaitingForConfirmation
// +0x10, CreaturePerformingAction +0x14 (CIntelligentPointer, 8 bytes), FrameEffectLastUsed +0x1c, ObjectWasGamePersistant
// +0x20.
// Shape: the derived vtable is stored first, the null def pointer (inline constructor) and the confirmation
// enum are cleared around the creature pointer's base call, its vtable follows; FrameEffectLastUsed starts at
// 0xC0000000 (a "long ago" frame); the body then asks the owner's def (CThing +0x70) for the sub-def into PDef.
struct CThing;
struct CWallMountEffectsDef;

struct CTCBase_Calls {                                      // engine/CTCBase.h layout (0xc bytes)
    virtual void v00();
    CThing* ParentThing;                                    // +0x04
    bool    ToBeRemoved;                                    // +0x08
    bool    IsInGlobalUpdateTCs;                            // +0x09
    CTCBase_Calls(CThing* thing);                           // 0x00686800
};

struct CBaseIntelligentPointer {
    virtual void v00();
    void* m_pItem;                                          // +0x04
    CBaseIntelligentPointer();                              // 0x00A01B10
};

struct CIntelligentPointer_CThingCreatureBase : CBaseIntelligentPointer {   // vtable 0x0123E7D8
    virtual void v00();
    CIntelligentPointer_CThingCreatureBase() : CBaseIntelligentPointer() { }
};

struct CDefPointer_CWallMountEffectsDef {
    const CWallMountEffectsDef* Object;
    CDefPointer_CWallMountEffectsDef() : Object(0) { }
};

struct CDefClassBase {
    bool GetSubDef(CDefPointer_CWallMountEffectsDef* out);  // 0x007DF9E0
};

struct CDefPointer_CThingDef {
    CDefClassBase* Object;
    CDefClassBase* operator->() const { return Object; }
};

struct CThing {
    unsigned char         _pad_00[0x70];
    CDefPointer_CThingDef PDef;                             // +0x70
};

enum EWallMountEffectsConfirmationType { WALL_MOUNT_CONFIRMATION_NONE = 0 };

struct CTCWallMountEffects : CTCBase_Calls {                // vtable 0x0126FC84
    CDefPointer_CWallMountEffectsDef        PDef;                       // +0x0c
    EWallMountEffectsConfirmationType       WaitingForConfirmation;     // +0x10
    CIntelligentPointer_CThingCreatureBase  CreaturePerformingAction;   // +0x14
    long                                    FrameEffectLastUsed;        // +0x1c
    bool                                    ObjectWasGamePersistant;    // +0x20
    virtual void v00();
    CTCWallMountEffects(CThing* thing);
};

CTCWallMountEffects::CTCWallMountEffects(CThing* thing)
    : CTCBase_Calls(thing)
    , WaitingForConfirmation(WALL_MOUNT_CONFIRMATION_NONE)
    , FrameEffectLastUsed((long)0xC0000000)
    , ObjectWasGamePersistant(false)
{
    ParentThing->PDef->GetSubDef(&PDef);
}