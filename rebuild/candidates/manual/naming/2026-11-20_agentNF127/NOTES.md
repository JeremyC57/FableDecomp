# NF127 notes (round 10, PRIORITY slice 9: q3965 .. q6400)

Hand-back on context: 21 of 31 entries closed, 10 open (APPLIED.tsv says what is left in each). Counts below come from STATUS.tsv /
APPLIED.tsv / recheck_final.log.

## Descriptive names
- No new made-up names. Every name applied is a PDB spelling (Ego_r.pdb and / or FableWin.pdb), or the landed Anniversary-PDB name
  EnableMusic (q4308, already the self row's name).

## Callee spellings that moved off a stand-in onto a PDB spelling (spelling count at the address does not grow)
- 0x0049C780 PeekTypeList: row 0x00910A90 now binds ?PeekTypeList@CThingSearchTools@@IBEABV?$list@PAVCClass@@V?$allocator@PAVCClass@@@std@@@std@@J@Z
  (Ego_r.pdb 0x004A6630 + FableWin.pdb 0x01CEDC90; already bound by landed rows) instead of the stand-in
  ?PeekTypeList@CThingSearchToolsMethods@@QAEPAUCThingList@@J@Z (still bound by ~86 other CThingSearchToolsMethods rows).
- 0x0091A630 HasSeller / 0x0091A9B0 AcquireSeller: row 0x0091A390 bound the non-template stand-ins ?HasSeller@CAIStateGroup_WorkBase@@QAE_NXZ /
  ?AcquireSeller@CAIStateGroup_WorkBase@@QAEXXZ; it now binds the self rows' template spellings (QBE HasSeller, IAE AcquireSeller): both
  addresses go from 2 spellings to 1.
- 0x009109B0: row 0x00910A90 bound ??RCUnprovenThingFilter_009109b0@@QBE_NABVCThing@@@Z; it now binds the PDB ??RCIsSeatWithSpaceOwnedBy@@QBE_NABVCThingObject@@@Z.
  The self row 0x009109B0 still defines ??RCIsSeatWithSpaceOwnedBy@@QBE_NPAVCThing@@@Z: 2 spellings before, 2 after (q4993 blocked, below).

## Leaf-only self renames outside the entries
- 0x0090AC50 (LSocketServer::GetAcceptedConnectionsOnSocket): moved with its q3965 binder fix to the PDB
  ?GetAcceptedConnectionsOnSocket@LSocketServer@LHNetworkLib@@QAEPAVLSocket@2@XZ (Ego_r.pdb 0x009A2140 + FableWin.pdb 0x02C70800); no landed
  row and no retail call / absolute reference reaches 0x0090AC50.
- 0x0091A9B0 AcquireSeller QAE -> IAE (Ego_r.pdb 0x0093E470 + FableWin.pdb 0x02BC8D20); its only REFS row 0x0091A390 moves in the same hand-back.

## Self file names that change with a self rename (the coordinator renames the files)
- 0x00946380 CAIStateGroupSitForMeal_GetPCookingPot -> CAIStateGroupPrepareMeal_GetFreeCookingPot
- 0x0098A8E0 CShaderRenderManager_UpdateBoneMatrices -> CShaderRenderManager_UpdateBoneTransform
- 0x00987E50 CPixelShader_Assign -> CPixelShader_operator=
- 0x00988630 CObservable_AddExclusiveObserver -> CShaderRenderManager_SetTextureTransformCount
- 0x00988730 E2_E2 -> CShaderRenderManager_SetUVCompressionVertexShaderConstant
- 0x0090AC50 LSocketServer_GetAcceptedConnectionsOnSocket -> LHNetworkLibLSocketServer_GetAcceptedConnectionsOnSocket
No EH funclet in rebuild/src/compiled/01/21 includes any of these parents (checked at hand-back, see below).

## Tests
- 0x009A5A50: test rewritten on engine/CASoundManager.h (a `Sound : CASoundManager` stub with the 47 pure virtuals, the base ctor / dtor
  and the out-of-line virtuals stubbed); same checks and PassPattern `009a5a50_TEST PASS`.
- Every other changed row keeps its landed test unchanged (they are standalone behaviour models).

## Shared-header opt-ins (hdr/README.md)
- engine/CAIStateGroup_PrepareMeal.h `FABLE_RETAIL_CAISTATEGROUP_PREPAREMEAL_GETFREECOOKINGPOT` (0x00946380).
- engine/CThingSearchTools.h `FABLE_RETAIL_CTHINGSEARCHTOOLS_LEAFBODY_CASTARG` (0x00910A90; nested in LEAFBODY).
- engine/CAIStateGroupBase.h `FABLE_RETAIL_CAISTATEGROUPBASE_PEEK_PROTECTED` (0x0097FEA0; access labels only).
No shared-header RENAME was needed (no "needs header claim" item).

## Flags lines
- None added. Every row keeps its catalog CompilerFlags.

## Follow-ups for the coordinator
- Self rows that still define stand-ins although every (or nearly every) landed binder binds the PDB name: 0x00988020 (Codex view
  CShaderRenderManagerApplyVertexShaderView), 0x0098B5E0 (CShaderRenderManagerSetLayoutView), 0x008FCFD0 / 0x008FD000 (free
  CAIStateGroupBase_GetPWorldSeed / _PeekThingSearchTools stand-ins: compiler-emitted out-of-line copies of header members, the nf86 / nf88
  pick-any form fits), 0x009109B0 (q4993). These are pre-existing splits; nothing here grows them.
- 0x0091A630 HasSeller: FableWin.pdb 0x02BC8C80 spells it ?HasSeller@?$CAIStateGroup_WorkBase@VCAIStateGroupBase@@@@IAE_NXZ (protected,
  non-const); the self row and now 0x0091A390 use the landed QBE. One PDB source only (Ego_r inlines it), so not in NAME_FIXES.
- 0x00B5E700 / 0x00B61690 / 0x00B88BC0 still use the CShaderEnableState stand-in over the made-up data global g_ShaderRenderManager
  (0x013BC470 = ?SingletonInstance@CShaderRenderManager@@0V1@A per NF52); q4650 needs 0x00B5E700 ported as NF52 did 0x00BAD2E0.
- REFS rows that are old no-Leaf rows on candidates/*_compiled.h (DIFFER by design, bind no entry spelling): 0x00BE6F70, 0x00BF4570
  (q4540 / q4695), 0x00BDC510, 0x00BE7D70, 0x00BE8920, 0x00BF3B60 (q6182). Left alone.
- REFS false hit: 0x0045912F (q4726): the bytes 30 86 98 00 at 0x004591FB.. are not an instruction operand.

## Rule slips
- Bash calls that exited non-zero (all read-only): the opening `wc` over directories, a `cat` of a missing nflocks owner file (q3965 lock
  check), a `grep -c` with zero matches as the last command (q4695 old-row check); a transcript review counts five in all.
- Two rows outside rows.txt were edited before they were claimed: 0x0092D7C0 (q5018 binder; its nflocks owner read `released`, taken
  over as NF127 at hand-back) and 0x00946F30 (q4946 binder; claimed at hand-back). Handed-off count 0 for both; no other agent's overlay
  or row touched them.
- Queue order: after the zero-compile closures I worked small families first (advisor triage), not strict queue order; q4946 (queue
  position 13) is left partial while later entries (q5126, q5018, q5032 ...) were finished.
- One st.py call passed `-` as a field value, which made it wait on stdin until stopped (the background task was stopped; nothing written
  twice). Later calls used `n/a` and `exec </dev/null`.
- A heredoc script for 0x009A5A50 asserted on text that its own inserted comment contained; it aborted before writing (no grade spent).
- grades_used in STATUS.tsv counts compiles per row; the highest is 3 (0x00910A90, 0x0091A390). The 0x009A5A50 test needed two extra
  vland runs (base ctor / dtor stubs), recorded in its STATUS note.

## Counts
- recheck_final.log: one vland --recheck-landed run over all 43 changed rows (rows/): 6 MATCH + 37 RELOCATION_MATCH, every test PASS.
- selfcheck.log: 0 problems.

## Decisions for the coordinator
- selfcheck.py only matches lines that start with `class` / `struct`, so its 0 problems does not cover two in-place respellings of
  row-local models: 0x0091A390 / 0x00946F30 turn their local non-template `CAIStateGroup_WorkBase` into
  `template<class Base> struct CAIStateGroup_WorkBase : Base` (precedent: the landed 0x00943C20 models the same template locally), and
  0x0090AC50 moves its local STLport `list` into namespace std with the allocator parameter and wraps its local LSocketServer in
  namespace LHNetworkLib (precedent: the landed 0x0090AA10 does both). If either form is held: holding 0x0091A390 strands the 0x0091A9B0
  AcquireSeller rename (its only REFS row), and holding 0x0090AC50 strands the 0x0090B8E0 self rename of q3965.
- 0x0092D7C0's lock read `released`; I took it over. If that lock means something else, q5018 must hold 0x0092C8A0 / 0x0092D600 /
  0x0092D7C0 together.
- 0x009104D0 (GetSeat) still compiles the same GetAllLeafThings<CThingObject, vector, CIsSeatWithSpaceOwnedBy> instance from LEAFBODY
  without CASTARG / SCANINCLUSION_ISALIVE, so two pick-any bodies of different bytes now share that symbol at link; follow-up: give
  0x009104D0 both macros and regrade it (not done: it is outside rows.txt and its own grade does not depend on the COMDAT).
- work/pdiff_all.txt: HEAD vs hand-back binding pairs for all 43 rows; every changed (row, target) pair is in RENAMES (38 pairs).
