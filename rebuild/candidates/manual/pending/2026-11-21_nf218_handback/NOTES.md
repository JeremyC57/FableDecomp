# NF218 notes (2026-11-21, round 10: the nf217 follow-ups and the UnassignString spellings)

Lane: q.py grades in a private `S=$A/pS` (HEAD objects in `$A/pH`); vland `--recheck-landed` payloads from tools/pay.py (rows/<addr>[_test].cpp
when present, else the landed files; flags from the `// Flags:` line, else the build entry's CompilerFlags minus /O2 /Oy /W3). REFS.tsv
from callers.py + absrefs.py (calibration: callers.py gives the 11 landed referencers of 0x00485DBC NF213 counted). tools/symdiff.sh grades
HEAD and the new source and diffs the defined / undefined external symbols (objdump -t). tools/lnk.py / comdat.py / cmpcopy.py /
comdiff.py are NF217's, copied.

## q12385: 0x00C442F0 CircularQueue::GetInfo (done; DESCRIPTIVE name)
- Name: NF217's descriptive CircularQueue::GetInfo (no PDB covers libpk; NF217 searched both PDB name sets, the exe strings and GameCODA 2.10).
  Body (35 bytes): copies capacity +8, last item +0x14, tail +0x10, items +0, times +4 of the +0x54 queue into the 20-byte record and returns
  it in eax. Leaf `?GetInfo@CircularQueue@@QAEAAUSQueueInfo@@AAU2@@Z` = the reloc_bindings witness (0x00C437C0), no other address bound
  under it.
- Self row re-authored on the landed sibling 0x00C437C0's SQueueInfo / CircularQueue model (header-less; no class of that name anywhere
  under rebuild/include; same shape as the landed siblings, no base): MATCH first grade. The placeholder SBufferMark / CMarkedBuffer_00c44550
  model is gone from this row. Defined-symbol diff vs HEAD: the leaf only.
- Binder 0x00C43810: its local GetInfo declaration returned void; now `SQueueInfo&` (the body returns the record). RELOCATION_MATCH,
  undefined-symbol diff one line. Census (REFS.tsv): 0x00C432F0 / 0x00C437C0 already bound the new spelling, 0x00C35AAC is in no landed
  row, absrefs none: 0x00C442F0 ends on ONE spelling (was 3).
- Catalog / file: CMarkedBuffer_00c44550::GetMark -> CircularQueue::GetInfo (RENAMES section 1); the file name moves
  (CMarkedBuffer00c44550_GetMark_00c442f0 -> e.g. CircularQueue_GetInfo_00c442f0). Test rewritten: links the row object (declares GetInfo,
  no body), checks the five words and the returned reference; PassPattern `00c442f0_TEST PASS` kept. 0x00C43810's test is unchanged
  (standalone model; it never links the row).
- vland warnings: `STANDIN_LEAF` on 0x00C442F0 and `RELOC_NAME ... labelled CMarkedBuffer_00c44550::GetMark` on 0x00C43810 clear with the
  catalog rename (HEAD had the same RELOC_NAME on the old spelling).
- Finding (NAME_FIXES.tsv, not acted on): 0x00C44550 (catalog CMarkedBuffer_00c44550::Release, July placeholder self leaf, no Leaf line) is
  bound as ?Release@CircularQueue@@QAEXXZ by 0x00C43810, and the table keeps three other spellings; queue it.

## q12384: 0x0041D165 NUISystem::CManager::OnCloseMenus (deferred: 1 row held by NF217)
- Name: both PDBs `?OnCloseMenus@CManager@NUISystem@@SIXXZ` (Ego_r 0x0042D999, FableWin 0x01B101F9; public static __fastcall);
  engine/NUISystem_CManager.h declares it (FABLE_RETAIL_CMANAGER_MENUCOUNT); the self row's leaf is already that. ResetMenus / CloseMenus
  are in no PDB.
- Census: 7 landed callers (callers.py; absrefs none). 0x0056F30B, 0x00589D30, 0x0059F304, 0x005FCE60 call it through the shared header
  (probed: each object references OnCloseMenus, work/c2_census.tsv). 0x0047D78B (ResetMenus, member of its HEAD-local NUISystem::CManager) and
  0x00621720 / 0x00621C80 (a free `CloseMenus()`) were wrong. 0x00621C80 is NOT in the queue line.
- 0x0047D78B: in-place member rename in the HEAD-local model. 0x00621720: the free declaration is removed and `static void __fastcall
  OnCloseMenus()` added to the row's HEAD-local NUISystem::CManager model (the class token was defined at HEAD; no new class token; the row
  cannot include the shared header beside that model). Both RELOCATION_MATCH + PASS first grade; the only symbol change is the one UND line;
  vland WARN set identical to HEAD (0x0047D78B loses its ResetMenus RELOC_NAME warning). Script: tools/closemenus.py.
- 0x00621C80's lock reads NF217 (not `released`), so it is not handed back. The same transform is graded in work/draft/00621c80.cpp:
  RELOCATION_MATCH, symbol diff the one UND line (its test is unchanged; not run). The coordinator can apply it (NAME_FIXES.tsv line).
  With the draft, 0x0041D165 goes 3 -> 1 spelling; without it, 3 -> 2 (no new split either way).

## q12386: 0x00C576E0 CAudioContext::SourceUnqueueBuffers one definition (done)
- HEAD: the owner's out-of-line definition is a COMDAT with selection 1 (strong); 0x00C57C90 / 0x00C73050 emit the same symbol from
  in-class definitions with selection 2. Control link (LINK unset, /DLL /NOENTRY /FORCE /NODEFAULTLIB) of the three HEAD objects: 2 LNK4006.
- Fix (GOTCHAS nf86 / nf88): the definition is `inline` and an anonymous-namespace helper `ReferenceSourceUnqueueBuffers()` returns
  `&CAudioContext::SourceUnqueueBuffers` (`// Fidelity: scaffolding, not retail`). Result: selection 2; RELOCATION_MATCH + PASS; comdiff vs
  HEAD: the leaf's bytes unchanged, the one new symbol is `?ReferenceSourceUnqueueBuffers@?A0x1cf41ed3@@...` (per-TU hash); cmpcopy 79 = 79
  for all three copies (GetProcAddress 0x00C4D740 and the IAT bound; the "alSourceUnqueueBuffers" literal is the same-named string
  COMDAT in all three objects). Link of the new owner with the two HEAD includers: 0 duplicates. 0x00C57C90 / 0x00C73050 untouched
  (0x00C73050's `// Inferred:` text kept as is).

## q12377: 0x0099E9B0 CCharString::UnassignString (blocked: one definition, 1 row left with a graded draft)
- Name: both PDBs `?UnassignString@CCharString@@IAEXXZ` (Ego_r 0x00AFA2E0, FableWin 0x02F624F0; protected thiscall); fable_string.h
  declares it protected with `// 0x0099E9B0`.
- Census (REFS.tsv; work/c4_census_final.tsv: every row compiled once, objdump -t for the family's symbols): callers.py 54 call sites,
  38 of them in 25 landed rows; absrefs none; out-of-class definitions (grep) in 8 rows. Family locks claimed up front (25 claimed; 0x00859040 held NF215,
  needed no change). Result: 23 of the 25 caller rows (0x0099ED10 / 0x0099EE20 among them) and the 3 same-TU strong copies (0x00859040, 0x0099ED10, 0x0099EE20; selection 1, 87 = 87 bytes
  and relocs equal) were ALREADY on IAEXXZ. The reloc_bindings witnesses for AAEXXZ (0x0099EAE0) and QAEXXZ (0x0099F690) are stale: those
  rows bind IAEXXZ today. Wrong: the self row (DEF QAIXXZ), 0x0099EC30 (ReleaseData), 0x0122A5E0 (Invoke@AutoTinyMethodTarget). So the
  "family pass" was three rows, not a batch; no transformer script was needed.
- Self row 0x0099E9B0: the July local CCharString / CCharStringData model (public `__fastcall` member) is gone; the row is on fable_string.h
  (FABLE_RETAIL_CCHARSTRING + CCHARSTRINGDATA_FRIEND + SHOVE_STRING_CLEANUP_PASS23) with the TU-mate body the three copies carry.
  RELOCATION_MATCH first grade. Defined symbols beyond the leaf: the header-inline pick-any COMDATs ~CBasicString<char>, ~CCharStringData,
  ??_GCCharStringData, CBasicString<char>::DeallocateData (selection 2, in no binding-table line, the set landed 0x00859040 emits too).
  The two with catalog owners equal them (cmpcopy, bytes and relocs): ~CBasicString<char> 44 = 44 (0x009A0190), ~CCharStringData 44 = 44
  (0x009A0430); ??_G / DeallocateData have no catalog row.
  The self rename is leaf-only (catalog name already right) and moves onto a spelling that already exists, so the address's spelling count
  drops; it does NOT wait for 0x0122A5E0 (nothing binds the old QAIXXZ). Flag: strictly, the "every referencing row on the new spelling"
  condition is not met while 0x0122A5E0 stays on its stand-in; no spelling is created.
- Link (LINK unset, /NODEFAULTLIB /FORCE; self + 0x00859040 + 0x0099ED10 + 0x0099EE20 + 0x0099EC30 + 0x0099EAE0): 8 duplicates at HEAD -> 9.
  The new one is the owner's strong IAEXXZ joining the three byte-equal same-TU strong copies (nf96a TU-mates; they already clashed with
  each other at HEAD). The other 7 are unchanged HEAD debt.
- Test rewritten on fable_string.h (links the row object): a derived probe calls the protected member; the block is laid out by byte
  offset; global new / delete replaced to count frees; four cases; PassPattern `UNASSIGN_STRING_OK` kept.
- 0x0099EC30 (copy ctor): in-place respell in its HEAD-local model (`protected: void UnassignString(); public:`, the 0x0099ED10 shape).
  RELOCATION_MATCH; one UND line changes; its RELOC_NAME warning goes. HEAD debt kept: it defines `_g_CCharString_liveCount` (0x013BD800,
  one of five spellings there).
- 0x0122A5E0 (blocked: one definition, graded draft): it is the dynamic atexit destructor for CCharString::NullString (0x013BD804), registered by landed
  0x01223EC0 (whose header proves it). The row is a free-function stand-in that cannot call the protected member. Probe: 0x01223EC0's TU
  compiled unchanged emits it as `_$E2`, RELOCATION_MATCH against 0x0122A5E0 and binding IAEXXZ (work/draft/0122a5e0_probe.cpp), but that
  row would define NullString and the count beside landed 0x01223EC0: the static-pair LNK2005 rule wants both in one landing. Coordinator
  decision; catalog name fix in NAME_FIXES.tsv. 0x0099E9B0 spellings: 4 -> 2.
- Finding: 0x0099B4D0's catalog name is CCharString::UnassignString, but its body and every binding are CWideString::UnassignString
  (NAME_FIXES.tsv).

## Summary for the coordinator
- Rows handed back (7): 0x00C442F0 (self rename; catalog + file name move), 0x00C43810, 0x0047D78B, 0x00621720, 0x00C576E0,
  0x0099E9B0 (self leaf rename), 0x0099EC30. recheck_final.log: all seven MATCH / RELOCATION_MATCH + PASS.
- Deferred: 0x00621C80 (lock NF217), graded draft in work/draft/. Blocked: 0x0122A5E0 (static-pair one definition against landed 0x01223EC0), graded RELOCATION_MATCH draft in work/draft/.
- Descriptive names: CircularQueue::GetInfo (0x00C442F0) and the sibling model's SQueueInfo member names; no others.
- Headers: no overlay (fable_string.h used through existing opt-ins only). Flags lines: none added. Funclets: no landed row includes any
  changed source (grep of rebuild/src + rebuild/tests).
- Tests rewritten: 0x00C442F0 (links the row), 0x0099E9B0 (on fable_string.h, links the row). Others unchanged.
- Rows claimed outside rows.txt: the 25 UnassignString caller rows (all free; one changed, 0x0099EC30; the self row was already in rows.txt); 0x00621C80 and 0x00859040 refused (NF217 /
  NF215 locks).
- New LNK duplicate: one (0x0099E9B0's strong IAEXXZ against the three same-TU strong copies, all byte-equal); see q12377.
- Rule slips: one lnk.py call ran without env.sh (failed on the Windows tool path; rerun). The RENAMES section 2 first had a column-header
  line (removed).
- Locks claimed and left unchanged (release them): 0x0099EAE0 0x0099EAF0 0x0099EB40 0x0099EBF0 0x0099EC70 0x0099ED10 0x0099EE20 0x0099EF40
  0x0099EFB0 0x0099EFE0 0x0099F040 0x0099F1F0 0x0099F2E0 0x0099F450 0x0099F4D0 0x0099F690 0x0099F830 0x0099F8B0 0x0099F930 0x0099F9B0
  0x0099FA50 0x0099FA90 0x009A0880 0x0122A5E0; from rows.txt unchanged: 0x0041D165, 0x0056F30B, 0x00C57C90, 0x00C73050.
- Follow-ups worth queueing: the 0x0099E9B0 duplicate (owner's strong IAEXXZ beside the same-TU strong copies in 0x00859040 / 0x0099ED10 /
  0x0099EE20, all byte-equal; 8 -> 9 in the family link); 0x0122A5E0 (graded draft, static pair); 0x00621C80 (graded draft, lock NF217);
  0x00C44550 and 0x0099B4D0 catalog names (NAME_FIXES.tsv); the stale reloc_bindings witnesses AAEXXZ / QAEXXZ at 0x0099E9B0.
