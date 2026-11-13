# 2026-11-13 acctB: parked rows and the final result of the last author workflow (bwf2)

The owner stopped workflow launches after bwf2 finished. This folder keeps everything the lane holds for the 42 reserved rows that
are NOT matched, so nothing is lost with the container. Nothing here is landed, nothing matches yet, and no retail bytes or
oracle files are in this folder (the retail listings stay on the lane).

- `STATUS.tsv`: one line per row (workflow, bytes, name, grade of the best draft, whether a dossier / outline exists).
- `rows/parked/<addr>.cpp`: the best draft (19 rows, 124,989 bytes of retail code). The grade and the exact residue are in the first lines.
- `notes/<addr>/`: NOTES.md (every idea tried and the best next idea), DOSSIER.md (the scout's proven identity facts), OUTLINE.md
  (block-by-block map for rows without a draft), HINTS.md, ATTEMPTS.log, PRIOR_NOTES.md (earlier agents), NAME_FIXES.tsv where present.
- `hdr/<addr>/`: the draft's header overlay (full copies of repo headers plus `#ifdef FABLE_RETAIL_*` additions; headers marked STALE
  in its README.md are copies of an older repo state and must be re-based before any hand-back).
- 23 rows (160,440 bytes) have no draft yet; 16 of them have a finished dossier and outline, 7 have neither.
- `BWF2_WORKFLOW_RESULT.json`: the workflow's own result for its 22 rows.

## bwf2 result: 12 VERIFIED, 6 MATCH_WITH_OPEN_ISSUES, 4 DIFFER
Every matched row was re-verified by the coordinator (read-only vland, strict headers) and is staged in
`2026-11-12_acctB_bwf2a`, `_bwf_b`, `_bwf_c`, `_bwf_d`, `2026-11-13_acctB_bwf_e`, `_bwf_f` (the newest copy of a row wins).
VERIFIED = both verifier lenses passed (after a repair where one was needed).

### MATCH_WITH_OPEN_ISSUES
- **0x005AD571** CTCInventoryTrade::ConstructTradeDescription (authors used: 1)
  - The source declares `template<> std::vector<CWideString>::~vector();  // 0x00412130`, an explicit-specialization declaration that exists only to steer the compiler and that a Lionhead programmer would not plausibly have written. Its comment gives only the address and names no retail bytes that require it. I deleted the line and regraded (/opt/fable_lane/w22/bwf2/005ad571/verify_evidence/novecdtor.cpp) and it still gives RELOCATION_MATCH, so it is not needed for the match. Remove it, or justify it with the retail evidence it relies on.
- **0x00A079B0** CRenderStateManager::CDeviceState (authors used: 1)
  - payload.json gives module "CRenderStateManager" and name "CDeviceState", and oracle.tsv names the row "CRenderStateManager::CDeviceState". That is the nested class, not its constructor. The leaf ??0CDeviceState@CRenderStateManager@@QAE@XZ is CRenderStateManager::CDeviceState::CDeviceState. Other nested-class constructors put the full class path in the module: in rebuild/manifest/functions.tsv, 0x0084F490 has name CPhysicsState and module NCameraModes::CPhysicsState. Other lane payloads do the same, for example 0x00D30030 with module NScript::CQ_EndGameBossBattleScript. So this row should have module "CRenderStateManager::CDeviceState" and name "CDeviceState", and the oracle name should be CRenderStateManager::CDeviceState::CDeviceState. As written, the row would land under the class name instead of the constructor name.
- **0x00B5FAB0** CEngineScreenEffectGlowRenderer::CaptureBackBuffer (authors used: 3)
  - The source (comments stripped) uses `volatile` on two function statics, `static volatile float BlurOffsetScale` (src.cpp line 94) and `static volatile long NoBlurPasses` (line 95). Purity check 3 forbids this. CLAUDE.md and ASM_RULES allow `volatile` only where retail used it, and the author admits that no PDB LF_MODIFIER or vendor source backs it; the only evidence is the loop-end instruction schedule. The row cannot land as genuine until the coordinator decides on this (flag volatile-needs-coordinator-decision).
  - payload.json has no `flags` field, so the volatile-needs-coordinator-decision flag the author reported is not in the hand-back. The lander will not see it unless the coordinator adds it.
  - The overlay hdr/engine/CShaderRenderManager.h adds the opt-in FABLE_RETAIL_CSHADERRENDERMANAGER_SETPSCONST_INDEX, but the row source never defines it (hdr/README.md says the current source does not use it). It is dead overlay text and should be dropped before landing.
  - `static volatile float BlurOffsetScale` / `static volatile long NoBlurPasses` (0x0139A6B0 / 0x0139A6B4) do not meet any of the three conditions in docs/pipeline/ASM_RULES.md for `volatile`. No read is dropped without it (the author says so in the Retail-volatile line and NOTES), there is no PDB LF_MODIFIER record, and there is no vendor source. The only evidence is the load/store order at 0x00B60C79..0x00B60C9C, so the row must not land as MATCHED until the coordinator explicitly accepts `volatile` as a scheduling-only fit or the row is regraded DIFFER (r/nv1.cpp, 3 hunks).
  - The Identity comment says 'Ego_r order and size match the retail neighbours', but the size does not match: Ego_r has CaptureBackBuffer at 0x00A11CD0 with size 0x1570 (next symbol ApplyGlow at 0x00A13240), while retail is 0x14B6 (5302) bytes. The order and the sole caller Render 0x00B619B0 do match, so the comment should say order only (non-blocking wording fix).
- **0x00D6D670** NScript::CQ_HangingTreeEvilScript::Conclusion (authors used: 1)
  - Unexplained compiler steer: `FABLE_FORCEINLINE` on CScriptThing::GetPos and CTimer::GetTime has no comment naming retail bytes that need it, and it is not needed. With both spelled plain `inline` the row still grades RELOCATION_MATCH ($W/verify_evidence/v3/d.cpp). Drop the forceinline or justify it.
  - MoveToPosition is passed `(EScriptEntityMoveType)0` (twice) although the PDB enumerator exists: rebuild/include/engine/EScriptEntityMoveType.h has ENTITY_MOVE_WALK = 0, from identical Ego_r and FableWin TPI values, enabled by FABLE_RETAIL_SCRIPT_MOVE_ENUM and used by other landed rows. With that define and ENTITY_MOVE_WALK the row still grades RELOCATION_MATCH, strict headers too ($W/verify_evidence/v3/e.cpp). Use the retail enumerator name.
  - The Identity comment gets the Ego_r order wrong. It says `Ego_r method order Introduction 0x00D6D080 -> Conclusion -> Alloc 0x00D6ED10`, but in Ego_r.pdb (egor_pdb_names.tsv) Introduction 00d88d80 is followed by Conclusion 00d89500 and then DoMission 00d8b450. Alloc 0x00D6ED10 is the retail neighbour, not Ego_r order. Reword the comment so it does not mix the two orders.
  - The test misses two decisions. A mutant without the loop-head `if (ExecutionerInterrupted) break;` still prints PASS, and so does a mutant without the `Prisoner.IsAlive()` term in the execution condition ($W/verify_evidence/v3/tm3.cpp, tm2.cpp; tm3 also passes under vt.py). Add a case that pins each one, for example an upper bound on frames after an interrupt and a prisoner killed while the timer runs out.
- **0x004830A1** CTCInGameMenu::OpenInGameMenu (authors used: 2)
  - Source line 414 `bool valid = VerifySignatureCached(...) && (!hasSignature || VerifySignatureCached(signaturePath));` is a dead local with no comment. Retail does evaluate this third check and throw the result away (0x00484A10..0x00484A38: GetPathNameFromFileName 0x00408120, then VerifySignatureCached 0x0047ED0A twice, with no store), but the source must say which retail bytes require the unused local, or a reviewer will read it as compiler steering.
  - Source line 195 `menu->FindChild("UI_MENU_ENTRY_SAVE");` throws its result away and has no comment. Retail does make this call and discard eax before the UI_MENU_ENTRY_HELP lookup, but the source should cite that retail call so it does not look arbitrary.
  - The NAME_FIXES.tsv row for 0x0042BED4 (??0CDeletion@NUISystem@@QAE@XZ, replacing the scout label CAction::CAction) gives only one behavioural fact as evidence (it builds the CDeletion local before SetDeletion). It names no PDB source, even though two exist: Ego_r 0x00439F9C (egor_pdb_names.tsv) and FableWin 0x0184C6B4 / 0x01AAB21B (_fablewin_api.tsv). The row needs the two required sources.
- **0x00607330** CTCInventoryMap::ConstructLogbookList (authors used: 2)
  - Stale, contradictory source comment (source_cpp lines 18-22, the 'Name note'): it says our object binds 0x0042ABCA as ??1?$_List_base@PAVCComponent... because the shared STLport model leaves ~list() empty. The row now defines FABLE_RETAIL_STLP_LIST_DTOR_IN_LIST, and the graded object vl/00/60/00607330/00607330.obj shows ??1?$list@PAVCComponent... as the real 0x3e-byte body (sec 391). The ??1?$_List_base COMDAT is only the 1-byte trivial compiler-generated one (sec 344), so the note must be rewritten to describe DTOR_IN_LIST, as NOTES.md repair item 6 already does.
  - Wrong citation in the hdr overlay: hdr/engine/CTCInventoryMap.h (FABLE_RETAIL_CTCINVENTORYMAP_LOGBOOK comment) says 'Ego_r.pdb CategoriesList, 12-byte STLport map at +0x144'. ghidra_out/struct_layouts_egor.tsv (Ego_r) puts CategoriesList at 352 (0x160). The +0x144 offset comes from the retail bytes (`add ecx,0x144` at 0x0060830E / 0x0060852D, `add esi,0x144` at 0x00607506), so the comment should cite those instead of the PDB.

### DIFFER
- **0x00B4D370** :: (authors used: 3)
  - best draft and residue: `rows/parked/00b4d370.cpp`, `notes/00b4d370/NOTES.md`
- **0x00B783F0** :: (authors used: 3)
  - best draft and residue: `rows/parked/00b783f0.cpp`, `notes/00b783f0/NOTES.md`
- **0x00BA2350** :: (authors used: 3)
  - best draft and residue: `rows/parked/00ba2350.cpp`, `notes/00ba2350/NOTES.md`
- **0x00795300** :: (authors used: 3)
  - best draft and residue: `rows/parked/00795300.cpp`, `notes/00795300/NOTES.md`

VERIFIED: 0x007CACB0, 0x00BBF4A0, 0x005AAF28, 0x00A04630, 0x00BB5040, 0x00B72180, 0x0045A159, 0x0073B130, 0x00BCFF10, 0x00D30030, 0x00B9ECE0, 0x005A04F7
