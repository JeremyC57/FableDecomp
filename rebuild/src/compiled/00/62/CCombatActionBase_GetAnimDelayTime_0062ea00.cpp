// Leaf: ?GetAnimDelayTime@CCombatActionBase@@UBEJXZ
class CCombatActionBase { public:
    virtual long GetAnimDelayTime() const;
};
long CCombatActionBase::GetAnimDelayTime() const
{
    return 0;
}
// Flatten route (flatten_pilot v4, NF324): CCombatActionBase_GetAnimDelayTime -> CCombatActionBase::GetAnimDelayTime, the RTTI-placed PDB member; class model: the row's local class model.
