# Hand chunk 2009 (CAIStateGroup_JackDragon .. CTCWillLightning): 2 of 4 byte-exact

Both matched rows grade RELOCATION_MATCH under VC7.1 RTM from the exact files in src/ (gall.sh with the overlay), their behaviour
tests print `<addr>_TEST PASS` under g++ (tall.sh) and VC7.1 (tall.py), and the read-only verifier (vland.py, overlay on) reports
2 WINS. Every call target / global was checked against retail (tmp/rel.py: each relocation of the built leaf against the retail
target at the same offset). The other two rows were NOT attempted (no grades): the session budget ran out after the two matches.

## What matched (2)
- **0x00745120 CTCPerceiveThing::UpdateLineOfSight** (1538 bytes; 5 grades in pass 1). Clears the three line-of-sight flags when the
  target is beyond the extended sight radius (radii scaled by the target's CTCStealth visibility), is an unviewable stealthy thing,
  or the world-map line between the two eye points (+1.5) is blocked, or a physics mesh of a thing found by
  GetAllThingsInRoughArea<const CThing>(box, Bind3(HasPhysicsMesh, RadiusIntersectsInfiniteLine, IsNot(parent))) intersects that
  line; otherwise sets the plain line of sight from the view cone (CThingFilter_IsInFOV, skipped in melee) in sight range, or the
  extended one beyond it. Shape levers (each measured): same-TU GetFOV / line-of-sight setters defined out of line in the TU
  (VC7.1 expands the constant-argument calls and calls the two variable-argument ones, as retail); visible reserve and search bodies
  (end() of the result vector in ebx, 0 / 0x36 in ebp; 1470 -> 1554 bytes, ratio 0.73 -> 0.86); value ternaries for the box min /
  max and member-by-member box stores in retail's TLX, BRX, TLY, BRY order (-> 1538 bytes, ratio 0.93); the mesh intersection record
  at function scope (frame 0xD0 -> 0xC8) and the empty HasPhysicsMesh filter as a NAMED local (its frame node then has size 4 and
  sorts at [esp+78h] as retail; measured with the c2 frame-packer hook, tmp/c2/pk.py).
- **0x00952890 CAIStateGroup_JackDragon::UpdateGround** (1509 bytes; 6 grades in pass 1). The ground-phase step of the Jack-dragon
  boss (see the row's line 1). The first natural spelling over the new overlay classes was 4 bytes off: only the exit layout (which
  release + epilogue copy survives c2's cross-jumping). Closed by defining the same-TU IsStillRunnable (0x0094FDF0) in the TU, using
  it in the final check, and writing that check as `if (!IsStillRunnable()) { Terminate(); return; }`.

## Parked for Fable 5.1 (0)
None.

## Skipped (2) -- NOT ATTEMPTED, session budget (not fragments)
- 0x0097FEA0 CAIStateGroupBase::AIAction_FindAGuardAndYellForHelp: Ego_r.pdb / FableWin.pdb
  ?AIAction_FindAGuardAndYellForHelp@CAIStateGroupBase@@IAE?AW4EAIActionReturn@@VC3DVector@@K_N@Z (protected; donor 0x0084B160 between
  RunAwayFrom 0x0084AE40 and FleeToThingProximity 0x0084B800 = retail 0x0097FBE0 / 0x0097FEA0 / 0x009804A0). Extent verified
  (int3 before 0x0097FEA0, `ret 14h` at 0x0098049A, int3 after). Outline from the bytes: CDefPointer<const CMessageEventDef> from
  PeekDefinitionManager().GetDef(CCharString(literal 0x012995C0), &def) 0x007923C0 (return 3 when missing); the creature's PeekPos
  0x006A4D00 copied into a local; a same-TU GetPBestLeafThing 0x00982450 search for a guard (filter of three references); return 3
  when none; CIntelligentPointer<CThing> guard local (ctor 0x00A01B10 / SetPItem 0x00A01B90 / dtor 0x00A01C10, vtable 0x0123E7E0);
  `(long)(def->[+0x44] * 0.8f)` frames; a loop: village membership via GetTC(TCI 0x23) -> CTCVillageMember::GetPVillage 0x007EE540
  (village +0x7C flag exits with 2), AIAction_MoveToThingProximity 0x0097E380 (CIntelligentPointer<const CThing> by value, 2.0f, ...),
  at 2 / 4: a CTCTalk::PlayAISpeech 0x006E3B30 (TCI 0x0F) with CCharString literal 0x0126C23C, the creature's virtual +8, IsStopping,
  StopSpeaking 0x00864580; CTCEntityEvents (TCI 0x42) list walk comparing CDefString == literal 0x0124F590 (0x005B3440); a periodic
  CMessageEventManager::AddMessage 0x00687730 (GetWorld +0x60) every N frames; on a found event: RemoveEvent 0x008112D0, the village
  crime calls 0x0065CE56 / 0x0065C1FC / 0x006585EE, the player creature (CMainGameComponent 0x013B86A0 -> PeekMyPlayer 0x0049D9D0 ->
  GetPControlledCreature 0x00487DC0) PeekTC TCI 0x51 -> CTCHeroOpinionDeedLog::PostDeed 0x006C6990 (deed 0x4E / 0x4F by a virtual
  +0x14 of this), IsHeroCriminal 0x0065BFCE, ClearLastAttackedByCreature 0x006691C0; exit 2 resets the guard's TC 0x23 +0xA8 and calls
  CTCCreatureOpinionOfHero::ReportAllCrimes 0x00684210 (TCI 0x4F). Large: new headers needed for the filter, CMessageEventDef, the
  village / talk / deed-log calls.
- 0x007A0D90 CTCWillLightning::GetTargets (8 stack arguments, `ret 20h`, 8-aligned ebp frame). Extent verified (int3 before,
  `ret 20h` at 0x007A137A, int3 after). Uses the same-TU beam list helpers (0x007A3870 push_back of CCountedPointer<beam>, 0x007A4210 /
  0x007A3200 counted-pointer ctor / dtor, 0x007A0630 beam ctor (`new` 0x7C), 0x007A4260 sort of a vector<CThing*> by distance from a
  C3DVector by value, 0x007A3CA0 GetThingsIn... search with a three-reference filter), GFRandom on the world seed (0x0049D810 /
  0x00497890), list<CThing*> node pushes (malloc 0x00BFEA0E, 12-byte nodes) and CIntelligentPointer SetPItem on the beam members.

## Names
All row names and callees are PDB-proven (two sources each, LABELS.tsv). No descriptive names were introduced (local variable names
only). Wrong landed / bound spellings found: NAME_FIXES.tsv (CPlayerGui Add/RemovePerceivingThing take CThing&; the folded
CThingFilter_IsInFOV::operator() 0x006A7390; vector<const CThing*>::reserve 0x007463E0; the GetAllThingsInRoughArea instantiation
0x00746560 (landed self row is a stand-in class); PeekThingSearchTools 0x00686D90 / IsInMelee 0x00733190 / GetFOV 0x007445D0 catalog
labels; JackDragon WaitForAnimLoop(float) / SetHeadTurning private spellings; ~CCreatureAction_DragonRearUpFull 0x009531A0).
Verifier warnings for the coordinator: RELOC_CONFLICT GetExtendedSightRadius (the landed 0x00864620 binds it to 0x007448F0, which is
GetSightRadius -- already filed by chunk 2004), RELOC_NAME for the catalog labels of 0x00437900, 0x00733190, 0x00686D90, 0x009531A0.

## Header debt
All header work is in hdr/ (README.md): 3 new headers and opt-in blocks in 9 copied repo headers. CThingFilter_IsInFOV::operator()
0x006A7390 is an ICF fold of the const CThing& and const CThing* overloads: the row binds the & form (already bound by another caller).
The generated engine/CAIStateGroup_JackDragon.h / CJackDragonDef.h / action headers keep their flat default paths; the retail
classes are opt-ins.

## Label corrections
0x007445D0 catalog `_global::GetAirResistance` is CTCPerceiveThing::GetFOV; 0x00733190 `Forward_GetAlignment_via0x24` is
CTCCombat::IsInMelee; 0x00686D90 `CTCBase::GetThingSearchTools` is PeekThingSearchTools; 0x009531A0 `Dtor_CCombatActionBaseBase` is
~CCreatureAction_DragonRearUpFull; 0x00437900 / 0x004378F0 `Forward_*_via0x1b8` are CPlayerGui::Remove / AddPerceivingThing.

## Tools (tmp/, read-only copies or new)
md.py / rel.py (from agent2004), fa.py (/FAs listing for frame symbols), pq.sh (preflight + grade + summary), c2/pk.py (runs the
hand886 phook.py frame-packer hook through c2dbg.probe and prints the slot list with sizes and weights, ~10 s), mc.sh (mini TU).
