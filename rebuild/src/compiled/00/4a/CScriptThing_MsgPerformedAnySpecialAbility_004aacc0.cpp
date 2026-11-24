// Leaf: ?MsgPerformedAnySpecialAbility@CScriptThing@@UBE_NXZ
struct Target;
struct TargetVtbl {
    char pad[0x98];
    bool (__fastcall *slot)(Target*);
};
struct Target {
    TargetVtbl* vtbl;
};
class CScriptThing { public:
        Target* m_target;
    virtual bool MsgPerformedAnySpecialAbility() const;
};

bool CScriptThing::MsgPerformedAnySpecialAbility() const
{
    CScriptThing* scriptThing = const_cast<CScriptThing*>(this);
    Target* t = scriptThing->m_target;
    if (t == 0)
        return false;
    return t->vtbl->slot(t);
}
// Flatten route (flatten_pilot v4, NF324): CScriptThing_MsgPerformedAnySpecialAbility -> CScriptThing::MsgPerformedAnySpecialAbility, the RTTI-placed PDB member; class model: the row's local class model.
