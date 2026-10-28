// CCreatureAction_RockTrollFallToDeath::Initialise() -- set the combat animation FALL_TO_DEATH; when the creature is an AI creature (thing type 1) suspend its AI; then run the CCombatActionBase initialisation
// Grade: DIFFER(87v87) -- register swap only: retail keeps this in esi and &PCreature in edi; this named-local form swaps them (this in edi). Without the local the registers match but the second GetPItem runs after `push 0 ; push 1`.
// Leaf: ?Initialise@CCreatureAction_RockTrollFallToDeath@@UAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x008C6160, 87 bytes, thiscall, plain `ret` (virtual).
// Identity (proven): slot 12 of the CCreatureAction_RockTrollFallToDeath vtable 0x01262B04 (RTTI .?AVCCreatureAction_RockTrollFallToDeath@@),
//   the Initialise slot of the landed CCreatureAction_* siblings; Ego_r.pdb ?Initialise@CCreatureAction_RockTrollFallToDeath@@UAEXXZ;
//   string 0x01280D24 "FALL_TO_DEATH".
// Callees: CCharString::CCharString(const char*, long) 0x0099EBF0 / ~CCharString 0x0099EAE0, CCombatActionBase::SetCombatAnimation(
//   const CCharString&) 0x008574D0 (Ego_r.pdb), CBaseIntelligentPointer::GetPItem() const 0x00A01B50 (twice, on PCreature +0x08),
//   CThingAICreature::SuspendAI(bool, void*) 0x00833A10 (Ego_r.pdb; landed), CCombatActionBase::Initialise() 0x00857520.
// Shape: the landed CCreatureAction_ScorpionSummon::Initialise 0x008C4A80 tail (PCreature addressed once in edi; the creature for the call
//   is a named local fetched before the two arguments are pushed) and the landed 0x008C7200 block-scoped animation name.
// Layout: CCreatureActionBase PCreature +0x08 (Ego_r.pdb CIntelligentPointer<CThingCreatureBase>); the thing-type byte at +0x10 is
//   descriptive (Ego_r CThing +0x10 is unnamed). Local model: engine/CCreatureActionBase.h carries a data-member vptr.
class CCharStringData;
class CCharString {
public:
    CCharString(const char* text, long length = -1);         // 0x0099EBF0
    ~CCharString();                                          // 0x0099EAE0
    CCharStringData* PStringData;
};
class CBaseObject;
class CBaseObjectPointer;
class CBaseIntelligentPointer {                              // 8 bytes (Ego_r.pdb)
public:
    virtual ~CBaseIntelligentPointer();
    CBaseObject* GetPItem() const;                           // 0x00A01B50
    CBaseObjectPointer* PData;                               // +0x04
};
template <class T> class CIntelligentPointer : public CBaseIntelligentPointer {
public:
    T* operator->() const { return (T*)GetPItem(); }
    T* GetPTypedItem() const { return (T*)GetPItem(); }
};
class CThing {
public:
    unsigned char _pad00[0x10];
    unsigned char ThingType;                                 // +0x10 (1 = an AI creature)
};
class CThingCreatureBase : public CThing { };
class CThingAICreature : public CThingCreatureBase {
public:
    void SuspendAI(bool suspend, void* suspender);           // 0x00833A10
};
class CCreatureActionBase {                                  // 0xA8 bytes in retail (vptr first)
public:
    virtual ~CCreatureActionBase();
    virtual void Initialise();                               // retail slot 12
    unsigned char _pad04[4];                                 // +0x04
    CIntelligentPointer<CThingCreatureBase> PCreature;       // +0x08
    unsigned char _body10[0xA8 - 0x10];
};
class CCombatActionBase : public CCreatureActionBase {      // 0x114 bytes in retail
public:
    virtual void Initialise();                               // 0x00857520
    void SetCombatAnimation(const CCharString& name);        // 0x008574D0
    unsigned char _bodyA8[0x114 - 0xA8];
};
class CCreatureAction_RockTrollFallToDeath : public CCombatActionBase {
public:
    virtual void Initialise();
};

void CCreatureAction_RockTrollFallToDeath::Initialise()
{
    {
        CCharString name("FALL_TO_DEATH");
        SetCombatAnimation(name);
    }
    if (PCreature->ThingType == 1) {
        CThingAICreature* creature = (CThingAICreature*)PCreature.GetPTypedItem();
        creature->SuspendAI(true, 0);
    }
    CCombatActionBase::Initialise();
}
