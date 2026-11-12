# 2026-11-12 landing dg -- naming agent NF24: queue db-de and the 21 held-back renames, 59 landed rows changed, 40 renamed

Source: `naming/2026-11-12_agentNF24/` (STATUS, APPLIED, REJECTED, RENAMES, NOTES, LESSONS, QUEUE, hdr README).

**Part 1: queue db-de, 25 entries (22 done, 3 partial, 0 rejected)**
- 0x00744430 CTCPerceiveThing::IsAwarenessOfThingDirect, const. It was the placeholder `C_00744430::M`.
- 0x00658DB3 IsCurfewTime, const.
- 0x0097F3A0, 0x006621C0 and 0x0095A420: access and const fixes, with their binders respelled.
- 0x007FEF40 CTCUserActionPointBase::SetActionPointAsFree.
- 0x00F33D90 std::vector<CFireHeartPatternDef>::resize.
- The CPredicateBase vtable binders, and 0x00EF7AB0 ~CIsThingConscious.
- The CArenaCreatureDef copy constructor and CMemAllocator::CreateInstance.
- The seven script-data rows 0x00CD4500..46E0, 0x00CDBD20 Alloc and 0x00CDBD70.
- Partial:
  - 0x005CC10E GetSubDef<CCarryableDef>: 19 binders use local CDefPointer stand-ins.
  - 0x0095A420: the CQueenNative stand-ins.
  - Vtable 0x012CCC50: 0x00D34EC0's vector<bool> copy.

**Part 2: the 21 renames FAM07 and NF23 held back**

All 21 are now explicit instantiations over stlport/fable_map.h plus the engine headers, with no local class models. They are
CQuestManager, CHeroLogBook, CRegion, CEngineTextureDecal, CCombatAbility, CShopItem, the CTCDivineWrath / CTCUnholyPower maps, the
CCombatSequence_* StatGetName rows, CDisplayModeDesc and CEngineResourceManager.

**Verification**
- vland --recheck-landed in two parts: all 59 grade MATCH or RELOCATION_MATCH, with no HARD finding.
- Running every test exe found its catalog PassPattern in 59 of 59.
- Bindings were re-derived. bindings_multi_check is at its NullCoord baseline.
- Headers were 3-way merged with base 2c149ce54. CQ_SunnyvaleMasterData.h and the three script-data headers FAM08 created meanwhile
  are supersets in NF24's copies, so those were taken whole. Every default is the same.
- New opt-ins: CTCPERCEIVETHING_AWARENESS_CALLS, CPREDICATE_AND_INLINE_DTOR, NSCRIPT_DATA_CTOR, CQ_SUNNYVALEMASTERDATA_ALLOC,
  COMBATSEQUENCE_STATGETNAME and CENGINERESOURCEMANAGER_EXTERNALTEXTURES.

**Funclet include repair**

The vendor naming commit 8919d6f50 renamed three strmbase parent rows (CBaseRenderer::StartStreaming 0x00CA4D80, ClearPendingSample
0x00CA4BF0, GetCurrentSample 0x00CA4B90) without updating the EH funclet rows that `#include` them:
- 0x01216900
- 0x012168A0
- 0x01216880

The includes and the header comments are repointed. All three recheck RELOCATION_MATCH as slices of their parent block
(logs/funclets_recheck.log). The new tools/.../coordinator/funclet_includes_check.py finds such breaks: 1,317 includes, 0 broken.

**Follow-ups (handed to NF25)**
- The partial entries above.
- 0x00658B20 is landed under three names (IsCookTime / IsWorkTime / ComputeB0).
- Four more ~CIsThingConscious binders.

Labels: ghidra_out/labels_landing_2026-11-12dg.tsv.
