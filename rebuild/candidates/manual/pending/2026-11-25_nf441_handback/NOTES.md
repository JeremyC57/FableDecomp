# NF441 hand-back (2026-11-25): libpk / CODA placeholder classes under the best-guess rule

Nothing was written to the repo. No git command was run. No batch is handed back and no address is closed: the header port the
family needs (below) was not attempted, and no changed row went through any gate. Every lock was released. No SDK file, code or text beyond cited names is in any file here
(work/cv_coda.json and work/sdk_rel.json are scratch name/size extracts and must not enter the repo).

## Measured, not as briefed
- The 1,000 `libpk_hold` addresses (T3.txt): 211 sit on the 63 vftables whose destructor chain reaches CCodaObjectBase
  0x012B5868 / CodaFree 0x00C3C370 (GameCODA: 96 identified, 35 candidate, 80 held); 5 on vftables that delete through
  the CRT but are GameCODA by evidence (an SDK `??_7` bound there, or stored by a GameCODA chain: CSalMutex, CSalFile ...,
  CReverb*/CFloat* have no CCodaObjectBase base in 2.10); 784 unclassified, most of it Lionhead libpk / audio (retail strings
  `d:\dev\audio\ver_fable_1.5_b\libpk\PKC*.h`, `.\PKC*.cpp`, `.\CCodaFrameStreamer.cpp`), where only such a string can name a class.
- Retail GameCODA is **older than 2.10**: CCodaObject (0x012B8778) has 1 slot, not 11 (no Is*/Release virtuals), the
  mixers have 11 slots against 10, CCoda2D/3DSource 9 against 16, the Sw voices 28 against 36. A class name ties; a method
  name by slot index does not, so methods need a body tie.
- **Prior GameCODA naming exists**: ~65 `rebuild/include/engine/` headers (CSalRender.h, CSalSw3DVoice.h, CCodaSource.h
  0x012B5A88, CCodaEAX.h 0x012B87F4, CSalOutputKS.h with CSalMutex 0x012B8FC8, CCodaObject.h, CSalVoice.h, CSalMixer.h, ...).
  Respelling a local `CVtable_<va>` model to one of those names makes the row define a class that has a shared header
  (SHARED_HEADER_AVAILABLE; the lander rejects). Every GameCODA chain passes through a headered class (CCodaObjectBase,
  CSalRender, CSalVoice ...), so a header-free "symbol-only respell" does not exist for this family. The route is a port of
  the rows onto the shared headers (a `_hdr` batch first), which this run did not reach.

## Method (tools/)
- vtscan.py: every vftable in 0x012B1000..0x012C0000 stored by a `mov [r(+d)],imm32` in retail .text; slot lists cut at
  the next stored VA (461 VAs; work/rvt.json). chain.py: slot 0 -> destructor -> the ordered vftables its chain stores.
  part.py: GameCODA = the deleting destructor reaches CodaFree.
- sdk441.py: CODAApi.lib `??_7` slot symbols (relocation order) + function relocations; cvtypes.py: CODAApiDbg.lib CodeView
  classes (size, bases, members, virtuals) via llvm-readobj. work/sdk_chains.txt.
- vmatch.py (DP slot alignment, token similarity) was **rejected** as a source: it scores the known CSalMixerLimiterMMX at
  0.40 and ties siblings. cntmatch.py (exact instruction-count equality of slot bodies, mnemonic ratio >= .8) discriminates:
  same-version SDK bodies have the retail count exactly (Pan SetPosition 119 = 119, Disable 15, PanDelay 150, Hrtf 122).
- Ties broken by fact only: derivation (shared slot addresses), mm vs xmm registers in Process / its kernel, and the
  Codex pass 23 method (SDK relocation order v retail call order).

## Class identifications (CLASS_TABLE.tsv: 43 VAs + 1 function; evidence per line)
- **Render family, two sources each** (SDK + retail): `CSalSw3DVoice::CreateRenderer` (SDK SalSw3DVoice.obj relocations
  CodaMalloc + ??0 DisableSSE2, PanSSE2, PanDelay, DisableMMX, PanMMX, PanDelay, HrtfMMX) = retail 0x00C55270, which calls
  0x00C550B0, 0x00C55090, 0x00C6CA20, 0x00C70B00, 0x00C55110, 0x00C550F0, 0x00C6CA20, 0x00C70120 in that order. With slot
  inheritance and instruction counts: CSalRender 0x012BEDA4, CSalRenderPanDelay 0x012BED7C, CSalRenderPan 0x012BED90,
  CSalRenderDisable 0x012BEDB8, CSalRenderHrtfMMX 0x012BEDCC, CSalRenderPanSSE2 0x012B8A78, CSalRenderPanMMX 0x012B8AB4,
  CSalRenderDisableSSE2 0x012B8A8C, CSalRenderDisableMMX 0x012B8AC8; one source + registers: CSalRenderPanDelaySSE2
  0x012B8A64, CSalRenderPanDelayMMX 0x012B8AA0; style: **CSalRenderHrtfSSE2** 0x012BEDE0 (retail-only SSE2 HRTF in the SSE2
  branch of CreateRenderer; 2.10 has only Hrtf + HrtfMMX).
- SDK method map for the render rows (retail slot bodies tie by instruction count; ret N agrees): slot 1
  ClearFilterHistory (landed ClearHistory / ClearScratch), 2 Process, 3 SetGain (SetVolume / SetParameters on Hrtf),
  4 SetPosition (Update / SetParameters on Disable); private helpers UpdateFilters(bool) (RebuildGain / UpdateVoices) and
  Params3DInit (ComputeGainRamp / ComputeTapGain / SetupVoice; struct tags FilterPan / FilterPanDelay / FilterHrtfMMX /
  FirFilter for SGainRamp / SDelayTap / SResampleVoice / SUnprovenImpulse52). SDK signatures differ from the landed models:
  Process(const short*, int*(&)[4]) v (const short*, long* const*); SetGain(float, const float*, bool) v (float, unsigned
  long, bool); SetPosition takes floats where Disable's row takes longs; the helpers are private (`AAE`).
- **GameCODA core** (pass 23 vector-deleting-destructor anchors): CCoda2DSource 0x012B7B88, CCoda3DSource 0x012B5AB0,
  CCodaListener 0x012B5A80, CCodaEnvironment 0x012B5A84; CCodaObject 0x012B8778 (candidate).
- Candidates (one source, exact-count matches; see each line): CSalMixer / Limiter, CSalVoice, Sw / OpenAL voices,
  CSalOpenAL, CSalContext / Platform, CSalOutput / KS / MM / Ds / DsReverb / ConfigDs, CFloatReverbDspEngine.
  Held ties: 0x012B8CC8 and 0x012BEE40 (CSalOpenALVoice v CSalOpenAL3DVoice).
- SDK vs style: 40 SDK-named VAs, of which 8 were already named before this run (anchors: CCodaObjectBase, CCodaSource, CCodaEAX,
  CSalMixerLimiterMMX / SSE2, CSalMutex, CSalOutput, CSalOutputKS) and 32 are new (11 render-family classes, the 4
  pass-23 core classes, and 17 one-source candidates incl. CCodaObject); 1 SDK function name (CreateRenderer); 1 style name (CSalRenderHrtfSSE2);
  2 held ties.

## Render family scope (the first unit to port; 62 rows)
- 52 rows mention a render `CVtable_` token, + 8 EH funclets of 0x00C55270 (`--with-includers`), + 0x00C55090 / 0x00C550F0
  (the `??0C3DAnimationUtil@@QAE@XZ` SIMD ctors that do not name a token). work/r_rows.txt, work/render_files.txt.
- 78 of the 1,000 open addresses sit on these 12 vftables (ctors, dtors, methods, 8 EH funclets, 8 vftable VAs), including the sdd stand-ins (`CLandscapeBackgroundPatch_vector_deleting_destructor`
  / `?VecDel@T@@`) at 0x00C6D350, 0x00C6E1D0, 0x00C6E6D0, 0x00C6ECB0, 0x00C554C0..0x00C555B0 (NF424 r2 generator).

## Not done / resume (advisor-reviewed route)
1. `_hdr` batch: add-only opt-in to engine/CSalRender.h projecting the members the rows read at retail offsets (SDK names:
   CSalMutex m_mutexUpdate +0x04 [retail lock is 0x1C bytes: CSalOutputKS.h's CSalMutex without m_bActive, i.e. its
   PASS23 layout], float m_fGain +0x20, bool m_bUpdate +0x24) + `CSalRender();`. equiv_check copy v copy over its includers
   (0x00C54C10).
2. Pilot rows 0x00C6E6A0 (??0) and 0x00C6E630 (??1) onto it (+ FABLE_RETAIL_CSALOUTPUTKS_INLINE_MEMBERS,
   FABLE_RETAIL_CCODAOBJECTBASE_INLINE_DTOR); tests must link the row object. Unverified: the HEAD tests define their own
   InitializeCriticalSection / DeleteCriticalSection, which may clash with the header's dllimport declarations; read the tests of
   landed 0x00C5ECA0 / 0x00C5ED90 (they include CSalOutputKS.h with FABLE_RETAIL_CSALOUTPUTKS_INLINE_MEMBERS) first as the precedent.
   HEAD grades: both RELOCATION_MATCH PASS, warn SHARED_HEADER_AVAILABLE CCodaObjectBase (work/vland_h1h.log).
3. Derived classes: new headers engine/CSalRenderPan.h etc. in `_hdr` batches, overriding with the header's signatures;
   rows whose codegen changes under the SDK types HOLD (Process likely).
4. libpk: class names only from `.\PKC*.cpp` / `libpk\*.h` strings the function references; many libpk classes already
   have rebuild/include/libpk headers (PKCAutoFile, PKCTRList, ...): same port rule.

## Repo-bound vs scratch-only
- Hand-back files: NOTES.md, CLASS_TABLE.tsv, NAME_FIXES.tsv, T3.txt, tools/ (readers; they open the SDK libs and the retail exe in
  place and write only names / sizes / addresses).
- Scratch only, never into the repo: work/ (work/p_*/oracle.tsv holds retail bytes; work/sdk_rel.json, cv_coda.json are SDK name /
  layout extracts; work/rsroot is a symlink tree of the repo).
- NAME_FIXES: the tracker lists the six SIMD ctors as split_fixable on `??0C3DAnimationUtil@@QAE@XZ`; the caller-respell lane must
  not move binders onto that stand-in (C3DAnimationUtil is a Fable class; these are GameCODA render ctors).
