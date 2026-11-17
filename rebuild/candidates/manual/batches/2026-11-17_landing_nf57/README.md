# 2026-11-17 landing nf57: name-fix round 7, NF57 (39 rows: follow-up entries whose name had to be settled)

Owner: "Continue the naming fix queue with two agents until complete" and "Keep going until queue is done".

NF57 took the 32 follow-up entries (`land1116fu` / `land1117fu`) whose correct name was not yet an exact decoration: descriptive
claims, `check` / `verify` claims and `settle: A vs B` cases. That is 79 rows: the entries' landed self rows, every landed binder of
their addresses, and callers named in the evidence. An interrupt stopped the first run after 18 rows; a fresh NF57 resumed from its
files. Brief, worklist and records: `naming/2026-11-17_agentNF57/`.

## Rows (STATUS.tsv: the 79 rows plus 0x0053BF20 / 0x0053CFC2)

| Status | Rows |
|---|---|
| ported | 39 |
| not-a-binder | 24: 22 confirmed by compiling the landed row (`work/nab_probe.tsv`), plus 0x0053BF20 / 0x0053CFC2, which NF56 handled |
| blocked | 18 (unchanged; each with a grade, the reason and what must be ported first) |

**Blocked rows:**
- **the six CMap rows of q10433** (0x0063A540, 0x0081FA20, 0x0081FA90, 0x008200B0, 0x00820100, 0x00820140): each models CMap locally.
- **0x00599E3F and 0x0069AD10:** about a dozen local models each.
- **0x008F7950:** a local NDust namespace.
- **libpk rows** 0x00C11F50, 0x00C12940, 0x00C21C20, 0x00C2E190, 0x00C2E2B0 and 0x00C49820: local models of CSoundEvent,
  CBankProcessor and friends, or address-suffixed stand-in classes.
- **0x00C3AB50 (authoring):** its class is the driver map libpk/CBankProcessor.h declares as the placeholder `CIndexMap_00c3abd0`.
  Renaming a header class is not an opt-in, so this waits for a header edit.
- **0x00B86C00:** needs a CRenderTarget header first. **0x008DF3A0:** emits a symbol its `// Leaf:` does not name; whole-row port.

## Entries (APPLIED.tsv, 32: each with the settled name and two sources)
- **18 done:**
  - q10406 / q10407: the 13-byte stubs labelled `NUISystem::CComponent::GetCurrentState` are STLport
    `std::_Destroy<CEngineTextureMorph*>` / `std::_Destroy<NPlayerTargeting::CTargetPoint*>`;
  - q10411 / q10412: the non-const `CChunkedFileSuperChunk::FindFirstSubChunkOfType` / `FindNextSubChunkOfType` (both PDBs);
  - q10421 / q10422 / (q10423 self): the private `InitialiseTextures` of the radial blur, glow and displacement renderers (AAE in
    both PDBs). Their `SetEffectQuality` binders now define each class's own virtual (retail vtable slot 13, the first after
    CEngineComponent's 13);
  - q10424: `CEngineWaterRenderer::SetupSeaBackgroundShaderConstantsAndFog` (not Midground: the body shares its statics with the
    Background helper inlined in 0x00B70230);
  - q10428: the Ego_r allocator spelling of `GFSerialiseVectorBinaryIn<CCharString>`;
  - q10429: `?pGlobalScriptDef@@3V?$CDefPointer@$$CBVCScriptDef@@@@A`; its atexit destructor 0x0122AE7E is now catalogued in the
    `` `dynamic atexit destructor for 'pGlobalScriptDef'' `` form;
  - q10430: `CTCAIScratchpad::Gossip` (GossipVillages is 0x013BAE50), the full STLport map spelling. Its initializer 0x0121F160 and
    atexit destructor 0x01229CA0 now come from one definition on the shared headers and are catalogued as its `_$E1` / `_$E2`;
  - q10438: STLport `_Rb_tree<EObjectAugmentationType, ...>::_M_insert` / `_M_create_node` (0x0076A110 / 0x00769980), the spelling
    the landed caller 0x0076A1E0 binds;
  - q10477: `CEngineShadowRenderer::UpdateShadowScene`, bound by its caller 0x00B444F0;
  - q10485: `??0CPKDecompressedStream@@QAE@PAVPKCFileManager@@PBGABUPKCBufferedReaderParams@@@Z` (0x00C62920);
  - q10487: the ctor at 0x00C3BB40 (catalogued as `__EH_epilog3`) is `CAudioBankTableReader::CAudioBankTableReader(CSegmentFile*)`;
  - q10490: three STLport helpers for `CCountedPointer<QuadricOptimiserInternals::COptimisedPrimitive>`;
  - q10492: `UpdateState(CSoundEventManager*, CStreamBuffer*)`; q10494: `HasWaveBankSegments` returns `long` over `class CSegmentFile`.
- **1 done-already:** q10479.
- **13 blocked**, each naming the rows left (q10403, q10423, q10432, q10433, q10436, q10440, q10441, q10482, q10483, q10484,
  q10493, q10495, q10496). q10440's self row 0x00594FA9 is ported as `CloseVirtualKeyboard`; only 0x00599E3F is left.
- **REJECTED.tsv (6 claims, no whole entry rejected):** `EnableInput` (no such member in either PDB; both name the body
  CloseVirtualKeyboard), the NF53 placeholder `CIndexMap_*` spellings, `SetupSeaMidgroundShaderConstantsAndFog`, the caller spelling
  of the CPKDecompressedStream ctor (the name argument is a wide string), and the Gossip stand-in names (`g_gossipVillagesMap`,
  `g_tree_013bae44`, ...).

## Descriptive names
- **Made up here:** `CBankSampleRecordReader` (the one-slot record reader of vtable 0x012B4D34, sibling of CWaveDataReader) and
  `CAudioBankTableReader`. NF57 searched the PDB name sets, the exe strings, the FSE manifest and the libpk headers. EgoCore and the
  sibling repos live on Windows paths this lane can not reach, so they were not searched (NOTES.md). They replace the barred
  placeholders `CUnprovenRecordReader_012b4d34` and `__EH_epilog3`.
- **Existing descriptive names reused:** `CSampleIndexMap` (libpk/CBankProcessor.h) and `CPKDecompressedStream` (landed rows).

## Rows and checks
- **Section 1 (31 rows; 32 file moves).** Two coordinator corrections:
  - 0x0076A110 / 0x00769980: NF57 put the whole `_Rb_tree<...>::member` in the name column with module `std`. The coordinator
    kept the catalog split NF50 used (module `std::_Rb_tree<EObjectAugmentationType, pair<const EObjectAugmentationType,
    CObjectAugmentationSoundSet> >`, name `_M_insert` / `_M_create_node`), so their catalog names are unchanged.
  - funclet 0x01216442 included 0x00C62920 under its old file name; the coordinator repointed the include (and its comment) to
    `CPKDecompressedStream_CPKDecompressedStream_00c62920.cpp`. funclet_includes_check reports 1,355 checked, 0 broken.
- **Recheck (`logs/recheck_part0..2.log`):** 48 graded from the repo after landing: the 39 rows plus the 9 funclets that include a
  changed parent (0x01214030, 0x01214049, 0x0121406C, 0x01214077, 0x01215910, 0x01215918, 0x01215920, 0x0121592B, 0x01216442).
  43 RELOCATION_MATCH and 5 MATCH, every test PASS. NF57's own final run agrees.
- **Checks:**
  - `naming/rebind.py` removed 82 binding lines and added 59;
  - bindings_multi_check is at its baseline (`NullCoord`).

## Headers (11 opt-ins in 7 files; lines added only, default path unchanged)
libpk/CBankProcessor.h (`CSAMPLEINDEXMAP_ADD`), libpk/CCodaFrameStreamer.h (`ONFRAMEDONE`), the radial blur (`_INITIALISETEXTURES`,
`_SETDEFAULTPARAMS`, `_SETEFFECTQUALITY`), glow (`_SETEFFECTQUALITY`) and displacement (`_INITIALISETEXTURES`, `_SETEFFECTQUALITY`)
renderer headers, CObjectAugmentationSoundSet.h (`_IMPLICIT_COPY`) and CFrontEndManager.h (`_GOTOPREVIOUSSCREEN`). Details:
`naming/2026-11-17_agentNF57/hdr/README.md`.

## Name fixes queued (`land1117fu`, 11)
From NF57's NAME_FIXES.tsv, among them:
- the 158 landed rows that bind `pGlobalScriptDef` (0x0143E90C) under a `CScriptDef*` or non-const spelling, and its `_$E1`
  0x01224C99;
- CAudioBankTableReader Load / LoadCriteria (0x00C3BB50 / 0x00C3BCB0), the SSampleIndexNode ctor, `CLockedRingBuffer::Pop`
  0x00C4A0E0;
- `__destroy_aux` instances 0x004DBE58 / 0x005BBF80, the Gossip map `erase` / `_M_erase`, and
  `CEngineScreenEffectRadialBlurRenderer::SetDefaultParams` 0x00B5BBC0.

## Rule slips (NF57 `slips.txt`)
- First run: one heredoc exited 2 before S was set; one Bash call held an in-place sed aimed at a repo test, which the permission
  system denied before it ran (the repo file is unchanged).
- Resumed run: one read-only grep chain exited 1.
