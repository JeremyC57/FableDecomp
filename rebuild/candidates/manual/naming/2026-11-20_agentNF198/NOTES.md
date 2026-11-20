# NF198 notes (2026-11-20, round 10; remaining queue, next 50: q11718 .. q12091)

Handed back early for context, at an entry boundary: the queue resumes at q12017 (APPLIED lines `open`).
Counts come from APPLIED.tsv / STATUS.tsv / recheck_final.log.

## Entries (APPLIED.tsv)
- done 8: q11911 (catalog label, NAME_FIXES), q11955, q11973 (claim corrected to SetThingScanInclusion), q11991, q11993,
  q12004, q12005, q12049.
- rejected 1: q11919 (claimed spelling in no PDB; real fix recorded in NAME_FIXES, binder 0x00418DCA blocked with the q11916 family).
- blocked 16: q11718 / q12068 (33-row GetSubDef family, census reused), q11724, q11902 (partial: 1 row ported), q11916, q11920, q11923,
  q11936, q11942, q11954, q11956, q11959, q11982 (17-row family, census reused), q11994, q11974 (partial: binder 0x00645CE9 respelled;
  self 0x0048DA62 held, one-definition), q12058 (partial: 0x00917DB0 respelled; 6 QAE binders left, listed in APPLIED).
- open 25: q12017 .. q12091 (not started).

## Rows changed (20, all in recheck_final.log: 20 RELOCATION_MATCH + PASS)
- Self renames (RENAMES s1, 9): 0x005ED270, 0x004C9C10, 0x0088C160, 0x008F1CD0, 0x007D5AD0, 0x007D7520, 0x007DB280,
  0x00CB0E90, 0x00416047 (catalog name changes: 0x005ED270, 0x00CB0E90, 0x00416047; the others are leaf-only).
- Binder respells (RENAMES s2): 0x006E13E0 0x007EECF0 0x008ECBE0 (q11973), 0x00645CE9 (q11974), 0x00833010 (q11991), 0x004D5A11 0x004D5A44
  0x004D5AAA (q11993), 0x0057C2E6 (q12049), 0x00917DB0 (q12058), plus the self row's own callee respell 0x004C9C10 -> 0x004C8C00.
- Binder port: 0x00A5C570 (q11902; local CBaseClassNonCopyable model replaced by engine/CBaseClassNonCopyable.h; binds IAE). Its lock
  read `released` (NF182); claimed for NF198.
- Every changed row: defined-symbol diff (objdump -t, HEAD vs new) shows only the row's own leaf, or nothing for binders, except
  0x005ED270, which also defines ??0CCPPointerInfo@@QAE@KP6IXPAX@Z0@Z (engine/CCountedPointer.h inline; no reloc_bindings line) and drops its
  two stand-ins. Pick-any: 0x005ED270 owns the CCountedPointer<NCameraModes::CScriptModuleBase>(T*) ctor by explicit instantiation of the
  header inline ctor (the landed 0x007A4210 recipe, so its copy is pick-any like the callers'); the six callers were re-probed at HEAD
  (RELOCATION_MATCH, binding the new spelling); their copies were not byte-compared with cmpbody.py (the header has `if (data)` / `if (Data)`
  variants under FABLE_RETAIL_CCOUNTEDPOINTER: the coordinator's symdiff check should compare them).
- One-definition check of every new self leaf (tree grep for other definitions): only CFontBank::GetStringWidth(const CWideString&) hit:
  landed 0x006572A0 emits ?GetStringWidth@CFontBank@@QBEMABVCWideString@@@Z pick-any from a local inline body (HEAD compile), so the
  0x0048DA62 self port (RELOCATION_MATCH + PASS, work/held/0048da62.cpp) was HELD: a strong owner would LNK2005 (GOTCHAS nf135 / nf140).

## Self renames: file moves and funclets
- The source file moves with the rename for 0x005ED270, 0x004C9C10, 0x00416047, 0x00CB0E90 (file names carry the old stand-in /
  catalog name). grep of rebuild/src/compiled/01/21 for every changed row's file name: no EH funclet includes any of them.

## For the coordinator
- q11973: the queue claim (SetAsGameStatePersisted) was wrong; the address is SetThingScanInclusion (both PDBs, Ego_r layout bit,
  callee UpdateThingScanInclusion, the catalog itself). REJECTED.tsv has the evidence; the applied spelling is the one 0x00705D40 bound.
- q11919: 0x009C0190 is CDisplayManager::AddDeviceResetCallback (both PDBs, catalog, header, 2 landed binders); the queue spelling is in no
  PDB. Only 0x00418DCA binds a stand-in there; it waits for the q11916 family port (NF152 header claims).
- q12004: 0x00CB0E90 is strmbase CAMThread::GetRequest (its own Vendor-proof line). Its TU-mates look misnamed the same way and are not in
  this slice: 0x00CB0E30 (catalog CThreadMessage::Send; Vendor-proof ?CallWorker@CAMThread@@QAEKK@Z) and 0x00CB0EB0 (CThreadMessage::TryReceive).
  Queue them for a light vendor pass.
- 0x00833010 (q11991): the row already cast its GetDisplayEngine stand-in (for CThing::PeekComponent 0x004C7A70) to CMainGameComponent*;
  the cast is kept, now dereferenced for the reference parameter (no new cast). Its 0x004C7A70 stand-in binding is unchanged.
- 0x0088C160 (q11991) carries an `// Inferred:` line: the PDB's const CMainGameComponent& parameter is stored into the PDB's non-const
  CMainGameComponent* member through a cast (same bytes).
- q11911: the catalog spelling CShaderRenderManager::`dynamic atexit destructor for 'SingletonInstance'' follows the sibling initializer
  0x01223D00 and CTBase 0x01229260 catalog lines; the PDB name sets write the class inside the quotes
  (`dynamic atexit destructor for 'CShaderRenderManager::SingletonInstance''). Coordinator decision which form the catalog takes.
- q12004 (0x00CB0E90): EXTRA_RULES 2026-11-04 vendor-code names rule (DirectShow BaseClasses name preferred) covers the rename; vland
  warns VENDOR_PROOF_MISMATCH only because the strmbase tree is not under FABLE_THIRDPARTY in this lane.
- WARN STANDIN_LEAF on 0x00416047 / 0x00CB0E90 is the old catalog label until RENAMES s1 is applied.
- Leaf lines written in full where a self row had a bare one (GOTCHAS nf188): 0x00A5C570, 0x0088C160, 0x008F1CD0; and added where missing
  (0x007D5AD0, 0x007D7520, 0x007DB280).
- Locks claimed beyond rows.txt: 0x00A5C570 (was released), 0x007EECF0, 0x008ECBE0, 0x007D7520 (released), 0x007DB280, 0x004D5A11,
  0x004D5A44, 0x004D5AAA, 0x00CB0E90.

## Descriptive names
- None new. 0x00CB0E90 takes the vendor source's name (strmbase wxutil.cpp), not a made-up one.

## Flags lines
- None added.

## Header opt-ins (hdr/README.md)
- engine/CThing.h: FABLE_RETAIL_CTHING_SETTHINGSCANINCLUSION (declaration only, nested in STATEACCESS). Add-only; no includer defines it.

## Not re-probed
- q12049 / q12058: only the binder the queue named was moved; the other REFS rows were not re-probed (the earlier landings recorded that
  binder as the only split). The gate's exe scan covers them.

## Rule slips
- Two Bash calls exited 1 (a tool named dis.py shadowed the stdlib module; a `cat` of a missing lock file at the end of a call).
- The first 0x00A5C570 grade ran without the cpp precheck (it compiled; every later grade was prechecked).
- Two early lock reads ran with $S unset and printed an empty owner (re-read with the full path before any edit).
- q12058 was first marked done on a table witness; the HEAD census (work/shoptc_census.txt) then found 6 more QAE binders: relabelled
  blocked/partial.
- Census for q11718 / q11902 / q11916 / q11936 / q11942 / q11982 / q12049 reused from earlier round-10 work, as the brief allows.
