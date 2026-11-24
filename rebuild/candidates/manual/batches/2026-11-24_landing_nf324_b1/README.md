# Landing nf324 (Account A, 2026-11-24): 678 flattened virtual members back onto their class (batches b1-b7, 122 header opt-ins)

Hand-back: `pending/2026-11-24_nf324_handback/` (NOTES.md first). The tool and helpers are under `naming_batch/agent_tools/nf324/`
(`flatten_pilot.py` v4, usage in its docstring). This README covers all seven batch folders `2026-11-24_landing_nf324_b1` ... `_b7`.

**Entries:** the 1,744 landed rows whose self symbol is a flattened free function `?Class_Method@@YI...(const Class* p, ...)`, while the
retail RTTI vftable placement names a virtual member of Class at that address.

| NF324 status | Rows |
|---|---:|
| done | 679 |
| skipped-port | 986 |
| skipped-held | 68 |
| rejected | 11 |

## The tool: flatten_pilot.py v4, extended by NF324 under the coordinator's brief
- The class and method come from the PDB symbol, so class names with underscores work.
- Calling conventions: `__fastcall` with stack parameters, `__stdcall` and `__cdecl`.
- Parameters:
  - pointer to reference rewrites at call sites (NF313 Q2);
  - same-width integer and `char` -> `bool` retypes onto the PDB parameter types (`RETYPES.tsv`, 25 rows; NF324 Q3, accepted because
    each is proven byte-equal);
  - enum returns as literal casts (NF307 D3).
- Three routes for the class model:
  - **local:** the row's own model, with vptr room taken from a `vtbl` stand-in, a leading pad, or an unused leading scalar;
  - **switch:** a landed, non-`_PASS` class-model switch;
  - **header:** an add-only `_RTTI_DECLS` opt-in, written by `hdr_rtti_decls.py`.
- Identity follows the coordinator's three-way rule by full mangled symbol. Rows are also held when a header or another landed source
  carries an in-class body.
- Landed byte-dump comments are dropped from the new sources.

## Proof
- **Per row:**
  - HEAD and new compiled at one work path;
  - the HEAD object grades the old flattened symbol, and the new one grades the PDB symbol;
  - objcheck: every non-debug section equal (bytes and relocations) under the one rename, equal DEF / UND sets under it, no new `??_7` /
    `??_R`;
  - `.drectve` equal, or plus `/DEFAULTLIB:"libcp"` on the switch / header routes (NF318 Q1);
  - MATCH / RELOCATION_MATCH with the unchanged landed test passing.
- **Across rows:**
  - one link of 681 HEAD and 681 new objects: 0 duplicates, equal unresolved sets;
  - new-name census: no other landed source defines any new name, apart from different-overload pairs (not a conflict).
- **Headers (b5, b6):** 122 add-only `_RTTI_DECLS` opt-ins; equiv_check over all 57,146 rows and tests: 0 lines, with a planted control.
- **At landing:**
  - all 795 orig/ copies, headers included, equal the repo;
  - no header overlaps the nf321-nf327 landings;
  - `land_scripted.py` checked staleness, then rechecked every row on the landed tree, and the graded symbol is the PDB symbol.
  - The first chain held all 679 rows because NF324's own locks were still live; the locks were released and the chain rerun.

| Batch | Route | Landed | Recheck |
|---|---|---:|---|
| b1 | local 86 + switch 306 | 392 | 392 PASS |
| b2 | local 3 + switch 99 | 102 | 102 PASS |
| b3 | switch; HEAD sources had byte-dump comments, so HEAD entries were built from the repo (unchanged since 2026-11-22) | 5 | 5 PASS |
| b4 | switch | 7 | 7 PASS |
| b5 | header (122 headers landed first) | 167 | 167 PASS |
| b6 | header | 0 | 0x008BFFB0 held: no HEAD payload (queued) |
| b7 | opt-ins NF323 already landed | 5 | 5 PASS |

## Rulings on NF324's questions (coordinator)
- **Q1 (396 rows on flat CCombatActionBase / CCombatSequenceBase family headers)** and **Q2 (79 rows whose own tests call the flattened
  name):** both are new routes. They were not started, because the owner said to launch nothing new. They are queued.
- **Q3:** the retypes are accepted (byte-equal proof, PDB parameter types).
- **Q4:** the HEAD entries were rebuilt from the repo; the new sources drop the byte-dump comments.
- **Q5:** the headers landed before b5 and b6.
- **Q6:** 0x008601B0 and 0x008601C0 stay held until a landed row defines GetUpFromSitting's macro.
- **Q7:** the extra RTTI-evidenced declarations in the opt-in blocks stay; they emit nothing.
- **Q8:** NF325's CEngineInternalPrimitiveBase change landed first. Its default branch is unchanged, and b2's recheck passed.

## Queued
- 1,066 T3 lines `nf324`: 986 port, 68 held, 11 rejected, and 0x008BFFB0.
- 1 NAME_FIXES line: 0x009E6FA0, a propagated catalog label.
