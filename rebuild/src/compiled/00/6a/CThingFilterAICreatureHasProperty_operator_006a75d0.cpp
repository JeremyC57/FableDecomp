// CThingFilter_AICreatureHasProperty::operator()(const CThingAICreature& creature) const -- whether the AI creature's property query (virtual slot 48) answers the filter's property value
// Retail: Fable.exe (TLC, VC7.1) function @ 0x006A75D0, length 26 bytes.
// Leaf: ??RCThingFilter_AICreatureHasProperty
// Callees: the creature's virtual slot 48 (+0xc0).
// Identity: Ego_r.pdb `??RCThingFilter_AICreatureHasProperty@@QBE_NABVCThingAICreature@@@Z`; called from
// GetPFirstLeafThing<CThingFilter_AICreatureHasProperty, CThingAICreature> (0x00685F21) and CTCTrophy::FrameUpdate.
// Layout (local model): the filter's property value (long) at +0.
// Shape: `this` kept in esi across the virtual call; `return creature.f() == m_property;` as `sub eax,[esi] ; neg ; sbb ; inc`.
struct CThingAICreature {
    virtual void v000();
    virtual void v001(); virtual void v002(); virtual void v003(); virtual void v004(); virtual void v005(); virtual void v006(); virtual void v007(); virtual void v008();
    virtual void v009(); virtual void v010(); virtual void v011(); virtual void v012(); virtual void v013(); virtual void v014(); virtual void v015(); virtual void v016();
    virtual void v017(); virtual void v018(); virtual void v019(); virtual void v020(); virtual void v021(); virtual void v022(); virtual void v023(); virtual void v024();
    virtual void v025(); virtual void v026(); virtual void v027(); virtual void v028(); virtual void v029(); virtual void v030(); virtual void v031(); virtual void v032();
    virtual void v033(); virtual void v034(); virtual void v035(); virtual void v036(); virtual void v037(); virtual void v038(); virtual void v039(); virtual void v040();
    virtual void v041(); virtual void v042(); virtual void v043(); virtual void v044(); virtual void v045(); virtual void v046(); virtual void v047();
    virtual long GetAIProperty() const;         // slot 48 (+0xc0)
};

struct CThingFilter_AICreatureHasProperty {
    long m_property;                            // +0x00
    bool operator()(const CThingAICreature& creature) const;
};

bool CThingFilter_AICreatureHasProperty::operator()(const CThingAICreature& creature) const
{
    return creature.GetAIProperty() == m_property;
}