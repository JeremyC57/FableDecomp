# Hand chunk 2020 (CTCWife .. CTCLight): 2 of 3 byte-exact

All work graded under VC7.1 RTM 13.10.3077 with `FABLE_HDR_OVERLAY=/opt/fable_lane/w22/agent2020/hdr` (hdr/README.md). Extents checked
for all three rows (int3 padding before each start; ends at `ret` / `ret 10h` + jump table / `ret 4`); no SPLITS.

## What matched (2)
- **CAIStateGroup_GoToPub::IsRunnable 0x00949AB0** (RELOCATION_MATCH, 2 grades; test PASS g++ / VC7.1; vland RELOCATION_MATCH PASS).
  Reads the creature's AI scratchpad (TC 0xAA): UnderScriptControl = seat name != "" && table name != "" (conditional string
  temporaries, ebx flag bits). Scripted: PTable = GetObjectWithScriptName(table name); the table's CTCOwnedEntity (TC 0x33) gives
  PPub = owner; `if (GetSeat(PTable, &seat_name) && GetBar()) return true;`. Otherwise, past NextFramePub and at tavern time in the
  creature's village (CTCVillageMember::GetPVillage, IsTavernTime): PPub = GetNearestBuildingWithProperty(pos, 0x20);
  IS_THING_ALIVE(PPub) -> `if (IsStaffed() && GetSeat() && GetBar()) { FreeSeat(); return true; } FreeSeat(); return false;`.
  Shape lessons: (1) the random-table half is GetSeat (0x009482A0) itself expanded with both arguments NULL -- GetSeat's body is in
  the TU with NULL defaults, VC7.1 expands the constant-argument site and calls the scripted one; (2) the scratchpad out pointer is
  NOT initialised (the GetTC result is ignored): VC7.1 then zeroes its home at entry and reloads it on the miss path with the creature in
  esi (`add esi,44h`); `= 0` folds to `xor esi,esi` and moves the creature to eax. Bonus: the same-TU GetSeat body is byte-identical
  to retail 0x009482A0 (relocation-masked, all call targets checked) -- the parked GetSeat (pending/2026-10-30_parked_retry2_sec4,
  DIFFER 1072v1072) can land from src/00949ab0.cpp with leaf `?GetSeat@CAIStateGroup_GoToPub@@IAE_NPAVCThing@@PBVCCharString@@@Z`.
- **CTCLight::DrawLight 0x006414A0** (RELOCATION_MATCH, 3 grades; test PASS g++ / VC7.1; vland RELOCATION_MATCH PASS; jump table
  checked case by case). Grows Lights on demand (resize + AddPersistentPrimitive of a type-3 CPrimitive), takes GetColour() as a
  CRGBFloatColour, blends OldAccumilative / Accumilative flicker by draw_info.Interpolate, attenuates by the def's daylight curve (four
  fade times / 24, 3-segment search with a midnight wrap; segment 0 lerp, 1 full, 2 1 - lerp, none 0) or the sunlight attenuator
  (facing vs the flattened negated sun vector, both GFFastOneOverSqrt-normalised; (dot - fade start) / (fade end - fade start) clamped
  to [0, 1]; colour = environment diffuse colour), inverts, scales colour and radii, picks the lighting-channel mask (switch on the
  def's EffectChannel) and sends a CEnginePrimitiveLight through SetPrimitiveGraphics, then counts the light (0x013BA85C).
  Shape lessons: segment result as an if / else-if chain (a switch gives `sub ecx,0 ; dec ecx`); `C3DVector sun(-v.X, -v.Y, 0.0f)`
  from a const reference and `sun.Dot(facing)` (the literal zero Z stays an expression temporary the Z product consumes, `fxch ; fmul`);
  GFFastOneOverSqrt NEWTON_PAREN (its no-code PAREN tuple moves the scheduling-region cut so `mov ecx,esi` lands between `fld` and
  `fmul st(1)`, 0x00641842).

## Parked for Fable 5.1 (1)
- 006d1250 CTCWife::CheckAppearanceChanges DIFFER(1629v1608): frame-slot order (this home [esp+14h] v [esp+1Ch]; list heads, covered_areas, end, modifiers, find / insert temporaries shifted from 0x006D1269 on), loop-1 `mov ebp,[ebx+40h]` before the covered_areas load (0x006D132E), post-block this reload one slot late (0x006D15ED), and the tattoo loop 0x006D16B0..0x006D1769 (tattoo ebp v ebx, this from memory v ebp, def_index reloaded from its home after ~CCharString v kept in edi, end() loaded before find because the tattoo-site find is written end()-first) | tried (pass 1, 3 grades + ~12 c2 traces + ~14 feature compiles): the prior best (0.7534 exact), end()-first find at each site separately (only the tattoo site flips this/hero: with the named-iterator form the hero piece prio -37 is coloured before the this piece prio -67 and takes ebp), reversed / negated / const-iterator / pre-declared iterators at the tattoo site, named / const / assigned-in-condition def_index (the latter gives `setg bl`), seven loop-2 spellings (only `long def_index = *it;` for the GetDef flips the &set / &*it tie: blocks trace #79 / #81 both prio 30, key decided) -> best v6 0.7953 exact / 0.9001 regs+frame masked (prior 0.7534) | next: a find-first tattoo-site spelling that keeps the post-block this in ebp -- the lever is the def_index candidate (retail keeps the GetDefGlobalIndexFromName result in edi across the name temporary's dtor and homes it in the tattoo CDefPointer slot [2Ch]): try a block-scoped named CCharString for the instantiation name, def_index declared before the GetSubDef, or the index read through a helper; then the frame order with the packer probe (tools/pk/pk.py: this weight 11 > hair list 10 > clothing list 8 in v6-like builds; retail needs both list heads before this -- this's spill loads drop once it stays in ebp after the block). Passes 2 and 3 were not used (working budget).

## Skipped
None.

## Names
- Proven (two sources each; LABELS.tsv): the three rows; GoToPub GetObjectWithScriptName / IsStaffed / GetSeat / FreeSeat / GetBar (Ego_r
  + FableWin name sets, IAE, at the Ego_r donor order); CTCVillage::IsTavernTime / GetNearestBuildingWithProperty; CTCTavernTable::GetSeat /
  GetTankard; CIntelligentPointer<CThingBuilding> ctor / operator=; IS_THING_ALIVE<CIntelligentPointer<CThingBuilding> >; CTCAIScratchpad
  GetTableName / GetSeatName; CTCLight GetColour / GetInnerRadius / GetOuterRadius / IsInverted; CEnvironment PeekSunlightAttenuatorAngleFade* /
  PeekDiffuseColour / PeekSunVector; CGameTimeManager::GetTime; CTCPhysicsBase::GetFacingVector (slot 70); ~CPrimitive.
- Descriptive: `g_LightsDrawnThisFrame` (0x013BA85C; no data-symbol name in either PDB name set, Fable.exe strings or the landed rows; the
  display engine zeroes it each frame at 0x004355BB); GetSeat's parameter names (table, seat_name) and its NULL default arguments
  (inferred from the expansion); the building-property value 0x20 is passed as `(ESimBuildingProperty)0x20` (enumerator names in no PDB
  name set); local variable names.
- Label corrections (NAME_FIXES.tsv): 0x00947EB0 bound as FindMarkerByName; 0x009486D0 bound public (QAE); 0x00658E38 bound under two
  older spellings; 0x0065F8CA bound as a non-template / struct-keyed stand-in; 0x004C4160 catalog label SPrimitiveHandleHolder.
- Prior notes corrected: the GoToPub outline's "TC 0x78" is TC 0xB8 (TCI_TAVERN_TABLE, bit 24 of +0x34) and "0x0065F8CA" is the
  IS_THING_ALIVE COMDAT; FreeSeat runs on both outcomes of the pub branch.

## Header debt
- engine/CEnvironment.h keeps GameTickState opaque; PeekDiffuseColour reads +0x58 through it (a typed CEnvironmentGameTickState with
  STLport sizes would replace the cast).
- engine/NGraphicAppearance_CTCGraphicAppearance.h: the LIGHTINGCHANNEL / MOVEMENT layout condition was wrong (fixed in the overlay).
- No ESimBuildingProperty enumerators anywhere in the repo.
