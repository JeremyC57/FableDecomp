# Landing nf326 (Account A, 2026-11-24): the CScriptThing pick-any unit, 10 members onto their PDB symbol with 101 copy rows (111 sources)

Hand-back: `pending/2026-11-24_nf326_handback/` (NOTES.md and CENSUS.tsv first); helpers under `naming_batch/agent_tools/nf326/`.
- **Entries:** 63. NF326 left every entry non-done, because the unit was held for this ruling. 52 stay skipped-port and 0x00904380 stays
  held.

## The unit
**The ten members:** CScriptThing::GetDefName, GetDataString, SetDataString, GetPos, GetAngleXY, MsgIsHitBySpecialAbilityFrom,
MsgIsHitByAnySpecialAbilityFrom, MsgIsHitByAnyAggressiveSpecialAbilityFrom, IsBeingCarriedBy and IsNull. Their self rows are at
0x004AA8D0, 0x004AA900, 0x004AA930, 0x004AA980, 0x004AAA40, 0x004AAD20, 0x004AAD40, 0x004AAD60, 0x004AAFE0 and 0x004AB150.

**Why it had to be one unit:**
- Retail expands these members inline in the script TUs: the exe census finds 0 to 2 copy rows among the retail callers per member.
- Many landed NScript / CGameScriptInterface rows emit them as pick-any COMDAT copies under the PDB names.
- A strong self is LNK2005 against those copies (control link).

**Decision (a), ruled NF326 Q1:**
- Each self defines its member `inline` on engine/CScriptThing.h and emits it pick-any (COMDAT selection 1 -> 2) through one per-TU
  `?A0x` helper (`ReferenceScriptThing<M>`), in the landed nf86 / nf88 form.
- **The ten helpers are scaffolding debt.** Retail emitted these bodies from caller TUs that did not inline them.
- 101 copy rows (74 out-of-class copies, 27 in-class bodies of local CScriptThing models) are respelled to the self's body shape. Their
  own functions and names do not change.
- All 20 retail-caller rows already bind the PDB spellings, so no table change.

## Proof
- **NF326's own proof:**
  - pair grades HEAD and new at one work path;
  - selfproof 10 OK and copyproof 101 OK;
  - COMDAT census after: 173 of 174 member COMDATs equal retail (before: 116 of 164 copies differed);
  - a full link of the 171 census objects HEAD and new: identical duplicate set (124 pre-existing lines) and unresolved set (794).
- **At landing:**
  - all 114 orig/ copies equal the tree after nf324;
  - no other landed self strongly defines any of the ten PDB symbols;
  - recheck of all 111 rows on the landed tree: 6 MATCH + 105 RELOCATION_MATCH, all PASS (the same as NF326's run);
  - each self grades its PDB symbol.
- **Re-run on the landing recheck's objects** (`pending/.../landing_proof/`), because a recheck grades only each row's own function and
  cannot see the copies' member COMDATs:
  - selfproof: 10 of 10 OK. The member bytes equal HEAD and Fable.exe at the address, the DEF set gains exactly the one helper, the UND
    set and `.drectve` are equal.
  - copyproof: 101 of 101 OK. Symbol tables are equal, and every member COMDAT equals both Fable.exe at the self address and the self.

## Rulings on NF326's questions
- **Q1:** the pick-any whole-unit form lands as one unit, for this unit only (BRIEF_NF_BATCH.md).
- **Q2:** GetDataString lands too. 0x00DF6010's copy is still 45 bytes against retail's 42 and is queued as debt: pick-any is not yet sound
  for that member.
- **Q3:** the mixed accessor / local spelling in copies stays (equal bytes). The accessor form adds two COMDATs per object (NF320 Q2).
- **Q4:** ~CScriptThing (0x004AA840) and the copy constructor (0x004ABE90) are queued as a follow-up unit with the same proof set.

## Queued
- T3 `nf326`: 53 port / held lines, plus the 0x00DF6010 debt and the two Q4 unit lines.
- NAME_FIXES `nf326`: g_EmptyCharString vs CScriptThing::NullString (0x0143E8EC); g_ZeroVector / g_ZeroPos vs CScriptThing::NullCoord
  (0x0143E8E0).

## Recheck
`logs/recheck.log` (landed tree, 111 rows): 111 PASS.
