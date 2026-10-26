// CTCWallMountEffects::OnPutOnMount() -- a teleport mount gives the object a CTCTeleporter; any effect makes it game-state persisted (remembering the old setting)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x007DF3E0, length 89 bytes (+ int3 padding).
// Callees: CCharString::CCharString(const char*, long) @ 0x0099EBF0, CThing::AddTC(const CCharString&, bool,
// CTCParamsBase*) @ 0x004C9D60, CCharString::~CCharString @ 0x0099EAE0 (manifest "_Cons_val<...>" is propagated),
// CThing::IsGameStatePersisted @ 0x004C8280, CThing::SetAsGameStatePersisted(bool) @ 0x004C7880.
// String 0x0123D820 = "CTCTeleporter".
// Layout (Ego_r.pdb): CTCWallMountEffects PDef +0x0c, ObjectWasGamePersistant +0x20; CWallMountEffectsDef Effect +0x28
// (1 = teleport, 0 = none).
// Shape: the class-name string is an implicit conversion temporary (re-addressed with `lea` for the push and the
// destructor, one `push ecx` frame slot); Effect is re-read through PDef for the second test.
class CCharString { public:
    void* m_pData;
    CCharString(const char* text, long length = -1);         // 0x0099EBF0
    ~CCharString();                                         // 0x0099EAE0
};

class CTCBase;
class CTCParamsBase;

class CThing { public:
    bool IsGameStatePersisted() const;                      // 0x004C8280
    void SetAsGameStatePersisted(bool persisted);           // 0x004C7880
public:
    CTCBase* AddTC(const CCharString&, bool, const CTCParamsBase*);   // 0x004C9D60
};

enum EWallMountEffects { WALL_MOUNT_EFFECT_NONE = 0, WALL_MOUNT_EFFECT_TELEPORT = 1 };

class CWallMountEffectsDef { public:
    unsigned char     _pad_00[0x28];
    EWallMountEffects Effect;                               // +0x28
};

class CTCWallMountEffects { public:
    void*                       __vftable;                  // +0x00
    CThing*                     ParentThing;                // +0x04
    bool                        ToBeRemoved;                // +0x08
    bool                        IsInGlobalUpdateTCs;        // +0x09
    const CWallMountEffectsDef* PDef;                       // +0x0c CDefPointer<CWallMountEffectsDef const>
    long                        WaitingForConfirmation;     // +0x10
    unsigned char               CreaturePerformingAction[8];// +0x14 CIntelligentPointer<CThingCreatureBase>
    long                        FrameEffectLastUsed;        // +0x1c
    bool                        ObjectWasGamePersistant;    // +0x20
    void OnPutOnMount();
};

void CTCWallMountEffects::OnPutOnMount()
{
    if (PDef->Effect == WALL_MOUNT_EFFECT_TELEPORT)
        ParentThing->AddTC("CTCTeleporter", false, 0);
    if (PDef->Effect != WALL_MOUNT_EFFECT_NONE) {
        ObjectWasGamePersistant = ParentThing->IsGameStatePersisted();
        ParentThing->SetAsGameStatePersisted(true);
    }
}