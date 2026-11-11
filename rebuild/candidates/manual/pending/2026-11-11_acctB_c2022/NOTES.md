# Hand chunk 2022 (CTCTavernGameCoinGolf .. CTCLightningOrb): 1 of 2 byte-exact

Row 0x007D3820 grades RELOCATION_MATCH under VC7.1 RTM from src/007d3820.cpp (gall.sh with `FABLE_HDR_OVERLAY=hdr`), its test prints
`007d3820_TEST PASS` under g++ (tall.sh) and VC7.1 (tall.py), and the read-only verifier (vland.py, overlay on) reports RELOCATION_MATCH PASS
(warnings RELOC_NAME for two known labels and RELOC_ALIAS __CIfmod only). Every call target was checked against retail (tools/rel.py: all
`same` or binding-name differences filed in NAME_FIXES.tsv). Row 0x007F26B0 is parked (frame layout only). Both extents are correct
(0x007D3820 `sub esp,44h` .. the flick path's `jmp` at 0x007D3F76, next function 0x007D3F80; 0x007F26B0 `sub esp,8Ch` .. `ret 8` at
0x007F2DB7, next 0x007F2DC0); no SPLITS.tsv.

## What matched (1)
- **0x007D3820 CTCTavernGameCoinGolf::ProcessInput** (vtable slot 28; catalog `vfunc_28`): camera stick turns/tilts the table camera while the
  action button is not held (movement scaled by its own magnitude, XAngle clamped to [0,1] with STLport std::max/min); in AIM_COIN the action
  button resets the aim and points the held coin's velocity along the camera heading (C3DVector::GetAngleXY of the camera's look direction minus
  the board's forward heading + 0.5, C2DVector::Rotate), the alternate button zeroes it, held movement blends the clamped / squared-off mouse
  effect into AverageJoystickPos and re-aims, and releasing with |Vel| >= JoystickMagBelow flicks (WATCH_COINS); quit buttons prompt as the
  siblings. Shape levers: chained member stores `MousePosEffect.X = AverageJoystickPos.X = 0.0f;` (retail's store order and the forwarded 0.0
  in the velocity product; `= C2DVector(..)` materialises a temporary, Initialise orders it differently); `look_at -= cam_pos; look_at.Z = 0`
  (retail rewrites the look-at slot); a named float for the camera turn's magnitude (`fstp st(0)` after the Y product); the camera-manager
  out pointer in its own block (frame sharing); GetCamera 0x007D3560 and GetSubDef<CTavernGameCoinGolfDef> 0x007D4BD0 defined in the TU
  (retail's TU; visible bodies keep the def / camera vectors uncaptured). **Inline-budget device** (owner 2026-11-08, preference 2): `// Flags:
  /d2inlT87` -- the XAngle clamp's std::max / std::min sit at static frequency 1/32 (five tests before them), c1 16, score 86 > 74; every
  traced control-flow spelling keeps 1/32; T87 is the smallest threshold, nothing else in the TU moves (GetSubDef stays called at 174). The
  sibling ShoveHaPenny row needed /d2inlT116 for the same kind of std::max site.

## Parked for Fable 5.1 (1)
- 007f26b0 CTCLightningOrb::Initialise DIFFER(1793v1802): frame layout only -- with [esp+N] masked all 512 instructions match (tools/md.py
  --frame); ours `sub esp,78h`, retail `8Ch`, so every frame offset differs. Retail slots: [10] reciprocal / approach_distance / lightning_def /
  effect / GetTC key; [14] approach_height / the AddTC and LIGHTNING_ORB CCharString temporaries / special_effects / helper name; [18..20]
  velocity / direction (+ the difference Z at [20]) / p1 and p3 sub-temporaries; [24] distance alone; [28..2C] the two CIntelligentPointer
  temporaries, the CDefString temporary, the &PSpline spill and the uninitialised emitter; 12-byte temporaries [3C] [60] [90]. Ours (c2 packer
  trace tools/pk): distance shares with the GetTC key (the key conflicts with the escaped `effect`), the 8-byte group absorbs three 12-byte
  temporaries plus `difference`, one temporary slot fewer. | tried (grades): o1 (difference / direction, `difference / distance`, implicit FPS
  conversion) 1871v1802; o4 (velocity named, in-place `direction /= distance`, parenthesised `(distance / Speed) * GetConstantFPS()` = fimul,
  named helper-name CCharString, GetEffectFromString declared only) 1795v1802; o5 (difference + copied direction: retail's X,Z,Y squares and
  stored reciprocal) 1793v1802 = frame only. Free probes: ~40 c2 frame-packer traces / fragment compiles (magnitude and normalise spellings,
  named-result Sub, `difference * (1.0f / distance)` (same x87 as o5), scope blocks around effect, GetSubDef/GetDef body visibility, dummy
  functions). Measured: the same-TU GetEffectFromString body (FABLE_RETAIL_CSPECIALEFFECTSDEF_GETEFFECT_BODY) makes VC7.1 reuse both dead
  parameter homes (distance etc. land at [esp+80h]/[84h]; retail never reuses them) while fixing the key/effect conflict; declaration-only
  keeps the homes unused but the escaped `effect` stays live at the key. | next: find the spelling that keeps `effect` unescaped (or its
  liveness short) without turning on parameter-home reuse (bisect what in GetEffectFromString's visible body enables it: v4 = GetSubDef +
  GetEffectFromString alone already reuses), then keep the 12-byte temporaries out of the [28] group (retail's [28] members must conflict
  with them: check whether the &PSpline CSE spill / emitter are one node in retail); grade only once tools/pk.sh reproduces retail's slot
  membership. Passes 2 and 3 were not used (context limit), 27 grades remain.

## Skipped (0)
None.

## Grades
- 0x007D3820: pass 1, 5 grades (c1 1964v1883 [std::max/min called, temporaries], c2 /d2inlT87 1879v1883, c3 RELOCATION_MATCH, the final
  src via gall.sh RELOCATION_MATCH, twice: once more after a comment-only edit). On top: ~20 fragment compiles (store-order / magnitude spellings) and 3 c2 inline traces, not graded.
- 0x007F26B0: pass 1, 3 grades (o1, o4, o5 above). On top: ~40 free probes (fragment compiles not compared, c2 frame-packer traces).

## Names
Row names, callees and members are PDB-proven (two sources each; LABELS.tsv). Descriptive: parameter and local names (action_held,
camera_manager, magnitude, cam_pos / look_at; parent / target, velocity, difference, direction, middle, up, flat_direction, approach,
approach_distance / approach_height, fireheart, helper_name), the EInputButtonEventType value 2 (written as a cast, as the sibling), the
ProcessPoints / Create parameter names. Landed spellings proven wrong: NAME_FIXES.tsv (12 entries, incl. the CoinGolf GetSubDef binding,
the three CTCLightningNode setters landed as `unattributed AssignTarget_*`, CCountedPointer<CLinearSpline>::Reset, GetDef<CLightningDef>,
ProcessPoints, CTCLightningOrb::OnInitialActivate (landed as LoadDefAndFlagAppearance) and the virtual FrameUpdate leaf).

## Header debt
hdr/README.md: 5 new headers (CTCTavernGameCoinGolf, CLightningOrbDef, CTCLightningOrb, CTCLightningNode, NBBBMaths_CLinearSpline) and opt-ins
in 5 copied repo headers (CTavernGameCoinGolfDef, C2DVector, C3DVector, NGraphicAppearance_CTCGraphicAppearance, CTCDParticleEmitter); the
c2007 overlay files are carried unchanged. The landed CoinGolf rows (GetCamera, UpdateValues, ...) and lightning rows (FrameUpdate,
OnInitialActivate, the node setters) still use local models; they can migrate to these headers.

## Label corrections
0x007D3820 catalog `vfunc_28` is CTCTavernGameCoinGolf::ProcessInput; 0x007F23D0 landed LoadDefAndFlagAppearance is OnInitialActivate;
0x007F39A0 / 0x007F39F0 / 0x007F3A40 are CTCLightningNode::SetPParent / SetPSourceOfFaction / SetPSourceOfHit; 0x00A58130 is
NBBBMaths::CLinearSpline::ProcessPoints (catalog CPSCSplineBase_GenerateCurvePoints).
