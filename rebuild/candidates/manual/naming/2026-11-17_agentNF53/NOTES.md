# NF53 notes (round 7): engine-side tail of the name-fix queue

The slice has 78 entries (ENTRIES.tsv) over 101 worklist rows, plus 2 extra self rows (0x009ADE10 nf44:80 and
0x00A23C60 nf41:e188), so STATUS.tsv has 103 rows. Every count below is taken from the files.

## Outcome
- **Rows (STATUS.tsv):** 58 ported, 7 not-a-binder, 38 blocked. No row is pending or carry.
  - Ported: all 58 pass one vland `--recheck-landed` run (`recheck_final.log`): 55 RELOCATION_MATCH + PASS,
    2 RELOCATION_MATCH[/EHsc] + PASS and 1 MATCH + PASS (0x00A23C60). Its WARN lines do not fail a row; each was checked:
    - 57 RELOC_NAME (binder symbol differs from the target's catalog label). 35 are bindings the row did not change
      (e.g. the `CString::operator<<` family, whose labels are just written in demangled form). 14 are section-2
      respells to a symbol that some landed row already binds (mostly the section-1 self spellings, such as
      GetPDefFromClassIndex). 8 are new spellings from this pass, all in RENAMES section 2: 0x0095C7C0, 0x00A23C60 and 0x00C1F4E0 are entry targets; 0x00AD1690 and 0x00B8E690 are already
      follow-ups; 0x00C31D80, 0x00C1EEE0 and 0x00C3BB40 are new follow-ups (below); 0x00C1F4E0 and 0x00C1EEE0 are
      also split bindings (below).
    - 45 SHARED_HEADER_AVAILABLE: every warned class is a local model the repo row already had (checked per row
      against the repo source); this pass added none. The one new local, `CSoundPlayParameters` in 0x00C119D0,
      has no shared header (see Claim decisions).
    - 6 STANDIN_LEAF: 0x009ADE10, 0x00A23C60, 0x00B86BB0, 0x00C21BA0 and 0x00C21C20 are section-1 renames or
      kept selves whose catalog label still names the old function; 0x00C119D0 has an address-only catalog name.
  - Of the 38 blocked rows, 3 are deferred to NF52: funclets 0x01213BC8 and 0x01213BDE (parent 0x00C2DBE0 is
    NF52's) and 0x00C2E080 (depends on NF52's ~CSegmentList). 30 are "port not attempted" (1 probe grade only;
    see below), 3 have the binding solved but not the bytes, 1 is an authoring row (0x00C13D20) and 1 is the
    0x008DF3A0 Leaf defect.
- **Entries (APPLIED.tsv):** 39 done, 37 blocked, 2 rejected (REJECTED.tsv: nf44:120 and nf44:122).
- **RENAMES.tsv:** 26 section-1 self renames and 93 section-2 binding respells. Each section-2 line comes from an
  `xr.sh` relocation diff of the changed object against its probe.
- **Grades:** 199 row grades in total (STATUS.tsv `grades_used`; each matches work/grades.log per row), at most 10
  per row (most is 9, the blocked 0x008F95D0). work/grades.log has 24 more lines that are not row grades: 14
  caller probes, 5 cpp prechecks that failed before compiling, 4 funclet grades and 1 probe of 0x00C3AA50. The
  vland rechecks are not counted either: the final run, and the funclet runs in work/vl_fun*.log (funclets, fun,
  fun3, fun4, fun5, and the two baselines funbase and funbase2). One compile at a time throughout.
- **Locks:** released for every row this pass did not change, including a stray lock on 0x00C3AA50. 58 NF53 locks
  remain, one per changed row.

## Header opt-ins (hdr/, documented in hdr/README.md)
Every overlay is a copy of the current repo header with lines added only (`diff repo overlay | grep '^<'` is empty
for all 10 files).
- CPlayerGui.h: `_PEEKGLOBALDEF`.
- CBankFileAsync.h: `_ASYNCUPDATEPENDING`.
- CEngineResourceManager.h: `_PROCESSLOADING_UNTIL`.
- CMeshDataBank.h: `_PROCESSLOADINGQUEUE`.
- CEngineShadowRenderer.h: `_UPDATESPACE` and `_PACKED_VECTORS`. The second is the only layout opt-in.
- C3DMeshInfo.h: `_ARETEXTURESLOADED`.
- CEngineTreeFiler.h: `_LISTTREE` and `_FINDNODE_DECLONLY`.
- New this session:
  - CContainmentVolume.h: `_MOVABLERESOURCE`, the RTTI base, no layout change.
  - CEnginePrimitiveManagerStaticMeshesBase.h: `_PROTECTED_TAIL`, access only.
  - CCreatureAction_ScorpionPrepareTailPlunge.h: `_FINISHACTION`, declaration only.
- Two draft opt-ins were withdrawn with their rows and are not handed back:
  - `work/drafts/NEntityEvents_EventFilters.h.draft` (CIsEventOfType / CConvFilterToCP, for 0x008A7980);
  - `work/drafts/CCodaFrameStreamer.h.draft` (CLockedRingBuffer::Pop, for 0x00C497E0).

## EH funclets (regraded against the changed parents)
Nine landed funclet rows `#include` a changed parent:
- 0x01212A58 (parent 0x00C21BA0);
- 0x012115F0 and 0x012115F8 (parent 0x00C119D0);
- 0x01216442 (parent 0x00C62920);
- 0x0121406C and 0x01214077 (parent 0x00C30F60);
- 0x01211E25 and 0x01211E30 (parent 0x00C172F0);
- 0x01213C1A (parent 0x00C2E610, ported last; graded at +0x0 of the parent's block, `work/vl_fun5.log`).

They were regraded through vland with their `funclet_offset` (`funclets_recheck.log`, run by `work/vlfun.sh`).
- Seven are RELOCATION_MATCH[/EHsc] PASS. 0x01213C1A is RELOCATION_MATCH PASS (`work/vl_fun5.log`, copied
  verbatim; its repo row carries `// Flags: /EHsc` and the parent's leaf, so no flag retry shows in the grade;
  without the funclet offset the same TU gives DIFFER(395v18) against the 18-byte oracle, `work/vl_fun4.log`).
- 0x01211E25 is **unverified**: it gives OBJDUMP_ERR, and its unchanged repo parent gives the same OBJDUMP_ERR in
  this harness (baseline run `work/vl_funbase2.log`). So the port did not change its result, but its bytes were
  not checked either way. The cause was not found.
- No funclet source changed and none is handed back.

## Claim decisions (libpk / descriptive claims; policy: two sources, body before rejection, no GameCODA-only)
- **Accepted, because the claim equals the landed target's own leaf:** nf44:75, 89, 90, 91, 107, 111, 112, 114.
- **Accepted on the shared libpk header plus Codex pass23 NAMING_EVIDENCE:**
  - nf44:110 CTestToneAudioStream (table 012bf014, ctor C73EF0);
  - nf44:113 CEventTriggerRef::InsertDriver (libpk/CBankProcessor.h; the 76-byte dispatcher returns eax);
  - nf44:117 RunBankProcessor (the landed caller 0x00C17640 spells it);
  - nf44:119 ReadBankInfoCount(class CSegmentFile*) (libpk/CScriptHost.h);
  - nf44:123 PKCAutoFile::GetChecked (retail PKCAutoFile.cpp assertion; libpk/PKCAutoFile.h).
- **Accepted for nf44:92:** IsAudible returns bool. libpk/CSoundEventGroup.h says so, and both callers test `al`.
  The landed 0x00C20C10 returns long; see the follow-ups.
- **Rejected nf44:120:** the retail trace "...\libpk\PKSerialiseLoad.h" line 34 names the template, and the landed
  0x00C17940 is `PKSerialiseLoad<CBankData>(IRemoteSource*)`.
- **Rejected nf44:122:** 0x00C0DF20 calls 0x00C0DE50, which sprintf()s `"%p"` (0x012B21DC), so the argument is
  `const void*`. The claimed `const unsigned short*` is wrong.
- **Engine claims nf44:11–67, 81, 97, 126:** each checked against Ego_r and/or FableWin name sets plus the body or
  layout. Two had no exact PDB symbol:
  - nf44:64 (0x00AD37E0): the Ego_r COptimisedSubMesh::Primitives is `CArray<CCountedPointer<COptimisedPrimitive>>`,
    and that vector's push_back instance is in the PDB.
  - nf44:67 / 97: STLport 4.6 `_M_clear` → `__destroy_aux`, and the CVectorMap<ulong,ulong> Sort/Update PDB
    instances.

## ICF and identity notes
- 0x009E45D0 is not-a-binder: an ICF fold, not respelled from one caller's guess.
- 0x009B89B0 and 0x009B8AD0 are not callers of the sort. They are CVectorMap<ulong,ulong>::Sort and Update. The
  PDB has both, and the bodies match engine/CVectorMap.h. They were rewritten as explicit instantiations (section 1).
- 0x00C1EA60 is CSoundSample::LoadData: the catalog label says so, and the landed caller 0x00C21780 binds that
  spelling. The row was spelled CTCCreatureNavigation::IsPosNavLineOfSightTo.
- 0x00C21BA0 and 0x00C21C20 are CSoundEvent's ctor and Initialise according to libpk/CSoundEventGroup.h (vtable
  0x012B3674). Their owners stay descriptive (follow-up).

## Descriptive (made-up) names introduced. No source has the original; each is used consistently.
- `CSoundSample::ReadPoolData` (slot 2 of the libpk sample, 0x00C1EA60).
- The local `CSoundPlayParameters::GetCategory`, reusing the landed 0x00C12940 spelling. There is no shared
  header, so this is not a new name.
- `CCRCCompare`, a typedef of `CKeyPairCompareLess<ulong,ulong>`. It is a typedef, not a name in any symbol.

## Blocked rows and why (STATUS.tsv has the reason per row)
- **The binding is solved but the bytes are not:**
  - 0x008A7980: 8 grades. A frame-slot residue: the CIsEventOfType temporary never shares the dead `script`
    argument slot. Drafts are in work/drafts.
  - 0x008F95D0: 9 grades, DIFFER(276v263).
  - 0x00C497E0: the class key of libpk CCodaFrame clashes with the landed Pop row, so porting would add a new wrong
    binding.
- **Port not attempted (30 rows, 1 probe grade each):** the probe shows the wrong binding, and the fix needs a
  port of the self or callee chain onto shared headers, which this pass did not try. STATUS.tsv says "port not
  attempted" for each. They include:
  - engine rows 0x00BD1F70 (24 local classes), 0x00A25BD0 and 0x00A42980;
  - the libpk CSoundEventManager / CBankProcessor families: 0x00C11F50, 0x00C120A0, 0x00C12460, 0x00C12520,
    0x00C2EB00 (self is the header's CBankProcessor::Load), 0x00C2E4D0 and 0x00C2E7A0 (AddNumberTriggers /
    AddEventTrigger are not declared in libpk/CBankProcessor.h, so they need overlay opt-ins), 0x00C08D40 and
    0x00C17400;
  - the sub-dozen-class rows 0x009C8890, 0x0073A8E0, 0x00B3FF70, 0x00B4F420, 0x00523860, 0x0085F6F0, 0x00A79A30,
    0x00966290 and 0x00915190. 0x00A79A30 (9 local classes; the self ctor takes a CRegionList stand-in for
    `std::list<CNavRegion>`) is the most promising.
- The advisor asked for one real diagnostic grade on each sub-dozen-class row. I did that for 0x00B936E0 and
  0x009BF350, and both are now ported. 0x00C2E610 was also ported late (libpk/CBankProcessor.h with
  BANK_SAMPLE_TRIGGERS_PASS23, 2 grades), so nf44:113 has 1 of its 3 binders done and stays blocked.

## Follow-ups (new queue lines; not changed here)
Callers probed for every section-1 rename are listed in `work/s1callers_probe.tsv`.

| caller row | target | binds now | should bind |
|---|---|---|---|
| 0x009F6890 std::vector<T>::operator= | 0x009F6720 | ?DestroyRange@@YIXPAUCElement34@@0@Z | ?_Destroy_Record34@@YIXPAVCInputEvent@@0@Z |
| 0x00B3B870 CEngineResourceManager::FrameEnd | 0x00B3B7F0 | ?ProcessLoading@CEngineResourceManager@@QAEXJN@Z | ?ProcessLoading@CEngineResourceManager@@QAEX_NN@Z |
| 0x00B3B8C0 CEngineResourceManager::ProcessLoading (virtual) | 0x00B3B7F0 | ?Do@SubObj@@QAEXHN@Z | ?ProcessLoading@CEngineResourceManager@@QAEX_NN@Z |
| 0x00B444F0 CEngineLightingManager::UpdateShadowScene | 0x00B4F390 | ?UpdateShadowScene@CEngineShadowRenderer@@QAEXPAXJ0@Z | the section-1 spelling of 0x00B4F390 |
| 0x00B9E950 (NF52-held) | 0x00A23C60 | ?IsBuilt@C3DMeshInfo@@QAE_NXZ | ?AreTexturesLoaded@C3DMeshInfo@@QBE_NXZ |
| 0x00B687D0 CEngineLandscapeRenderer::BuildThemes | 0x00B6BD40 | unverifiable (old candidates-header row, no // Leaf:) | GetDef<CEngineThemeDef> |
| 0x00C3AA50 (self, unchanged) | - | ?SetRange@CSegmentFile@@QAEJKK@Z | ?SetRange@CBufferedRangeReader@@QAEJKK@Z (libpk/CSegmentFile.h) |
| 0x00C17260 LoadBank | 0x00C30F60 | ?ReadBankData@@YIXPAXPAVCBankData@@KPAVCLoadLog@@@Z | ?ReadBankData@@YIXPAXPAUCBankData@@KPAVCLoadLog@@@Z |
| 0x00C16D90 | 0x00C30F60 | ?FillEntry@@YIXPAVCBankFile@@PAUCBankData@@KPAVCLoadLog@@@Z | the section-1 ReadBankData spelling |
| 0x00C12940 | 0x00C21C20 | ?Initialise@CSoundEvent@@QAEX...@Z (better than both self spellings) | respell the self to the caller's CSoundEvent::Initialise |
| 0x00C2E2B0 | 0x00C3AB50 | ?Add@Map@@QAEXPAUContext@@PAXI@Z | the section-1 spelling of 0x00C3AB50 |
| 0x00C2E190 | 0x00C3B0E0 | ?Add@CSampleMap@@QAEXPAUCLoadLog@@PAXK@Z (struct CLoadLog key) | the section-1 spelling of 0x00C3B0E0 |
| 0x00C47F10 | 0x00C62920 | ??0CPKDecompressedStream@@QAE@PAVIStreamOpener_00c62920@@... | the section-1 spelling (CPKDecompressedStream is the better self name) |

| 0x00C31D80 (self row `global_EHepilog3_00c31d80`) | - | 0x00C172F0 now binds ??0PKCAutoFile@@QAE@PAVCAudioFile@@@Z (libpk/PKCAutoFile.h) | respell the self to the PKCAutoFile ctor |
| 0x00C3BB40 (self row `global_EHepilog3_00c3bb40`) | - | 0x00C30F60 now binds ??0AudioBankTableReader_00c3bb50@@QAE@PAVCSegmentFile@@@Z | respell the self to that ctor |
| 0x00C1EEE0 (self row CItemAction::CItemAction) | - | 0x00C2E610 now binds ??0CEventTriggerRef@@QAE@PAVCNamedSet@@VCString@@_N@Z | respell the self to the CEventTriggerRef ctor |

Split bindings left by the partial nf44:113 port (the coordinator must keep 0x00C2E610's section-2 lines even
though nf44:113 stays blocked):
- 0x00C1F4E0 InsertDriver: 0x00C2E610 binds `?InsertDriver@CEventTriggerRef@@QAEJKPAX@Z`; the blocked 0x00C2E4D0
  and 0x00C2E7A0 still bind their old local spellings until they are ported.
- 0x00C1EEE0 CEventTriggerRef ctor: 0x00C2E610 binds the `PAVCNamedSet` form; 0x00C2E7A0 still binds
  `??0CEventTriggerRef@@QAE@PAUCItemDispatcher@@VCString@@_N@Z`.

Provisional section-1 self names (a landed caller spells them better; see the table): 0x00C21C20 (caller 0x00C12940
says CSoundEvent::Initialise) and 0x00C62920 (caller 0x00C47F10 says CPKDecompressedStream).

Other follow-ups:
- 0x00AD1650, 0x00AD1690 and 0x00AD37E0 should take the self names of the
  `CCountedPointer<QuadricOptimiserInternals::COptimisedPrimitive>` instances that 0x00AD68D0 now binds.
- 0x00B8E690 (gapscan "DeleteIfSet") should be `?DeleteData@?$CCountedPointer@VCVertexBufferWin32@@@@SIXPAX@Z`.
- 0x00C1D950 declares slot-1 UpdateState(void*, void*). It should be UpdateState(CSoundEventManager*, CStreamBuffer*).
- 0x00C20C10 IsAudible: the landed body returns long. Its callers and the header say bool.
- 0x00C30E90 HasWaveBankSegments is landed as bool over a struct CSegmentFile. Its caller declares it long over the
  class.
- CCodaFrame class key: header `class` against the landed Pop `struct`. Once one is chosen, 0x00C497E0 can be ported.
- engine/CString.h line 29 calls 0x00C0DF20 a "wide appender". It is the `%p` pointer appender.
- The 008df3a0 Leaf defect (from earlier in this session) stands.

## Rule slips (work/slips.txt)
- Five non-zero Bash exits (the `dis.py` shadowing below is the cause of the IndexError, not a sixth exit):
  - an `ls` of a missing header (exit 2);
  - a side-by-side python IndexError (exit 1);
  - a `cat` of a missing lock owner at the end of a chain (exit 1);
  - an `ls` of NOTES.md / LESSONS.md before they existed (exit 2);
  - cause of the IndexError: a helper named `dis.py` that shadowed the stdlib (renamed rdis.py);
  - a `grep -c` with no match at the end of the probe-row relabel chain (exit 1).
- Duplicate TIMEOUT lines were written to grades.log and then removed.
- No git, no writes outside `$S/w22/agentNF53` or `$S/w22/nflocks`, and no NF52-locked row touched.
