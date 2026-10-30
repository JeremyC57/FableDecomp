# Retry sweep wave 2, section 4: notes from the resumed agent

The first sec4 agent stopped when its container restarted, at about 20:51. This file covers the resumed run only. The full per-row record is in STATUS.tsv: address, status, number of graded edits, levers tried, residue.

## Totals for the 59 WORKLIST rows that had no STATUS line
- 5 matched:
  - 008db030 RELOCATION_MATCH
  - 0090f770 RELOCATION_MATCH
  - 008eea80 MATCH
  - 00935ad0 RELOCATION_MATCH
  - 0092afe0 RELOCATION_MATCH
- 12 improved; each has a source in differ/ with a new Grade line: 0092d6f0, 008eeb40, 008af6e0, 008a9880, 0090bd40, 00970df0, 009478a0, 0091e270, 00931200, 00939670, 0091ddd0, 00921990.
- 34 held. Each held row had at least one graded edit, and most had 3 to 7.
- 8 NOT_ATTEMPTED. These are the parked skeletons of 1 to 5 bytes: 008bc1a0, 008c6db0, 008d5aa0, 008dcc80, 008de7a0, 008de080, 008e8060. The eighth is 008dd0f0, which has no draft. Each needs a first full draft of 1083 to 1784 retail bytes. I did not reach them in this run.

## Step 3: re-opened the rows the earlier agent held with at most one grade
- 008e8ff0: 5 more forms tried, still held.
- 008f99a0: 4 more forms tried, still held.
- 00966030: 3 more forms tried, still held.
- 00941690: 2 more forms tried, still held.
- Not re-opened, because their twins already carry 4 to 9 attempts:
  - 008adcc0, 0095c7c0 and 00978250 (twins of 00959380).
  - 00968510 (twin of 00968460).
  - 0090ebe0 and 00934e50 (twins of 0090e3d0).
  - 008af990: its "a=84" note says about 30 earlier attempts.

## Wins per lever (this run)
| Lever | Matched | Improved |
|---|---|---|
| `CThing::IsDead() const` (Ego_r.pdb `?IsDead@CThing@@QBE_NXZ`) inside a nested-if IS_THING_ALIVE | 00935ad0 | 00939670 (now register-only), 00921990 |
| Function-local static for a module global (store schedules after the argument push) | 0092afe0 | - |
| Inline `GetCreature() const` accessor | 0090f770 | - |
| Perpendicular taken on the `(point - Pos)` temporary | 008eea80 | 008eeb40 |
| Graded over the true extent (ext.py) | 008db030 | - |
| Inline `FramesSince(since, world)` helper (right-to-left argument order) | - | 0092d6f0 |
| `?true:false` on a bool call (`test al,al; setne al`) | - | 008af6e0 |
| Named local instead of a packed push_back temporary (4-aligned frame slot) | - | 008a9880 |
| Named C3DVector local for a computed argument (x87 adds before the pushes) | - | 0090bd40 |
| Filter constructor takes the vector by const reference | - | 00970df0 |
| `runnable = !near;` falling through to `return runnable` (a real bl flag) | - | 009478a0 |
| Object fetched before the argument: a named local, or an inline `operator->` on the intelligent pointer | - | 00931200 |
| Enemy read before creature; `range*range < distSq` | - | 0091e270 |
| Rotation offset built as a C3DVector before the scale | - | 0091ddd0 |
| Respelled to the PDB / landed names (bytes unchanged) | 0094d020, 0093dfc0, 0096eb40, 0090f110, 008b7350, 008d90b0, 009647c0 | 00957c30 (template IS_THING_ALIVE<CThing*>) |

## Residue families that held
- this/member esi-edi or ebx-edi colour swaps:
  - 008b9dd0, 008da7a0, 009210a0, 00911a90, 008af500, 009673b0, 00966030
  - The rest of 00939670.
- Frame-slot order or packing: 00968fa0, 0090bd40 (rest), 00970df0 (rest), 008a9880 (rest).
- A preset-to-zero local kept in the key's memory slot by retail and promoted to a register here: 009216e0, 00961ca0.
- Loop not rotated, or loop head aligned, in retail: 00926060, 00944300, 008a9880 (rest).
- x87 schedule and CSE: 008ef200, 009122a0, 0089c910, 0091ddd0 (rest).
- Pure scheduling: 008a8010, 008e8ff0, 008f99a0, 008930f0.
- Dead lookup loop that retail keeps and VC7.1 here deletes: 00941690.

## Names changed in this run (already in src/ or differ/)
- 0092afe0: the temporary's destructor now resolves to `??1CCombatActionBase@@QAE@XZ` (0x0062E900). The model now has an intermediate CCombatActionBase. Before, it called `??1CCreatureAction_PlayCombatAnimation`, which raised RELOC_NAME.
- 0092afe0: the radius global 0x013BB6B4 is now a function-local static, `lastMapRadius`. The name is descriptive; I found no PDB symbol.
- 00957c30 (differ): `template<class T> bool __fastcall IS_THING_ALIVE(const T&)`. It mangles as `??$IS_THING_ALIVE@PAVCThing@@@@YI_NABQAVCThing@@@Z`, matching the landed 0x0040DCE0.
- 0091ddd0 (differ): `long __fastcall GFROR13(unsigned long)` (`?GFROR13@@YIJK@Z`, NAME_FIXES row 00497890).
- 7 sources respelled before the context reset: 0094d020, 0093dfc0, 0096eb40, 0090f110, 008b7350, 008d90b0, 009647c0.
