# Landing 2026-11-04y: chunk 761 (53 unworked rows from the groups ledger), 47 landed

## Result
- **Landed 47 of 53** (`logs/land.log`), each graded read-only on the merged tree first (`logs/new2.log`): RELOCATION_MATCH + PASS.
  39 matched on the first grade.
  - The rows are mostly destructors and deleting destructors, plus small members, a few `_Insert_n` / container bodies, and
    libpk audio (CReverbProcessor, CCodaFrameStreamer, PKRAC objects).
  - One row is strmbase (DirectShow base classes) code, as are 253 already-landed sources.
  - Attempts per row are in `STATUS.tsv`, and what fixed each one is in `agent_NOTES.md`.
- **0x00C78150 CReverbProcessor::UpdateTapDelays is grade `retail_asm`.** Retail rounds with three bare `fistp dword` sites, and
  the row calls `FtoL` from the shared `libpk/FtoL.h` exactly as the landed 0x00C3FF70 does.
  - The row source and its test have no asm, and the source carries the `// Retail-asm:` evidence line.
  - NOTES.md has the per-site "C++ attempts before helper" table: 11 spellings at each site, which give `__ftol2`, `fistp qword` or
    `cvt(t)ss2si`.
- **Other rows:** no asm, helper, pragma or `volatile`.
  - A test identifier `M_emitter` contained `_emit`. It was renamed to `EmitterMember` before landing (GOTCHAS).
- **Not landed (6;** `pending/2026-11-04_linux_hand761/`):
  - **0x00C89850 CReverbFilterTap::ProcessCrossfade:** DIFFER(607v588) after 27 attempts. VC7.1 keeps the loop index, while
    retail bases the loop on `m_historyDelay`.
  - **0x00C54C10 CAudioSource::ApplyParameters:** DIFFER(651v651) after 47 attempts, 3 diff lines. VC7.1 reorders the
    unparenthesised float multiply chain.
  - **0x00C8E2E0 / 0x00C95A10:** third-party libvorbis (`_vorbis_unpack_books`, `_best`). Not attempted.
  - **0x00D6BB91:** a catalog bounds error. It starts 0x11 bytes inside the landed SpawnEnemies 0x00D6BB80, so the row is to be
    retired.
  - **0x009AA440:** a byte-identical duplicate COMDAT of `pair<const CCharString,CBankInfo>::~pair` 0x009A8FE0. It grades
    RELOCATION_MATCH with the same source, but it waits on a twin-record decision.

## Headers (12 changed + 1 new; each behind a new opt-in FABLE_RETAIL_* macro)
- New macros: C3DANIMATIONINFO, CCHUNKEDFILERAWDATACHUNK, CDECAL, CDEFPOINTER_RESET, CPIXELSHADERDATA, CRENDERMANAGERCORE_SYSTEMFONT,
  CRESOURCE_RESOURCELIST, CTCHIGHLIGHTITEM, CTCINVENTORYCLOTHING_MENUENTRIES, CTCMAPWHO, CTHINGMAPMANAGER and CWORLDMAP_THINGMAPMANAGER.
- `engine/CSystemFont.h` is new.
- hdrcheck2: `default=same` for every changed header. CDefPointer.h is `same` with FABLE_RETAIL_DEF_CLASS_BASE, without which it
  has no default path.
- `ppcmp.py`: all 1,475 includers same. The agent also rechecked 46 landed includers with `--recheck-landed`, and all passed.

## Names
- **Labels:** `ghidra_out/labels_landing_2026-11-04y.tsv` (55). `apply_labels.py` made 33 catalog and 38 manifest renames.
- **Queued PRIORITY** (`land0411y`, 10), among others:
  - CBankFile::BeginUpdateEntries 0x009CE8D0;
  - CSoundEvent::GetPlaybackKind 0x00C206E0;
  - the CSoundEventSet ctor 0x00C222D0, which takes `JJJ` (three longs);
  - CStateVariableFilter::Reset 0x00C6B720.
- **Two CBaseObject classes:** strmbase's (`??1CBaseObject@@QAE@XZ` 0x00CACAF0) and the engine's (`??1CBaseObject@@MAE@XZ`
  0x0099A430). The landed 0x00AF10E0 binds the engine dtor with the strmbase spelling, which is the cause of its RELOC_CONFLICT warning.
- **Open naming decisions:**
  - one spelling for the three libpk master-object classes (CMasterObject_00c4c8b0 / _00c4cd80 / _00c4c7a0);
  - the class of 0x00C78150, which the dispatcher calls CEffectPart260 while its siblings use CReverbProcessor;
  - twin bodies: CBankInfo ctors 0x009A7670 / 0x009AA210, pair dtors 0x009A9010 / 0x009AA470, and
    `__uninitialized_copy<C2DLineF>` at 0x00513BB0 / 0x00A11210.
- `bindings_multi_check.py`: 0. `sync_manifest_from_catalog.py --dry-run`: 0 / 0 / 0.

## Coverage
46,751 genuine of 49,568; 5,829,763 function bytes (57.03%).
