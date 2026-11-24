# NF293 notes (2026-11-23): NF290's ruled decisions (entries 1-5) and T1 signature entries 6-74 (partial)

Base: the repo working tree as of today. **Rebase:** engine/CWorldMap.h changed in the repo during my pass (02:15, NF292's add-only FABLE_RETAIL_CWORLDMAP_FLAT_ISLOADINGANYMAP block at line 520, below mine). hdr/engine/CWorldMap.h is re-based onto the current file (orig/hdr = the current repo copy; my block re-applied, asserts already `_AT`). My three CWorldMap users preprocess to identical text under the old-base and rebased trees (cpp, md5), so their grades / rechecks stand; equiv_check was re-run on the rebased pair (evidence/equiv_hdr2.log). Every other orig/ file (52 rows + 2 headers) still equals the repo. I never wrote to
the repo and ran no git. STATUS / APPLIED use my ENTRIES numbering (data lines 1-390; 1-5 = NF290's ruled decisions, 6-390 = NF290 61-445).

## Result
- **Reached 44 entries:** 27 done, 5 done-partial, 1 done-already, 5 skipped-port (T3), 6 open-deferred (families, not started; reasons in STATUS).
  - The other 346 entries are `open` / not started.
- **51 rows changed** (rows.txt == my held locks, 51). Every changed row is objsec ALL_EQ against its HEAD object: every non-debug section's bytes and relocations are equal under the renames, `.drectve` included, and the DEF sets are equal (evidence/proof_g1..g5).
- **Retail grades:** every HEAD and new object grades MATCH / RELOCATION_MATCH (evidence/grades.log).
- **vland `--recheck-landed`:** 51 / 51 MATCH / RELOCATION_MATCH + test PASS, 0 RELOC_CONFLICT (recheck_final.log = rc1..rc4). rc2, the header group, ran with FABLE_HDR_OVERLAY = a full copy of today's include tree plus my three headers. No RELOC_NAME names a spelling I introduced; the RELOC_NAMEs listed are pre-existing names of other callees.
- **Links** (evidence/link_g1..g5; /FORCE; HEAD set v new set, both with the HEAD control objects): no duplicate on either side, and no reference HEAD resolved is unresolved in the new set.
  - Every moved spelling is unresolved at HEAD and resolved in the new set, except where the self was not compiled: 0x00A0BF20 (entry 6) and 0x009C0880 (entry 66). Both landed tests fail clang in their own lines, so I left them out as controls. Their Leaf lines are the PDB spellings.
- **Hand-back counts:** RENAMES has 6 self renames and 46 caller lines. TABLE_CHANGES has 31 lines: 29 from moved witnesses, plus 2 stale deletes (entries 7 and 9). NAME_FIXES has 5 lines.
- **Shared headers:** 3 add-only opt-ins (hdr/, orig/hdr/, evidence/hdr_*.diff).

## Decisions 1-5 (ruled by the prefix)
1. **0x006628E0 (R1):** the binder 0x006A98F0's TU emits no table (`tables=-`). `virtual` was added on both the base model and the override. The leading pad shrinks on its own line (`_pad_0x04[0x92 - 0x04]`), so every member offset is kept.
   - The override's own DEF moves QAE -> UAE: the PDB spelling, and a self rename of 0x006A98F0.
2-4. **Header opt-ins**, all add-only and declaration-only, with both PDB name sets agreeing:
   - engine/CThreadedFileManager.h FABLE_RETAIL_CTHREADEDFILEMANAGER_PROCESSOBJECTS_PRIVATE.
   - fable_user_profile.h FABLE_RETAIL_CUSERPROFILEMANAGER_LOAD_PUBLIC, in the #else class that 0x0040D400 sees.
   - engine/CWorldMap.h FABLE_RETAIL_CWORLDMAP_ALLOCATEMAPUID_PROTECTED, nested in the LOADFROMFILE branch.
   - **Asserts:** CThreadedFileManager.h's 3 unpinned asserts were pinned first (pin_asserts.py). The other two headers were already fully `_AT`-pinned.
   - **equiv_check:** two full tree copies (old = today's repo snapshot, new = + opt-ins), all 57,146 landed rows, 0 differing (evidence/equiv_hdr.log).
   - **Planted control:** a real token `NF293_PLANTED_CONTROL` in each header's default declaration, restricted to 5 includers; it flags all 5, src + test (evidence/equiv_planted_control.tsv).
   - **Line deltas:** the three users define the macro on one added line each (0x0098ECD0, 0x0040D400, 0x00507C30). 0x0040D400's landed test defines its Load / Save stubs through the same header, so rows/0040d400_test.cpp defines the macro too (one added line).
   - **Entry 4 as one unit:** self 0x004FCE20, 0x00506F30 (`protected:` in their local models) and 0x00507C30 were linked together.
5. **0x009CC530 (allowed):** the object is equal, and the only other renamed symbols are the unreferenced pick-any COMDATs GetSymbol and CArray<CCharString>::operator[], QAE -> QBE (inlined, no relocation to either).
   - The slot-19 virtual it calls had to become const. Counting engine/CBankFile.h's virtuals shows slot 19 is the protected const GetEnumerationType (0x009CBD10, MBE), so the made-up GetBankHandle was respelled to it. It is a virtual call: no symbol, no table in the TU.

## Method (signature entries)
- **Triage:** census (exe scan) -> decl.py / declsum.py (each census row's local declaration lines; rows that only include a shared header are counted, and the header's declaration was read) -> calls.py (call sites).
  - Every HEAD object of a changed row and of each self I used as a control was compiled with VC7.1.
  - The table witness of entries 7 / 9 was compiled and found already right (stale lines).
- **Edits:** a pointer -> reference parameter becomes `T&` in the declaration and `*arg` at the call, only where the HEAD argument already had the parameter's class type (or a derived one).
  - `long` -> `unsigned long` / an enum the row already declares / `bool`, only where the call passes a literal or a same-size value.
  - `const` / access where the PDB says so.
  - No header, flag, catalog or test change except the header group above. 0x009AD6E0's call lost a HEAD `const_cast` that the PDB-const callee makes unnecessary.
- **All-or-nothing check:** before moving a binder I checked that the self already defines the PDB spelling, or that the self is in the same set. Otherwise the binder's HEAD reference would become unresolved.
  - I first edited 0x00855AF0 (entry 25's self), then reverted it uncompiled: it calls the entry-10 stand-in base ctor that ~45 rows bind.
- **Catalog lookup** (coordinator note, NF294): nf.py's field() was bounded by the first `}` after `Address =`. Address is the first field of all 57,146 blocks, and for all 81 rows I compiled the old lookup equals a proper per-block parse (0 mismatches), so no grade used another row's flags.
  - blk() now parses the row's own `[pscustomobject]@{ ... }` block (0x00997800 now reads no CompilerFlags).
  - All 132 grade lines (evidence/grades.log) carry empty flags: none of the rows I graded has a CompilerFlags entry or a `// Flags:` value, so every retail grade used the base flags the real build uses.

## What is source-triaged, not compiled
The spelling counts (`N -> 1`) and the stale-line deletes rest on compiled objects only for the changed rows, the selves I used as controls and the two stale witnesses; the other census rows of each entry were classified from source (decl.py: a local declaration with the right types, or no local declaration and a shared header whose declaration I read). evidence/uncompiled_census_rows.tsv lists them (46: 17 local text, 29 shared header) for the split gate to recompile. 0x00BF7100 (entry 74) binds QAEK through engine/CEngineOcclusionManager.h line 195. 0x006A98F0's self rename: an exe scan finds no call to it, only the vtable 0x012457FC slot-3 word (0x01245808), which no landed row covers.

## Deferred families (STATUS `open`, not started), each one unit
- **Entry 10:** CCombatActionBase ctor; the self 0x00857F30 is a PAV stand-in, ~45 PAV binders.
- **Entry 20:** CCombatSequenceBase ctor, ~60 census rows.
- **Entry 24:** CCreatureActionBase ctor; the self 0x00693B30 is a stand-in, ~30 binders that also take CAnimationEntry*.
- **Entry 35:** PlayAnimation(CCharString) ctor; the self 0x008424F0 is a stand-in, ~28 binders.
- **Entry 19:** RegisterCombatant, 24 rows, mostly casts or stand-ins.
- **Entry 36:** not opened.

## Decisions / questions
1. **The deferred families:** may a pass take one whole family (self + every binder, ~30-60 rows) as a single all-or-nothing unit? Or should they go to T3_PORT_QUEUE?
2. **Entries 25, 30, 37 (done-partial):** each keeps one binder whose argument is a base type (CThing* for CThingCreatureBase&) or a stand-in. I classed those binders as T3 (cast debt per nf105).
3. **0x0040D400's Save (PDB QAE)** can take the same add-only shape as the Load opt-in. It is not in my ENTRIES: should the coordinator queue it?

## Rule slips
- `sleep_never=1` appears once in a command line (a variable assignment, not a sleep). There were no sleeps and no foreground cl / link waits.
- Clang prechecks ran in the foreground under `timeout 120`. VC7.1 cl /Zs ran in the background under `timeout 900`.
- equiv_check (gcc cpp, not a compile) ran in the background under `timeout 3000`, since a full run takes about 25 min.
- **No-rm rule:** I removed only my own rows/orig copy of 0x00855AF0, and did it through clean.py.
- No retail or donor bytes are in the hand-back files; the oracles stay in work/.
