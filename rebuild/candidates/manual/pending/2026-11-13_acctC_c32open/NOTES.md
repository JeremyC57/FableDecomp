# agentC32 notes

## 0x006A1A00 CTCCarrying::GetCarriedThingInterpolatedPosAndOrientation -- DIFFER(864v864, 3 hunks), parked (breadth first), ~40 grades
Name: Ego_r 0x006C61A0 / FableWin 0x02216AF0 (C30). Extent 0x006A1A00..0x006A1D60 confirmed (ret 14h at 0x006A1D5D, UpdateCarriedObjects
0x006A1D60 follows). Draft differ/006a1a00.cpp (= tmp/a8.cpp); alt differ/006a1a00_b2.cpp (= tmp/b2.cpp, right call targets, 11 hunks).
Header work (overlay): new engine/CCarrySlotDef.h (generated one is in _quarantine); CVectorMap FABLE_RETAIL_CVECTORMAP_VALFIND_CONST
(FableWin 0x0221C0F0 ??$ValFind@PBVCThing@@ const, FORCEINLINE -- closed the find loop exactly); CTCCarrying
FABLE_RETAIL_CTCCARRYING_INTERPOLATEDPOS decl; CDefString FABLE_RETAIL_CDEFSTRING_FORCEINLINE_CONVERSION_INLINE_GETSTRING (tried, NOT used
by the best draft -- may be dropped).
Later (grades 40-55): /d2inlT shows both GetString sites flip TOGETHER (T<=17 both called, T>=19 both expanded), /d2inlS12 declines the
forwarder at site 2, S13 inlines forwarder+GetString there; `#pragma inline_depth(1)` does not stop GetString inside a FORCEINLINE forwarder.
So in our TU the two sites are always decided alike; in retail they differ (site 1 expanded + copy, site 2 forwarder expanded + GetString
called). Conclusion so far: site 1 is probably NOT the same inline path as site 2 in the original -- unresolved.
Closers that worked: GETDEF_GLOBAL_BODY (def release without null test after success); `CCharString name = def->DummyPosName;`
(conversion: GetString expanded + copy ctor 0x0099EC30, as retail); `flags = 0x10; if (!b) flags = 0x31;` (retail store/jne/store);
no `parent` local in the second block (retail re-reads [esi+4]); implicit conversion for the secondary name (temp built first);
named `C3DVector up(0,0,1)` (slot 0x18 as retail).
Residue (a8): (1) the up-vector stores: retail schedules them after `push ebx ; lea eax,[esp+38h] ; push eax` (0x006A1B73..0x006A1B83),
ours before the physics loads (temp form: slot 0x1C, before `mov edx,[ecx]`). Tried named-before-null-test, temp, ParentThing->,
decl order of the secondary locals. (2) CALL TARGET: retail second site = conversion inlined + GetString 0x00415DD0 called
(0x006A1C5A) with the temp built before the GetDummyObject pushes; default headers call ??B 0x00415D70 there (op declined);
op FORCEINLINE (new opt-in) expands GetString there too; explicit .GetString() (b2) calls 0x00415DD0 but in mid-push order.
/d2inlS12 declines the op, S13 inlines op+GetString together (bottom-up); /d2inlT no effect >= 60. Next idea: find why retail keeps
GetString un-inlined inside the inlined forwarder at site 2 only (first site may not be the forwarder: try other first-site spellings
with the FORCEINLINE_CONVERSION (noinline GetString) opt-in + a separate expanded form), or a row-local inline control per EXTRA_RULES
2026-11-08 if proven a pure budget split.
Global 0x0137574C: read only here, no writer, no data pointer to it; sits after NGlobalConsole bools 0x01375748..4A. Descriptive name
g_bTurnPrimarySlotCarriedThing (made up).

## 0x007FC9D0 CTCKraken::FrameUpdate -- DIFFER(696v704), parked (breadth first), ~8 grades
Name: Ego_r 0x007F7DB0 / FableWin 0x02777500 ?FrameUpdate@CTCKraken@@UAEXXZ (Ego_r order after OnInitialActivate, as retail 0x007FC8E0 ->
0x007FC9D0). Extent 0x007FC9D0..0x007FCC6A + jump tables 0x007FCC6C (Phase 0..3: 0->0x7FCB92, 1,2->0x7FCB4E, 3->0x7FCC22) and
0x007FCC7C (Mode-1: 1 UpdateTentaclesOnly, 2 AttackingTentacleIterator = Tentacles.end(), 3 DoHeadTracking, 4 beam delay, 5 DoHeadTracking +
beam delay). Draft differ/007fc9d0.cpp (= tmp/k4.cpp; shared CTCKraken.h, no header change).
Decoded: PeekComponent().IsEditorActive() (slot 9) -> return; KillBeamCounter countdown kills PBeamWeaponEffect (Kill(true) 0x004C9B80) and
nulls it; Tentacles.remove(0) (0x0065DE8A); SetPhase(PTCBoss->GetBossPhase() 0x007AED10) when different; GetCentreMarker when the marker is
dead; PTarget = GetPlayerManager().GetMainPlayer().GetPControlledCreature(); Phase==1 -> IsReadyForPhaseChange sets ReturnHeadToWater; else
Health(+0xB4) < 1.0f -> GetTC<CTCTargeted>->SetTargetable(false), CreateHeadEffect("ON_DIE"), SetPhase(3); tail SetPosition(centre + HeightOffset).
Closers: FABLE_RETAIL_CINTELLIGENTPOINTER_FORCEINLINE_CTOR + _ASSIGN_FORCEINLINE (retail expands both `= 0` temporaries and assignments).
Residue: (1) retail lowers the Phase switch to a 4-entry table with case 0 = default target; ours a compare tree (case 0 label folded into
default). case 3 must skip the death block without a Phase re-test (retail re-reads Phase only for `== 1`). Tried: case 0 + default shared,
`if (Phase != 3)` guard after the switch (k5, tree + re-test), duplicated tail in case 3 (k6, not cross-jumped). Idea: Ego_r has EMPTY
private UpdateUnderwater / UpdateTentacles / UpdateTentaclesAndHead (16-byte slots 0x007F6220/30/0x007F64F0) -- the original switch probably
called them (case 0 distinct at lowering time, inlined away later). (2) frame 0x18 v 0x14 (one 4-byte slot more; the temp at +0x10 v +0xC).

### 0x007FC9D0 follow-up (grades 9-16): best now differ/007fc9d0.cpp = tmp/k8.cpp, DIFFER(704v704, 22 hunks, all frame offsets)
CLOSER (switch table): the Ego_r EMPTY private helpers defined in the row's TU (`void CTCKraken::UpdateUnderwater() {}`,
UpdateTentacles, UpdateTentaclesAndHead -- Ego_r 0x007F6220 / 0x007F6230 / 0x007F64F0, FableWin 0x02777E30 / 40 / 0x02778220) and
`case 0: UpdateUnderwater();` falling into `default:` (k8; k7 also calls UpdateTentacles / UpdateTentaclesAndHead in cases 1 / 2, same
bytes): case 0 stays a distinct label at lowering time -> the retail 4-entry table, and the length matches (704). DECISION NEEDED: the
three empty definitions are emitted by the row source (retail has no out-of-line copy: verify COMDAT folding / whether landing an extra
definition is acceptable; else a header opt-in with inline empty bodies should do the same).
Residue: frame 0x18 v 0x14. Diagnostic p3 (death-block GetTC<CTCTargeted> removed) gives the retail frame: the address-taken
`CTCTargeted* targeted` out-pointer keeps its own slot at +0 (no store is emitted) where retail shares +0 between the GetTC key, the
CIntelligentPointer temporaries and the "ON_DIE" CCharString. Tried: targeted at function scope (k9), `= 0` (k10), test of the pointer
(k11, longer). Next idea: GetTC spelling without an address-taken local (HasTC + GetTC(TCI_TARGETED) reference form with
CVECTORMAP_FORCEINLINE_FIND, or PeekTC/GetTC variants of landed rows that show the retail `test ah,2 ; ... ; mov eax,[eax+4] ; mov byte
ptr [eax+10h],0` shape without a frame slot).
Kraken grades 17-19: k12 (HasTC + GetTC(TCI_TARGETED) reference form, CVECTORMAP_FORCEINLINE_FIND; no address-taken local) 20 hunks, k13
(GetTC(id, &p)) 22 -- frame still 0x18. Slot map (k8): ours "ON_DIE" CCharString temp at +0, CIntelligentPointer temps and the GetTC key at
+4, pos at +0xC; retail ALL of temp / key / string at +0 and pos at +8. So it is the packer refusing to overlap the string temp with the
IP temps / key once the GetTC block exists (p3 without the block packs like retail). Next: efm.py frame-slot map on both, try the string as
an explicit `CCharString("ON_DIE")` / named local, or the GetTC block after CreateHeadEffect (order check vs retail first: retail does the
TC store BEFORE building the string, so order is fixed).

## Untouched rows (context): what was established
- 0x0074AF50 CTCCreatureGenerator::CreateCreaturesUndead: window 1 decoded -- `if (CreatureFamilies.size() == 0 || HasHitCreationLimit()
  0x0074A190) return;` UndeadTriggered = true; random index = world seed (PeekWorld 0x00686A80 -> GetWorldSeed 0x0049D810: seed =
  seed*0x24A1+0x24DF, GFROR13 0x00497890) % size; CreatureFamilies.at(i) EXPANDED (range check puts("vector")+abort 0x00BFEAF2/0x00BFEAEC);
  GetDefinitionManager().GetDef<CCreatureGenerationFamilyDef> 0x0074D220; loop over def+0x3C..+0x40. The shared CTCCreatureGenerator.h
  has CreatureFamilies / CreatedCreatures as opaque bytes: needs a typed opt-in (CArray<CDefIndex> +0x0C, list +0x18).
- 0x0075D580 CTCCameraPointScriptedSpline::UpdateCameraParams: Ego_r 0x0076D6E0
  ?UpdateCameraParams@CTCCameraPointScriptedSpline@@QAEMMAAVC3DVector@@0AAM1AA_NPBV2@PBVCRightHandedSet@@@Z (8 args = ret 20h). Retail
  members (Ego_r order, STLport vector 12 bytes): PTimingSpline +0x60, KeyCameras +0x68, SelectedKeyCamera +0x74, PPositionSpline +0x78,
  PViewDirectionSpline +0x80, PScalarSpline +0x88, next CCountedPointer<CCardinalSpline> +0x90 (all null-tested first); callees
  CCardinalSpline::GetSplinePosition 0x00A567A0, GFLimit<float> 0x0048DAE0, AccurateNormalise 0x00A14440. No shared header exists.
- 0x007BC38A / 0x006B2CA0: FP-heavy (ebp frame / 205 FP ops); 0x007BC38A starts with a call 0x007BEDD0 into a local C3DVector, the
  PTarget (+0x28) IS_THING_ALIVE 0x0040DCE0 test, GetPos, a GFSquared-style `fld st(0); fmul st(1)` pair and the fast inverse sqrt table.
