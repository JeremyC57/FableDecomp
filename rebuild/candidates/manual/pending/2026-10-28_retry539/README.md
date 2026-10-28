# Retry chunk 539 (agent539, 2026-10-28): 0 of 8 parked rows matched; all re-parked under PDB names

Every row was regraded once under RTM; residues are register colouring, store / x87 scheduling (the `push ecx` / `fsubp`
swap shared with the CTCInGameMenu rows), frame-slot order, or cross-jumping (0x00584B86 CTCInventory::AssignThingPC,
873v854: retail merges two `push_back` arms). Each differ/ file's `// Grade:` line has the residue, what was tried and the
next step. 0x005726A0 is CTCInventoryExperience::RecheckAvailabilities (catalog label UpdateAbilityDisplay is wrong).

NAME_FIXES.tsv: 35 rows, deferred to the naming pass (0x0060EEB0 CTCInventoryMap::ConstructPCMapList(bool, bool*);
0x0041E5F2 CManager::GetInstance; 0x0053FDE0 / 0x0053FDB0 CAction::AddParam overloads; the CTCBase getter block
GetWorld 0x00686A70, PeekWorld 0x00686A80, PeekWorldMap 0x00686D20, PeekThingSearchTools 0x00686D90; 0x005800D6
CThingSearchTools::GetPFirstLeafThing<...> -- the landed leaf's CThingSearchToolsMethods spelling is wrong).
