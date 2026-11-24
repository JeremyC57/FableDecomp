# NF328 (2026-11-24): the ~CScriptThing / copy-constructor pick-any unit (NF326 Q4)
Brief: BRIEF.md + rebuild/candidates/manual/naming/briefs/BRIEF_NF_BATCH.md. Read UNIT.md first (census, proof, decisions D1-D6).

## Result
- **Held, nothing to land yet.** The stop condition hit: 19 of 529 emitted ~CScriptThing copies and 2 of 296 copy-constructor copies
  differ from retail (UNIT.md D1), and the retail-faithful fix for most of them adds a COMDAT (D2).
- **Ready if D1 is ruled "debt":** 9 rows (rows.txt) that turn every strong definer pick-any: the two selfs (one helper each), the
  ??_G row 0x004AB170 (one helper, D3) and six script rows (`inline` only). Each is pair graded at one work path, objsec-proven
  (tools/uproof.py: one helper at most, selection 1 -> 2, every other section equal, member COMDAT equal to retail) and vland
  rechecked (9 RELOCATION_MATCH PASS on both sides, identical warnings). A link of all 1,017 census objects drops exactly the 528 + 295
  duplicate lines of the two members, adds none, and keeps the unresolved set identical.
- Port lines (stand-in / two-view binders) are skipped-port with per-row reasons (STATUS.tsv). 0x00DF6010 not reached.

## Process notes
- Census by object (NF326 lesson): 1,023-row text gate, 1,017 compiled (all MATCH / RELOCATION_MATCH at HEAD), 6 Codex-reserved holes
  (D5). The queue line's counts (3 strong, 3 differing, 25 copies) came from NF326's 171 objects; the full census finds 8 strong dtor
  definers, 19 differing dtor copies, 296 copy-ctor objects.
- Prechecks: clang on every compiled source (evidence/pre.log); 64 HEAD sources that fail clang on their own MSVC-only text passed
  `cl /Zs` in an earlier call (evidence/pre_zs.log, D1 / NF300 extension); the 3 changed 0x00CE rows fail clang identically on HEAD
  and new (NF305 Q2) and passed `cl /Zs` both sides; tests: 5 clang PASS, 4 `cl /Zs /Od /W3` PASS.
- Locks: the 9 changed rows stay locked for the landing; the 1,008 census-only rows and the probe row 0x00D1F610 were released.
- Repo drift during the run: engine/CUserProfileManager.h and a few B rows changed; none is in the include closure of the 9 rows
  (clang -M) or named by any census source except a comment in 0x00D34470. orig/ equals the repo for all 9 rows at hand-back.

## Rule slips
- The landed tests of the 9 changed rows were prechecked after their locks were claimed (the brief says before). No grade compiled a
  test before its precheck passed.
- Probe grade `pb` wrote its three variants to one object name (the grades are logged per variant; tools/gradefile2.py fixes it).

## Names
- The two helper names (`ReferenceScriptThingDestructor`, `ReferenceScriptThingCopy`) had 0 hits under rebuild/ before this run. Each
  lives in its TU's own anonymous namespace: the two `ReferenceScriptThingDestructor` copies (0x004AA840, 0x004AB170) carry different
  `?A0x` hashes (?A0xf0e82e09 / ?A0xe6b8f9ba in the pair-grade objects), so they never meet at link.
- No symbol is renamed. The member spellings are in both PDB name sets; ??1?$CCountedPointer@VCScriptThing@@@@QAE@XZ (D2) is in
  FableWin.pdb only.
- T3_PORT_QUEUE screen: none of the seven non-self changed rows appears in column 2 or column 3 of any other queue line.
