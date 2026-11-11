# Hand chunk 2026 (CAIStateGroup_MinionSleep .. CAIStateGroup_EarthTroll): 2 of 2 byte-exact

Both rows grade RELOCATION_MATCH under VC7.1 RTM from the exact files in src/ (gall.sh with the overlay, re-run after the last header
change), their behaviour tests print `<addr>_TEST PASS` under g++ (tall.sh) and VC7.1 (tall.py), and the read-only verifier (vland.py,
overlay on) reports 2 WINS (RELOC_NAME warnings only: the catalog labels of 0x006BB390, 0x0091F990, 0x00733210 -- see NAME_FIXES.tsv).
Every call target, vtable, global and literal was checked against retail (tmp/rel.py). Row extents verified (int3 before each start,
0x0092DA20 ends `ret` at 0x0092E129, 0x0094E7B0 ends `ret` at 0x0094EF6B, int3 padding after): no SPLITS.

## What matched (2)
- **0x0092DA20 CAIStateGroup_MinionSleep::Update** (1802 bytes; 2 grades + 2 re-grades). Line 1 of the source describes it. Started from
  chunk 2017's decoded outline (prior/NOTES.md), which was right. Levers: (1) the bed side as `if (..) BedSide = "RIGHT"; else BedSide =
  "LEFT";` (VC7.1 cross-jumps the two operator= calls into retail's `push ; jmp` / `push` pair; the ternary gives a register select);
  (2) `GetAnimFromName("BED_SLEEP", 0)` as an implicit conversion temporary (built before the creature load); (3) the two wake-up paths
  hang on a local `bool disturbed = true;` cleared at the sleep-time and home-building exits; VC7.1 threads the flag through the loop
  exits (those two jump straight to the normal get-out 0x0092E00C) and, under `disturbed && name != ""` (0.25), calls
  IS_THING_ALIVE<CIntelligentPointer<CThing> > 0x004CC340 at the fast site while expanding it at the normal one -- exactly retail;
  (4) CAIStateGroup_MinionSleep::IsInSleepTime (0x0092C520) defined in the TU before Update is expanded in the loop; (5) the bed def is
  `CDefPointer<const CBedDef> bed_def = bed->GetDefinition();` from a `const CTCBed* bed = 0; PBed->PeekTC(&bed);` (const LowerBound
  0x004365B0, the zeroed out pointer), its release has no null test.
- **0x0094E7B0 CAIStateGroup_EarthTroll::Update** (1980 bytes; 5 grades + 2 re-grades). Line 1 describes it. The catalog name is right
  (vtable slot 12 + Ego_r/FableWin protected `MAEXXZ`); no part of it is a neighbour expanded inline except the Ego_r helper
  CAIStateGroup_TrollBase::GroundPound's body (see below). Levers: (1) FABLE_RETAIL_CDEFCLASSBASE_GETSUBDEF_BODY (noinline GetSubDef<T>)
  for the `dec [ecx+4]` releases and the def/flag init order; (2) GetMagnitude2D's two-statement body (X squared first); (3) the
  ground pound written out as `AIAction_PerformAction(CCreatureAction_TrollGroundPound(GetCreature(), *PEnemy))` -- a GroundPound()
  call (Ego_r helper, no retail copy) is NOT expanded by VC7.1 here (c2 inline trace tmp/inl/t2.txt: c1 62, f 0.023, score 431 v 74);
  (4) the throwing range as `else if (distance <= TROLL_MAX_THROWING_DISTANCE) { ... } else { Terminate(); return; }`, which makes
  VC7.1 lay out the shared idle/boast tail right after the close-range arm and the throwing arm after it (retail's order); (5) the
  closing `if (IsStopping()) { Terminate(); return; }` (the return keeps a separate release copy and fixes every exit block's
  cross-jumping: 1973 -> 1980 bytes, RELOCATION_MATCH).

## Parked for Fable 5.1 (0)
None.

## Skipped (0)
None.

## Names
All row names and callees are PDB-proven (two sources each, LABELS.tsv); no descriptive function or class names were introduced.
Local variable names (`disturbed`, `home_alive`, `get_out_fast_anim`, `knocked_down`, `right`, ...) are descriptive and do not mangle.
ECombatAnimationHandedness is an MS forward enum (its enumerator names are in neither PDB name list; the row passes the value 0 cast).
Wrong landed / bound spellings: NAME_FIXES.tsv (GetSleepTimes 0x00833390, GetOutOfBed ctor 0x00860060, UnsheatheWeapons ctor 0x008B8590
landed definition, MinionSleep::IsInSleepTime 0x0092C520 (landed as the invented GoToBed::IsInSleepTimeWithWakeDelay), TrollGroundPound
ctor 0x0094E5F0, CIntelligentPointer<const CThing> converting ctor 0x0091F990, TrollBase ForceRiseUp / GetEnemyPredictedPos /
HasEnemyBeenKnockedDown / ThrowRocks landed definitions, CTCCombat::ResetComboChainPos 0x00733210 (landed as CWADFile::Close),
CGameTimeManager::IsTimeBetween 0x006BB390 (catalog `Game_IsTimeOfDayInRange`), IsPerformingAnyAction 0x006622A0 extra bindings).

## Unproven construct (say so at landing)
- 0x0094E7B0 writes the ground pound out instead of calling CAIStateGroup_TrollBase::GroundPound (Ego_r.pdb has the helper, donor
  0x009722C0; retail has no copy and both troll Updates contain its body). With the helper defined in the TU VC7.1 calls it (measured);
  either retail's source wrote the call out, or the helper was expanded by a means not reproduced here. The bytes are identical.

## Header debt
All header work is in hdr/ (README.md): 3 new headers (CTCBed, CBedDef, CCreatureAction_SheatheWeapons) and opt-in blocks in 17 copied
headers (CAIStateGroupBase.h is chunk 2017's copy). The repo's generated CAIStateGroup_MinionSleep / TrollBase / EarthTroll /
CCreatureAction_GetInBed / GetOutOfBed / UnsheatheWeapons headers are flat Ego_r layouts; the retail classes are opt-ins.

## Same-TU bonus
src/0092da20.cpp also defines CAIStateGroup_MinionSleep::IsInSleepTime (0x0092C520): it grades RELOCATION_MATCH against 0x0092C520 with
leaf ?IsInSleepTime@CAIStateGroup_MinionSleep@@QBE_NXZ (one probe), so the landed 0x0092C520 row can be respelled from this TU.

## Tools (tmp/)
md.py / rel.py / fa.py / c2/ (copies from agent2017), cc.py (compile only, no retail diff), ann.py (relocation-annotated disassembly of
a built leaf), lay.sh (compile + layout markers), calls.py (relocation counts), inl/ + inl.sh (c2 inline-decision trace, copied from
agent2003). Probe compiles on top of the graded ones: 12 compile-only + 2 c2 inline traces + 1 IsInSleepTime check.
