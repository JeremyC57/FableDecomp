# 2026-11-12 landing de -- naming agent FAM08: the `CVtable_<vtable>` placeholder family, 29 landed rows renamed

Source: `naming/2026-11-12_agentFAM08/` (STATUS, RENAMES, LEFT, NAME_FIXES, NOTES, LESSONS, hdr README).
- The family had 764 landed rows whose class was the placeholder `CVtable_<vtable address>`. None of its 355 vtables has RTTI, and the
  PDB name sets hold no `??_7` entries. Each class was proven from what stores its vtable (constructor, allocator or another landed
  row), its already-named slots, reloc_bindings and the script-info registration strings.
- 29 renamed: 14 scalar deleting destructors, 10 constructors, 3 destructors, 2 `vfunc_4` SuspendableProcess and 1 Alloc.
- 735 left (LEFT.tsv):
  - 733 are libpk (0x00C00000-0x00C8FFFF), which has no RTTI and no PDB. 53 sit in vtables with a libpk file-name string; that is
    one source only, so nothing was applied.
  - 0x00D30000 and 0x00DA0130 are proven CActiveEntityScript<T> deleting destructors, but their bytes could not be reproduced.

**Verification**
- vland --recheck-landed: 27 RELOCATION_MATCH and 2 MATCH, tests PASS (catalog TestSource), no HARD finding.
- The EH funclet rows 0x01216DC0, 0x01217210 and 0x01217218 `#include` the landed sources of 0x00CAA410 and 0x00CAF480 by path.
  - Those files were renamed, so the three includes were updated.
  - The funclets were rechecked as slices of the parent block with the repo's lane_tools/recheck.py, which reads `funclet block offset`:
    all RELOCATION_MATCH (logs/funclets_recheck.log).
  - The generic payload grades a funclet against its whole parent and shows DIFFER, even for an untouched funclet (control
    0x01216EC0).
- Bindings were re-derived. bindings_multi_check is at its NullCoord baseline. The new `??1CUnknown` binding is listed under its
  parent 0x00CAF480, not under the funclet.

**Changes**
- Scalar deleting destructors:
  - CScriptDataBase and the NScript data classes CalextestData, CEmptyData, Cjoss_test_scriptsData and CDanyalTestScriptsData.
  - CLevelScriptNoMasterBase, CSpawnedFuncBase, NScript::CPredicateBase, NScript::CIsVariableFalse and the five-term
    CPredicate_And<...CIsThingAlive...>.
  - CActiveEntityScriptBase, CFireHeartPatternDef, CConversation and NScript::CQ_HeroSoulsArenaScript.
- Constructors:
  - The default constructors of CArenaRoundDef, CHeroSoulsRoundDef, CArenaWaveDef, CHeroSoulsWaveDef and CFireHeartPatternDef.
  - The CArenaCreatureDef copy constructor.
  - NScript::CQ_SunnyvaleMasterData, CLevelScriptNoMasterBase and the strmbase CRendererPosPassThru.
- Destructors: CScriptDefinitionManager, and the strmbase CMemAllocator and CDeferredCommand.
- CSpawnedFunc<...>::SuspendableProcess for CQ_WaspBossScript and CCS_OakValeRevisitedScript, and NScript::CV_HeroDuelScript::Alloc.
- Destructor rows emit their vtable through an inline constructor plus an external `New<Class>` emitter function. Those emitter names
  are descriptive and never graded.

**Headers**
- New:
  - CScriptDefinitionManager.h, CLevelScriptNoMasterBase.h and CCS_OakValeRevisitedScript.h.
  - CArenaCreatureDef.h and CArenaWaveDef.h, each with an IMPLICIT_DTOR opt-in.
  - CalextestData.h, Cjoss_test_scriptsData.h and CDanyalTestScriptsData.h.
- Opt-ins: CQ_SUNNYVALEMASTERDATA_CTOR and CSCRIPTDATABASE_CTOR, both declaration only.

**Follow-ups**
- Queued as land1011de: 15 name fixes. These are old vtable spellings bound by other rows, plus the row identities 0x00CD4500,
  0x00CD4620, 0x00CD4660, 0x00CD46A0 and 0x00CD46E0 (data-class constructors, landed as C3DAnimationUtil), 0x00CDBD70 and
  0x00CDBD20. Naming agent NF24 has them.

Labels: ghidra_out/labels_landing_2026-11-12de.tsv.
