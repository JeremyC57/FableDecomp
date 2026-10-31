# Codex parked-50 pass -- 11 of 50 landed (2026-10-31)

Outside Codex pass over the 50 parked rows reserved in `pending/2026-10-31_codex_parked50/` (brief: README_CODEX.md there). The return
was regraded on our lane like ChatGPT pass 9: own oracle from our exe (`oracle.tsv`), read-only vland.py regrade, then `vland.py --land`
(`verify.log`: 11/11 RELOCATION_MATCH + PASS, multi-check 0). Per the owner's instruction the naming was checked symbol by symbol before
landing (73 bound symbols against the Ego_r / FableWin / Ego_d name sets and `reloc_bindings.tsv`): one Codex name was wrong and was fixed
before landing (0x00C0ACC0 was `CUnprovenFunction_00c0acc0`; the established caller binding from 0x00C0B130 is `MultiplyMaskedFactors`,
leaf `?MultiplyMaskedFactors@@YIMMPBMKK@Z`); one test (0044c6c2) embedded rebuild_abi.h with the two-line FABLE_STATIC_ASSERT macro, which
the host test transform (drops lines containing `static_assert`) breaks, so the macro was written on one line. Every other Codex spelling
is the PDB's. Landed: 006f50b0 std::_Rb_tree<..CCombatantInfo..>::erase(const key&), 00845b10 CCreatureAction_TavernGameMasterInteraction::
Initialise, 007cbf60 ~CTCMultiArrow, 00c0acc0 MultiplyMaskedFactors, 008c6160 CCreatureAction_RockTrollFallToDeath::Initialise, 0044c6c2
CGameDefinitionManager ctor, 006772d0 CThingBaseDef ctor, 009395f0 CAIStateGroup_SitRoundCampfire::GetPNearCampfire, 007f9010
CTCAnimatingObject::PlayAnimation(long,long,bool), 0085fb30 NCreatureMode::IsAbleToBeAttacked, 008bf1c0 CCreatureAction_UnblockableMidAttack::
Initialise. Those rows carry PDB spellings for callees that eleven landed callers bound under stand-ins; the callers were respelled and
rechecked (`verify_caller_respell.log`, 11/11 RELOCATION_MATCH): 00af1670 + 00b3b240 (CResourceList::SetMaximumMemory 0x009FC520, from
the Ego_r member MaximumMemory +0x28 and the CResourceList neighbourhood), 00433693 + 00cd3f00 (CDefinitionManager ctor takes a
`void (__fastcall*)(const CCharString&)`; the two empty callbacks are `?ErrorCallback@CStartupDefinitionManager@@KIXABVCCharString@@@Z`
0x0043368C and `?ErrorCallback@CScriptDefinitionManager@@KIXABVCCharString@@@Z` 0x00CD3EE0), 007f8e20 (PlayAnimation(const CAnimInit&,
long) is private), 0072af30 + 006ebdd0 (NCreatureMode::IsKnockedDown(const CThing&)), 006650e0 (GFGetAngleXYTo), 006e97f0 + 006eabb0
(NCreatureMode::IsAbleToBeAttacked(const CThing&)), 006ed8c0 (STLport `_Rb_tree::erase` / `_M_find` instead of the CCombatantMap stand-in).
The 39 parked rows' best attempts are in `pending/2026-10-31_codex_parked50/differ/` (residue on line 2, STATUS.tsv there); they re-enter
the chunk pool (RESERVED.tsv renamed RETURNED.tsv). Codex's 30 NAME_FIXES + 6 review rows queued (prefix codex50): the two 1-byte stubs
to relabel, 0x009FC520 label, the 0x006F0250 `_M_find` family (eight callers still descriptive), 0x006F0350 label, CDefinitionManager
header debt. Lessons: LESSONS.md (Codex) -- genuine STLport tree types with the struct key, complete exit chains, address-taken keys
declared inside the arm that uses them, HasTC as `if (bit) return true; return false`. Labels: ghidra_out/labels_codex_parked50_2026-10-31.tsv.
