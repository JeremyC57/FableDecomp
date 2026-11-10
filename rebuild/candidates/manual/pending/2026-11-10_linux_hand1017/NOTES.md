# chunk 1017 notes (agent1017, 2026-11-10)

**Every grade ran with `FABLE_HDR_OVERLAY=$A/hdr` exported** (p.sh exports it; also for the compile-only probes). Exception noted per
row: 00409b70 was graded with the prior's chunk-733 overlay, which is not part of `$A/hdr`.

Result: 0 MATCH / 14 rows. 7 rows worked to a measured DIFFER (differ/), 2 parked by kind (hand asm, libpk private conventions),
5 not reached (1-1.2 KB bodies). `src/` and `test/` are empty.

## Identities (catalog labels wrong on 6 rows; see LABELS.tsv for the evidence)
- 0x008365F0 = STLport `for_each<const pair<unsigned long,CAnimationEntry>*, CDeadCreatureAnimPreloader>` (/Gr). Functor
  CDeadCreatureAnimPreloader { const CMeshDataBank* MeshBank; } (Ego_r ??0CDeadCreatureAnimPreloader@@QAE@ABVCMeshDataBank@@@Z),
  operator() = PeekAnimation(entry.BankIndex) and a strcmp against the def string. Not CTCInventoryAbilities.
- 0x006A3E00 = STLport `__introsort_loop<pair<long,CIntelligentPointer<CThing>>*, ..., int, CKeyPairCompareLess<...>>`; std::sort
  0x006A3F20 calls it; __unguarded_partition 0x006A3440; __partial_sort 0x006A3BF0. Not a heap sift-down.
- 0x0081EA50 = `CMap::GetGroundSizeZAt(const C3DVector&) const` (protected). Extent 391 bytes, not 832 (SPLITS.tsv: the C2DCoordI
  overload starts at 0x0081EBE0).
- 0x00AE3920 = private static `__fastcall CParticleProgram::RemapBoneIndices(map<unsigned long,unsigned long>&, CArray<C3DGroup2>&)`.
- 0x004B2CA0 = `CQuestManager::GetActiveQuestSectionNames(set<CCharString>&) const`.
- 0x00733CF0 = `CTCCombat::UpdateGetHitCounterAttack(const CThingCreatureBase&)`.
- 0x00C1C460 is libpk audio code (0xC08AE0..0xC40000 range, protection-instance lock 0x0143BAFC), not ParticleMorphs_UpdateEditor;
  no PDB name found, left unnamed (LABELS has none for it).
- Catalog names confirmed: 0x00409730 VerifySignature, 0x00446EF0 SetControlScheme, 0x00A5CB90 AlignedTransformTransposedAssumeW
  (the 4-arg PBV/PAV2 overload, ret 10h), 0x00659800 CalculateSecuritySweepTasks, 0x00409B70 SetDefaultValuesForNewProfile,
  0x00B68070 LoadForegroundTextures, 0x00A4D330 CalcLocalBoneTransformation.

## Callee names used
0x009D49F0 ?GetStringAsCharArray@CDefStringTable@@QBEPBDJ@Z (const; landed rows spell it non-const -> NAME_FIXES), 0x00A242C0
CMeshDataBank::PeekAnimation, 0x00AC4070 C3DGroup2::SetBoneIndex, 0x00AE80F0 vector<C3DGroup2>::reserve, 0x00AE83D0
CArray<C3DGroup2>::push_back, 0x00AE7FE0 vector<C3DGroup2>::operator=, 0x00553E60 map<unsigned long,unsigned long>::operator[]
(spelling not re-verified, not in LABELS), 0x004AC380 set<CCharString> insert_unique, 0x00686A80 CTCBase::PeekWorld,
0x00733620 CTCCombat::GetCombatAbilityData. Globals 0x013BAC18 / 0x013BAC1C carry two names in landed code (NAME_FIXES).

## Header debt (all in hdr/, documented in hdr/README.md)
New: engine/CHeightMap.h, engine/C3DGroup2.h. Opt-ins in fable_definition_table.h, CDefString.h, CMeshDataBank.h,
fable_algorithm.h (for_each body), fable_containers.h (__introsort_loop family), CMap.h, CArray.h (noinline push_back),
CParticleProgram.h, CQuestManager.h (CActiveQuest members + section-names decl + DayActive/NightActive).
Debt left: 00409b70 still depends on the chunk-733 overlay that edits 19 headers outside #ifdefs (needs proper opt-ins before it
can land); 00733cf0 still uses the prior's local model of CTCCombat / CThingCreatureBase.

## Per-row next steps
- 008365f0 DIFFER(119v119), 4 lines: only the eax/edx rotation of the strcmp zero vs BankIndex. Try evaluating PeekAnimation's
  argument before the def-string compare (operand order of the && / the two calls), or a functor operator() with the bank
  index read into a named local first; c2prio --rotation shows the zero is the 2nd rotation temp.
- 006a3e00 DIFFER(277v277): first/last colouring. c2prio: __first prio 248 > __last 244 (the +24 is two `return __a` median
  blocks). Needs a __median spelling that gives __last the extra uses (e.g. STLport's exact `__median(*__first, *(__first +
  (__last - __first)/2), *(__last - 1))` with the value copies taken in a different order, or __median returning by value).
- 0081ea50 DIFFER(382v391): retail homes temporaries in the dead `localPos` argument slot and re-reads this->HeightMap; ours
  CSEs. Try reading the height map through a `CHeightMap*` re-fetched in each clamp (accessor call per use) and storing the
  floored x/y back through a reused local.
- 00ae3920 DIFFER(270v270), 4 lines of scheduling: the key store [esp+18h] must follow the tree-root load. 12 compile-only
  probes of the key spelling do not move it; next: map::find spelled through `remap.lower_bound` + compare, or the find
  inlined from a different STLport _Rb_tree::find form (`_M_find` with __y/__x declared in the other order).
- 004b2ca0 DIFFER(856v832): ours gets `and esp,-8` (8-byte-aligned frame) only when the active-quest list loop and a vector
  insert loop coexist; look for the 8-byte local (likely a pair<iterator,bool> or a double) that retail does not keep, e.g.
  spell set::insert's result as a discarded call.
- 00409b70 DIFFER(1089v1105): first re-home its chunk-733 overlay behind opt-ins; residue is the deferred non-null-def arm.
- 00733cf0 DIFFER(285v288): blocked by the LTCG this-less call to PeekWorld (ecx not loaded at 0x00733D1C). Park unless an
  owner-approved spelling for an LTCG-dropped `this` exists.
- 00a5cb90 PARKED: hand-written SSE (movaps/shufps on an aligned frame), i.e. original inline asm; per ASM_RULES it needs a shared
  header helper with a FABLE_PORTABLE twin, out of chunk scope.
- 00c1c460 PARKED: its callees use private LTCG register conventions (args in eax/esi etc.) that no C++ declaration emits.
- 00409730 / 00446ef0 / 00659800 / 00b68070 / 00a4d330: not reached (0 grades); each is 1.0-1.2 KB. 00659800 sits in an
  ebp-framed CTCVillage TU whose helpers are at odd addresses (0x0065F6A6, 0x0066000A, 0x0065F744: unaligned, likely /O1 or
  function-level alignment off) - check the TU's flags before authoring.
