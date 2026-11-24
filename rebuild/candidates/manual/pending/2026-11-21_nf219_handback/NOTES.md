# NF219 notes (2026-11-21, round 10)

Lane: q.py grades in `S=$A/pS` (HEAD objects in `$A/pH`); vland payloads from tools/pay.py (rows/<addr>[_test].cpp when present,
else the landed files). tools/ = NF218's helpers (lnk.py, cmpcopy.py, comdat.py, comdiff.py, symdiff.sh, census.sh, pay.py), repointed.

## Brief items not fully met (read first)
- q12389 (0x0099B4D0): only 1 of its 8 landed binders (0x0099C120) is on the new self leaf ?UnassignString@CWideString@@IAEXXZ, so the
  strict self-rename condition ("every referencing row already binds it") is unmet; the address goes 6 -> 5 spellings (nothing bound the
  old free-function leaf), as NF218's 0x0099E9B0 rename. The new link duplicate is against 0x0099C120's DIFFERING strong copy (59 v 62
  bytes): coordinator's call.
- 0x01223EC0 is a header port (local CCharString model -> fable_string.h), not "unchanged" as the brief's probe wording says; its object
  is identical to HEAD (comdiff, symbols).
- Outside the queue lines: the 0x00C3CF30 (~CircularQueue) and 0x00C44360 (CircularQueue::Allocate) self renames, both forced by the
  q12388 family (no split otherwise).
- q12387: the brief asks that the pair "must not define NullString ... twice against each other". It does: VC7.1 emits a static's
  atexit destructor ($E2) only in a TU that defines the static, so both rows define CCharString::NullString. This is exactly the
  LNK2005 the 2026-11-20 static-pair rule describes ("no row-level respelling removes it"; fix = the TU-emitter rule). Recorded as DEBT
  (one LNK4006/LNK2005 on ?NullString@CCharString@@2V1@A). The count is NOT defined twice (0x0122A5E0 declares it extern).
  Measured and dropped: `__declspec(selectany)` on NullString in 0x0122A5E0 grades too (COMDAT sel 2), but no rule admits it and
  0x01223EC0's definition stays strong.

## q12387: 0x01223EC0 + 0x0122A5E0, the CCharString::NullString initializer / atexit pair (done)
- Identity: Ego_r.pdb `dynamic initializer for 'CCharString::NullString'` 0x0101AAD0 / `dynamic atexit destructor ..` 0x0101E240;
  FableWin.pdb 0x0400A4F0 / 0x04012360; Ego_d.pdb PUB32 ?NullString@CCharString@@2V1@A (the access digit: public static).
  Retail 0x0122A5E0 = `mov ecx,13BD804h ; call 0099E9B0 ; dec [13BD800h] ; ret`; 0x01223EC0 pushes 0x0122A5E0 to _atexit.
- 0x0122A5E0: the HEAD row was a free-function stand-in (`AutoTinyMethodTarget::Invoke` on g_AutoTinyMethodObject, --g_AutoTinyMethodCounter).
  Now the TU's $E2 on fable_string.h (new add-only FABLE_RETAIL_CCHARSTRING_NULLSTRING declaring the static; hdr/README.md), with the
  inline ctor/dtor bodies of 0x01223EC0 (`// Inferred:`) and the count declared extern. RELOCATION_MATCH first grade (forms: NF218's
  probe = 0x01223EC0's TU verbatim, then extern count, then header form; all three grade; the header form is handed back because the
  first two add a local `class CCharString` model to a row that had none, the nf95b case).
- 0x01223EC0: the same header form with the count defined (`= 0`, keeps the `mov [count],1` fold). Object identical to HEAD (comdiff
  4 = 4 functions; same defined / undefined symbols), so the row only loses its local CCharString model; stale label comment updated.
  Its test (local CCharString model, standalone) is HEAD debt, unchanged.
- Pick-any copies in the new 0x0122A5E0 object (new copies, p25 check): ??0CCharString@@QAE@XZ 15 = 15 v 0x0099E4B0, ??1CCharString@@QAE@XZ
  12 = 12 v 0x0099EAE0, bytes and relocs equal (cmpcopy). The same two copies are in 0x01223EC0 at HEAD.
- Link (tools/lnk.py, LINK unset, /NODEFAULTLIB /FORCE): new pair = 1 duplicate (NullString). New pair + owners 0x0099E4B0 / 0x0099EAE0:
  3 (NullString + the ??0 / ??1 owner-strong v pick-any lines); HEAD 0x01223EC0 + the owners: 2 (the same ??0 / ??1 lines). So +1 = NullString.
  No other landed row defines ?NullString@CCharString@@2V1@A or ?g_CCharStringInstanceCount@@3JA (source grep: only 0x01223EC0).
- Census (no split created; reloc_bindings witnesses + source grep):
  - 0x0099E9B0: callers.py = 25 landed caller rows, NF218's census had 24 on IAEXXZ plus 0x0122A5E0 (Invoke@AutoTinyMethodTarget);
    0x0122A5E0 now binds IAEXXZ -> one spelling in landed rows (table still has stale AAEXXZ / QAEXXZ witnesses: NAME_FIXES).
  - 0x013BD800: 5 table spellings -> 4 (g_AutoTinyMethodCounter used only by 0x0122A5E0 and its test). Finding: Ego_d PUB32
    ?NoStrings@CCharString@@1JA (NAME_FIXES; not acted on).
  - 0x013BD804: 6 spellings (5 table + NullString) -> 5 (g_AutoTinyMethodObject gone). The four reader stand-ins are in NAME_FIXES.
- Test 0x0122A5E0 rewritten on fable_string.h (self-contained; the static's initializer runs before main, the test runs the destructor
  on NullString and checks UnassignString + count; PassPattern `AUTO_TINY_0122a5e0_TEST PASS` kept).
- vland (work/b1.log): both RELOCATION_MATCH PASS. Warnings: 0x01223EC0 RELOC_CONFLICT `_$E2` (per-TU local name, unchanged object)
  and RELOC_NAME on 0x0122A5E0's old label (clears with the catalog rename). 0x0122A5E0: none (no SHARED_HEADER_AVAILABLE).
- Catalog / file: 0x0122A5E0 -> CCharString::`dynamic atexit destructor for 'NullString'' (module CCharString; RENAMES section 1).

## q12388: 0x00C44550 CircularQueue::Release (done; DESCRIPTIVE names)
- Name: ?Release@CircularQueue@@QAEXXZ, descriptive (libpk has no PDB; NF217 searched both PDB name sets, the exe strings, GameCODA;
  the libpk asserts naming PKCStaticQueue.h / PKCOneShotQueue.h are referenced only from 0x00C1A..-0x00C4B.. code outside this queue).
  It is the spelling landed 0x00C43810 already binds and matches the siblings GetInfo 0x00C442F0 / Clear 0x00C443D0.
- Census (callers.py + absrefs.py; absrefs none for 0x00C44550 / 0x00C3CF30 / 0x00C44360): binders 0x00C3CF30 (jmp), 0x00C3CF40,
  0x00C43810, 0x00C43EC0; 5 spellings at HEAD (self placeholder, CircularQueue, CMarkedBuffer, ??1CCoverMember_00c3cf40,
  _ForwardTarget_00c44550; the table also keeps a stale CMarkedBuffer_00c43810 witness). After: 1.
- The destructor finding (advisor's discriminating test, 1 compile): 0x00C3CF40 (vtable 0x012B5A88 = CAudioObject's) calls 0x00C44550
  directly for its +0x54 member while its unwind funclet 0x01214F88 jumps to 0x00C3CF30 (`jmp 0x00C44550`); HEAD bound both under one
  ??1CCoverMember_00c3cf40. With the member as the sibling CircularQueue plus in-class `~CircularQueue() { Release(); }`: the parent stays
  RELOCATION_MATCH binding Release at 0x00C44550, the funclet binds ??1CircularQueue@@QAE@XZ at 0x00C3CF30, and that pick-any copy is
  0x00C3CF30's 5 bytes (cmpcopy, bytes and relocs equal). So 0x00C3CF30 = CircularQueue::~CircularQueue (two sources: the funclet
  target and the emitted body), renamed from the free forwarder `_Forward`.
- Rows (vland work/b2.log, b3.log: all RELOCATION_MATCH + PASS): 0x00C44550 (self, sibling model), 0x00C3CF30 (self), 0x00C3CF40 (member),
  funclets 0x01214F80 / 0x01214F88 (regraded with the staged parent; files unchanged), 0x00C43EC0 (CMarkedBuffer -> CircularQueue; the
  layout keeps Lease at +0x78 because the model's 0xC stats bytes replace BufferState[0xc]), 0x00C44360 (self: CEventDataBuffer::Allocate
  -> CircularQueue::Allocate, so 0x00C43EC0's Allocate binding moves with it: 2 spellings -> 1), 0x00C43810 unchanged (rechecked).
- New CircularQueue tokens (header-less class: `grep -rlE '(class|struct) +CircularQueue\b' rebuild/include` = none; landed siblings
  0x00C437C0 / 0x00C43810 / 0x00C442F0 use the same shape, no base): 0x00C44550 (replaces CMarkedBuffer_00c44550), 0x00C3CF30 (new; the
  row had none), 0x00C3CF40 (replaces CCoverMember_00c3cf40), 0x00C43EC0 (replaces CMarkedBuffer), 0x00C44360 (replaces CEventDataBuffer).
- Leaf lines added: 0x00C3CF40 `??1CAIStateGroup_CreatureUseCoverBase@@QAE@XZ`, 0x00C43EC0 `?AllocateMarks@CMarkedBufferOwner@@QAE_NJP6GXPAU1@PAX1@Z1@Z`
  (both from the HEAD objects; the rows had none and the catalog names give no usable leaf). 0x00C43EC0's Leaf line makes vland print
  STANDIN_LEAF (its catalog label CExclusiveAssert::OnQuestItemUsed is propagated): queued with the CAudioObject finding.
- Link (tools/lnk.py, the five new objects): 1 duplicate, ??1CircularQueue@@QAE@XZ (strong 0x00C3CF30 v the byte-equal pick-any copy
  in 0x00C3CF40). DEBT, queued: the nf88 address helper cannot name a destructor.
- Callees kept: 0x00C3C370 stays FreeBlock_00c3c370 in 0x00C44550 (HEAD spelling; 0x00C3C370 has many spellings, untouched).
- Tests: 0x00C44550's standalone model respelled on the CircularQueue fields (PassPattern kept); others unchanged (standalone).
- Out of scope, NAME_FIXES: 0x00C443D0's self name (CircularQueue::Clear), 0x00C3CF40 / 0x00C43EC0 owner = CAudioObject.
- File names that move: 0x00C44550, 0x00C3CF30, 0x00C44360 (RENAMES section 1). The funclets include 0x00C3CF40's file, which keeps its name.

## q12389: 0x0099B4D0 CWideString::UnassignString (done: catalog name + self leaf)
- Both PDBs: ?UnassignString@CWideString@@IAEXXZ (Ego_r 0x00B026F0, FableWin 0x02F66B70; protected). The build entry's Module and file
  name already say CWideString; only the catalog name (auto-re-candidates) said CCharString. CCharString's is 0x0099E9B0.
- The self row was a July free `__fastcall CWideString_UnassignString(CWideString*)` over a local CWideString model (a class with a
  shared header). Form 1 (one grade): fable_string.h with FABLE_RETAIL_CWIDESTRING + _SPRINTF_ORIGINAL (the landed 0x0099BA70 setup);
  the header defines UnassignString inline, so an nf88 anonymous-derived address helper emits it (`// Fidelity: scaffolding`).
  RELOCATION_MATCH, 62 = 62 (cmpcopy: bytes and relocs equal). Leaf pick-any (sel 2).
- Census: callers.py = 8 landed binder rows; nothing binds the old free-function leaf (grep: only the row and its own test).
  Spellings at 0x0099B4D0: 6 -> 5 (the self moves onto IAE, which 0x0099C120 already defines). The binders are not respelled
  (the entry asked for the catalog / leaf fix; NAME_FIXES lists each binder's spelling).
- Link: self + 0x0099C120 = 1 duplicate, the self pick-any IAE v 0x0099C120's strong (selection 1) same-TU copy, which is 59 bytes
  against retail's 62 at HEAD already (a differing copy: debt, queued). Flag: this duplicate is new with the rename.

## q12226-q12229 (CCountedPointer ReleaseData) and q12383 (size-TU == opt-in): open
- Not started beyond the census: handed back at an entry boundary for context (not size: 18 distinct caller rows for q12226-q12229).
  Census (callers.py, REFS.tsv, work/c4_callers.tsv; absrefs.py none): 0x00419108 14 caller rows, 0x004190B6 3, 0x0042F974 3 (0x0042FD04 already on ReleaseData), 0x0042F9A0 2
  (0x0042FD6F already on it); thunks 0x00419423 / 0x0042FCFF / 0x0042FBCB no landed caller, 0x004194B5 one (0x0041AFA0).
  The ??1<NUISystem::CComponent> conflict joins it: 0x004291DE (ReleaseData) has 576 call sites in 108 landed rows, its thunk
  0x00429C53 26 sites in 6 rows; reloc_bindings has ??1<CComponent> and ReleaseData<CComponent> at 0x004291DE, no line for 0x00429C53;
  how many of the 108 rows bind ??1 there is unmeasured. Expect the q12388 pattern (inline `~CCountedPointer() { ReleaseData(); }`: parents bind ReleaseData,
  funclets / explicit out-of-line calls bind ??1 at the thunk) and NF202's FABLE_RETAIL_CCOUNTEDPOINTER_RELEASEDATA route.
- No family row was locked beyond rows.txt; rows.txt rows 0x004190B6 0x00419108 0x0042F974 0x0042F9A0 0x00429C53 0x004291DE 0x005A4CDD
  0x00CBFB7D 0x005A5B31 0x005A04F7 0x0065A075 are untouched (release or hand to the next agent).

## Summary for the coordinator
- Entries (8): done 3 (q12387, q12388, q12389); open 5 (q12226-q12229, q12383). Rejected / blocked / deferred: none.
- Rows handed back (rows/: 8 sources + 2 tests): 0x01223EC0, 0x0122A5E0 (+ test), 0x00C44550 (+ test), 0x00C3CF30, 0x00C3CF40, 0x00C43EC0,
  0x00C44360, 0x0099B4D0. Regraded unchanged: funclets 0x01214F80 / 0x01214F88, 0x00C43810. recheck_final.log: 11 RELOCATION_MATCH + PASS.
- Self renames (RENAMES section 1): 0x0122A5E0, 0x00C44550, 0x00C3CF30, 0x00C44360, 0x0099B4D0; binder respellings section 2.
- Header: one add-only opt-in, FABLE_RETAIL_CCHARSTRING_NULLSTRING (fable_string.h; hdr/README.md).
- New link duplicates: 3, all recorded debt: NullString (static pair rule), ??1CircularQueue (strong owner 0x00C3CF30 v byte-equal
  pick-any in 0x00C3CF40), ?UnassignString@CWideString@@IAEXXZ (self pick-any v 0x0099C120's strong differing copy).
- Descriptive names: CircularQueue::Release / ~CircularQueue / Allocate (libpk; the class name is the landed siblings').
- Flags lines: none added. Leaf lines added: 0x00C3CF40, 0x00C43EC0. EH funclets: 0x01214F80 / 0x01214F88 include 0x00C3CF40's file
  (unchanged name). No landed row includes any other changed file.
- Locks claimed outside rows.txt (keep until landing): changed 0x00C3CF30 0x00C3CF40 0x00C43EC0 0x00C44360; unchanged but rechecked
  0x00C43810 0x01214F88 (release at landing); 0x00C443D0 claimed and released.
- Rule slips: none known. Selfcheck by hand: every hand-back row RELOCATION_MATCH + PASS in recheck_final.log; defined-symbol diffs
  per row in STATUS; spelling census per renamed address in the entry sections (none grows).
