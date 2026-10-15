// CHitLocationDef::operator=(const CHitLocationDef&) -- memberwise copy: CParentDefClassBase part, the shared physical-primitive counted pointer, armour, the five hit-response actions, effects, flags and priority; returns *this
#pragma optimize("s", on)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00461ACD, length 143 bytes.
// Pragma: #pragma optimize("s", on) -- the def module is size-optimised (every copy goes through eax/al).
// Callees: CParentDefClassBase::operator= @ 0x00431ED0,
//          CCountedPointer<CPhysicalPrimitiveBase>::ShareData(T*, CCPPointerInfo*) @ 0x00461B5C (landed).
// Identity: the catalog label "CParentDefClassBase::operator=" is propagated. Slot 19 (Copy) of
// CHitLocationDef's vtable 0x01234B54 is the 5-byte `jmp 0x00461ACD` right before this body; PDB members
// +0x40..+0x7c sit at retail +0x3c..+0x78 (retail base 0x39 bytes).
// Shape: CPhysicalPrimitiveInit is one CCountedPointer (Data, Info); its assignment is the inline
// `ShareData(other.Data, other.Info)` -- the two words pushed straight from the source (info first,
// arguments right to left). The init record and the pointer need written __forceinline operator=s: the implicit one is
// emitted out of line because its member has a user assignment, and a size TU out-lines plain inline ones. Enums/longs are dword moves, bools byte moves, in declaration order.
#pragma pack(push, 1)
struct CParentDefClassBase {
    virtual void v00();
    unsigned char _defClassBase[0x35];      // +0x04 CDefClassBase part, InstantiationName (+0x25), SubDefInfoMap (+0x29)
    CParentDefClassBase& operator=(const CParentDefClassBase& other);   // 0x00431ED0
};
#pragma pack(pop)

struct CPhysicalPrimitiveBase;
struct CCPPointerInfo;

struct CPhysicalPrimitivePointer {      // CCountedPointer<NHitLocations::CPhysicalPrimitiveBase> (8 bytes)
    CPhysicalPrimitiveBase* Data;
    CCPPointerInfo*         Info;
    void ShareData(CPhysicalPrimitiveBase* data, CCPPointerInfo* info);   // 0x00461B5C
    __forceinline CPhysicalPrimitivePointer& operator=(const CPhysicalPrimitivePointer& other)
    {
        ShareData(other.Data, other.Info);
        return *this;
    }
};

struct CPhysicalPrimitiveInit {         // 8 bytes
    CPhysicalPrimitivePointer PPrimitive;
    __forceinline CPhysicalPrimitiveInit& operator=(const CPhysicalPrimitiveInit& other)
    {
        PPrimitive = other.PPrimitive;
        return *this;
    }
};

struct CHitLocationDef : CParentDefClassBase {  // vtable 0x01234B54
    CPhysicalPrimitiveInit PhysicalPrimitive;   // +0x3c
    long Armour;                                // +0x44
    long HitResponseAction;                     // +0x48 EActionRegisteredType
    long HitResponseToEpicSpellsAction;         // +0x4c
    long HitKnockdownAction;                    // +0x50
    long HitKnockdownDeathAction;               // +0x54
    long BlockResponseAction;                   // +0x58
    long HitDamageEffect;                       // +0x5c
    long HitBlockedEffect;                      // +0x60
    long VulnerableEffect;                      // +0x64
    long Flags;                                 // +0x68 EHitLocationFlags
    bool PlayGetHitResponse;                    // +0x6c
    bool DecapitateOnHit;                       // +0x6d
    bool DefendableAgainstShots;                // +0x6e
    bool DefendableAgainstMelee;                // +0x6f
    long Priority;                              // +0x70
    bool Default;                               // +0x74
    bool EnableOnCreate;                        // +0x75
    long HitLocationCodelet;                    // +0x78 EHitLocationCodelet
    CHitLocationDef& operator=(const CHitLocationDef& other);
};

CHitLocationDef& CHitLocationDef::operator=(const CHitLocationDef& other)
{
    CParentDefClassBase::operator=(other);
    PhysicalPrimitive = other.PhysicalPrimitive;
    Armour = other.Armour;
    HitResponseAction = other.HitResponseAction;
    HitResponseToEpicSpellsAction = other.HitResponseToEpicSpellsAction;
    HitKnockdownAction = other.HitKnockdownAction;
    HitKnockdownDeathAction = other.HitKnockdownDeathAction;
    BlockResponseAction = other.BlockResponseAction;
    HitDamageEffect = other.HitDamageEffect;
    HitBlockedEffect = other.HitBlockedEffect;
    VulnerableEffect = other.VulnerableEffect;
    Flags = other.Flags;
    PlayGetHitResponse = other.PlayGetHitResponse;
    DecapitateOnHit = other.DecapitateOnHit;
    DefendableAgainstShots = other.DefendableAgainstShots;
    DefendableAgainstMelee = other.DefendableAgainstMelee;
    Priority = other.Priority;
    Default = other.Default;
    EnableOnCreate = other.EnableOnCreate;
    HitLocationCodelet = other.HitLocationCodelet;
    return *this;
}