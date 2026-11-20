# NF166 notes (round 10: game component / world / animation data naming)

## Counts (from APPLIED.tsv, STATUS.tsv and recheck_final.log)

**Entries (31):**
- 16 done, 10 done-already, 2 deferred (q4940 and q11948: held by acctB), 3 blocked.
- The 3 blocked are q11679 / q11691 / q11725 (ConstantFPS, one unit): 4 rows left.
- 0 rejected; REJECTED.tsv has its header only.

**Rows (70 in STATUS.tsv):**
- 33 ported: 3 MATCH and 30 RELOCATION_MATCH, every test PASS. recheck_final.log is one vland run over all 33.
- 33 not-a-binder or self row unchanged.
- 4 blocked: the ConstantFPS rows.

The census of every REFS row (work/census.tsv and work/census_fps.tsv; probe.py, one compile per row) gave the exact list of wrong binders.

## Decision needed from the coordinator

1. **0x00AE34D0: rename CSortedMapOwner_00ae34d0::SortAndProcess to CParticleProgram::Update (q4364).**
   - This changes the class and the name, on a row that is not an entry row. That is beyond a pre-authorised leaf-only rename.
   - The evidence:
     - (a) The body passes `this+8` (InitialSimulation) to the PRIVATE CParticleProgram::CalculateAveragePatchSize on `this`, so the caller must be a CParticleProgram member. It also updates ExportParticles (+0x88), the same CVectorMap Update that ReadFromFile 0x00AE3C30 makes, binding 0x009B8EF0 under the same spelling.
     - (b) In both PDBs, the only no-argument non-const void members are Update, Clear (0x00AE3360, landed), ClearGroups (0x00AE3510, landed), AddConstraintEndRepeat and the private OptimiseConstraints. The last two write or read Constraints (+0x114), which this body never touches.
     - (c) FableWin.pdb Update spans 0x40 bytes, the same as the retail span.
   - Both PDBs hold ?Update@CParticleProgram@@QAEXXZ. No landed row references 0x00AE34D0.
   - If this is not accepted, drop rows/00ae34d0.cpp and treat q4364 as `blocked: caller identity`.
2. **0x006BB810: rename CThingDrawInformation::SetBarPercentage to CGameTimeManager::SetTimeOfDay (q11765).**
   - The entry asked for this. The evidence is two PDBs plus two callers.
   - The row file moves (CThingDrawInformation_SetBarPercentage_006bb810.cpp becomes a CGameTimeManager_SetTimeOfDay name).
3. **q4940 / q11948 (0x009FDE80, acctB).**
   - Not touched.
   - 29 REFS rows already bind ?BeginLoading@CFrameData@CGraphicInfoFrame@@QAEXKK@Z.
   - Five no-Leaf July rows grade DIFFER in this lane and were not edited: 0x00BE6F70, 0x00BEC000, 0x00BEC470, 0x00BEC950, 0x00BF4570. q11948 says 0x00BEC000 / 0x00BEC470 / 0x00BEC950 emit ?BeginLoading@CFrameData@@QAEXKJ@Z through rebuild/include/candidates/preload_bump_texture_compiled.h.
   - Question: may these July rows move while B holds the self row? They do not need the self row's source.

4. **0x00A99610: same-TU strong copy of 0x00A99110 renamed GetFirstObject -> FindFirstObject (q4207).**
   - The row's object now defines the strong symbol that the landed owner 0x00A99110 defines:
     ?FindFirstObject@C3DAnimFileRootChunk@@QBE?AV?$CCountedPointer@$$CBVC3DAnimFileObjectChunk@@@@XZ. At HEAD it defined the
     stand-in GetFirstObject instead.
   - Its body is the row's HEAD local-model spelling, not the owner's body verbatim.
   - The reason for the copy is not the nf96a register-live case. The row's HEAD comment says that VC7.1 sees the object pointer
     does not escape.
   - Fallback if this is not accepted: drop rows/00a99610.cpp. q4207 is then `blocked`, with 0x00A99610 left.

## Stand-in-to-real casts that now reach a PDB spelling (all existed at HEAD; disclosed per the hard limit)
- **0x00B82100:** `(C3DAnimationBlendState*)owner->BlendState` (an opaque byte array) and `GetContext()` = `(CParticleContext*)this`
  now feed the PDB-spelled SetupSimulation / SimulationStep (dereferenced into references).
- **0x0088FDC0:** the `Field04Overlay` stand-in's `field1C` (reached by casting `this` / `field04`) is retyped from the local
  CThingDrawInformation* to `CGameTimeManager*`.
- **0x004163B7:** the HEAD local CMainGameComponent model's `m_pDisplay` (+0x28) is retyped from CDisplayHolder* to `CDisplayEngine*`.
- **0x00892FD0:** `(CGameScriptInterface_Methods*)this` (HEAD stand-in, drops const) is kept for the 0x006E71F0 call; the CQuestManager
  calls go through the shared singleton accessor (no cast).

## Self renames (RENAMES section 1; the row file moves with each)

| Address | Change | Kind |
|---|---|---|
| 0x00A4CDD0 | pool CSimpleMemoryPool* to & | leaf-only, two PDBs |
| 0x00494520 / 0x00494680 / 0x00494770 | QBE to ABE: `private:` added in place in the HEAD local model | leaf-only |
| 0x00891520 / 0x00892FD0 | free `__fastcall` stand-ins to the virtual members CGameScriptInterface::FailAllActiveQuests / SetQuestAsFailed | leaf-only; catalog names were already right; two PDBs; no landed referencer |
| 0x00AE34D0 | see decision 1 | class and name |
| 0x006BB810 | see decision 2 | class and name |

- No EH funclets include any of these rows: grep of rebuild/src/compiled/01/21 for the old file names finds none.

## Extra fixes inside rows I changed (no split: each lands on the landed self Leaf or on an existing spelling)
- **0x00A4EBB0** also moves 0x00AAE7B0 FindFirstEvent and 0x00AAE840 FindNextEvent to their QBE self Leaves.
- **0x00A4CDD0** also moves its callee 0x00A4C5E0 to the AAV self Leaf. 0x00A4CDD0 was the only referencer of 0x00A4C5E0.
- **0x00B82100** also moves 0x00AE2D20 SimulationStep to its QBE self Leaf.
- **0x00892FD0** also moves 0x004B0160 from CQuestMgr::notify to ?OnQuestFinishScreenDisplayed@CQuestManager@@QAE_NABVCCharString@@@Z (both PDBs; a spelling already bound elsewhere).
- **0x00AE34D0** moves 0x009B8EF0 from its own stand-in SortPairs to the std::sort spelling that 0x00AE3C30 already binds.

## Kept as they were (not proven here; descriptive / stand-in names left untouched)
- **0x00892FD0:** g_someFlag (0x01375741) and CGameScriptInterface_Methods::applyFloat (0x006E71F0). These are HEAD stand-ins; both addresses already carry three spellings each.
- **0x006E11F0:** X_006e11f0::Scale.
- **ClampFloat (0x006BB810)** is the row's HEAD inline helper.

## In-place respells of HEAD local models (no new local model added)

| Row | Local model respelled |
|---|---|
| 0x00A4EBB0 | the chunk classes |
| 0x00A99610 | C3DAnimFileRootChunk; the same-TU copy renamed FindFirstObject, with a `// Fidelity: same-TU strong definition of 0x00A99110` line |
| 0x00A4CDD0 | C3DAnimationSequence / Data |
| 0x00A4DF10 | C3DAnimationSequence |
| 0x00B82100 | CParticleProgram |
| 0x004164E0 / 0x00417001 | CDisplayEngine |
| 0x00416E78 / 0x005EADC0 / 0x005EAF30 | CMainGameComponent |
| 0x00494520 / 0x00494680 / 0x00494770 | CFrontendGameComponent |
| 0x00908DA0 | CMainGameComponent: added `static long ConstantFPS` + the inline GetConstantFPS, the header spellings, the same shape as the 235 landed rows with that model |

- **0x00A99610 caveat:** that copy's body is the row's own local-model spelling, not the owner's body verbatim. The owner uses the shared CCountedPointer::SetStaticCast, which needs the CChunkedFileChunk base the row does not model. The object defines ?FindFirstObject@C3DAnimFileRootChunk@@QBE..., which is 0x00A99110's Leaf: the nf96a same-TU pattern.
- **Ported onto shared headers (stand-ins deleted):**

  | Row | Stand-in deleted | Header |
  |---|---|---|
  | 0x004163B7 | CDisplayHolder | CDisplayEngine.h |
  | 0x00AE34D0 | CSortedMapOwner, CPairVectorMap, SSortedPair, CPairCompareLess | CParticleProgram.h |
  | 0x00891520 / 0x00892FD0 | CQuestMgr | CGameScriptInterface.h + CQuestManager.h |
  | 0x006BB810 / 0x0088FDC0 | local CThingDrawInformation | CGameTimeManager.h |
  | 13 ConstantFPS rows | the stand-in global | CMainGameComponent.h |
- **0x00A4DF10** keeps its HEAD include-under-renaming-macro pattern (`#define C3DAnimation2 C3DAnimation2Donor`). It was not added by me and is unchanged.

## ConstantFPS (q11679 / q11691 / q11725)
- **Census:** 416 REFS rows already bind ?ConstantFPS@CMainGameComponent@@1JA.
- **Moved: 14 rows**, all onto that existing spelling, so no split.
  - Through engine/CMainGameComponent.h `CMainGameComponent::GetConstantFPS()`: 0x004DF48C, 0x006CDB00, 0x0066FAB0, 0x006E2840, 0x0079F5F0, 0x007F2100, 0x0093B8B0 and 0x006E11F0.
  - The same, with the frame difference as a named local: 0x00697710, 0x0091A390, 0x006CEC90, 0x0094FE30 and 0x0094FEB0. Form 1 was DIFFER because VC7.1 hoists the inlined accessor's load ahead of the GetFrame call.
  - In place in its local model: 0x00908DA0.
- **No longer define their stand-in global:** 0x007F2100, 0x006E2840 and 0x0079F5F0.
- **Tests rewritten for LINK_FAIL:**
  - 0x0066FAB0: the test defines `long CMainGameComponent::ConstantFPS` and sets it through a derived accessor struct.
  - 0x00697710: the test defines `long CMainGameComponent::ConstantFPS`.
  - PassPatterns kept.
- **Blocked after 1 form only (context budget, not 3):** 0x0057CB7E DIFFER(572v567), 0x0075AEC0 DIFFER(306v306), 0x006D8580 DIFFER(346v330), 0x008C6DB0 DIFFER(1088v1090).
  - Drafts: work/<addr>.cpp.
  - Next form: a named local ahead of the use, as for the five form-2 rows.

## Coordinator notes found in passing (not changed)
- **0x0065A5BB** grades DIFFER(211v203) at HEAD in this lane (no Flags line). Its object references only the PDB symbols for 0x0049D9D0 / 0x01375550.
- **0x00417001:**
  - The Leaf line says ?Render@CMainGameComponent@@MAEXXZ (both PDBs), but the object defines QAEXXZ: NAME_FIXES.
  - **0x004164E0:** the object defines QAEXJ, against the PDB IAEXJ: NAME_FIXES. Its binders would move with it.
- **0x007F2100** (catalog CTCLightningOrb::SetLifeTime, Leaf SetLifeTime) defines CAIStateGroup_BuyFromSeller::BoughtSomething. q.py grades the largest function. This was the state before my change.
- **WORKLIST** binders already right at census: 0x00643F13, 0x00907670, 0x0095F9A0.

## Descriptive names
- No new made-up names.
- Kept parameter names (show_screen, silent, ...) are descriptive, as in the header opt-ins.
- **The test helper CConstantFPSTestAccess (0x0066FAB0 test) is a descriptive test-only name. Flag for the coordinator:**
  - It is a new test-local class `struct CConstantFPSTestAccess : CMainGameComponent` with one static setter.
  - It does not model CMainGameComponent. It derives from the shared class only to assign the protected static, because the test
    links the row object, which needs the `@@1JA` (protected) symbol.
  - No header exists for it. Its base comes from the shared header.
  - Landed tests that assign ConstantFPS (0x008EDFF0 and others) use standalone host models with a public static and do not link a
    row, so there is no same-shape precedent.
  - If this is not accepted, an alternative is to fix one value per test case.

## Flags lines
- None added.

## Rule slips
- **grades_used** is counted from work/grades.log plus the vland runs (maximum 6, 0x00417001). The census compiles are not included:
  probe.py compiled every REFS row once, 253 + 435 compiles (work/census*.tsv).
- **Late claim:** 0x004164E0 and 0x00417001 (q10180) were edited before I claimed them. They had no lock directory and no UNMATCHED
  status. Both were claimed (NF166) before hand-back.
- **Two Bash calls exited non-zero:** a payload-inspection one-liner (KeyError), and the first three-row ConstantFPS grade loop (a
  `grep -m2` with no match as its last command).
- **Four ConstantFPS rows were blocked after 1 form, not the brief's 3,** to hand back within context.
- **One STATUS line was briefly overwritten with placeholder text** (0x00908DA0) and restored.
