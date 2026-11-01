# Naming pass wave 25 (2026-11-01): 75 landed rows respelled, all byte-identical (75/75 RELOCATION_MATCH + PASS)

## Inputs and index
- Inputs:
  - the brief naming/briefs/BRIEF_W25.md;
  - QUEUE_W25_input.tsv (the c692 rows, the DECISIONS_W24_RESOLVED per-row items, and the carried c689 / c690 / c691 / w20 rows);
  - DEFERRED_W24_INPUT.tsv.
- Index:
  - idx/full.jsonl is HEAD: the wave 24 after.jsonl plus the 67 rows changed since then (idx/delta_head.jsonl).
  - Before hand-back I re-checked against the repo tree by modification time (no git). Nothing in the repo outside .git is newer than idx/delta_head.jsonl, and there is no labels_chunk693: chunk 693 had not landed.
  - idx/after.jsonl is HEAD plus ov_final (75 rows, idx/ovfinal.jsonl).

## Commands
Run from $S/naming/w25 with the lane env sourced:
1. `python3 scripts/compose25.py ov_final` runs the hand stages scripts/hand25/h25_<fam>.py on fresh repo copies and writes runs/<fam>.rows.txt and ov_<fam>/.
2. `bash scripts/recheck.sh final ov_final $PWD/runs/final.rows.txt 4`; the result is logs/recheck_final.log.
   - That log covers 76 rows. 0x009F31C0 was withdrawn afterwards (see pixfmt and D7).
   - The other 75 overlay files are md5-identical before and after the withdrawal (runs/prev/ovfinal.md5 vs runs/ovfinal.md5); see logs/recheck_final.NOTE.txt.
3. The generator chain, in order:
   - `qscan25 idx/after.jsonl runs/qscan_final.tsv`
   - `mkq25` (with mkq25_notes)
   - `mkdef25 idx/after.jsonl`
   - `mkbind25 runs/rk_final.pairs.jsonl idx/after.jsonl runs/final.rows.txt`
   - `mkren25 $PWD/idx/after.jsonl`, then `fixren25`
   - `mklabels25`, then `fixlab25`
4. `bindings_multi_check.py runs/reloc_bindings.after.tsv` reports 0 (logs/bindings_multi_check.log).

New helpers:
- scripts/who.py lists every bound spelling at a target and the rows naming it.
- scripts/pxb.sh compiles one row's overlay source and prints the errors.

## Families
Listed in commit order.

### oak (2 rows): 00ee8100 00ee8c10
- The class is NScript::CCS_OakValeRevisitedScript, on the wave 16 NScript model: ctor (CScriptDataBase*, CGameScriptInterfaceBase&) and a virtual dtor.
- 0x00EE8C10 is its ??_G. 0x00EE8100 is its ctor; CQuestScriptObject takes (CScriptDataBase*, CGameScriptInterfaceBase&).
- BINDINGS adds ??1...@NScript@@UAE@XZ at 0x00EE8C30 and the vftable at 0x012F53C0.

### pkc (1 row): 00c09920
- The row is ?GetChecked@?$PKCAutoDelete@VCUnprovenPKObject_00c099f0@@@@QAEPAVCUnprovenPKObject_00c099f0@@XZ, the class template the Release row 0x00C099F0 instantiates (D6).

### cfl (1 row): 00aa7540
- The row is ??1CChunkedFileLoader@@UAE@XZ. It was ??1T@@QAE@XZ.

### smp (5 rows): 00987f10 00b3b8e0 0122ab10 0122ab30 0122ab50
- ClearMemoryBlocks is private (AAE, the Ego_r spelling).
- 0x00987F10 is the out-of-line ~CSimpleMemoryPool (FableWin.pdb, plus vtable 0x0129A064 / RTTI).
- 0x0122AB10 / AB30 / AB50 are the _$E2 atexit destructors of MatrixBufferPool / VectorBufferPool / GroupVectorBufferPool.
  - Ego_r and FableWin both list them, and the initialisers 0x01224450 / 70 / 90 register them.
  - Each row compiles the initialiser row's definition.
  - RENAMES moves them off ClearCachedDefs.

### ibuf (4 rows): 00ac2070 00b6d480 00b6db80 00bc5420
- CIndexBuffer::Uninitialise (was Release) and CVertexBufferWin32::Uninitialise.
- The _Retail / Native / address stand-ins become the PDB classes, using the `#define X XLayout` header alias.
- 0x00BC5420 is ??1CEnginePrimitiveManagerParticleLineGroup@@UAE@XZ.
- 0x00B6DB80 gets the Leaf ?CloseStaticMap@CEngineWaterRenderer@@QAEXXZ.
- The shader _Retail stand-ins in 0x00BC5420 remain (DEFERRED).

### esb (7 rows): 00cdf620 00d864b0 00d9c2b0 00e05360 00e76920 00efb8c0 00efe8d0
- Each entity derives from an empty global CEntityScriptBase { bool IsActiveThreadTerminating() const; } (QBE, Ego_r). Explicit vptr and fields are kept, so the layout is unchanged through EBO.
- IsDone becomes IsActiveThreadTerminating.
- SetPredicate moves into the base for 0x00D864B0 / 0x00E05360.
- Left: 0x00EE76F0 / 0x00D9F270, which need the NScript predicate re-model (DEFERRED).

### peek (5 rows): 006af630 006afd40 0076c810 0077db90 007e00d0
All five are re-authored over the empty-base CTCBase::PeekWorld:
- 0x006AFD40 is CTCBase::PeekThingSearchTools.
- 0x0076C810 is CTCEnemy::SetFaction (Leaf added).
- 0x0077DB90 is CTCHeroCentre::OnHeroWalkedThroughDoor (Leaf added). It calls 0x0077D4B0, ?OpenTrainingScreen@CTCHeroCentre@@QAEXXZ (ADD).
- 0x006AF630 has an unknown owner.
- 0x007E00D0 is CTCTeleporter::FrameUpdate.

### coh (3 rows): 00681940 00681980 006819d0
- The Ego_r / FableWin donor order fixes three misnames:
  - 0x00681940 is GetToleranceToBeingHit;
  - 0x00681980 is GetToleranceToBeingHitRemaining (was CViewBase::CalculateDisplayDistance);
  - 0x006819D0 is OnHit(bool).
- RENAMES and LABELS follow.

### bcnc8 (8 rows): 0042db40 00523540 0055d070 00770e50 00778ae0 0077b250 0077b9b0 0077be40
- Each row's local CBaseClassNonCopyable declares the PDB protected virtual dtor (MAE).

### pixfmt (39 rows)
- Every row declares void Initialise(_D3DFORMAT) on CPixelFormat with the inline format ctor.
- The stand-ins CPixelFormatFromD3D, Native, _Methods, PixelFormat, PixelFormatMethods and CGlowPixelFormat are folded.
- 0x009F3FF0 / 0x009F4120 / 0x009F4190 now bind ConvertRGBToPFDData.
- 0x00A681F0 is ?GetLevelByteLength@CVolumeTexture@@QBEJJ@Z.
- 19 rows gained a Leaf line.
- 0x009F31C0 was withdrawn and its file left on HEAD. Its WriteColour takes CRGBColour by value, so keeping it would have bound the non-PDB ?WriteColour@CPixelFormat@@ to 0x009E3F60 (D7).
- 0x009F9EE0 is left: its stand-in lives in the shared fable_texture_lifecycle.h.

## Hand-back files
- BINDINGS.tsv: 46 RETIRE, 3 ADD, 6 KEEP, 0 CONFLICT. runs/reloc_bindings.after.tsv is the repo table with these applied.
- RENAMES.tsv has 12 renames (repo-rooted paths; new_name is the catalog's bare spelling). The pixfmt caller rows keep their files (fixren25).
- LABELS.tsv has 13 labels, including:
  - 0x00F35B30 CEntityScriptBase::IsActiveThreadTerminating, superseding the IsDone labels;
  - 0x00AA7540 ~CChunkedFileLoader, superseding the ctor label;
  - 0x0077D4B0 OpenTrainingScreen;
  - 0x0122AB10 / 30 / 50, the atexit destructors.
- QUEUE_W25.tsv outcomes: respelled 9, respelled-partial 3, done 8, hold 6, deferred 16.
- DEFERRED.tsv: 15 queue entries, 9 new wave-25 findings (runs/deferred25_manual.tsv) and 443 carried entries. Scope 2 (per-row re-models) and scope 3 (wave 24 deferred families) were not reached and stay deferred.
- DECISIONS_W25.tsv has D1 to D7.
