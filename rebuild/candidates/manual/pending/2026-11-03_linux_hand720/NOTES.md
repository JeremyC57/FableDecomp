# chunk 720 (CTCVillage + CTCInventoryWeapons)

Nothing matched; payload.json is empty.

RemoveThingFromTask<T> (six instantiations, 123 bytes each): T resolved per address from the erase/find_if callees and the
dispatcher 0x00659EFC (00660ae2 Torch, 00660b5d Horn, 00660bd8 Patrol, 00660c53 PickCrops, 00660cce SecuritySweep,
00660d49 CarryCrate; the manifest calls all six <CVillageTaskPickCrops>). Best source (differ/<addr>.cpp, T swapped) is
DIFFER(123v123) with one operand left: the discarded erase() result temporary is homed in the dead `tasks` argument slot
[ebp+14h] where retail reuses the find_if result slot [ebp+10h]. 26 probes (iterator declaration/assignment forms, iterator
default/copy ctors, one-expression erase(find_if(..)), erase/find_if bodies in the TU, /Gr, optimize letters) -- rt11
(`iterator it; it = find_if(..)`) gets both temporaries into [ebp+10h] but then copies through `mov eax,[eax]`.

Not attempted (out of time): 00659800 CalculateSecuritySweepTasks, 0065b854 AddThingToVillage, 005c36c2 (fastcall ecx/edx
file helper; no PDB name found), 005c552c (= CTCInventoryWeapons::RemoveWeaponsFromSheathes, see NAME_FIXES), 005c6b26
ConstructWeaponDescription, 005c90fc ConstructWeaponDescriptionContainer, 005cacb9 RefreshWeaponDescriptions.
