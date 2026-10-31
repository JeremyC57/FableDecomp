// CCreatureAction_CastPhysicalShieldSpell::Initialise() -- base Initialise, then unless the creature already has a physical-shield component (interface type 147), add "CTCPhysicalShieldSpell" to it, give it the action's level and time limit (creature class type 1 only) and its shield health
// Grade: DIFFER(125v125) -- register colour only: retail keeps this in edi and &PCreature in esi, ours the other way round; every instruction otherwise identical
// Leaf: ?Initialise@CCreatureAction_CastPhysicalShieldSpell@@UAEXXZ
// Retail: Fable.exe (TLC, VC7.1) function @ 0x008B9DD0, 125 bytes (+ int3 padding). Ego_r.pdb
//   ?Initialise@CCreatureAction_CastPhysicalShieldSpell@@UAEXXZ (donor 0x008F1790).
// Callees: CCombatActionBase::Initialise() 0x00857520 (Ego_r UAE), CBaseIntelligentPointer::GetPItem() const 0x00A01B50 (x3),
//   CCharString::CCharString(const char*, long) 0x0099EBF0, CThing::AddTC(const CCharString&, bool, const CTCParamsBase*)
//   0x004C9D60 (Ego_r ?AddTC@CThing@@QAEPAVCTCBase@@ABVCCharString@@_NPBVCTCParamsBase@@@Z), CCharString::~CCharString 0x0099EAE0.
// Shape: HasTC(147) is the inlined bitset probe returning the masked word (test [eax+30h],80000h); the name is a scoped named
//   local; the component rides in ebx across the string destructor, not null-checked; the class test is CClass::Type == 1
//   (byte +0x10). Component members from Ego_r CTCPhysicalShieldSpell: SpellLevel +0x10, DurationTimer +0x1c, ShieldHealth +0x20.
// Tried (this session): PCreature as CIntelligentPointer<CThingCreatureBase> with inline operator-> + PDB HasTC/AddTC (same swap);
//   earlier sessions: named PCreature reference, inline HasTC member, this copy, pShield at function scope, early return.
// Layout: engine/CCreatureAction_CastPhysicalShieldSpell.h agrees (+0x114..+0x120, STLport sizes); its base is flattened, so a
//   local model carries the CCombatActionBase virtuals and PCreature (+8).
class CTCParamsBase;
class CTCBase;
class CCharString {
public:
    void* PData;
    CCharString(const char* text, long length = -1);   // 0x0099EBF0
    ~CCharString();                                     // 0x0099EAE0
};
class CTCPhysicalShieldSpell {
public:
    unsigned char _pad0[0x10];
    long  SpellLevel;                       // +0x10
    long  ShieldFadeOutTimer;               // +0x14
    long  CurShieldFadeOutTimer;            // +0x18
    long  DurationTimer;                    // +0x1c
    float ShieldHealth;                     // +0x20
};
enum ETCInterfaceType { TC_PHYSICAL_SHIELD_SPELL = 147 };
class CBaseObject { public: virtual ~CBaseObject(); void* IntelligentPointer; };
class CClass : public CBaseObject {
public:
    void* ClassListPos;                     // +0x08
    void* TypeListPos;                      // +0x0c
    unsigned char Type;                     // +0x10
};
class CThing : public CClass {
public:
    unsigned char _pad14[0x20 - 0x14];
    unsigned long UsingComponents[9];       // +0x20 bitset<274>
    unsigned long HasTC(ETCInterfaceType type) const { return UsingComponents[type >> 5] & (1UL << (type & 31)); }
    CTCBase* AddTC(const CCharString& name, bool activate, const CTCParamsBase* params);   // 0x004C9D60
};
class CThingCreatureBase : public CThing { };
class CBaseIntelligentPointer {
public:
    virtual ~CBaseIntelligentPointer();
    void* PData;
    CBaseObject* GetPItem() const;          // 0x00A01B50
};
template <class T> class CIntelligentPointer : public CBaseIntelligentPointer {
public:
    T* operator->() const { return static_cast<T*>(GetPItem()); }
};
class CCombatActionBase {
public:
    virtual ~CCombatActionBase();
    virtual void Initialise();              // 0x00857520
    unsigned char _pad04[4];
    CIntelligentPointer<CThingCreatureBase> PCreature;   // +8
    unsigned char _pad10[0x114 - 0x10];
};
class CCreatureAction_CastPhysicalShieldSpell : public CCombatActionBase {
public:
    bool  ImmediateCast;                    // +0x114
    bool  OneAnimCast;                      // +0x115
    long  LevelToCast;                      // +0x118
    long  TimeLimit;                        // +0x11c
    float ShieldHealth;                     // +0x120
    virtual void Initialise();
};

void CCreatureAction_CastPhysicalShieldSpell::Initialise()
{
    CCombatActionBase::Initialise();
    if (!PCreature->HasTC(TC_PHYSICAL_SHIELD_SPELL)) {
        CTCPhysicalShieldSpell* shield;
        {
            CCharString name("CTCPhysicalShieldSpell");
            shield = (CTCPhysicalShieldSpell*)PCreature->AddTC(name, true, 0);
        }
        if (PCreature->Type == 1) {
            shield->SpellLevel = LevelToCast;
            shield->DurationTimer = TimeLimit;
        }
        shield->ShieldHealth = ShieldHealth;
    }
}
