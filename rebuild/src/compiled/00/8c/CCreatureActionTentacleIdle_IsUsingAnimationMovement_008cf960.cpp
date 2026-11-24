// Leaf: ?IsUsingAnimationMovement@CCreatureAction_TentacleIdle@@UBE_NXZ
class CCreatureAction_TentacleIdle { public: virtual bool IsUsingAnimationMovement() const; };
bool CCreatureAction_TentacleIdle::IsUsingAnimationMovement() const
{
    return false;
}
// Flatten route (flatten_pilot v4, NF324): CCreatureAction_TentacleIdle_IsUsingAnimationMovement -> CCreatureAction_TentacleIdle::IsUsingAnimationMovement, the RTTI-placed PDB member; class model: the row's local class model.
