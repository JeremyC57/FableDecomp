# Hand chunk 2017 (CAIStateGroup_JackDragon .. CAIStateGroup_MinionSleep): 2 of 3 byte-exact

Both matched rows grade RELOCATION_MATCH under VC7.1 RTM from the exact files in src/ (q.py with the overlay; gall.sh re-run at the
end), their behaviour tests print `<addr>_TEST PASS` under g++ (tall.sh) and VC7.1 (tall.py), and the read-only verifier (vland.py,
overlay on) reports 2 WINS (only RELOC_NAME warnings for demangled catalog labels of 0x008AAC50 / 0x008AB710). Every call target,
vtable and global was checked against retail (tmp/rel.py). Row extents verified (int3 before each start; 0x00951430 ends `ret` at
0x00951B14, 0x00974110 ends `ret 8` at 0x0097476C): no SPLITS.

## What matched (2)
- **0x00951430 CAIStateGroup_JackDragon::IntoGroundSwoop** (1765 bytes; 3 grades + 1 re-grade). Builds the swoop route (creature,
  the side's IntoGround1/2 points, IntoGround3, a heading-rotated offset behind the landed marker) for StartFlying; then the
  CCreatureAction_DragonBurninateInto run (flame from the marker to 20 ahead, exit 10 ahead + 8 up, then OutOfGround1); then a random
  side (GFRandom(2)) and the fly-out route (OutOfGround2/3/4 of that side) and ChooseHoverOrStrafeState; IsStillRunnable checks
  (expanded) terminate. Levers: `+ C3DVector(0.0f, 0.0f, 8.0f)` for the exit point (frame 0x1B0 -> 0x1A4 and the Z temp), and the
  landed DragonBurninateInto ctor defined in the TU (visible, noinline) for the flame points' scheduling. VC7.1 reproduces the
  13-expanded / 3-called push_back split by itself.
- **0x00974110 CAIStateGroup_JackOfBladesBase::Summon(long, bool)** (1631 bytes; 4 grades + 1 re-grade). See line 1. Levers: the body
  inside `if (GetDef(...)) { }`, FABLE_RETAIL_CDEFINITIONMANAGER_GETDEF_GLOBAL_BODY (release form + whole register colouring), and the
  one-expression `GFFastSqrt(X*X + Y*Y)` 2D distance (x87 order).

## Parked for Fable 5.1 (0)
None.

## Skipped (1) -- NOT ATTEMPTED (session working budget; not a fragment)
- 0x0092DA20 CAIStateGroup_MinionSleep::Update (1802 bytes, plain `ret`, frame 0x150; extent verified: int3 before, `ret` at
  0x0092E129). Ego_r.pdb ?Update@CAIStateGroup_MinionSleep@@UAEXXZ (donor 0x00950F30, last of the class); MinionSleep : GoToBed (vtable
  0x0128B140; Ego_r adds WasCarryingWeapon +0x60 to GoToBed's PBed +0x14 .. Rhset +0x48). Same TU as the landed
  CAIStateGroup_GoToBed::Update 0x0092D7C0 (model for headers). Decoded outline:
  `if (!FindFreeBed(true) && !FindFreeBanditBed()) { Terminate(); return; }` (0x0092D1F0 / 0x0092C8A0); a bool = IS_THING_ALIVE of the
  creature's CIntelligentPointer at +0x168 (kept in a frame byte); `CTCSleep* sleep; if (!GetCreature().GetTC(&sleep)) { Terminate();
  return; }` (TCI 0xD4); PlayEmergencyGetOutOfBed = false; BedSide = BedToRightOfCreature ? "RIGHT" : "LEFT" (CCharString::operator=
  0x0099EFE0); a CAnimFilterAnyGroup (vtable 0x0127261C) local; BedGetOutOfBedFastAnim = 0; if HasAnim("BED_GET_IN_" + BedSide, filter)
  (operator+(const char*, const CCharString&) 0x0099F690, HasAnim 0x00662C70): get_in = GetAnimFromName("BED_GET_IN_"+BedSide, 0)->first
  word, get_out = GetAnimFromName("BED_GET_OUT_"+BedSide, 0), get_out_fast = GetAnimFromName(("BED_GET_OUT_"+BedSide) + "_FAST", 0)
  (operator+(const CCharString&, const char*) 0x0099F600) falling back to get_out, sleep = GetAnimFromName("BED_SLEEP", 0),
  BedGetOutOfBedFastAnim = get_out_fast (else the anim locals stay uninitialised); the bed's CDefPointer-like counted def (PBed->GetTC
  TCI 0x35 (+0x24 bit 21) -> +0x14, `inc [+4]`); g_DefStringTable 0x013CA828 GetString 0x009D49B0 of its +0x28 name; a
  GetRandomAnimFromNameWithFilter 0x00662EF0 with a second CAnimFilterAnyGroup; GetFastTrack 0x0092C3B0 on the sleep TC; if set:
  WasCarryingWeapon = CTCCarrying::IsThingCarryingWeapon 0x006A1540, SheatheAllWeapons 0x006647F0 and CTCCreatureModeManager::RemoveMode(4)
  0x0073ABA0 (TCI 0x31), the physics TC's virtual +0x7C(&BedPos) and +0x80(RHSet built from Rhset with Up (0,0,1)), the navigation
  TC (TCI 7) reset 0x006CB6B0; WasCarryingWeapon again -> PerformActionAndWait(CCreatureAction_SheatheWeapons 0x008CC370); PlayEmergency
  = true; then PerformActionAndWait(CCreatureAction_GetInBed 0x0092E300 (same TU: creature, PBed, def string, anim ids...)); a sleep
  loop: frame stamps via PeekWorld 0x008FCFA0 / CWorld::GetFrame 0x0049D870 against CMainGameComponent::ConstantFPS 0x01375550 * 4,
  CThingAICreature::GetSleepTimes 0x00833390, the game-time range test 0x006BB390, IsHeroTrespassingPerceived 0x0092CB30, the +0x168
  alive re-test, AIAction_DoNothing 0x0097E200 == 4 (`nop` loop head 0x0092DE7F); two wake-up tails (CDefString != "" 0x005FA740 test of
  the def's +0x34 name): StartGetOutOfBedAnimation 0x00746A70 on TCI 0x35 when PBed is alive, PerformActionAndWait(CCreatureAction_GetOutOfBed
  0x00860060 (creature, PBed, name)) == 2 -> PlayEmergencyGetOutOfBed = false; finally if WasCarryingWeapon:
  PerformActionAndWait(CCreatureAction_UnsheatheWeapons 0x008B8590), WasCarryingWeapon = false; Terminate(); the counted def released
  (`dec [ecx+4]`). Header debt to start: CAIStateGroup_MinionSleep (retail class : CAIStateGroup_GoToBed), CTCSleep fields, the
  GetInBed / GetOutOfBed / Sheathe / Unsheathe action classes, CDefStringTable, CAnimFilterAnyGroup.

## Names
All row names and callees are PDB-proven (two sources each, LABELS.tsv); no descriptive names were introduced (parameter / local
names only: `family_def_index`, `force_push_close_enemy` are descriptive parameter names, which do not mangle). Wrong landed / bound
spellings: NAME_FIXES.tsv (StartFlying 0x00950C30, ChooseHoverOrStrafeState 0x0094FFF0, DragonBurninateInto ctor 0x009537B0,
RotateXY 0x00A14040 (bound as RotateAboutZ), GetDef<CCreatureGenerationFamilyDef> 0x0074D220, AIAction_PerformAction 0x0097D8B0, the
8.0f literal 0x0125916C bound as an invented global).

## Header debt
All header work is in hdr/ (README.md): 2 new headers and opt-in blocks in 8 copied headers (2 of them from the c2009 overlay).

## Label corrections
0x0125916C is the float literal 8.0f (`__real@41000000`), not a global; the landed 0x00977820's `g_MinTeleportMarkerDistance` is
invented. The rest are in NAME_FIXES.tsv.

## Tools (tmp/)
md.py / rel.py / fa.py / mc.sh / c2/pk.py (copies from agent2009), rd.py (read floats from Fable.exe), callees.py (row call targets with
bound names). Probe compiles (fragment probes and one c2 frame-packer trace) on top of the graded compiles: about 8.
