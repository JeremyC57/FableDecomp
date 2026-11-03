# 0x006AA720 CThingPlayerCreature::SetPlayerCreatureActionForMelee: parked (coordinator decision, 2026-11-03)
The only residue (1218v1212, 5 lines) is one argument, `GetCombatManager().GetProximityForNavigation()` (a FableWin inline returning
float by value): every getter form leaves a dead store of the return value. Reading the member `ProximityForNavigation` directly
matches (work/p13x.cpp) but needs CCombatManager's data public, which no PDB fact on disk shows. Not landed on an invented access
level. Untried idea: the getter returning `const float&` (by reference) or a getter whose body VC7.1 can see is a plain load.
Best rule-clean source: differ/006aa720.cpp.
