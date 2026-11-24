# NF299 notes (2026-11-23): T1 "other" made-up leaf names, continuation of NF292 / NF296 (hand-back 1)

## Summary
- **38 rows changed** (rows.txt), all RELOCATION_MATCH PASS in vland `--recheck-landed` (recheck_final.log = rc1 37 PASS + rc2 1 PASS (0x00A67FF0), 0 DIFFER,
  0 RELOC_CONFLICT, 0 LINK_FAIL / TCC_FAIL; the WARN SHARED_HEADER_AVAILABLE lines are pre-existing local models). No header, flag,
  catalog or test change (hdr/ empty; every test of a changed row is a standalone copy that does not link the row).
- **Entries reached: 46 of 835** (STATUS.tsv; APPLIED.tsv is per ENTRIES address, work/applied_perbinder.tsv per binder):
  done 11, done (table only) 7, done-partial 6, done-partial (table only) 1, skipped-port 9, skipped-held 12; plus one line
  outside ENTRIES (0x00794D90, a follow-up of NF296 entry 571). 790 entries not reached (`open`).
- Table: TABLE_CHANGES.tsv (bare lowercase addresses, NF292 / NF296 layout): 27 delete, 4 add, 2 rewitness (both CONDITIONAL: 0x00996610
  WriteData -> witness 0x006BA030, in the NF297 / NF298 stay-off set, not compiled; 0x0099B4D0 UnassignString QAE -> 0x0099BE70, a landed row
  that declares it public and defines a same-TU copy, source read). RENAMES.tsv: section 1 = the
  0x0077CCA0 leaf change; section 2 = 39 caller respellings (caller old new callee). NAME_FIXES.tsv: 3 owner self rows. REJECTED.tsv: none.

## Entry 1 (0x0077CCA0, the access fix NF296 handed over)
Owner row: `void DoStopRunning();` -> `protected: void DoStopRunning(); public:` and `// Leaf: DoStopRunning` -> the full
`?DoStopRunning@CTCHighlightItem@@IAEXXZ` (RENAMES section 1; GOTCHAS nf188: a bare Leaf makes the split gate see two spellings).
Binder 0x0077D140 (??_G) wrapped the same way. Link with the HEAD objects of 0x0077CD30 / 0x0077CDD0 (which bind IAE): HEAD left IAE
unresolved twice, the new set resolves it. 0x0077CDE0 (`CTCHighlightItem_Methods` stand-in) left: done-partial.

## Method
1. Triage as NF296 (cls1 -> cls2 -> filt -> tri5, outputs in evidence/triage_*.tsv), then bk.py buckets of EVERY landed exe caller
   (evidence/binder_buckets.txt) and dchk.py / rcalls.py / dcalls.py donor checks (evidence/donor_calls.txt, names only). New helpers:
   drefs.py (does a donor function take a target's ADDRESS: the thread proc passed to _beginthread), acc.py (the access label in effect
   at a declaration line, used to show no landed binder declares a public CThing::GetWorld).
2. Units: e1 (entry 1), a1 (12 binders, 8 owners), a2 (12 binders, 2 owners), a3 (7 binders, 4 owners, 1 table witness), t (3 binders +
   8 table witnesses, 1 owner), b (1 binder, 1 owner). Per row: test precheck (clang) BEFORE the lock, lock, HEAD precheck, HEAD grade in
   a later call, orig/ copy, respell by explicit line list (tools/nfix.py units a1 / a2 / a3 / t / b, every listed line asserted to
   change; access wraps on the declaration line with the section reopened as it was: `public:` or, in 0x0099B7D0, `private:`), new
   precheck, new grade in a later call, objsec under an explicit map (tools/proof2.py), link HEAD v new with the HEAD owners
   (tools/lkset.sh / lk.sh), owner and table-witness locks released after use. Locks held now = the 38 changed rows.
3. Table-only entries (STALE in filt.tsv: the witness's code no longer names the stand-in): compiled the witness's HEAD object and read
   what it binds (NF289 / NF291 Q1); bk.py showed every landed caller names the PDB identifier, so the made-up line is deleted (and the
   PDB line added where the table had none). Entry 37 looked stale too, but three OTHER landed callers still declared the stand-in:
   respelled them (unit t).

## Proof
- objsec (evidence/proof_e1 / a1 / a2 / a3 / a3b / t / b): every changed row ALL_EQ, no UNMAPPED symbol, table counts unchanged; each
  new symbol is the ENTRIES pdb_spelling exactly. proof_a3.txt shows UNMAPPED anonymous-namespace symbols for 0x008D48F0 / 0x008D49B0:
  the `?A0x` hash follows the TU path (rows/ v the repo path); proof_a3b.txt maps them (`?A0x` option): ALL_EQ.
- R1 (byte-identical, PDB-backed) wider fixes: `const` (0x0093AE80 GetPVillage, 0x006A1E00 IsProjectile); access wraps (0x0077CCA0,
  0x0077D140, the five CCompressorZlib rows, 0x0098F3A0, 0x0099B560, 0x0099B7D0, 0x00666EA0); parameter types from the PDB
  (0x00AC3B00 `unsigned long` -> `long`, 0x009FCCF0 `int` -> `bool`, 0x009FCD10 `long` -> `bool`; calls unchanged); one friend naming the
  real class (0x00666EA0: `friend class CThingCreatureBase;` on the PeekWorldMap line, because the row flattens CThingCreatureBase
  with no CThing base).
- Links (evidence/link_*.txt): every made-up spelling HEAD left unresolved is gone; no new unresolved reference; duplicate lines are the
  same in both sets (pre-existing: ??0CCompressorZlib in 0x0052ADA0 / 0x00996230; ~CVolume in four rows; ForwardToVolumeLevel defined
  by BOTH 0x009FCD40 and 0x00A69550 under one symbol, a pre-existing one-name-two-addresses case worth a look). Still unresolved, each
  explained: ?Uninitialise@CVolume@@QAEX_N@Z and ?Clear@CPackedUIntArray@@QAEXXZ (their owners define made-up names: NAME_FIXES).
- Census (evidence/name_census.txt): each new name is defined only by its owner row, except pre-existing definitions I did not add:
  PushNewSourceDef in 0x00771620 (the other overload, the row's own function) and CWideString::UnassignString in the landed rows
  0x0099BF30 / 0x0099BE70 / 0x0099BFF0 (same-TU copies under the PDB name already; not touched).
- tblafter.py: every `done` entry's target keeps only the PDB spelling after TABLE_CHANGES (it moved 0x00810FD0 to done-partial:
  0x008D43C0 binds a stand-in struct).

## Decisions / questions
0. **Two in-place forms for a ruling:** (a) 0x00666EA0 gets `friend class CThingCreatureBase;` on its PeekWorldMap line; that row models
   CThingCreatureBase through the `#define CThingCreatureBase CThingCreatureBaseDonor` header alias (pre-existing), so the friend names the
   real class but sits in an alias-pattern row: accept, or send the row to T3? (b) 0x0077CCA0's `// Leaf:` is now the full mangled IAE
   symbol (was the bare `DoStopRunning`), recorded in RENAMES section 1; the payload leaf follows it (mkpay.py).
1. **NF296 follow-up (0x00794D90).** NF296 entry 571 deleted ?CancelAbilities@CTCUnholyPower@@QAEXXZ (its only table witness,
   0x0069F8B0, was respelled), but 0x0072BA40 / 0x0072B420 bound the same stand-in with no table line. They are respelled here (same rows
   as entry 442); no table change. The table's one-witness-per-spelling limit (GOTCHAS nf98) hides such binders: an exe scan per target
   (xsites) before deleting a line would have caught it.
2. **NAME_FIXES (owner self rows, T3 per the standing rulings):** 0x00A67460 (defines ?ReleaseVolume@CVolume@@QAEXH@Z; PDB Uninitialise),
   0x00A629D0 (defines ??1CPackedUIntArray@@QAE@XZ; PDB Clear; identity caveat: the donor destructor is only a Clear call, so an ICF fold is
   possible, but the retail callers CloseEntries / PackRuntimeData are Clear callers), 0x00771620 (its own PushNewSourceDef returns long,
   both PDBs bool), 0x0077CCA0 handled in place.
3. **VIRT entries not attempted** (66 ChangeState, 82 FinishAction, 83 OnAnimationLoaded, 133 Initialise, 291 OnActivate, 572 WorldUpdate):
   the binders call a PDB-virtual base method through a non-virtual local declaration. Making it virtual adds a vtable slot to the local
   model (a vtable change: the brief says stop and hand back). Is `virtual` on those local declarations wanted, given the qualified calls
   stay direct?
4. **Bulk classification.** About 780 of the 835 entries are TEMPLATE/OP (721 table lines) or OTHERSCOPE (581) in cls1, i.e. stand-in
   classes / scopes, which are T3 ports by the standing rulings. E2 asks to open every binder before classifying; may the rest be marked
   skipped-port per class from the triage, with spot checks, or should the next agent keep opening them one by one?
5. 0x008E9710 (a table witness here) defines ?ClearStrafingCombinationAnims@CModeStrafe@NCreatureMode@@QAEX_N@Z; the donor names it AAE
   (private). Not an NF299 entry; noted for the tracker.

## Rule notes
- Late check (advisor): every `delete ... no landed binder left` line was re-read against bk.py's full caller list. It found
  0x00A67FF0 (respelled, full cycle, rc2) and the 0x0099BE70 / BF30 / BFF0 same-TU copies (delete turned into a CONDITIONAL rewitness);
  the other callers (0x00838950, 0x00666D40, 0x00663F50, 0x006946C0, 0x00666EF0, 0x00832840, 0x005CA68E, 0x00794C10) were confirmed by
  source (access label / shared header / other class / other target).
- Precheck control: tools/pre.sh on a planted missing-include file prints the fatal error (work/pre_control.log), so a silent
  pre.sh run means a clean clang pass.
- Every grade / link / vland ran in the background with `timeout 900` on its own launch line (gr.sh, lk.sh, rf.sh); completion read with
  kill -0 / log tails in later calls. No sleep, no rm (clean.py), no git, nothing written under the repo, no __asm / naked / _emit.
- One overlap: the a2 link ran while the a3 HEAD grades ran (both my own PIDs, both backgrounded under timeout).
- Stay-off set (work/stayoff.txt): NF297 / NF298 ENTRIES targets and spelling witnesses, their row files, their held locks, the standing
  held rows, Codex p28 still-reserved rows; T3_PORT_QUEUE rows (column 2) checked per binder. No lock conflict met.
