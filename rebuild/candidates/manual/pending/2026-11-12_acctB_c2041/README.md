# acctB hand-back, chunk 2041 (1 row PARKED, 4 instructions from retail; cause is a shared header body)

Authored by an Opus 5.5 agent, parked state re-checked by the acctB local coordinator. Nothing is landed.

| address | result | notes |
|---|---|---|
| 0x006AB820 `?ResolveFacingDirection@CThingPlayerCreature@@IAEXXZ` | **parked**, DIFFER 2328v2328, 652 of 656 instructions exact; test PASS (g++; the agent also ran VC7.1) | `rows/parked/006ab820.cpp` + `.test.cpp`; 7 grades used (see below) |

Coordinator check: `rows/parked/006ab820.cpp` regrades DIFFER(2328v2328) under `hdr/`, the test prints
`006ab820_TEST PASS` under g++. Purity grep clean.

## The whole residue and its cause (agent's c2 scheduler-hook measurement)

One scheduling swap in the z-target arm: retail 0x006ABB81 `mov edx,[esp+48h]` / 0x006ABB85 `fld qword [0.0]` /
0x006ABB8B `mov [esp+6Ch],edx` / 0x006ABB8F `fsin`; ours `fld` / `fsin` / `mov` / `mov`. Everything else (operands,
allocation, frame words) is equal.

The shared `C3DVector::InitialiseFromAngles` body in `engine/C3DVector.h` writes the Y line as
`(float)(cos(..)*(cosYZ*length))`. The outer parenthesis leaves a no-code PAREN tuple between the Y `fmul` and its
`fstp`, the `fstp` issues one cycle early, and on the next cycle `fld 0.0` wins over the Y-copy `mov`.

**With the cast on the trig result only (`(float)cos(..)*(cosYZ*length)`) the whole row is byte-exact** (agent's free
probe). But that body breaks four landed rows: 0x00A22CA0 and 0x00A0C570 (through the header) and 0x00A14040 /
0x00A14260 (their local copies); 0x00746EC0 and 0x008BD3F0 match either way. One body has to serve every row, so the
agent did not use it and the row is parked. **This is a cross-row header question for the main coordinator**: either
those four rows have another spelling under the paren-free body, or retail's z-target direction is not
`InitialiseFromAngles` here. The row goes to the acctB near-miss lane next for the second possibility.

## Other review items

1. Grades: pass 1 used 7 of 10, passes 2 and 3 unused. After grade 7 the agent ran about 40 compile-only spellings of
   the z-target arm; none moved the deciding instruction, so it had no candidate whose grade could differ. The agent
   flags this itself as a deviation from "use the passes".
2. `GFGetRoundedSignedAngle` is `FABLE_FORCEINLINE` in the `GFAngles.h` overlay, with an `// Inline-budget:` line in the
   row (plain-inline cost 87, score 377). The clamps are written out by hand (as plain inline `GFLimit` would stay a
   call at these sites, scores 83 / 93).
3. Headers (`hdr/README.md`; started from c2039's overlay, 0x006ACA30 regraded RELOCATION_MATCH under it):
   `CThingPlayerCreature.h` (`GetMaxTurnSpeed`), new `CPlayerMovementDef.h`, `CPlayer.h`
   (`IsPCProjectileWeaponThirdPersonAimingMode`), `CTCStealth.h` (`StatGetInterfaceType`), `GFAngles.h` (`GFMod`,
   `GFWrapReturn`, `GFGetRoundedSignedAngle`, `GFArcCos`: bodies match retail's unreferenced copies at 0x006ADDB0 /
   0x005DB180; `GFGetAngleXYTo` body grades RELOCATION_MATCH out of line against 0x00661540).
4. `NAME_FIXES.tsv`: 0x006A99D0 is `?GetMaxTurnSpeed@CThingPlayerCreature@@IBEMXZ` (protected const, both PDBs); the
   landed row spells it `QAEMXZ`.
5. Landed 0x006A9690 names physics slot 74 (+0x128) `SetRHSet` in its local model; the RigidBody vtable and
   `CTCPhysicsBase.h` put `SetRHSet` at slot 75 (+0x12C). Catalog 0x006A9520 is probably
   `CThingPlayerCreature::GetTargetPosition` (one source only).
6. Not checked: the pending r3002 row 0x007F26B0 under this overlay (it needs its own overlay).
