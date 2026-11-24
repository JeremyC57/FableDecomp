// Leaf: ?IsActionReadyForInterruption@CCreatureAction_BlockBroken@@UBE_NXZ
class CCreatureAction_BlockBroken { public: virtual bool IsActionReadyForInterruption() const; };
bool CCreatureAction_BlockBroken::IsActionReadyForInterruption() const
{
    return false;
}
// Flatten route (flatten_pilot v4, NF324): CCreatureAction_BlockBroken_IsActionReadyForInterruption -> CCreatureAction_BlockBroken::IsActionReadyForInterruption, the RTTI-placed PDB member; class model: the row's local class model.
