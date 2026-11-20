# 00eebdf0 (p25B, Account B, 2026-11-20): NScript::CExpression_StealScript::Main -- READY

| Row | Function | Bytes | Grade | Test | Mutants |
|---|---|---|---|---|---|
| 0x00EEBDF0 | NScript::CExpression_StealScript::Main() | 1240 | RELOCATION_MATCH (no flags) | PASS (206 targeted + 20,000 random runs) | 46 / 54 killed (3 equivalent, 5 same code) |

Strict vland (LANDVERIFY_STRICT_HEADERS=1, FABLE_HDR_OVERLAY=hdr/): `RELOCATION_MATCH PASS`; one warning, RELOC_ALIAS `_rand` -> the
MSVCR71 import thunk 0x00BFEB16 (expected). Purity: plain C++, no asm. Grades: 2 (the first compile of the port matched; the commented final).

## What the parked diagnosis missed
The parked draft (DIFFER 1239v1240, local models, "register allocation only: the constant 0 in edi, framesLeft in ebp") was a
hand-shaped text. On the shared headers, written in the generated quest-script text with the Ego_d.pdb locals and line table, it
matched at the first compile: `StealTimer` stays in memory and the zero register goes away without any lever.

## Source shape
Ego_d.pdb line table (egod_lines/, lines 965..1085) and locals: function scope `TargetedThing`, `_Fail_Val`; one block {Terminated,
Percentage, Finished, speech_movie, DeedHandle, StealTimer, TimerDuration, {Percentile}}. Line sizes pick the statement forms (4-byte bool
stores, `Percentage=0.0f` 5 bytes, `StealTimer=(long)TimerDuration` 11 bytes with `_ftol`, `Percentage=1.0f` 5, the 9-byte decrement, the
5-byte `jmp` closing `while(!Finished)`, `long Percentile=rand()%100;` 17). Each terminate check sits on its statement's line, as the
landed generated Mains (0x00E377D0, 0x00DDAE40).
- No `// Inferred:` lines.
- Header contract bodies: inline CScriptThing::IsAlive (VC7.1 expands it in the loop and calls the row TU's COMDAT, retail 0x004AB130,
  after it -- c2 site frequency), inline CIsThingConscious ctor / dtor; FABLE_RETAIL_CSCRIPTGAMERESOURCEOBJECTMOVIEBASE_OUTOFLINE_DTOR
  (retail expands the implicit movie ctor and calls ~ 0x006E7B80).

## Headers (hdr/, hdr.diff: two NEW files, no edit to an existing header)
- NEW engine/CExpression_StealScript.h -- NScript::CExpression_StealScript : CScriptBase (Game +0x40, sizeof 0x44 = Ego_r 88 - 0x14),
  ctor 0x00EEBDC0, Alloc 0x00EEC390, RegisterMain 0x00EEC2D0 (slot 1), Main (slot 2), Init 0x00EEBDE0 (slot 3); retail vtable 0x012F5700
  read from Fable.exe. Same shape as the landed engine/CExpression_FishScript.h.
- NEW engine/EMiniGameType.h -- MINIGAME_NONE .. MINIGAME_ORACLE (0..7): Ego_d.pdb LF_ENUM 0xCEC4 and the installed game's
  misc_def_types.h (ghidra_out/installed_game/header_enums.csv) agree.
- NEWLY FLAGGED (owner policy: listed, not a blocker) -- landed rows that define CExpression_StealScript locally:
  0x00EEBDC0 (ctor), 0x00EEBDE0 (Init), 0x00EEC2D0 (RegisterMain), 0x00EEC360 / 0x00EEC370 (CSpawnedFunc<CExpression_StealScript>
  SuspendableProcess / scalar deleting dtor), 0x00EEC3C0 (scalar deleting dtor); landed rows that define `enum EMiniGameType` locally:
  0x0064FC23, 0x00656CAA, 0x00EEB7B0.

## Test (test/00eebdf0.cpp)
Links the real row object (with its IsAlive COMDAT, the expanded CIsThingConscious ctor / dtor and movie ctor). Every external bound by
/alternatename to __fastcall stubs; fake Game vtable (21 slots), the hero {vptr, Data, Info} with slot 21 MsgIsHitBy, the target's
implementation slot 75 IsAlive, raw script object with Game +0x40. Scripted terminate / button-held / hit / witnessed / alive sequences,
stealable / in-movie / null-target flags, steal duration (incl. 0 and negative), stealth level / max, FPS, rand values. Checks the event
order against an independent model (float steps reproduced in double with the row's float stores), every UpdateMiniGameInfoBar value
bit for bit, argument identities (targeted thing, hero, deed handle, quest-name string, movie object, enumerators 5 / 5 / 6) and the
release of every string, thing and the movie; a terminate at every check position.

## Mutants (mutants/)
54 one-decision mutants: 46 killed. Survivors are equivalent on every reachable input: `Percentage=1.0f` -> `0.5f` (Percentage > 1
needs StealTimer and TimerDuration of opposite signs, impossible: StealTimer = (long)TimerDuration and only decreases while > 0), and
`rand()%100` -> `%50`, `<` -> `<=` (a success reaches the roll only with Percentage exactly 1.0, so every roll passes). SAME_CODE (5): a
literal or float-pool constant swap changes only a relocation target (mut2 compares bytes); the test's string / value checks would catch them.
