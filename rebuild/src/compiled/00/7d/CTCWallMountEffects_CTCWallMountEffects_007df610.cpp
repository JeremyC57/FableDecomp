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
class CThing;
class CWallMountEffectsDef;

struct CTCBase_Calls {                                      // engine/CTCBase.h layout (0xc bytes)
    virtual void v00();
    CThing* ParentThing;                                    // +0x04
    bool    ToBeRemoved;                                    // +0x08
    bool    IsInGlobalUpdateTCs;                            // +0x09
    CTCBase_Calls(CThing* thing);                           // 0x00686800
};

class CBaseObject;
class CBaseObjectPointer;
class CBaseIntelligentPointer {                       // vtable 0x0129C95C (Ego_r: vptr + PData, 8 bytes)
public:
    CBaseIntelligentPointer();                           // 0x00A01B10
    virtual ~CBaseIntelligentPointer();                  // 0x00A01C10
    CBaseObject* GetPItem() const;                       // 0x00A01B50
    void SetPItem(CBaseObject* item);                    // 0x00A01B90
private:
    CBaseObjectPointer* PData;                           // +0x04
};

class CThingCreatureBase;
template <class T> class CIntelligentPointer : public CBaseIntelligentPointer {   // vtable 0x0123E7D8
public:
    CIntelligentPointer() : CBaseIntelligentPointer() { }

    T* GetPItem() const { return (T*)CBaseIntelligentPointer::GetPItem(); }
    void SetPItem(T* item) { CBaseIntelligentPointer::SetPItem((CBaseObject*)item); }
};

struct CDefPointer_CWallMountEffectsDef {
    const CWallMountEffectsDef* Object;
    CDefPointer_CWallMountEffectsDef() : Object(0) { }
};

class CDefClassBase { public:
    bool GetSubDef(CDefPointer_CWallMountEffectsDef* out);  // 0x007DF9E0
};

struct CDefPointer_CThingDef {
    CDefClassBase* Object;
    CDefClassBase* operator->() const { return Object; }
};

class CThing { public:
    unsigned char         _pad_00[0x70];
    CDefPointer_CThingDef PDef;                             // +0x70
};

enum EWallMountEffectsConfirmationType { WALL_MOUNT_CONFIRMATION_NONE = 0 };

class CTCWallMountEffects : public CTCBase_Calls { public:                // vtable 0x0126FC84
    CDefPointer_CWallMountEffectsDef        PDef;                       // +0x0c
    EWallMountEffectsConfirmationType       WaitingForConfirmation;     // +0x10
    CIntelligentPointer<CThingCreatureBase>  CreaturePerformingAction;   // +0x14
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