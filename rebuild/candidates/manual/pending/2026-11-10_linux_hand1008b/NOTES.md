# chunk 1008b notes (CTCInventoryAbilities)

**Every grade and probe ran with `FABLE_HDR_OVERLAY=$A/hdr` exported** (hdr/README.md lists the six overlay files and their opt-ins).

## Result
1 matched (0x005EE49A RELOCATION_MATCH), 3 parked with a differ/ source (0x005F11A9, 0x005F59C7, 0x005F6BD6), 2 held
(0x005F64DD, 0x005F6793: CASoundManager slot names), 1 not reached (0x005F6E40 OnSerialise).

## Identities proven
- 0x005EE49A `?IsAbleToUseAbility@CTCInventoryAbilities@@QBE_NW4EHeroAbility@@_N@Z` -- all 23 call targets checked against retail
  (calls.py): bitset::test 0x410DE0 + CVectorMap LowerBound 0x4365B0 (the inline PeekTC), CCharString 0x99EBF0/0x99EAE0,
  CGameDefinitionManager::Get 0x44C6B0, GetGlobalIndexFromClassIndex 0x9AD390, GetDef instances 0x5F8A57 (fireball) / 0x5F8AB9
  (unholy power) / 0x5F8B1B (divine wrath), CTCSummonSpell::GetResetCreatureManaCost 0x79A610.
- Name corrections in NAME_FIXES.tsv: 0x0079A610 (landed "CountReferences", reloc_bindings "CTCSpellCost::GetCost"), 0x00445940
  (landed "GetNumAssignableSpecialMoveButtons" on CGamePlayerInterface; PDB spelling GetNumberOfAssignableSpecialMoveButtons),
  0x005F8150 (landed as _Umove<EWeaponClass>; it is STLport find<EHeroAbility*,EHeroAbility>), 0x005EDCF0 (brief fact; landed
  file global_Destval), 0x009AD390 (landed 0x005EE3B3 calls it GetDefGlobalIndex).

## Header debt
Three new shared headers (CTCSummonSpell, CSpecialAbilitiesFireballSpellDef, CSpecialAbilitiesDivineWrathDef) and three opt-in
blocks (CTCInventoryAbilities _BUTTONS, CGamePlayerInterface _SPECIALMOVEBUTTONS, fable_algorithm STLP_FIND) -- all needed to land
0x005EE49A (the first three) or to rebuild 0x005F6BD6. CTCSummonSpell models no data members (pointer use only).

## Rows
- **0x005EE49A** (g1, RELOCATION_MATCH): plain C++ on shared headers. Same-TU `CDefinitionManager::GetDef(long, CDefPointer<const T>*)`
  template body copied from landed 0x005F4C8F under `#pragma auto_inline(off)` (real body, not a stand-in; it emits the three COMDAT
  instances the row calls). FABLE_RETAIL_CTHING_GETTC_EXPANDED for the inline PeekTC. Test test/005ee49a.cpp PASS under g++ and VC7.1 /Od.
- **0x005F11A9** (7 grades incl. chunk 1008's 6; DIFFER 373v375): the brief's head residue (one `&AssignedAbilities[slot]` CSE) is gone:
  compare a *re-read* `AssignedAbilities[slot]` against the saved slot, not the loaded local (wk/11a9_v19). Left: retail homes
  `removed` (`mov edx,ecx ; mov [ebp-4],edx`) before the je and scans with edx; ours stores it after the arms and merges it with the
  load temp. ~30 spellings probed (wk/11a9_v2..v31, s1, s2). Next: c2prio on v19 for the `removed` / load-temp coalescing.
- **0x005F59C7** (g1; DIFFER 2649v2652, unchanged): carrying the real ConstructPCExpressionCategoryList 0x005F45B5 body same-TU (as the
  landed console twin carries 0x005F3F7B) did not move it. c2prio: textBank prio 56 > const 0 prio 48 -> textBank edi, const 0 ebx;
  retail keeps textBank in memory ([ebp+5Ch]) and gives ebx the tail -1. Next: a natural spelling that makes the bank pointer a
  non-candidate (chunk 1008's d5 escape probe reproduced retail's colouring, at +4 frame).
- **0x005F6BD6** (4 grades; DIFFER 617v618, difflines 90 -> 72): needs FABLE_RETAIL_CVECTORMAP_FORCEINLINE_FIND (the button Find loop is
  inline in retail, LowerBound 0x5F7F4C), the STLport `find` fastcall declaration, PeekPlayerManager / GetMainPlayer / the interface
  getter. Best: wk/6bd6_r (= differ/005f6bd6.cpp): `EHeroAbility removed = ability;` for the value search and the find (the param is no
  longer address-taken, so ability gets a register), a cached `orderEnd`, a map reference. c2prio: const 0 has a byte use (allowed
  ebx only), so ours never gets retail's `xor edi,edi` zero at the top with ability in ebx; spelling the top without the entry
  reference (wk/6bd6_ra) does put the zero in a register, but in ebx (`cmp byte [ebp+0Ch],bl`) with ability in edi. Duplicate scan:
  retail eliminates i (i*4 in ecx, i+1 in [ebp+0Ch]); ours keeps i. Retail passes &ability ([ebp+8]) to find. Next: find what removes
  const 0's byte use in retail (c2prio --trace on wk/6bd6_ra).
- **0x005F64DD / 0x005F6793**: held/ -- PSoundManager 0x013B8394 slot 9 (+0x24) then slot 3 (+0x0C) of the returned player (retail
  0x005F66B6/0x005F66C7, 0x005F6923/0x005F6934); only CASoundManager.h's unaccepted `Virtual09_00a3e3a0` spells slot 9.
- **0x005F6E40 OnSerialise**: not reached.
