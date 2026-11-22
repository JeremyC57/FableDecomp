# MEASURE 0x008E18D0 NCreatureMode::CModeThunderLightningStorm::UpdateLightningBeams (2073 B) -- AUTH11, 2026-11-22
S = /tmp/claude-0/-home-user-FableDecomp/5221bbec-fb18-575f-8649-a32d96bb1c2d/scratchpad; W = S/w22/auth11/008e18d0. Measurement only.
Instrument: odall10.py (VC10 SP1), preset fablelib (Ego_d compiland FableLib\debug\creature_modes.obj), lane headers + W/hdr.
Ego_d: Ego_d.pdb line table (3407 B; FableWin.pdb's record is the same size, locals and blocks identical).

## Calibration (first)
Ego_d CThing::GetTC<CTCDParticleEmitter>(CTCDParticleEmitter** ptc), 74 B, lines 9/21/17/15/4/2/6, block local `CTCBase& base`
(FableWin). W/cal/gettc.cpp reproduces it exactly (74 = 9/21/17/15/4/2/6) with: `if (HasTC(T::StatGetInterfaceType())) {
CTCBase& base = GetTC(T::StatGetInterfaceType()); *ptc = GFDebugPDynamicCast(&base, (T*)0); return true; } return false;`
(the 15-B line is the fastcall GFDebugPDynamicCast<CTCBase *,CTCDParticleEmitter *>(original_val, new_type), FableWin proc).
The same text inside the row's own header context (e5) also gives 74 = 9/21/17/15/4/2/6. e4's wrapper (repo
FABLE_RETAIL_CTHING_GETTC_EXPANDED) is one 31-B line forwarding to GetTC(id, ptc), so it differs from Ego_d's wrapper; but that
arm is documented as fitted to retail across the migrated rows, and the path below the wrapper is uncalibrated, so C5 is an
Ego_d-calibrated wrapper over an unchanged, uncalibrated path, not a proof that the expanded arm is wrong.

## Line table (Ego_d bytes, e4 bytes, corrected = e5 bytes); e4 has no line for 3292
| Ego_d line | what | Ego_d | e4 | corrected (e5) |
|---|---|---|---|---|
| 3278 | prolog `{` | 46 | 36 | 36 **≠** |
| 3279 | num_beams = size() | 20 | 17 | 17 **≠** |
| 3282 | angle_between_beams | 10 | 10 | 10 |
| 3283 | lifetime_in_seconds (dead) | 44 | 44 | 44 |
| 3285 | lifetime_of_main_beams (dead) | 26 | 26 | 26 |
| 3288 | storm_rotation = StormRotation (C2) | 15 | 18 | 15 |
| 3291 | storm_rotation_int = (long)storm_rotation (C2) | 11 | 20 | 11 |
| 3292 | storm_rotation -= storm_rotation_int (C2; no e4 line) | 9 | 0 | 9 |
| 3294 | beam_length = StormRadius | 15 | 15 | 15 |
| 3296 | CRightHandedSet rhs (C3: before weapon_pos) | 8 | 8 | 8 |
| 3297 | weapon_pos = GetParentThing().GetPos() (C1+C3) | 27 | 23 | 27 |
| 3298 | GetDummyObject (+C1) | 582 | 89 | 93 **≠** |
| 3300 | start_pos | 12 | 12 | 12 |
| 3302 | loop 1 for (C4 idiom) | 39 | 30 | 39 |
| 3305 | loop 1 IS_THING_ALIVE | 39 | 36 | 36 **≠** |
| 3308 | beam_direction | 29 | 29 | 29 |
| 3310 | RotateXY | 36 | 36 | 36 |
| 3312 | end_pos (C1) | 58 | 54 | 58 |
| 3313 | end_pos.Z (C1) | 27 | 23 | 27 |
| 3315 | ptc = 0 | 7 | 7 | 7 |
| 3316 | GetTC site 1 (C5 wrapper) | 517 | 36 | 36 **≠** |
| 3318 | vtStartPos | 40 | 40 | 40 |
| 3319 | vtEndPos | 43 | 43 | 43 |
| 3320 | upParams | 18 | 18 | 18 |
| 3322 | Insert | 18 | 18 | 18 |
| 3323 | Insert | 18 | 18 | 18 |
| 3324 | Update | 11 | 11 | 11 |
| 3326 | SetEmitterUpdateParameters | 15 | 15 | 15 |
| 3327 | } (~CParticleUpdateParameters) | 18 | 18 | 18 |
| 3328 | loop 1 back edge | 5 | 5 | 5 |
| 3330 | num_beams_ends | 20 | 20 | 20 |
| 3332 | loop 2 for (C4 idiom) | 39 | 30 | 39 |
| 3334 | loop 2 IS_THING_ALIVE | 39 | 39 | 39 |
| 3336 | beam_direction | 29 | 29 | 29 |
| 3338 | RotateXY | 36 | 36 | 36 |
| 3340 | end_pos (C1) | 58 | 54 | 58 |
| 3341 | end_pos.Z (C1) | 27 | 23 | 27 |
| 3343 | ptc = 0 | 10 | 10 | 10 |
| 3344 | GetTC site 2 (C5 wrapper) | 544 | 42 | 42 **≠** |
| 3346 | SetEmitterPosition | 18 | 18 | 18 |
| 3348 | loop 2 back edge | 5 | 5 | 5 |
| 3352 | if IS_THING_ALIVE(PSkyBeam) | 28 | 28 | 28 |
| 3355 | ptc = 0 | 10 | 10 | 10 |
| 3356 | GetTC site 3 (C5 wrapper) | 534 | 31 | 31 **≠** |
| 3358 | sky end_pos (one statement) | 49 | 49 | 49 |
| 3360 | vtStartPos | 40 | 40 | 40 |
| 3361 | vtEndPos | 43 | 43 | 43 |
| 3362 | upParams | 18 | 18 | 18 |
| 3364 | Insert | 18 | 18 | 18 |
| 3365 | Insert | 18 | 18 | 18 |
| 3366 | Update | 11 | 11 | 11 |
| 3368 | SetEmitterUpdateParameters | 18 | 18 | 18 |
| 3369 | } dtor | 18 | 18 | 18 |
| 3370 | epilog | 14 | 14 | 14 |

Totals: Ego_d 3407, e4 1377, e5 1416. 47 of 54 lines now equal Ego_d (e4: 37; e4 has no line for 3292).

## The corrections (one set; each has its own Ego_d evidence; all VC10-calibrated above)
- C1 `GetParentThing()` for all six ParentThing reads (3297 / 3298 / 3312 / 3313 / 3340 / 3341): every such line is +4 B in Ego_d
  (= `call GetParentThing ; mov ecx,eax` v `mov ecx,[ecx+4]`). Ego_d / FableWin procs CTCCreatureModeManager::CModeBase::GetParentThing
  (17 B, const and non-const); the repo opt-in FABLE_RETAIL_CMODEBASE_PARENTTHING (already in e4); the landed TU-mate 0x008DD7E0
  (same class, same file) spells it this way.
- C2 storm rotation: Ego_d 3288 / 3291 / 3292 = 15 / 11 / 9 = `float storm_rotation = StormRotation; long storm_rotation_int =
  (long)storm_rotation; storm_rotation -= storm_rotation_int;` (11 = fld local + __ftol2 + store; 9 = fild / fsubr / fstp on locals). The `-=` spelling is Inferred within its byte-equal
  family (`storm_rotation = storm_rotation - storm_rotation_int;` gives the same bytes).
  e4's `(long)StormRotation` / `StormRotation - storm_rotation_int` were 20 / 18. beam_length = StormRadius follows (3294 = 15).
- C3 `CRightHandedSet rhs;` before `weapon_pos` (3296 = 8 is the ctor, 3297 = 27 is weapon_pos with C1). **This is the
  "unexplained 27 B at line 3297": the weapon_pos statement, 23 B + 4 for GetParentThing.**
- C4 VC6 for-scope idiom `if (false) {} else for (n = 0; ...)` on both loops: Ego_d 3302 / 3332 = 39 = e4's 30 + 9 (xor/je/jmp,
  near exit). The landed TU-mate 0x008DD7E0 uses it (FableWin `xor edx,edx ; je` at 0x0285A2AC).
- C5 (separate in kind from C1..C4) the Ego_d GetTC<T>(T**) wrapper (opt-in FABLE_RETAIL_CTHING_GETTC_EGOD_WRAPPER in W/hdr/engine/CThing.h, add-only), as calibrated.
  Below it the path is HasTC -> CThing::GetTC(ETCInterfaceType) (Ego_d 303 B: 30/190/67/16) -> CVectorMap::Find (291 B, local
  `found`) -> LowerBound (519 B: left, count, count2, mid). Those three bodies are Dinkumware-iterator code in Ego_d and are NOT
  calibratable against the STLport model, so their repo text was left as is (GetTC(id) = `*Components.Find(id)->second`, Find default).

## Lines still different, and why (no correction proposed)
- 3279 (+3) and 3305 (+3): layout. Ego_d's LightningBeamParticleEmitters is at +128 (TPI 0x2B26D; Dinkumware 20-B vectors), so its
  `add ecx` / member reads are imm32 / disp32; retail's is +0x74 (disp8). The +128-and-up members (3330, 3334) already agree.
- 3298 (+489), 3316 (+481), 3344 (+502), 3356 (+503): each holds one debug assert: FableWin's record has exactly four
  S_CALLSITEINFO `int ()` (IsDebuggerPresent) at +0x322 / +0x61E / +0x9FA / +0xC4D, one inside each of these lines. The call
  stays in retail (GetDummyObject is called, GetTC expanded), so it is a VERIFY-style macro whose release form is unknown. One
  FableWin-style assert with inline asm measures 281 B on that line (cal/e5_assert.cpp), so these are larger asserts; not modelled.
- 3278 prolog (+10): unexplained. Not the EH frame or `this` slot (both present in ours), not the inline-asm assert (the probe
  keeps 36), epilog equal (14). No text candidate.
- Wrappers on the other forced sites: ~CParticleUpdateParameters / its ctor have no carved Ego_d proc (names in TPI only); the
  `}` line 3327 / 3369 (18) and Update (Ego_d 33 B) agree; CVectorMap<ulong,C3DVector>::Insert -> vector insert ->
  __uninitialized_copy is STLport v Dinkumware: not calibratable.
- Checked and equal: the sky end_pos 3358 = 49 (one statement), the dead locals 3283 = 44 and 3285 = 26.

## For-scope idiom and inl.py's f (readouts, no forcing; /Gr /d2inlT65 as AUTH10, i.e. NOT the grade's flags)
Three unforced readouts with the same flags: e4, e4i (= e4 + idiom only), e5 (all corrections). Only the row's own decisions
(between the previous pipe event and the row's pipe; W/inl/row.py, outputs W/inl/<v>.row.txt), blocks labelled by IS_THING_ALIVE.
c2 calls a site when its score > T (checked on e4 at T65: 66 / 68 / 82 call, 63 / 51 / 24 expand).
| site | retail | e4 f / sc | e4i f / sc | e5 f / sc |
|---|---|---|---|---|
| loop 1 IS_THING_ALIVE | expand | 5.0 / 24 | 2.5 / 26 | 2.5 / 26 |
| loop 1 LowerBound | call | 1.25 / 82 call | 0.625 / 95 call | 1.0 / 88 call |
| loop 1 _M_insert_overflow x2 | call | 1.25 / 66 call | 0.625 / 77 call | 0.625 / 77 call |
| **loop 1 ~CParticleUpdateParameters** | CALL | 2.5 / 23 expand | 1.25 / 27 expand | 1.25 / 27 expand |
| loop 2 IS_THING_ALIVE | expand | 25 / 22 | 7.5 / 23 | 7.5 / 23 |
| **loop 2 LowerBound** | CALL | 6.25 / 63 expand | 1.875 / 74 call | 1.875 / 74 call |
| sky IS_THING_ALIVE | expand | 25 / 22 | 9.0 / 23 | 9.0 / 23 |
| sky LowerBound | expand | 6.25 / 63 expand | 2.25 / 71 call | 2.25 / 71 call |
| sky _M_insert_overflow x2 | expand | 6.25 / 51 | 2.25 / 57 | 2.25 / 57 |
| sky __uninitialized_copy (first of each Insert) x2 | expand | 6.25 / 63 expand | 2.25 / 71 call | 2.25 / 71 call |
| **sky __uninitialized_copy (remainder) x2** | EXPAND | 3.125 / 68 call | 1.125 / 84 call | 1.125 / 84 call |
| sky ~CParticleUpdateParameters | expand | 12.5 / 20 | 4.5 / 21 | 4.5 / 21 |
Row decisions: e4 277, e4i 255, e5 270 (e5 adds three Find decisions, f 1.0 / 1.875 / 2.25, all expand).
(The e4 values equal AUTH10's forced-run record for the four sites, so that record's inputs were sound.)

What the idiom does: yes, it moves f at every site, unevenly: loop 1 x0.5, loop 2 x0.3, sky x0.36. Against AUTH10's three
"no single T" arguments:
- REMOVED: "LB loop 2 call and LB sky expand at the identical record" -- the records now differ (74 v 71), so T in [71, 74)
  would split them the retail way.
- REMAINS (moved): the sky's remainder copies need expand at sc 84 (T >= 84) while loop 2 LB needs call at 74 and loop 1
  overflow call at 77 (T < 74). And in that window the sky's first copies (71) still call where retail expands.
- REMAINS: the loop-1 dtor (sc 27) still expands; its call in retail depends on the inner ~vector, as AUTH10 described.
So retail still behaves as if the sky were hotter relative to the loops than this text makes it; the idiom narrows one gap and
leaves the ratio problem. C1..C3 and C5 do not move the four sites' f (e4i = e5 there).

## The one grade (pre-registered in LOG.md)
e5.cpp, stock VC7.1, row flags none, no hook, W/hdr: **DIFFER(1948v2073)**, 287 reloc-masked differing instructions (mdiff.py).
The grade (no flags) and the readouts (/Gr /d2inlT65) are different builds. There is no same-config e4 baseline (AUTH10: e3 stock
2061v2073, e4 /Gr /d2inlT65 2044v2073), so 1948 cannot be read as better or worse than e4, and the one grade bundles C1..C5, so it
does not say which correction moved the size. Not a match; no further grading. The c2 decision-routine question goes back to the coordinator.
