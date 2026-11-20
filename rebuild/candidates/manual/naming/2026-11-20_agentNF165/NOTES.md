# NF165 notes (round 10, sound / stream naming; 21 entries)

Counts come from APPLIED.tsv / STATUS.tsv / recheck_final.log (see the report for the final tallies).

## Descriptive names (made up: no original found)
- libpk has no PDB / RTTI. Kept or applied descriptive names, one per address:
  - CSoundPlayParameters::GetStreamed 0x00C1BAE0 and CSoundPlayParameters::CSoundPlayParameters 0x00C1BD10 (catalog / the binder
    0x00C12940 already used them).
  - CPKVoiceParams::GetTrackBit0/1/2 0x00C27B70 / 80 / 90 (binder 0x00C12940 already used them).
  - GetStreamBlockOffset 0x00C1E970 / GetStreamBlockTail 0x00C1E990: the names libpk/CStreamBlockReader.h already declares.
- GameCODA rows keep the D3 descriptive names on CEAXListener (SetEffectProperty_00c68f90, RemoveSource). The SDK names are now
  known (EAX4FXSlot, DeleteBuffer) and are in NAME_FIXES.tsv. Renaming them needs a header claim (below).

## Needs header claim (coordinated `header` items; not applied)
- engine/CEAXListener.h: rename `SetEffectProperty_00c68f90` -> SDK `int EAX4FXSlot(unsigned long, unsigned long, void*, unsigned long, _CHANGES**)`
  (0x00C68F90). This unblocks one spelling for self 0x00C68F90 and binders 0x00C51EB0 / 0x00C52840 (both now on the header) and
  0x00C52980 (still a local CEAXListener model).
- `RemoveSource` -> SDK `int DeleteBuffer(CEAXBuffer*)` (0x00C5FF60). Declared in my overlay opt-in
  FABLE_RETAIL_CEAXLISTENER_REMOVESOURCE. Rows: self 0x00C5FF60, 0x00C60B90 (local CEAXListener model), 0x00C44170 (opt-in).
- q4779 / q4781 (voice family): NOT a libpk class. The SDK release members match retail (NAME_FIXES):
  - 0x00C5A1F0 CSalSwBaseVoice::Release
  - 0x00C3F490 ~CSalSwBaseVoice
  - 0x00C5A2A0 CSalSwBaseVoice::InitPlay
  - 0x00C5C290 CSalSw2DVoice::InitPlay
  - vtable 0x012B5C60 = CSalSwBaseVoice; base 0x012B5C10 = CSalBaseVoice
  engine/CSalSwBaseVoice.h already declares Release / InitPlay / the dtor. These four rows (plus the base dtor 0x00C3F3D0 binding)
  are one family rename onto the SDK headers. I did not apply the queue's CSoftwareVoice::Stop / Reset spellings, because they are
  superseded. A first reading here had blamed a missing libpk header; the advisor review corrected it with the SDK comparison.
- libpk/CSoundEventManager.h + libpk/CStreamBuffer.h members for the stream-buffer queues (+0x284 / +0x2B4 / +0x2E4 / +0x314),
  LRU stamp +0x35C, pool +0x360 and the CStreamBuffer layout + Attach / Release / FinishLoading. This unblocks q4897..q4899 (6 rows).
  0x00C12140 also needs the CSoundEvent decision.

## Findings outside my entries (NAME_FIXES.tsv)
- 0x00C54F30 is GameCODA CSalSw3DVoice::Mix. Also 0x00C54EA0 ProcessReverbMix, 0x00C5CF20 CSalSwBuffer::Remove.
- 0x00C51EB0 is CCodaEAX::SetEAX4FXSlot; 0x00C51A20 is CCodaEAX::ProcessChanges.
- 0x00C5CA60 catalog -> CSalSw2DVoice::CalcParams (858 bytes = retail length).
- Pre-existing splits I did not create and did not fully fix:
  - 0x00C1BD10: still bound as CRecordBuilder_00c16310 / CMessageEventList / Params ctors by 0x00C16310 / 0x00C203A0 / 0x00C2E2B0.
  - 0x00C1BB20..0x00C1BC70 setters: 4-5 spellings each.
  - 0x00997780 MakePath: free spelling in 0x00997800 vs CAFile::MakePath in 0x004FAFF0. The CAFile opt-in that declares it is
    nested in FABLE_RETAIL_CAFILE (C2039 without it).
  - 0x0143BC2C: g_pSal plus 9 stand-in spellings.

## Rows ported with a stand-in change the coordinator should know
- 0x00C52840: the 0x00C51A20 stand-in `Apply` now takes `_CHANGES*` (was `unsigned int`), because the change-list local is a
  `_CHANGES*` handed to CEAXListener. RENAMES s2 lists it. It is count-neutral: one stand-in spelling for one row.
- 0x00665F90: the stand-in class CTCCreatureSound is gone. SetVoiceName was added in place to the row-local CTCSoundPlayer model the
  row already had. The pointer cast `(CTCSoundPlayer*)GetTC(0x10)` existed at HEAD (it was `(CTCCreatureSound*)` plus a second cast).
  No new cast was added.
- 0x00C44170: the +0xF8 member of the row-local CMarkedBufferOwner is now `CEAXBuffer*`; retail pushes it as the buffer to remove.
- 0x00C54F30: SAudioBlock is now `short m_samples[0x80]` (the SDK's short[128] buffers). Short arrays keep the /GS cookie. No cast.
- Tests in a few rows (0x00C52840, 0x00C44170) take a CEAXListener* from a static buffer through reinterpret_cast. This is test
  scaffolding only: the header class has no defined ctor/dtor.

## New local models (narrow rule)
- 0x00C1BAE0 and 0x00C1BD10 define `class CSoundPlayParameters` (no header anywhere under rebuild/include; no base). The landed
  sibling 0x00C1BC30 SetStreamed uses the same shape. The HEAD rows modelled CTCPhysicsStandard (a shared-header class) and a
  CClass_00c1bd10 placeholder.

## Self renames (RENAMES s1) and their referencers
- 0x00C1BAE0, 0x00C27B70 / 80 / 90: the only referencer is 0x00C12940, which already binds the new leaves.
- 0x00C1BD10: no landed row bound the old leaf ??0CClass_00c1bd10; 0x00C267E0 binds the new one.
- 0x00C1E970 / 0x00C1E990: all referencers (0x00C1E990, 0x00C1E9C0, 0x00C1E9F0) move in this hand-back. Each address goes from 2
  spellings (self vs binders already differed: YII..I vs YIK..K, KK vs JJ) to 1.
- No EH funclet includes any changed row (grep of rebuild/src/compiled for each parent file name).

## Flags lines
- None added. Every changed row grades with its catalog CompilerFlags.

## Rule slips
- 0x00C44170: the first grade ran although the cpp precheck in the same call had reported a missing include. The overlay copy of
  CEAXListener.h could not reach its directory-relative includes. cl spun, and I killed it and its q.py parent by PID. Fixed by
  copying the five unchanged dependency headers into hdr/engine (hdr/README.md). From then on I graded only through g.sh, which gates
  on the precheck.
- 0x004A05C0 (q6184 binder, not in rows.txt) was edited before its lock was claimed. No other agent held it, it is not acctB's and
  it was not in HELD.txt. I claimed it afterwards (owner NF165).
- The brief says st.py reads stdin on `-`. My copy reads it on `@stdin`; I passed every note as an argument, so nothing was affected.

## HEAD pass (acceptance test)
- headdiff_HEAD_vs_final.txt (made by work/headpass.sh + work/headdiff.py): every changed row's HEAD source was compiled from the repo
  headers into a separate pSH, and its objdump -t compared with the final object.
  - Defined symbols: only the self-renamed rows change, and each changes only its own leaf (7 rows; RENAMES s1 old leaves taken from
    these objects).
  - Referenced symbols: exactly the RENAMES section-2 pairs, plus nothing else.
  - The 0x0066CA80 HEAD compile reports DIFFER(18v99) only because the pass ran q.py with a dummy leaf, which graded the largest
    inline. The symbol diff is unaffected.
- One-definition check: no landed row or header path defines FABLE_RETAIL_STREAMED_BLOCK_CALLEES_PASS23, the inline bodies of
  GetStreamBlockOffset / GetStreamBlockTail. No landed source has an inline GetStreamed() const / GetTrackBit0..2() const body. No other
  landed row defines CSoundPlayParameters' ctor. So the seven strong leaves meet no pick-any COMDAT twin.
- COORDINATION_CLAIMS.tsv has no line for 0x004A05C0 or 0x00A01950.

## More rule slips
- Rows with no `// Leaf:` line were edited: 0x00C44170, 0x0066CA80, 0x00A01950. Each defines one function, and I graded each with its
  explicit leaf or largest-function fallback, then confirmed with objdump.
- One Bash call exited 1 (a background grade whose grep found nothing). The HEAD baselines were not saved up front for most rows;
  the HEAD pass above was run at the end instead.
