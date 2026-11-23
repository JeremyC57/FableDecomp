# NF312 notes (2026-11-23): T1 signature, NF310's continuation (ENTRIES 1-19 = NF310's skipped-held, 20-85 = its open entries)

Base: the repo working tree of today (after landing nf310). No repo writes, no git.

## Reading
Read in full: my BRIEF.md prefix, the standing brief BRIEF_NF_BATCH.md (with the NF299-NF310 rulings), docs/pipeline/GOTCHAS.md (all
513 lines), NF310's hand-back (NOTES, LESSONS, STATUS, held/README + RENAMES_held) and its landing README; skimmed EXTRA_RULES.md
(naming rules) and COORDINATION.md.

## Off-limits lists
tools/mkoff.py, retargeted to NF309 / NF311 and widened: work/offlimits.txt (brief's held rows, Codex p28 still-reserved rows, every
T3_PORT_QUEUE address), work/other_agents_entries.txt (NF309 / NF311 ENTRIES targets), work/other_agents_witnesses.txt (every 8-hex
address inside the spellings column's parentheses, every reloc_bindings.tsv witness of an NF309 / NF311 target, and their rows.txt).
Per row tools/cstat.py / aflags.py / whose.py, and lock.sh for every compiled row. No lock held by another agent was hit.

## Result: all 85 entries reached
- **85 classified:** 12 done, 4 done-partial (table only: 73, 78, 83, 84), 1 done-equivalent (32), 24 skipped-port, 14 skipped-port
  (census), 30 skipped-held. Order: held 1-19 re-checked first (only 18 freed by NF308's landing; a port), then the in-place group
  g1 = 20, 21, 72, 75 (withdrawn), 77, 80, 81, 82, 85; g2 = 23, 37, 74 (withdrawn), 76 + the table-only 78 / 83 / 84; g3 = 22; g4 = 73
  (table only); every other entry classified from its source (evidence/port_census.tsv; every port binder's source opened).
- **14 rows changed** (rows.txt == the 14 locks I still hold; every control lock released). No test changed (all 14 landed tests are
  standalone: none includes its row or stubs the respelled callee). No header changed (hdr/ empty). All 14 orig/ copies equal the repo at
  hand-back (cmp).
- **2 self renames** (RENAMES section 1): 0x0088F400 SetCreatureGeneratorsCreatureGroupAsEnabled (UBI -> UBE, `__fastcall` dropped
  with its callee's, new full Leaf), 0x006A7B20 CThingFilter_PosInContainmentVolume::operator() (PAV -> const CThing&, full Leaf).
- **RENAMES section 2:** 14 caller lines. **TABLE_CHANGES:** 16 lines (8 delete: 5 for moved witnesses, 3 hand deletes of stale lines
  whose witnesses already bind the PDB form; 7 replace; 1 rewitness). **NAME_FIXES:** 2 lines.
- **Proof per changed row:** HEAD + new VC7.1 objects RELOCATION_MATCH (evidence/grades.log); objsec ALL_EQ for all 14
  (evidence/proof_all.txt: 14 ALL_EQ, 0 NOT_EQ; every non-debug section incl. `.drectve`, relocations under the renames, DEF sets);
  tables emitted by 009432C0 / 00EFB8C0 / 00DF2AB0 / 00D89380 / 00DF2380 / 00DFE0F0 unchanged (inside ALL_EQ). `nf.py syms` HEAD v new
  per row (evidence/syms_g1..g3): only the respelled symbols change; DEF renames only in the two selves and the pick-any copies of
  entry 22 (below).
- **Links** (evidence/link_g1, g2, g3; /FORCE, HEAD set v new set with the selves + census controls): no new duplicate; nothing HEAD
  resolved is unresolved in the new set; the new set resolves every PDB name HEAD left unresolved (IsPotentialCustomer, ObjectHasProperty,
  TransferIMEColour, GetDummyObjectRecursive, FoundMessageIsThingHitBy, FadeInThing, FadeOutThing, IsPlayerThreateningEntity,
  SetCreatureGenerationGroupAsEnabled, GFSerialisePointer, GetOldTargetFlagsAndReplaceWith, PosInContainmentVolume). The
  CThingFilter_IsPotentialCustomer ctor 0x00799D80 stays unresolved both sides (no landed self in the set).
- **vland --recheck-landed** (recheck_final.log, all 14 rows from rows/ + their landed tests): WINS 14 (14 RELOCATION_MATCH), every test
  PASS; no RELOC_CONFLICT / NOT_GENUINE / HEADER_POLICY; the RELOC_NAME warnings name other callees' labels (pre-existing; 0x009432C0's
  CrouchAndTalk / SitAndTalk v the catalog labels Teach / CrouchAndTalk look like a swapped pair worth a later look).
- **Table lines v the full caller list:** every target touched was censused in the exe (nf.py census: calls, jumps, absolute dwords in
  landed code; evidence/census_exe.tsv) and every census row compiled as a control and read with bind.py (evidence/bind_g*_head/new),
  except rows of other agents / off limits, whose sources were read (named in STATUS). The only virtual target touched is 0x0088F400
  (slot 560): no landed code references it, and the only landed CGameScriptInterface vftable emitter 0x006E7740 (compiled) emits a
  1-slot local table, so no vtable binder of the old UBI spelling exists.
- **Name census** (evidence/name_census.txt): every old (HEAD) symbol of the changed rows and every new one looked up in reloc_bindings.tsv,
  and the touched names' definitions grepped in rebuild/src/compiled: each new symbol is defined only by its self, except the entry 22
  pick-any copies (decision D1) and the header-inline GetPhysicsTC const getter (pick-any in 0x006689E0 too, copy EQUAL). Path includers
  (same file): no landed source, test or header names any of the 14 changed files. Locks: the NF312-owned locks are exactly rows.txt.
  Two sources per name: evidence/pdb_namesets.tsv (every entry spelling in both PDB name sets; the Bind2 instance, the const GetPhysicsTC
  and GetPhysicsMeshInfo are FableWin-only, second source the landed 0x00669110 Leaf / NF310's 0x006A7930 / engine/CTCPhysicsBase.h:203
  + the 0x0075BFD0 self).

## Decisions / questions
- **D1 (entry 22, pick-any copies beside a strong self):** 0x006698D0's force-inline CFilter_Bind2 / CThingFilter_OverlappingSphere
  operators are emitted as pick-any COMDATs; under the PDB reference form they are renamed onto the PDB names. The Bind2 copy is byte- and
  relocation-equal to the landed 0x00669110 (its own explicit instantiation, also pick-any); the OverlappingSphere copy is equal to
  0x00669110's pick-any copy. The OverlappingSphere self 0x006689E0 defines that name strongly with equal bytes but a different
  GetBoundingInfo relocation (it binds the PDB QBE AAV form, 0x00669110 / 0x006698D0 the stand-in QAE PAV form): HEAD already links with
  that one duplicate (0x006689E0 v 0x00669110), the new set has the same single duplicate symbol. I landed it under NF304 Q1 / NF310 D1
  (copies equal to the landed pick-any copy, no new duplicate symbol). Agree, with 0x006689E0 queued for the nf86 / nf88 pick-any form and
  the GetBoundingInfo respell of 0x00669110 / 0x006698D0 in NAME_FIXES? Or hold entry 22 until that respell? NF304 Q1 read literally
  ("otherwise T3") may refuse it: then drop together exactly rows/006a7b20.cpp + rows/006698d0.cpp, RENAMES section 1 line 0x006A7B20,
  section 2 line 0x006698D0, and the TABLE_CHANGES delete of ??RCThingFilter_PosInContainmentVolume@@QBE_NPAVCThing@@@Z (witness
  0x006698D0); the other 12 rows and 15 table lines are independent of it. Evidence: evidence/comdat_census.txt (cmpcomdat EQUAL lines,
  COMDAT select values, the GetBoundingInfo relocation per copy, the GetPhysicsTC const copy EQUAL to 0x006689E0's), evidence/link_g3.txt.
- **D5 (entry 73, rewitness onto a DIFFER row):** the new YIKK witness 0x008ED590 grades DIFFER(141v166) on this lane at HEAD, so
  bind.py could not map its call site; the proof that it binds ?GFROR13@@YIKK@Z is its object's UND symbol plus its source (the only
  caller including candidates/object_family_compiled.h). Accept the rewitness on that basis, or delete the line and leave 0x008ED590's
  binding untabled until the header is fixed?
- **D2 (entry 22, virtual slot name):** 0x006A7B20's local slot 93 `void* GetPhysicsMeshParameters()` is now
  `const class CThingPhysicsMeshInfo* GetPhysicsMeshInfo() const` (both PDB name sets UBE, engine/CTCPhysicsBase.h:203, the slot-93
  comments in CTCPhysicsRigidBody.h / CTCPhysicsStandard.h): the PDB-const body (`const CTCPhysicsBase*` from the const GetPhysicsTC)
  forces the const (R1); the rename binds no symbol (virtual call). Flagging the made-up name it replaces.
- **D3 (entry 23, calling convention):** the self and its callee both lose `__fastcall` (thiscall, PDB UBE / QAE). Retail's
  `mov ecx,[ecx+4] ; jmp` is the same for a thiscall -> thiscall tail call (RELOCATION_MATCH, ALL_EQ). A convention change is not a
  `.drectve` / flag change; I read it as an in-place self fix (NF304 Q6, no landed binder). Agree?
- **D4 (entry 37, re-scope out of a namespace):** 0x008D66E0's local CThing model moved from NCreatureMode:: to the global `class CThing`
  (NF300 Q2 in reverse: the TU emits no vftable / RTTI for it and none of its own definitions change name). The local model was already a
  HEADER_POLICY local copy (inside the namespace) and stays one; vland reports no HEADER_POLICY for the row.
- **Queue suggestions (T3 / NAME_FIXES):** 0x00502170 (stand-in global vector + a made-up same-TU body of 0x00501D30), 0x007786F0 (a
  made-up same-TU body of 0x00778610 under the PDB name's callee: respelling it duplicates the self), 0x005580B0 (stand-in CActionList
  whose front() COMDAT would be renamed), rebuild/include/candidates/object_family_compiled.h:21 GFROR13 `unsigned long` (header edit,
  sole includer 0x008ED590, which also grades DIFFER(141v166) on this lane at HEAD), 0x006689E0 (pick-any form), and the port families in
  STATUS (local `namespace std` models of vector / list / map / deque / pair; CTaskList, CMessagePtrList, CRetailVector,
  CThingSearchToolsMethods / CResultVector, CThingConstPtrVector, CScriptThingVector, ProgressPointer, CMovableResourceHandler,
  CThingDefClass stand-ins).

## Method
- Groups: g1 (9 rows + 16 controls), g2 (3 rows + 35 controls incl. the table-only witnesses; 5 controls whose clang precheck failed only
  on MSVC-only semantics passed a VC7.1 `cl /Zs` first, NF300 D1 extension, evidence/pre_zs.log), g2b (vftable emitter 0x006E7740),
  g3 (2 rows + 0x00669110), g3b (0x006689E0), g4 (0x0091B010 / 0x008ED590). Per group: changed + controls lists, pre_tests.sh on every
  landed test BEFORE the locks, locks, medit.py / ins.py specs (work/spec_g*.json, ins_g2.json), clang prechecks of every new and HEAD
  source, grades in up to 4 background lanes (`timeout 900` per grade line), then bind / proof / syms / link / gen_ren / assemble, then one
  vland recheck of all 14 rows.
- Two probes withdrawn after the precheck / proof (kept in work/rejected/, their orig copies moved there too): 0x007786F0 (entry 75) and
  0x005580B0 (entry 74); their locks released.

## Rule notes
- No sleeps; every VC7.1 grade, cl /Zs, link and vland ran in the background with `timeout 900` on its own launch line (inside
  grade_lane.sh / zsq.sh / link_g.sh / recheck.sh) and was checked with kill -0 / done files in a later call; clang prechecks ran in the
  foreground (each under `timeout 120`). No rm: nothing was deleted (withdrawn probes moved within my folder). Every heredoc quoted. No
  retail or donor bytes in hand-back files (the vland oracle stays in work/; rdis.py output only on screen).
- Housekeeping: an empty `D:\Documents\FableTLC\rebuild\build\landverify` directory tree appeared in my folder at 06:14 (a helper importing
  verify_and_land with cwd = my folder); removed with clean.py (no files inside). /home/user/FableDecomp has one at its root dated 00:30,
  before this run: not created by me, left untouched.
