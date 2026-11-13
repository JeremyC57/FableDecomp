# Hand chunk 2006 (NScript::CV_BodyGuardScript::CBodyGuard .. CEngineInternalAttachedPrimitive): 3 of 4 byte-exact

All three matches are RELOCATION_MATCH under VC7.1 RTM (cl 13.10.3077) with the hdr/ overlay
(`FABLE_HDR_OVERLAY=/opt/fable_lane/w22/agent2006/hdr`), behaviour tests PASS under g++ and VC7.1, read-only vland: 3 WINS
(one non-blocking `WARN RELOC_NAME` for 0x00E5FC70, see Label corrections). Each matched on its first official grade; the
work was done with free probe compiles and the c2 traces (inline trace tools/inl.sh, frame-packer trace tools/pk.sh, copied
from agent2003 and re-pointed at this directory).

## What matched (3)
- **0x00E621F0 NScript::CV_BodyGuardScript::CBodyGuard::SetHenchmanAsHired(bool)** (1340 bytes, RELOCATION_MATCH). Hire: follow /
  ally the hero, hero faction, join HenchmanList and the wage bill, health bar per henchman type. Dismiss: stop following, bodyguard
  faction, leave the list and the wage bill; when the home level is not loaded, warp out (CreateEffect "HENCHMAN_WARP" one unit up),
  mark the type dead, zero its respawn timer and FadeOutAndKillEntity. Shape lessons: `vector<CScriptThing>::erase(position)` is
  the out-of-line COMDAT (FABLE_RETAIL_STLP_VECTOR_ERASE_OUTOFLINE); GetGoldCost must be an *inline* member (its COMDAT sits right
  after its first caller OnPredicateFail) -- an out-of-line same-TU body tells c2 the call keeps edx, and ParentClass goes to edx
  instead of retail's ebp; the four health-bar cases are written in retail's layout order (2, 3, 0, 1 -- jump table checked by
  hand); `Me.GetPos() + C3DVector(0, 0, 1)` inside the CreateEffect argument (GetPos after both string temporaries; the +0 adds
  fold).
- **0x00D91E00 NScript::CQ_HobbeCaveScript::CHobbeSpawn::Main()** (1399 bytes, RELOCATION_MATCH). The Hobbe Cave alarm spawner
  (count from the def or the marker's data string, capped by HobbesToSpawn; per frame: alarm, spawn timer, wait for the roaming
  population to drop under HC_HobbeMaxNumberInRegion, spawn a grunt off camera and > 8 from the hero; decrement HobbesToSpawn).
  Shape lessons: the generated-script `TRetVoid _Fail_Val; ... return _Fail_Val.Return();` form keeps the first three terminate
  exits' expanded DeregisterTimer pads in line as retail (plain `return;` moves them out of line); clear() is the out-of-line
  erase(first, last). Inline split: retail calls ~vector<CScriptThing> at 9 exits and expands it at 4 (plus the natural exit), and
  calls ~CTimer only at the spawn's terminate exit. The c2 inline trace showed all 28 sites right except the terminate check at the
  top of the dead-hobbe scan loop (score 64 < 74, expanded; retail calls it): `if (IsActiveThreadTerminating() && true)` there
  scores exactly 74 and matches. The row carries an `// Inline-budget:` line (owner 2026-11-08 device rule; retail's construct
  for that site is unproven).
- **0x00B399A0 CEngineInternalAttachedPrimitive::UpdateOutputPrimitive(float)** (1414 bytes, RELOCATION_MATCH). Parent transform
  (animated-mesh parents) pre-multiplied by the attachment transform (flag 2), the child inverse (flag 1; failure disables the
  output) and the attachment transform again (flag 4), converted back with RedoMatrixConversion, set as the output's object
  transform and enabled. Everything but the x87 term order matched within a few probes (RedoMatrixConversion's visible body gives
  retail's `mov ecx,edx` argument reuse). The three 12-element matrix products: VC7.1 reorders every unparenthesised sum
  canonically -- the written term order does not matter at all once the nesting is right -- and the canonical order depends on the
  inline structure: a single PreMultiply body with its own copy, SetMultiply on a caller copy, a free helper and hand-written code
  all leave 2..20 of the 36 sums in other orders. `PreMultiply(m) { CMatrix3x4 copy(*this); SetMultiply(m, copy); }` (both PDB
  members) leaves only block 1's E11 / E12 sums; writing the expanded GetParentTransform with a `result` local and `break`
  (instead of `return` from the case) closes those two. 

## Parked for Fable 5.1 (1)
- 00550dc0 NUISystem::CTable::Draw DIFFER(1399v1399): two frame-slot swaps of x87 colour temporaries -- retail 0x00550EFE `fstp
  [esp+74h]` (parent R) / 0x00550F53 `fstp [esp+3Ch]` (table R), ours 3Ch / 74h; retail 0x00550F59 `fstp [esp+38h]` (alpha) /
  0x00550F63 `fstp [esp+34h]` (blue), ours 34h / 38h; the reads at 0x00550F77..0x00550F92 and 0x00551080/84 follow (20 masked
  lines; every other instruction, offset and size equal) | tried: ~70 probe spellings (free / member operator* (the PDB has the
  member ??DCRGBFloatColour@@QBE?AV0@ABV0@@Z), named parent / table colours, Initialise ctor variants, declaration positions,
  operator*= forms, `const`, direct-init) | measured (frame-packer trace): the two 16-byte CRGBFloatColour temporaries of
  `CRGBFloatColour(Colour) * CRGBFloatColour(parent->GetRenderColour())` both have IL weight 2 and reach it in the order table
  temp, parent temp (the operator's parameter binding order), so the table temp joins `colour`'s slot (0x74) and the parent temp
  child_colour's (0x3c); retail has them reversed | next: a spelling where both IL references of the parent temporary precede
  the table temporary's second one, or trace which tuple supplies the second reference (list insert c2.dll 0x10788642).
  What the parked source already gets right (each measured): the Initialise-style CRGBFloatColour(const CRGBColour&) ctor (A, B,
  G, R conversion order: opt-in FABLE_RETAIL_CRGBFLOATCOLOUR_INITIALISE_CTOR), `child_colour = ... * colour` assigned to a
  loop-outer local (retail's product temp + copy), a named `render_position` for SetRelativeRenderPosition (fixes retail's `fst`
  and the fmul placement), `zoom.X = GetZoom().X; zoom.X *= RenderZoom.X` (operand order), the visible STLport map::operator[]
  (FABLE_RETAIL_STLP_MAP: the state key shares a slot; frame 0xd4 -> 0xcc), the operand orders of the position sums.

## Skipped
None. All four rows start at a real function start and end where the oracle says:
- 0x00E621F0: 1308 bytes of code, `mov edi,edi`, two 4-entry jump tables, ending at 0x00E6272C = 1340 (the oracle length). The
  reservation's 1384 ran over the int3 padding and the next function 0x00E62730 (an Alloc: `new 0x94` + the CV_BodyGuardScript
  ctor 0x00E5D140) to 0x00E62758. No SPLITS.tsv entry is needed.
- 0x00D91E00 (ends `ret` 0x00D92376), 0x00550DC0 (`ret 14h` 0x00551334), 0x00B399A0 (`ret 4` 0x00B39F23): int3 padding follows.

## Names (two sources each)
- Rows: ?SetHenchmanAsHired@CBodyGuard@CV_BodyGuardScript@NScript@@AAEX_N@Z (Ego_r 0x00EB7FA0, FableWin 0x039A84C0);
  ?Main@CHobbeSpawn@CQ_HobbeCaveScript@NScript@@EAEXXZ (both PDBs; landed Alloc 0x00D8F310 is the same class);
  ?Draw@CTable@NUISystem@@UAEXAAVCIEngine@@PAVCEnginePrimitiveHandle@@JPAJPAVCComponent@2@@Z (both PDBs);
  ?UpdateOutputPrimitive@CEngineInternalAttachedPrimitive@@QAEXM@Z (Ego_r 0x009D8810, FableWin 0x02D975C0; bound by the landed
  caller 0x00B3A0D0).
- Callees (all bound in reloc_bindings.tsv with the same spelling unless listed in NAME_FIXES.tsv): GetGoldCost 0x00E5FC70
  (?GetGoldCost@CBodyGuard@...@@AAEJXZ, both PDBs); GetParentTransformAnimatedMesh 0x00B39080, GetChildInverseTransform 0x00B39900,
  RedoMatrixConversion 0x00B38E30, GetParentTransform 0x00B398E0 (both PDB name sets; retail TU order); GFCharStringToInt
  0x0099E7F0; IsDistanceBetweenThingsOver 0x00CBE3EA; FadeOutAndKillEntity 0x00CBE716; CMorphingSprite::GetGRaphicIndices (slot
  149) / SetGraphicIndex (slot 145) from the CMorphingSprite vtable 0x0124A49C and both PDBs; game-interface slots from the shared
  CGameScriptInterfaceBase.h.
- No descriptive (made-up) names were introduced.

## Label corrections
- 0x00E5FC70 is ?GetGoldCost@CBodyGuard@CV_BodyGuardScript@NScript@@AAEJXZ (Ego_r / FableWin), not the landed descriptive
  `GetHenchmanGoldCost() const` (vland WARN RELOC_NAME; NAME_FIXES.tsv).
- 0x00B38E30 is ?RedoMatrixConversion@CEngineInternalAttachedPrimitive@@QBEXAAVCMatrix3x4@@@Z (const, reference); the landed row
  spells it QAEXPAVCMatrix3x4 (pointer, non-const) in a local model.
- 0x00B393D0 is ?GetChildInverseTransformStaticMesh@CEngineInternalAttachedPrimitive@@QAE_NAAVCMatrix3x4@@@Z (reference); the landed
  caller 0x00B39900 binds it with a pointer parameter (QAE_NPAV).

## Header debt / header changes (all in hdr/, additions only, opt-in; hdr/README.md)
- CEngineInternalAttachedPrimitive.h: FABLE_RETAIL_CENGINEINTERNALATTACHEDPRIMITIVE_TRANSFORMS (declarations of the TU's transform
  members).
- CMatrix3x4.h: FABLE_RETAIL_CMATRIX3X4_SETMULTIPLY (SetMultiply + PreMultiply through it). The existing
  FABLE_RETAIL_CMATRIX3X4_PREMULTIPLY body (chunk 764) is a different, landed-row-specific spelling; both cannot be on together.
- CRGBFloatColour.h: FABLE_RETAIL_CRGBFLOATCOLOUR_INITIALISE_CTOR (parked row only). The header's free `operator*` is not the PDB's:
  both PDBs have the member ??DCRGBFloatColour@@QBE?AV0@ABV0@@Z (same bytes in this row).
- NUISystem_CTable.h: FABLE_RETAIL_CTABLE_DRAW (the Draw override; parked row only).
- CV_BodyGuardScript.h: the pending hand-back H40g's FABLE_RETAIL_CV_BODYGUARDSCRIPT_CBODYGUARD_MAIN block (copied unchanged).
- CQ_HobbeCaveScript.h: H40g's header (its CBANDITSACRIFICE block copied unchanged) plus FABLE_RETAIL_CQ_HOBBECAVESCRIPT_CHOBBESPAWN_MAIN.
