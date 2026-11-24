// Leaf: ?IsToEvadeAllHits@CCreatureAction_TentacleStrikeOutOf@@UBE_NXZ
class CCreatureAction_TentacleStrikeOutOf { public: virtual bool IsToEvadeAllHits() const; };
bool CCreatureAction_TentacleStrikeOutOf::IsToEvadeAllHits() const
{
    return true;
}
// Flatten route (flatten_pilot v4, NF324): CCreatureAction_TentacleStrikeOutOf_IsToEvadeAllHits -> CCreatureAction_TentacleStrikeOutOf::IsToEvadeAllHits, the RTTI-placed PDB member; class model: the row's local class model.
