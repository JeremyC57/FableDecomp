// CCombatAction_GenericStrikeResponseKnockdownRecoverBase::CCombatAction_GenericStrikeResponseKnockdownRecoverBase(CThingCreatureBase*, bool flip) -- base(owner, 250, zeroed CSoundPair), CanBeHit = false, Flip = flip
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00867AF0, length 60 bytes (+ int3 padding).
// Callees: CCombatActionBase::CCombatActionBase(CThingCreatureBase*, long, const CSoundPair&) @ 0x00858030.
// Vtable 0x01271BAC = CCombatAction_GenericStrikeResponseKnockdownRecoverBase (rtti-vtables.tsv).
// Members: CanBeHit (+0x114) and Flip (+0x115) carry the Ego_r.pdb names (there at +0x12c/+0x12d: the PDB
// build's CCombatActionBase is 0x12c bytes, retail's is 0x114, so engine/<Class>.h disagrees with the bytes
// and the layout is modelled locally). The second argument is read after the base call (`mov dl,[esp+14h]`).
struct CThingCreatureBase;

struct CSoundPair {
    long m_first;
    long m_second;
    CSoundPair() : m_first(0), m_second(0) { }
};

struct CCombatActionBase {
    virtual void v00();
    unsigned char _body[0x114 - 4];
    CCombatActionBase(CThingCreatureBase* owner, long actionCode, const CSoundPair& sound);   // 0x00858030
};

struct CCombatAction_GenericStrikeResponseKnockdownRecoverBase : CCombatActionBase {
    bool CanBeHit;                              // +0x114
    bool Flip;                                  // +0x115
    virtual void v00();                         // own vtable 0x01271BAC
    CCombatAction_GenericStrikeResponseKnockdownRecoverBase(CThingCreatureBase* owner, bool flip);
};

CCombatAction_GenericStrikeResponseKnockdownRecoverBase::CCombatAction_GenericStrikeResponseKnockdownRecoverBase(CThingCreatureBase* owner, bool flip)
    : CCombatActionBase(owner, 250, CSoundPair()), CanBeHit(false), Flip(flip)
{
}
