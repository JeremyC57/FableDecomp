// CTCResurrectionItem::OnItemUsed(CThingCreatureBase& creature) -- the resurrection item revives the creature: full health, the current action finished and replaced by the knockdown recover-from-death action, the def's particle effect attached at its position, then the item thing is destroyed
// Leaf: OnItemUsed
// Retail: Fable.exe (TLC, VC7.1) @ 0x007F04C0, 203 bytes (`ret 4`; PDB ?OnItemUsed@CTCResurrectionItem@@QAEXAAVCThingCreatureBase@@@Z).
// Callees: CThingCreatureBase::FinishCurrentAction() @ 0x00662650, CCombatAction_GenericStrikeResponseKnockdownRecoverBase(
//   CThingCreatureBase*, bool) @ 0x00867AF0 (then the RecoverFromDeath vtable 0x01271A64, rtti-vtables.tsv), SetCurrentAction(const
//   CCombatActionBase&) @ 0x006644F0, ~CCombatActionBase @ 0x0062E900, CThing::GetPosition() const @ 0x004C73D0, CreateEffectThing(
//   long, const C3DVector*, bool) @ 0x006E0880 (`__fastcall`), CCharString(const char*, long) @ 0x0099EBF0 / ~ @ 0x0099EAE0,
//   CTCDParticleEmitter::AttachToThing(CThing*, long, const CCharString&, long, float) @ 0x006E0BE0, CThing::Kill(bool) @ 0x004C9B80.
// Layout: Ego_r.pdb CTCResurrectionItem PDef +0x0c, CResurrectionItemDef OnUseParticleEffect +0x28; CThingPhysical MaxHealth +0xb0
//   / Health +0xb4 (PDB +0xc8/+0xcc); the effect thing's interface-0x4b component cached at +0x5c (CThing::PTCDriver, PDB +0x6c).
// Shape: the action is a temporary of a derived class with an inline ctor over the out-of-line base ctor and an implicit
//   destructor (only the base destructor call remains); the def is read into a named local (kept in edi for both tests); the
//   emitter is `T* p = 0; if (HasTC(k)) p = member;`; the "" helper name is an implicit conversion built before the pushes.
struct C3DVector { float X, Y, Z; };
struct CCharString {
    void* PStringData;
    CCharString(const char* text, long length = -1);   // 0x0099EBF0
    ~CCharString();                                    // 0x0099EAE0
};
class CTCDParticleEmitter;
class CThing {
public:
    unsigned char  _pad00[0x20];
    unsigned long  m_presentBits[9];                   // +0x20 std::bitset of interface types
    unsigned char  _pad44[0x5c - 0x44];
    CTCDParticleEmitter* PTCDriver;                    // +0x5c (PDB +0x6c, the cached interface-0x4b component)
    unsigned char  _pad60[0x91 - 0x60];
    unsigned char  m_flags91;                          // +0x91 bit 0: dead
    unsigned char  _pad92[0xb0 - 0x92];
    float          MaxHealth;                          // +0xb0 CThingPhysical (PDB +0xc8)
    float          Health;                             // +0xb4
    const C3DVector& GetPosition() const;              // 0x004C73D0
    void Kill(bool immediately);                       // 0x004C9B80
    __forceinline bool HasTC(long type) const
    {
        if (m_presentBits[type >> 5] & (1UL << (type & 31)))
            return true;
        return false;
    }
};
class CTCDParticleEmitter {
public:
    void AttachToThing(CThing* thing, long attachType, const CCharString& helperName, long helperIndex, float offset);   // 0x006E0BE0
};
CThing* __fastcall CreateEffectThing(long type, const C3DVector* position, bool flag);   // 0x006E0880
struct CCombatActionBase {                             // retail sizeof 0x114
    virtual void v00();
    unsigned char _pad04[0x114 - 0x04];
    ~CCombatActionBase();                              // 0x0062E900
};
class CThingCreatureBase : public CThing {
public:
    void FinishCurrentAction();                        // 0x00662650
    bool SetCurrentAction(const CCombatActionBase& action);   // 0x006644F0
};
struct CCombatAction_GenericStrikeResponseKnockdownRecoverBase : CCombatActionBase {
    unsigned char _body[4];
    CCombatAction_GenericStrikeResponseKnockdownRecoverBase(CThingCreatureBase* owner, bool flip);   // 0x00867AF0
};
struct CCombatAction_GenericStrikeResponseKnockdownRecoverFromDeath : CCombatAction_GenericStrikeResponseKnockdownRecoverBase {   // vtable 0x01271A64
    CCombatAction_GenericStrikeResponseKnockdownRecoverFromDeath(CThingCreatureBase* owner)
        : CCombatAction_GenericStrikeResponseKnockdownRecoverBase(owner, false) { }
    virtual void v00();
};
struct CResurrectionItemDef {
    unsigned char _pad00[0x28];
    long          OnUseParticleEffect;                 // +0x28
};
struct CTCResurrectionItem {
    virtual void v00();
    CThing*       ParentThing;                         // +0x04 (CTCBase)
    bool          ToBeRemoved;                         // +0x08
    bool          IsInGlobalUpdateTCs;                 // +0x09
    unsigned char _pad0a[2];
    const CResurrectionItemDef* PDef;                  // +0x0c CDefPointer
    void OnItemUsed(CThingCreatureBase& creature);
};

void CTCResurrectionItem::OnItemUsed(CThingCreatureBase& creature)
{
    creature.Health = creature.MaxHealth;
    creature.FinishCurrentAction();
    creature.SetCurrentAction(CCombatAction_GenericStrikeResponseKnockdownRecoverFromDeath(&creature));
    const CResurrectionItemDef* def = PDef;
    if (def && def->OnUseParticleEffect > 0) {
        CThing* effect = CreateEffectThing(def->OnUseParticleEffect, &creature.GetPosition(), false);
        if (effect && !(effect->m_flags91 & 1)) {
            CTCDParticleEmitter* emitter = 0;
            if (effect->HasTC(0x4b))
                emitter = effect->PTCDriver;
            emitter->AttachToThing(&creature, 8, "", 0, 0.0f);
        }
    }
    ParentThing->Kill(true);
}
