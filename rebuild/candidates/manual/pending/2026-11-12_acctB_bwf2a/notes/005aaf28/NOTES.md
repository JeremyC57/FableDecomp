## 0x005AAF28 CTCInventoryTrade::ConstructTradeDescriptionClothing -- RELOCATION_MATCH (6 grades: 3 compile fixes, 3 real)
- What made it match: full draft from OUTLINE (4538v4550) -> declare `CWideString::operator!=(const CWideString&)` (macro
  FABLE_RETAIL_CTCQUESTCOMPLETIONUI_DESCRIPTION, otherwise the NullString compare converts) + `combatDef = experience->GetPHeroCombatDef();`
  as an assignment into a declared CDefPointer (`inc;inc;dec`) -> 4547v4550, frame-slot residues only -> ONE `NUISystem::CText* line`
  local re-assigned after every Reset instead of a separately named pointer per line: RELOCATION_MATCH (slot colouring follows symbols).
- Headers/flags: pragmas optimize("y",off)+("s",on), no /EHsc; overlay hdr/ (CTCInventoryTrade _TRADEDESCRIPTION, CTCShop _STOCKINFO,
  CTCInventoryClothing _NOARMOUR, see hdr/README.md); CThingPlayerCreature via the retail CThing chain macros (CCLASSFACTORY, CTHINGPHYSICAL,
  CTHINGGAMEOBJECT, CTHINGCREATUREBASE, CTHINGPLAYERCREATURE); same-TU GetDef/GetSubDef template bodies under auto_inline(off) as 0x5C90FC.
- Names: GetDef<T> at 0x42AF3C written as GetDef<CThingBaseDef> (manifest label; T otherwise unproven). Dossier NAME_FIXES confirmed in use
  (GetBoughtPrice, GetInventoryType, GetBuildingThing, GetTrainableStatLevel, GetGlobalIndexFromClassIndex, ToCharString, CTCShop::GetQuantity).
- Repair (verifier findings, all applied, still RELOCATION_MATCH incl. strict headers): `weaponDef->Class == WC_HEAVY` (EWeaponClass, same
  `cmp [eax+2Ch],2`); test comment fixed + loss case (bought 95 / sell 80 -> "Loss: 15", B/G tint) and profit-0 case (kills `>= 0`->`> 0`
  mutant, checked); NAME_FIXES.tsv gains 0x005BE8D3 -> GetPerItemDataForItem@CTCInventoryBase (pre-existing collision with 0x005BEAD5
  GetBoughtPrice, queue the respelling) and the 0x005BEAD5 GetBoughtPrice binding.
