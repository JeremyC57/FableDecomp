# NF109 notes (2026-11-19/20, round 10): GameCODA follow-ups (two runs; the first stopped by a container restart)

Counts (from APPLIED.tsv / STATUS.tsv / recheck_final.log): 18 entries: 15 done, 0 done-already, 0 rejected, 1 blocked (q11517),
0 deferred, 2 open (q11584, q11559). 18 rows ported (all in rows/), 0 not-a-binder, 5 rows.txt rows listed blocked (untouched:
0x00C54190 / 0x00C53AE0 / 0x00C6AE90 for the open entries, 0x00C74420 / 0x00C74450 for q11517). 19 EH funclets of 0x00C6A780 /
0x00C6A650 regraded (unchanged files; their parents changed). Header opt-ins: 7 macros in 5 headers (hdr/README.md).

## Run 1 (before the restart) and the resume
- q11593..q11599 (seven unlanded callees): catalog fixes only, two sources each re-verified (NAME_FIXES.tsv); each one's only landed
  binder already binds the SDK spelling (NF105).
- q11600..q11606: the seven self rows 0x00C74CE0, 0x00C74D30, 0x00C756C0, 0x00C75A20, 0x00C75AB0, 0x00C76E90, 0x00C770B0 ported onto
  the shared GameCODA headers with the CodeView spellings (q.py grades in work/grades.tsv: 9 for 7 rows). Run 2 regraded all seven
  with vland --recheck-landed + the landed tests (work/b1.log, RELOCATION_MATCH + PASS each) before continuing, wrote their
  RENAMES / STATUS / APPLIED lines and hdr/README.md (missing after the restart), and removed an unused CFloatPitch.h overlay copy
  (identical to the repo header).
- Splits healed: every landed binder of the seven (NF105 rows 0x00C69C60, 0x00C891A0, 0x00C89750, 0x00C75820, 0x00C89350,
  0x00C6A200, 0x00C6A080; compiled from HEAD, work/oldsyms3.txt) already binds the SDK spelling, and the remaining stand-in binders
  were the ported rows themselves. The ported rows also move their callees 0x00C74510 (ApplyFilter), 0x00C75820, 0x00C891A0,
  0x00C89350 and the data 0x0143CCDC onto the SDK spellings (Section 2); a refs scan of all 20 targets finds no landed referencing row
  outside rows/ except those NF105 rows (work/gtrefs.tsv).

## q11607 (0x00C75000 CFloatDiffuser::Release): done, family of 11 rows
Before: self + all three binders bound ?Detach@CStereoDelayLine@@QAEXXZ (compiled from HEAD, work/oldsyms2.txt: the
reloc_bindings witness rows were stale), so the address was NOT split and the rename had to move all four. 0x00C6A3D0 (SDK
CFloatReverbDspEngine::Destroy) and 0x00C6A8B0 (SDK Create) only call it, but 0x00C6A780 (the engine dtor) also destroys the member,
so a shared-header CFloatDiffuser member rebinds its dtor 0x00C75440, whose body binds ~CFloatAllpassSection 0x00C75400 and the two
vtables; closure: CFloatDiffuser ctor / dtor / ??_G 0x00C75410 / 0x00C75440 / 0x00C754A0, CFloatAllpassSection ctor / dtor / ??_G
0x00C74F60 / 0x00C75400 / 0x00C75470, and the engine ctor 0x00C6A650 (binds the diffuser ctor). Claimed in nflocks as NF109 (none
held, none in UNMATCHED_ROWS / COORDINATION). Refs scan of every target: no referencing row outside the 11 (plus funclets).
- Names: Release / ??0 / ??1 / ??_G CFloatAllpassSection are byte-identical to CODAApi.lib release members (sdkfind); the CFloatDiffuser
  ctor / dtor are not (the release lib expands the section ctor / dtor; retail calls them out of line through the eh vector
  iterators, the CODAApiDbg.lib shape): named from the CodeView m_diffuser at engine +0x25c (the retail engine ctor / dtor call them
  there) and the Dbg bodies (Release, then ??_M with ??1CFloatAllpassSection). ??_GCFloatDiffuser is folded in the release lib (24
  hits): named as the only slot of vtable 0x012BF078 and by its call to 0x00C75440. Each row's `// Name` paragraph cites both.
- The four engine rows (0x00C6A3D0, 0x00C6A8B0, 0x00C6A780, 0x00C6A650) are in-place respellings: only the local
  CStereoDelayLine / CDelayLine stand-ins were removed and the +0x25c member retyped to the shared-header CFloatDiffuser (member name
  m_stereoDelay kept: the local models already use m_diffuser for their +0x144 stand-in). Their self leaves and every other binding
  are unchanged (binds diff in work/new_q11607.txt vs work/oldsyms2.txt: exactly the five intended pairs moved).
- 0x00C75440: Release is written out as its body (m_allpass[i].Release() loop; the release build expands its TU-mate). A same-TU
  `inline` definition of CFloatDiffuser::Release also matched, but leaves a ?Release@CFloatDiffuser COMDAT in the object beside the
  strong 0x00C75000 definition, so it was dropped.
- 0x00C754A0 keeps the landed vtable-emitting helper, renamed NewFloatDiffuser (file-local helper, not retail code: DESCRIPTIVE).
- EH funclets: 0x00C6A780's ten (0x01216640, 0x01216648, 0x01216653, 0x0121665E, 0x0121666C, 0x0121667A, 0x01216688, 0x01216696,
  0x012166AE, 0x012166BC) and 0x00C6A650's nine (0x012165B0, 0x012165B8, 0x012165C3, 0x012165CE, 0x012165DC, 0x012165EA, 0x012165F8,
  0x01216606, 0x0121661E) regraded with the new parent text (work/tools/mkpay2.py: funclet_offset from each funclet's comment, leaf
  Unwind@<addr>): all RELOCATION_MATCH + PASS. No self rename moves a parent of a funclet (0x00C6A780 / 0x00C6A650 keep their
  leaves); the renamed rows' files move at rerow but no funclet includes them (grep of rebuild/src/compiled/01/21).

## q11517 (0x0143CCDC m_pDSP): blocked (closure, 0 grades)
m_pDSP is a protected static of engine/CReverbDspProcessor.h, so the binders 0x00C74360 / 0x00C74420 / 0x00C74450 / 0x00C74480 must
become CReverbDspProcessor::CreateProcessor / ctor / dtor / ??_G (self renames; SDK sdkfind: 0x00C74420 / 0x00C74450 / 0x00C74480
byte-identical to ??0 / ??1 / ??_GCReverbDspProcessor, 0x00C74360 no hit). That pulls every row binding 0x00C74420 / 0x00C74450 /
0x0143CCE0 / 0x012BF050: 33 rows (00c69660 00c69680 00c69720 00c69740 00c697a0 00c697c0 00c69880 00c698a0 00c698d0 00c69930
00c69990 00c6a280 00c6a2e0 00c6a340 00c6a650 00c6a780 00c6a8b0 00c74360 00c74420 00c74450 00c74480 00c75530 00c75980 00c77440
00c775a0 + funclets 012164e0 01216500 01216520 01216540 01216560 01216580 012167f0 01216810), each DSP part ctor / dtor over its own
local base model (CVtable_012bf050 / CDspObject). The classes that must move first: CReverbDspProcessor (base), then each part on
its SDK header (CFloatFilter, CFloatEcho, CFloatPitch, CFloatReflection, CFloatShortReverb, CFloatLongReverb, CReverbDspEngine).
0x00C6A8B0 now binds only its data stand-in ?Backend_0143ccdc@@3PAUBackend@@A for 0x0143CCDC (the existing split is unchanged).

## q11584 (CDecodedStream = CSalSwReverb): open, step 1 done
The CODAApi.lib ?Update@CSalSwReverb@@UAEXPBVCCodaEnvironment@@@Z (889 bytes, NF102: relocations at the same offsets as 0x00C54190)
relocates +0x1c to ?SetReflectionsPan@CSalSwReverb@@AAEXPBU_CVECTOR@@@Z (retail 0x00C53D50) and +0x27 to
?SetReverbPan@CSalSwReverb@@AAEXPBU_CVECTOR@@@Z (0x00C53E40); both setters compare 144 v 144 bytes with 8 differing (member offsets:
the SDK version skew). So the brief's condition holds: rename. Not started (context). Plan for the resume, sized at three rows
(0x00C53D50 / 0x00C53E40 / 0x00C54190 are each other's only binders; 0x00C54190 has no landed referencing row: virtual):
the slot-5 argument becomes the shared engine/CodaEnvironment.h CCodaEnvironment (GetParams() = the CENVIRONMENT at +0xc; the
local SSourceProperties is field-for-field CENVIRONMENT, 0x6C bytes: Position = vReflectionsPan, Velocity = vReverbPan, Room = iLevel
...), the local class is renamed CSalSwReverb (no shared header exists), the inline helpers take the SDK private names (SetLevel,
SetLevelHF, SetLevelLF, SetReflectionsLevel, SetReverbLevel, SetSize ...), and the two setters take `const CVECTOR*` (their pending
copies at +0x2DC / +0x2E8 and the dirty word +0x290 sit inside the CHLRvbParams block). The other CDecodedStream rows (0x00C54090 /
0x00C54110 ctor / dtor storing vtable 0x012B89C8, 0x00C54510 Initialise = SDK private Initialize per NF102, 0x00C546E0, 0x00C547A0)
would stay CDecodedStream unless each gets two sources: say so at landing (class named twice, no address split).

## q11559 (vtable 0x012B89C4): open, needs a ruling
sdkfind over 0x00C53AD0 (7 bytes: vptr store, ret) hits 30 CODAApi.lib members storing 0x012B89C4-equivalent vtables:
??1CCodaObjectBase, ??1CSalSwBuffer (storing ??_7CCodaObjectBase), ??1CReverbConstants and ??1CFloatReverbConstants (storing
??_7CReverbConstants). The retail vtable is shared by identical-COMDAT folding, so "the CReverbConstants ctor stores it" does not
exclude ??_7CCodaObjectBase; its one slot 0x00C53AE0 decides nothing either. Rule needed: which spelling a folded vtable takes.

## Descriptive names
NewFloatDiffuser (0x00C754A0 row helper, file-local, not retail code); FloatCopySamples in 0x00C74D30 (file-local inline with the SDK
helper's name: the Dbg body calls the C helper _FloatCopySamples, no header carries it); RoundHalfDownToLong in 0x00C76E90 / 0x00C770B0
(file-local inline, the landed 0x00C6A460 spelling; stands for the SDK header inline ds_ftoi expansion); parameter and local names
(pParams, pNewParams, pfSrc, pfDest, afBlock, fOld, uiSamples ...) are descriptive SDK-style Hungarian (no CodeView local records read). Every class, member, data and vtable name in the RENAMES is a
GameCODA 2.10 CodeView spelling. Local-model names inside the in-place rows (CReverbProcessor, BufferState, m_stereoDelay ...) are the
landed rows' own and unchanged.

## Casts
No cast in the 14 rows ported onto headers (grep `_cast<` / C-style pointer casts over rows/). The in-place 0x00C6A8B0 keeps its landed
`(void*)(((unsigned long)Allocation+15)&~15UL)` alignment (the SDK Create's own 16-byte rounding of the CodaMalloc block, untouched).
Tests: all 18 landed tests unchanged (self-contained; none #includes a row path, so the rerow file moves do not touch them).
Ownership: COORDINATION_CLAIMS.tsv line 70 is this slice's own claim; none of the 7 extra closure rows (0x00C74F60, 0x00C75400,
0x00C75470, 0x00C75410, 0x00C75440, 0x00C754A0, 0x00C6A650) appears there, in COORDINATION.md or UNMATCHED_ROWS.tsv.

## Rule slips
- Run 1 wrote STATUS / APPLIED but not RENAMES, hdr/README.md, NOTES or LESSONS before the restart (written in run 2).
- Run 2: one python traceback inside a Bash call (swapdiff.py on the leafless 0x00C6A8B0; nothing written, call exit 0) and one bad
  in-place edit of a draft (0x00C75440, a string index matched the comment line; compile failed, file rewritten; counted as a grade).
- One foreground `sleep 60` poll was blocked by the harness (no effect).
- A draft (0x00C75000) sat in rows/ for a few minutes before its family was closed (moved to work/drafts until the family passed).
