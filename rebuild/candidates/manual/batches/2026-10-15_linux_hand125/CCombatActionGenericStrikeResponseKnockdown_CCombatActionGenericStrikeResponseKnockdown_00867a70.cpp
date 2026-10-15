// CCombatAction_GenericStrikeResponseKnockdown::CCombatAction_GenericStrikeResponseKnockdown(CThingCreatureBase*, const CThing*, const CCharString&) -- constructor
// Leaf: ??0CCombatAction_GenericStrikeResponseKnockdown
// Retail: Fable.exe (TLC, VC7.1) @ 0x00867A70, 128-byte slot (116 bytes of code).
// Callees: CCombatActionBase::CCombatActionBase(CThingCreatureBase*, long, const CSoundPair&) @ 0x00858030,
//          CCharString::CCharString(const CCharString&) @ 0x0099EC30, CBaseIntelligentPointer::CBaseIntelligentPointer() @ 0x00A01B10,
//          CBaseIntelligentPointer::SetPItem @ 0x00A01B90.
// Shape: one zero register (ebx) builds the CSoundPair temporary for the base constructor (action code 250)
// and later clears GotPointOfHit / GotDirectionOfHit; vtable 0x01277344; AnimName copy-constructed out of
// line; PThingThatHit (CIntelligentPointer<const CThing>, vtable 0x01237F1C) built inline with
// SetPItem(thing). The layout is byte-packed exactly as the PDB header declares it (DirectionOfHit at an
// unaligned +0x12d, GotDirectionOfHit at +0x139); retail offsets = header offsets - 0x18 (the retail
// CCombatActionBase is 0x114 bytes), so a local model with #pragma pack(1) is used.
struct CThingCreatureBase;
struct CThing;
struct CBaseObject;
struct CCharStringData;

struct CCharString {
    CCharStringData* PStringData;
    CCharString(const CCharString& other);              // 0x0099EC30
};

struct CSoundPair {
    long FirstSound;
    long LastSound;
    CSoundPair() : FirstSound(0), LastSound(0) { }
};

struct CBaseIntelligentPointer {
    virtual void v00();
    CBaseObject* m_pItem;
    CBaseIntelligentPointer();                          // 0x00A01B10
    void SetPItem(CBaseObject* item);                   // 0x00A01B90
};

struct CIntelligentPointer_CThingConst : CBaseIntelligentPointer {   // CIntelligentPointer<const CThing>, vtable 0x01237F1C
    virtual void v00();
    CIntelligentPointer_CThingConst(const CThing* item) { SetPItem((CBaseObject*)item); }
};

struct CCombatActionBase {                  // retail sizeof 0x114
    virtual void v00();
    unsigned char _body[0x114 - 4];
    CCombatActionBase(CThingCreatureBase* owner, long actionCode, const CSoundPair& sound);   // 0x00858030
};

#pragma pack(push, 1)
struct CCombatAction_GenericStrikeResponseKnockdown : CCombatActionBase {   // vtable 0x01277344
    CCharString AnimName;                   // +0x114
    CIntelligentPointer_CThingConst PThingThatHit;  // +0x118
    float PointOfHit_X;                     // +0x120
    float PointOfHit_Y;                     // +0x124
    float PointOfHit_Z;                     // +0x128
    bool GotPointOfHit;                     // +0x12c
    float DirectionOfHit_X;                 // +0x12d
    float DirectionOfHit_Y;                 // +0x131
    float DirectionOfHit_Z;                 // +0x135
    bool GotDirectionOfHit;                 // +0x139
    bool FlipFacing;                        // +0x13a
    virtual void v00();
    CCombatAction_GenericStrikeResponseKnockdown(CThingCreatureBase* owner, const CThing* thingThatHit, const CCharString& animName);
};
#pragma pack(pop)

CCombatAction_GenericStrikeResponseKnockdown::CCombatAction_GenericStrikeResponseKnockdown(CThingCreatureBase* owner, const CThing* thingThatHit,
                                                                                           const CCharString& animName)
    : CCombatActionBase(owner, 250, CSoundPair()),
      AnimName(animName),
      PThingThatHit(thingThatHit),
      GotPointOfHit(false),
      GotDirectionOfHit(false)
{
}
