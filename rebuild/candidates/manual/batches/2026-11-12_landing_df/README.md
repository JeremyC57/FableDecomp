# 2026-11-12 landing df -- chunk Q18 (Q17 reworks, Opus 5.5): 6 rows

vland --land: 6 wins (RELOCATION_MATCH), tests PASS. bindings_multi_check is at its NullCoord baseline.
- **0x008BF500 CCreatureAction_GenericFireProjectileWeapon::FinishAction.** A named `target` pointer takes one temporary out of the
  scratch-register rotation.
- **0x00555A90 NUISystem::CScrollableList::UpdateVisibleList.** `Children[i].Get()` in place of reading `.Data`.
- **0x00DE3CE0 NScript::CQ_RansomVictimScript::CNPCComment::Main.**
  - Ported onto CQ_RansomVictimScript.h with the new NPCCOMMENT opt-in.
  - A `for(;;)` loop with the terminating return first, and scoped inline_depth with a line-2 Inline-budget note.
  - The ENPCComment enumerator names are DESCRIPTIVE (made up). The enum name is the PDB's.
- **0x008886E0 NEnvironmentInternal::CEnvironmentThemePriorityStackEntry::BlendInLevel** (/Gr). CBlendedEnvironmentTheme::operator=
  is declared out of line (0x006B5DF0), which removes the inlined `rep movsd`.
- **0x009644F0 CAIStateGroup_StealStealableItem::OnReplace.** The sibling 0x00965640's body plus the scratchpad kill tail, with a new
  class opt-in and CThing::SetAsDeathPersistent.
- **0x00ED94D0 NScript::CV_TalentlessBardScript::WatchForResetWanderCentre.**
  - New header NScript_CV_TalentlessBardScript.h.
  - `PImp.Get() && PImp->IsAlive()` gives `test al,al`.
  - An inline CIsThingAlive constructor, and the sibling's `do { if (...) return; } while (0);` exits.

**Headers**

Q18's overlay started as a copy of Q17's (now landed). Headers Q18 changed were 3-way merged with base Q17's copy or cfe13baec. All
were clean, every default is the same, and two guards gained opt-in macros:
- NThingMap_CThingMap.h: PEEKCELL_INLINE.
- CThingSearchTools.h: ROUGHAREA_THISEND.

New: NScript_CV_TalentlessBardScript.h and CThingFilter_MinAllowedRadius.h.

**Names**
- Queued as land1011df:
  - Q18's three NAME_FIXES: 0x008780D0 is a GetAllThingsInRoughArea<CThingBuilding, list, Bind2<...>> instance; the BlendInLevel
    callee spelling at 0x008889C0; the UpdateVisibleList IAE leaf and its landed callers.
  - 0x006E92C0: the PDB `IsAttacker` (const). Landed binders spell it IsCreatureAttacking or Add@Sub.
  - 0x005DA4E0: a claim to verify. Binders spell it ReadyProjectile (in neither PDB) or ResetAiming.
- Labels: ghidra_out/labels_landing_2026-11-12df.tsv.
