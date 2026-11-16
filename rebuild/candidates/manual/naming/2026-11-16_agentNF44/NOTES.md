# NF44 round 6 notes (slice land1115ga..land1116p23, 129 entries, continuing NF42)

## Outcome (counted from APPLIED.tsv)
done 2, done-already 0, rejected 0, partial 10, blocked 0, deferred 0, carry 117 (total 129).
- **done:** 88 (the 0x00C4B010 follow-up: 1) and 103 (CString::operator=: 2). For both, a fresh compile of the self row shows it already
  spells the claim: 0x00C4AC70 `?ReturnBuffer@PKCBufferedReader@@QAEXPA_N@Z`, 0x00C0C200 `??4CString@@QAEABV0@PBD@Z`.
- **partial:** 23 (3: every binder is fixed, but the self row 0x00F36EE0 still spells the placeholder `?M@S@@QAEPAXH@Z`; see APPLIED),
  29 (2), 30 (2), 31 (1), 34 (3), 35 (19), 37 (1), 79 (1: the same row as 37; same target), 40 (18 follow-up callers of 0x0042D5B1),
  41 (1 follow-up caller of 0x0042B6B8).
- The binders_respelled column sums to 54. Some rows serve two entries: 0x00B87680 / 0x00B89A20 / 0x00BAC0F0 count under 23 and 35, and
  0x00B242A0 under 37 and 79.

Rows changed: 50 (STATUS.tsv). The final recheck (recheck_final.log) is one run over all 50: 50 RELOCATION_MATCH, 50 tests PASS.
- Only 0x00C4B040 has a new test (rows/00c4b040_test.cpp). The old test defined the helperA / helperB stand-ins the row no longer calls;
  the new one fakes PKCBufferedReader::ReturnAllBuffers / Refill and keeps the PassPattern.
- No EH funclet row includes any changed row (grep of rebuild/src/compiled/01/2* for each row's file name: none).

Grades: 60 compiles in work/grades.log.
- 59 are on the 50 changed rows (max 3 on one row).
- 1 is on the abandoned 0x00B68610: SRC_FAIL, row unchanged, lock released.

Compiles not counted as grades (no row changed by them; private dir pO/, deleted):
- the 50 original landed sources and the self rows 0x00F36EE0 / 0x00C4AC70 / 0x00C0C200, compiled to read the old self symbols and relocations for RENAMES.tsv (work/xr_old, work/xr_new);
- the landed 0x0053CFC2, compiled once to read its own symbol;
- the start-of-round probe of 1,051 caller rows (work/probe.tsv), plus the follow-up probe of the callers of 0x0042D5B1 / 0x0042B6B8 /
  0x00C4B010 (work/fu_probe.tsv).

Locks: NF44 holds nflocks for exactly the 50 changed rows. Rows that were claimed but left unchanged were released: 0x00427F31,
0x0042819A, 0x00545B10, 0x00555440, 0x005556F0, 0x0059DE04, 0x0059DE76, 0x00B86A70 and 0x00B68610. NF41 held no row I needed, so no
entry is deferred.

## The coordinator's three partials (queue lines 40, 41, 88)
The fresh probe found more stale callers than NF42 listed. 0x0042D5B1 had 23 wrong landed callers, not 7; 0x0042B6B8 had 3, the third
being 0x0059DE76. Results:
- **0x0042D5B1 (entry 40): 18 of 23 ported.** The local CStateMap / CUIState / C2DVector / CRGBColour stand-ins were replaced by
  engine/C2DVector.h, engine/NUISystem_CUIState.h and stlport/fable_map.h.
  - Rows whose old note says the operator[] needs a same-TU body use `FABLE_RETAIL_STLP_MAP` (visible body): 0x00546320, 0x005518E0,
    0x00551BC0. All others use `FABLE_RETAIL_STLP_MAP_INDEX_DECL_ONLY`.
  - Member respellings this needed: SComponentState `.Size` (+0x10) is CUIState::Zoom; the alpha byte +0x1B is CUIState::Colour.A; the
    iterator rows read the value through `(*it).second` / `it->first`.
  - **Left 5:** 0x00427F31 / 0x0042819A (local CRGBColour(const CRGBFloatColour&) ctor), 0x00545B10 (a whole local namespace std),
    0x00555440 / 0x005556F0 (own leaf takes the local NUISystem::C2DVector; see APPLIED line 40).
- **0x0042B6B8 (entry 41): 1 of 3.** 0x0042BEAD is now CTCInventoryWeapons::AddComponentToAvailableWeaponsEntries (self rename,
  RENAMES section 1).
  - **Left:** 0x0059DE04 / 0x0059DE76. Their local std::list<T> pushes the CComponent* key by reference, while the shared CAction
    list is list<CActionParam*>.
- **0x00C4B010 (entry 88): done.** 0x00C4B040 now calls PKCBufferedReader::ReturnAllBuffers / Refill on libpk/PKCBufferedReader.h. Its
  own leaf, the placeholder `_Dest_val_pair_CCharString_CSymbolInfo`, is unchanged: no name evidence, and the row has no caller.

## Families ported this round
- **Scratch-texture family, 0x01436E98 +0x3190 (entries 23 and 35; 19 rows).**
  - Before: CEngineGlobal_01436e98 / SEngine_01436e98 / CEngine_Retail / g_pEngine_01436e98 / CTexturePool_00f36f00 /
    CTexturePool_00f36ee0 / CTexturePool_Retail / local CGPUScratchBufferTextureManager.
  - After: `NEngineInternal::GPResourceManager->GetGPUScratchBufferTextureManager()` on engine/CEngineResourceManager.h, with the Ego_r
    member spellings EndTextureUse / StartTextureUse / FreeMemoryPage / StartSurfaceUse / EndSurfaceUse (new overlay opt-ins). Mostly done by the
    transformer work/scratch.py; 0x00B5EC90 / 0x00B6D600 (other stand-in names) by hand.
  - 0x00BAC0F0 named 0x00F36EE0 / 0x00F36F00 "StartSurfaceUse / EndSurfaceUse" and 0x00F36F10 / 0x00F36F30 "...2". They are
    StartTextureUse / EndTextureUse and StartSurfaceUse / EndSurfaceUse (header comment: entry +4 texture, +8 surface; Ego_r donor order).
  - The texture-handle fields that the stand-ins typed `void*` are typed `long` (the handle EndTextureUse takes) in 0x00B5BD30,
    0x00B6D630, 0x00B869A0 and 0x00B86A30.
  - 0x00B5C050, 0x00B5EE80 and 0x00B898D0 lost their local CEngineComponent model; the shared 0xC-byte class replaces it. In two of
    them, the derived class's `_pad_0x08[4]` (the old base was 8 bytes) became the base's RenderEnabled.
- **GPEngineWorldMap (entry 34):** 0x00B3F5E0, 0x00B40BD0 and 0x00B52DD0 opt into FABLE_RETAIL_CENGINEMAP_GPENGINEWORLDMAP. The repo
  header itself declared the wrong `extern g_pEngineWorldMap` (hdr/README.md).
- **GPRenderManager (entry 29):** 0x00B903D0 and 0x00B67510 were already on engine/CRenderManagerCore.h. They now include
  engine/CRenderManager.h and read NEngineInternal::GPRenderManager. CRenderManagerCore is at offset 0 of CRenderManager2D /
  CRenderManager, so the calls are unchanged.
- **GPOcclusionManager (entry 30):** 0x00B2F8E0 and 0x00B2FA40.
- **GPShadowRenderer (entry 31):** 0x00B99B50, through the FableWin inline PeekRenderingTechnique.
- **GPCamera (entries 37 / 79):** 0x00B242A0 is an extern respell; the type was already CEngineCamera. The row's one-method
  CEngineCamera view is pre-existing and not ported. Round 3 already queued this row as a plain respell (deferred then: held by NF37). Hold it
  if the coordinator wants the view ported first.
- **CString::operator= (entry 103):** 0x00C31230 and 0x00C313C0 moved their local `struct CString` onto engine/CString.h.

## Descriptive / made-up names
- `ClearVector(C2DVector&)` (row 0x005339B0, a file-local `static inline` helper): descriptive. It replaces the row's local
  `NUISystem::C2DVector::Clear()` member, which neither PDB has. The row note says the zeroing must go through an inline function on the
  vector reference.
- No new class models. The typedef aliases (`CStateMap`, `CUIStateMap`, `RetailUI::Vec` / `State` / `StateMap`) all name shared-header
  types.
- PKCBufferedReader::ReturnAllBuffers stays NF42's descriptive libpk name. The STLport map / list spellings are descriptive-by-library
  (STLport 4.6.2, as NF42).

## Self-leaf changes (RENAMES.tsv section 1) and caller notes
- **0x0042BEAD:** `?Add@T@@QAEXH@Z` became `?AddComponentToAvailableWeaponsEntries@CTCInventoryWeapons@@QAEXPAVCComponent@NUISystem@@@Z`
  (both PDBs). Its one landed caller, 0x0041E6D3, already binds the new name. The old test only prints its PassPattern.
- **0x00533B30:** the catalog name `vfunc_50` becomes GetCurrentState. The leaf return type is now V NUISystem::CUIState (Ego_r 0x005AC4C0;
  _fablewin_api.tsv 0x017E8C77 / 0x022999E0). It has no direct landed caller (vtable slot 50).
  - CONFLICT: the catalog also labels 0x005BC0C6 and 0x004DFBC6 NUISystem::CComponent::GetCurrentState. Those are 13-byte stubs that
    cannot be this virtual (probably propagated labels). Check them before landing the name.
- **0x005518E0 / 0x00551BC0:** leaf only. The catalog names are unchanged and the rows stay in the RetailUI placeholder namespace (ROW
  DEBT), but the position parameter now mangles as ::C2DVector. They have no landed caller (vtable 0x0124A460 / 0x0124A464).
- **RENAMES.tsv section 2:** 66 changed relocation bindings, as `0xROW<TAB>old<TAB>new<TAB>0xTARGET` (the target is a fourth column), from a fresh compile of each
  original and each new source. Label (`$L`) renumbering is omitted.
- **0x0053BF20:** its call at 0x0053CFC2 (std::vector<map<unsigned long,CUIState>>::push_back) is spelled through a local
  `CScrollLayout::AddChild(CStateMap&)`. The parameter type now mangles as the STLport map, so the bound spelling changed from one wrong
  spelling to another.
  - Not fixed: the landed self row 0x0053CFC2 spells itself `?push_back@?$vector@VCUIStateMap@std@@@std@@QAEXABVCUIStateMap@2@@Z`, a
    local one-parameter model. Binding the full STLport spelling would still disagree with it.
  - Follow-up for the coordinator: respell 0x0053CFC2 itself, then its 4 callers (0x0053B91E, 0x0053BEAC, 0x0053BF20, 0x0053C332).

## Header overlay
See hdr/README.md. It holds 3 files and 5 opt-ins. CEngineMap.h also routes its nine global reads through `FABLE_CENGINEMAP_WORLDMAP`,
which is token-identical for every existing includer.

## Rule slips and deviations (work/slips.txt)
- One Bash call was refused by the harness: a foreground `sleep 200` used to wait for the probe. Nothing ran.
- Probable non-zero exits; the harness showed no error, but neither call guards its last command with `; true`:
  - a test lookup `cat rebuild/tests/*0042bead*_test.cpp || ls rebuild/tests | grep -i 0042bead`, whose grep likely matched nothing;
  - a trailing `grep -n "Leaf" ..` on two rows that have no Leaf line.
- **Deviation from "opt-ins only":** the CEngineMap.h overlay also rewrites nine body lines (`g_pEngineWorldMap->` became
  `FABLE_CENGINEMAP_WORLDMAP->`, which is token-identical without the opt-in). Recorded in hdr/README.md. No cast addresses. No `__asm` / `_emit` / `naked` / `register` in any changed row or test, and no
  `volatile` added.
- The 0x00B5BD30 / 0x00B6D630 / 0x00B869A0 / 0x00B86A30 handle-type edits are inside pre-existing local models of classes that have
  shared headers. No new member was added; these models remain row debt.
