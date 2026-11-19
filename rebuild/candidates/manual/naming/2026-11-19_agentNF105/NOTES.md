# NF105 notes (2026-11-19, round 10): the short-reverb / DSP-engine family, resumed from NF102 and closed whole

Counts (from APPLIED.tsv / STATUS.tsv / recheck_final.log): 29 entries: 27 done, 0 done-already, 0 rejected, 1 blocked (q11517), 0 deferred,
1 open (q11559). 37 rows ported (all in rows/; recheck_final.log: 37 x MATCH / RELOCATION_MATCH + PASS: 12 MATCH, 25 RELOCATION_MATCH).
STATUS.tsv also lists the 27 other rows.txt rows (NF102 step-1 rows and q11517 / q11559 binders): 22 not-a-binder (0x00C88E60 among
them: held by acctB, read-only, already the SDK spelling), 5 blocked (0x00C53AE0 / 0x00C6A780 / 0x00C6AE90: q11559 not started;
0x00C74420 / 0x00C74450: q11517). selfcheck.log: 0 problems. No new split (work/n5/gatecheck.py over every target the 37 objects bind: "CREATED splits: 0").

## Closure (why 37 rows, not NF99P2's ~27)
The family hands back whole, so every address whose spelling changes had all its exe-scan referencing rows ported with it
(work/n5/target_refs.txt, from $S/w22/nftools/gate.py scan; spellings before from work/n5/oldsyms*.txt, after from work/n5/newpairs.json).
Beyond the 29 entries' rows the closure forced eight more (claimed in nflocks as NF105; none in UNMATCHED_ROWS / COORDINATION_CLAIMS):
- any row on engine/CReverbFns.h binds every ring wrapper under its SDK spelling: 0x00C76B60 (CFloatEcho::Process) pulls 0x00C766F0
  (CReverbFns::AddSamples, buffer form; only other binder 0x00C76C20), 0x00C76C20 (the fading CFloatEcho::Process) pulls 0x00C76560 /
  0x00C767E0 (the fading ScaleSamples / ScaleAddSamples wrappers), whose other binders are 0x00C75820 (the fading
  CFloatLongReverb::ProcessInputFilter) and 0x00C89350 (the fading CFloatLongReverbModule::Process, 850 bytes);
- 0x00C69B60 (CFloatReverbDspEngine::Clear) pulls 0x00C75010 (CFloatDiffuser::Clear; only binder 0x00C69B60);
- 0x00C6A460 (CreateComponents) pulls 0x00C75F80 (CFloatReflection::Create) and 0x00C75C30 (CFloatShortReverb::Create, also the binder
  of q11549's 0x00C896C0), each with 0x00C6A460 as only binder.
Left alone because their targets were already split before this hand-back (or have no other landed binder), SDK names in NAME_FIXES:
0x00C6A8B0, 0x00C75A20, 0x00C75AB0, 0x00C756C0, 0x00C74CE0, 0x00C74D30, 0x00C770B0, 0x00C76E90, 0x00C75000. The family binders now
bind the SDK spelling of 0x00C74CE0 / 0x00C74D30 / 0x00C75A20 / 0x00C75AB0 / 0x00C76E90 / 0x00C770B0 (only the self rows, outside
this slice, keep the old ones: a clean follow-up). Existing splits healed outright: 0x00C763E0, 0x00C76460, 0x00C76B60, 0x00C75E90,
0x00C89B40, 0x00C6A080, 0x00C88E60 (B's leaf).

## Self renames beyond the entry list (all forced by the closure, each with two sources in its row header and RENAMES)
Ten of the 37 self renames are not asked by an entry: 0x00C6A5D0 (ProcessReverb; its five callees are entries) and 0x00C75C30
(CFloatShortReverb::Create; binder of q11549) plus the eight claimed rows 0x00C766F0, 0x00C76560, 0x00C767E0, 0x00C76C20, 0x00C75820,
0x00C89350, 0x00C75010, 0x00C75F80. q11415 (0x00C88E60, acctB's row, not an entry here): its only binder 0x00C75CE0 now binds B's leaf
?ApplyFilterOrder2_quad@CReverbFnsSSE@@QAEXPAMPBMPAPAMPAPAVCFloatFilterParams@@I@Z, so that split is healed.

## Sources (two per name)
Every one of the 37 rows is byte-identical (relocations masked) to exactly one CODAApi.lib release member of the spelling it now
defines (NF102 work/sdkfind_family.txt for 34; NF105 sdkfind for 0x00C766F0 / 0x00C76560 / 0x00C767E0 / 0x00C89350 / 0x00C75010 /
0x00C75F80), and the second source is the byte-identical SDK caller whose relocation names it at the retail call site (or, for the
virtual 0x00C69B60, slot 3 of the engine vtable = CReverbDspEngine::Clear). Each row's `// Name` paragraph cites both. Not-landed
callees (NAME_FIXES): Fable.exe bytes compared to the SDK member (work/n5/sdkcmpx.py): 0x00C69A70, 0x00C755B0, 0x00C76120, 0x00C76AB0,
0x00C76DD0, 0x00C89850 identical; 0x00C75050 (CFloatDiffuser::Create) NOT identical (another SDK build: nearestPrime expanded), named by
the identical caller's REL32 +0x154 and the same prime-length constants.
Inline bodies added to the overlay (Clear / Create of CFloatCyclicBuffer, CFloatAllpassSection::Clear, the module Clears, Token,
MatchToken) are the release COMDATs (disassembled) or the Dbg out-of-line copies; each comment names its source.

## Row notes
- Ported from the landed files with CodeView member names; bodies keep the landed statement shapes (all graded at the first form;
  29 q.py grades in work/n5/grades.tsv, no failed form, plus NF102's 9 draft grades and two vland runs per row).
- NF99 held drafts used as-is (they already carried the nf99p2 data respell): 0x00C75CE0, 0x00C6A180, 0x00C6A5D0. NF102 drafts used
  as-is except 0x00C75FF0, whose two float* parameter names were swapped to match their use (arg 3 gets the direct tap = the engine's
  reverb input, arg 4 the reflection's output; ProcessReverb 0x00C6A5D0 passes its block as ProcessReflections' last argument and
  then feeds it to ProcessShortReverbInputBuffer). Names only; regraded RELOCATION_MATCH.
- 0x00C89110 / 0x00C896C0: the Dbg build calls the header-inline CFloatFilter::Create (release COMDAT `jmp Clear`); the release
  relocation at that call names ?Clear@CFloatFilter@@QAEXXZ, so the rows call Clear (said in each row's Shape line).
- 0x00C89750 / 0x00C69FB0: the Dbg build clears with `_FloatClearSamples`; the release expands a memset (kept from the landed rows).
- Casts: the SDK interface hands the parameter blocks / outputs as void* / void**: static_cast<CFloatReverbParameters*>(m_pvParams) in
  0x00C69C60 / 0x00C69FB0 / 0x00C6A080 / 0x00C6A180 / 0x00C6A200 (2 each), static_cast<float*>(pvMemory) in 0x00C75F80 (1) /
  0x00C89110 / 0x00C896C0 (2 each), 0x00C6A5D0 (static_cast<float*>(pvInput) + 3 reinterpret_cast<float**>(apvOutputs), NF99's draft),
  0x00C75CE0 (static_cast<CReverbFnsSSE*>(m_pDSP), NF99's draft). Pointer conversions of the SDK's own void* members / parameters,
  no cast addresses; the coordinator may prefer another spelling.
- Tests unchanged for all 37 (self-contained local copies; they compile without the rows and PASS; PassPatterns unchanged, 0x00C89B40's
  is BEHAVIOUR_OK). No row is an EH-funclet parent (grep of rebuild/src/compiled/01/21 for the 37 file names: none); the 37 self
  renames move files at rerow but no funclet includes them.
- Header opt-ins: hdr/README.md (11 headers, all add-only, pin_asserts pinned 0; no landed includer defines any new macro).

## Descriptive names (mark for the coordinator)
- Parameter / local names (uiCursor, uiStart, fOld / fNew, afBlock, pfReverbInput ...) are descriptive Hungarian in the SDK style; no
  CodeView local records were read. ReadPosition (0x00C75FF0, 0x00C89750, 0x00C89B60) and RoundHalfDownToLong (0x00C6A460, landed) are
  file-local descriptive inline helpers with no retail symbol. Class, member function, data member and parameter-type names are SDK
  CodeView spellings.

## q11517 (0x0143CCDC m_pDSP): blocked
12 family rows plus the landed 0x00C74510 now bind ?m_pDSP@CReverbDspProcessor@@1PAVCReverbFns@@A. Left on stand-ins: 0x00C6A8B0,
0x00C74360, 0x00C74420, 0x00C74450, 0x00C74480 (CReverbDspProcessor ctor / dtor / CreateProcessor through CVtable_012bf050 /
CDspObject / CDspOps stand-ins): a class port of those five rows (vtable 0x012BF050 = ??_7CReverbDspProcessor) is the next step.

## q11559 (0x012B89C4): open
Not started. The binders are the CReverbConstants / CFloatReverbConstants destructor rows 0x00C53AD0 (??1T@@ stand-in) / 0x00C53AE0
(scalar deleting dtor scaffold) / 0x00C69870 (??1CVtable_012b9584) and the engine dtor 0x00C6A780 + 0x00C6AE90; 0x00C53AD0 / 0x00C69870
are outside rows.txt. A separate family (the header has FABLE_RETAIL_CREVERBCONSTANTS_INLINE_CTOR for the inline dtor).

## Rule slips
- One Bash call exited 1 (a trailing grep loop over headers without `; true`; nothing written, no grade spent).
- One python traceback inside a Bash call (the RENAMES generator's first run; no file written, call exit 0).
- STATUS.tsv / APPLIED.tsv were written at the end of the run, not after every row (the work state was kept in work/n5/grades.tsv and
  work/fam/FAMILY.tsv during the run).
- Entries were not taken in queue order: risk-first (0x00C89350, 0x00C69C60, 0x00C69B60, 0x00C6A460 as the go / no-go), then leaves,
  then binders, as the whole-or-nothing family needed.
- After the first full vland run, row text was edited (cast spelling unified to static_cast in six rows; the three NF99 drafts' headers
  corrected: 0x00C6A5D0 is 125 bytes and identical, not "123, another SDK build"; `// Flags: none` added); recheck_final.log is the run
  over the final files.
