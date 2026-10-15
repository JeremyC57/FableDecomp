// CThingFilter_AICreatureHasProperty::operator()(const CThingAICreature& creature) const -- whether the object's virtual slot 1 answers the filter's value (a second functor of the same shape; propagated label)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x006C4560, length 23 bytes.
// Leaf: ??RCThingFilter_AICreatureHasProperty
// Callees: the argument's virtual slot 1 (+4).
// Identity: the catalog label is propagated from the byte-similar 0x006A75D0 (which calls slot 48); no direct callers
// were found (the filter is used through an inlined template), so the class name is kept as catalogued.
// Layout (local model): the filter's value (long) at +0.
// Shape: `this` kept in esi across the virtual call; `return object.f() == m_value;` as `sub eax,[esi] ; neg ; sbb ; inc`.
struct CThingAICreature {
    virtual void v000();
    virtual long GetType() const;               // slot 1 (+4)
};

struct CThingFilter_AICreatureHasProperty {
    long m_property;                            // +0x00
    bool operator()(const CThingAICreature& creature) const;
};

bool CThingFilter_AICreatureHasProperty::operator()(const CThingAICreature& creature) const
{
    return creature.GetType() == m_property;
}