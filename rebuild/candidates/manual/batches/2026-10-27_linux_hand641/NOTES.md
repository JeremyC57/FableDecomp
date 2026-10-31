# Hand chunk 641 (NTCScriptedControl::CActionPlayAnimation .. CSymbolMap): 3 of 10 byte-exact

All 10 rows were parked same-length retries (see CHUNK_BRIEF.md). Every best attempt was re-graded first (all reproduced
their recorded DIFFER), then one lever per grade. Probes and logs are in probe/ (QTAG a641*).

## What matched (3)
- **CGameScriptInterface::GetAllThingsInLevel 0x008A8AF0** (RELOCATION_MATCH): resolve the level's map number, force the map
  loaded, collect the things in that map and push back the script thing of every living, non-null one; returns size().
  Lever (agent638's 0x008A8890 lesson, applied for the first time to this row): the same-TU stand-in for
  `CThingSearchTools::GetAllThings<CThing, vector<CThing*>, CThingFilter_InMap>` takes `&filter`, and the filter is a
  temporary in the call. The address alone grew the frame by 8 (a named filter cannot share the script-thing slot); the
  temporary gives retail's frame and the map-number store after the found-vector push. Then respelled to the real model
  (agent638's explicit STLport `std::vector` specialisations, engine/CScriptThing.h, engine/CGameScriptInterface.h,
  CThingFilter_InMap(const CWorldMap&, long), CWorld::GetWorldMap returning CWorldMap&, CThing::GetScriptThing const,
  CThing::IsAlive const, CScriptThing::IsNull (slot 77) expanded on the local) -- still RELOCATION_MATCH.
- **CThingManager::DebugVerifyUniqueIDs 0x0051E630** (RELOCATION_MATCH): renumber managed things that clash with a unique ID
  from the input vector; the first clash builds both def names. Lever: the PDB says `CThing::GetDefName() const` returns a
  **CDefString** (not a CCharString) and `CDefStringTable::GetString(long) const`; with `CDefString::GetString()` inline
  (named result -> the caller's copy construction retail shows) the row was back at the old residue, and a **user copy
  constructor on CDefString** fixed the scratch-register rotation (a named `long tablePos` local in GetString also matches;
  the copy ctor was kept). Respelled: private `DebugVerifyUniqueIDs(const std::vector<CThing*>&)` (PDB AAE), GDefStringTable,
  slot 10 GetInstantiationName, the all-things list as an STLport 4-byte list.
- **NScript::CQ_HobbeCaveScript::CBanditInRoomSample::Main 0x00D910B0** (RELOCATION_MATCH): bandit sample in the Hobbe cave.
  Two levers: (1) the timer is a **CTimer local** (engine/CTimer.h with the ctor/dtor/GetTime/operator= bodies inline after
  the include, as 53 landed rows do): its destructor at each exit makes the dtor copies VC7.1 hoists from and cross-jumps
  into (`mov ecx,[g_pGameScriptInterface] ; jne exit+6`) -- explicit Register/Deregister calls never hoist; (2) the
  generated-script shape `if (!ParentClass->KilledSideRoomHobbes) { ... } else { if (IsActiveThreadTerminating()) return;
  Game->RemoveThing(Me, false, true); return; }` -- this sets the edx/eax choice of the exit copies (the
  `if (!terminating) RemoveThing; return;` spelling swaps them). The best attempt's "RemoveSamples" / "BIR_SpeechDelay"
  are the PDB members KilledSideRoomHobbes (retail +0x4E = Ego_r +0x62) and HC_BanditCallsForHelpDelay (retail +0xCB0 =
  Ego_r +0xD60), and IsDone is CEntityScriptBase::IsActiveThreadTerminating.

## Parked for Fable 5.1 (7)
- 00712900 NTCScriptedControl::CActionPlayAnimation::Update DIFFER(362v362): played-path creature read `mov eax,[esi+4] ; mov ecx,[eax+4]` vs retail `mov ecx,[esi+4] ; mov ecx,[ecx+4]` | tried: FableWin accessors CActionBase::GetParentCreature/GetParentThing + CTCScriptedControl::GetParentCreature (PDB respell), PTCScript->GetParentCreature() in the played test, played arm as the then-arm (gives ecx/ecx but lays the block inline), early return out of the not-played arm | next: lane gap: register colouring (the then-arm gets ecx/ecx, the else-arm eax/ecx; retail has the else-arm layout with the then-arm registers)
- 00956e70 CAIStateGroup_BriarRose::Update DIFFER(363v363): sub-def held in ebx (pushed in the prologue) vs retail edi reuse, ebx pushed mid-function | tried: the landed sibling 0x00956FE0 single reused `def` local + hand release, GetCreature() accessor for AddTC, GetCreature() for the action | next: lane gap: ebx/edi register colouring tie (third time for this pattern)
- 006d4e60 CTCFollowed::IsCreatureValidFollower DIFFER(363v363): key-address temp eax/ecx/edx at the three lookups (ours ecx/eax/ecx; retail forms &map before &key) and the RenownLevel push via eax (ours edx) | tried: PDB signature (const CThingAICreature&, IsAIFollower(const CThingAICreature*)), PeekTC(ETCInterfaceType, const T**) with the landed 0x008A0FD0 body (worse: ebx/ebp swap), map-before-key order, LowerBound result before the map local | next: lane gap: register colouring (same family as 0x00773950)
- 0047ed76 CTCInGameMenu::SetToSave DIFFER(366v366): `push ecx` (SetPosition float slot) before `fsubp st(1)` in ours, after it in retail | tried: GetUIScale const (PDB), position temporary in the call (moves frame slots), named y local with and without the C2DVector copy ctor (frame shrinks, swap stays) | next: lane gap: /Os scheduling (GOTCHAS open family 0x0047E651 / 0x0047F30F / 0x0047D296)
- 00a22ca0 CRightHandedSet::GetAngleXZ DIFFER(368v368): cross product Z term `fld [ecx+4] ; fmul [ecx+0Ch]` vs retail `fld [ecx+0Ch] ; fmul [ecx+4]` | tried: natural X*v.Y factor order (same), named `angle` local (fixed a second swap in the tail: `fmul 1/2pi` before `fmul sign`), sign*(...) | next: lane gap: x87 canonical operand order (agent634/635/640)
- 00a1ad40 CNavigatorAStar::GetPosInDirection DIFFER(370v370): height-test add `last + offset`: retail Y `fld last.Y ; fadd off.Y` and Z `fld off.Z ; fadd last.Z`, ours the reverse | tried: duplicated `return C3DVector(...)` in the distance exit (fixed the prologue ebx push, frame slots, the PInterface hoist and the cold `fstp st(0) ; jmp` exit), C3DVector::operator+=(const C2DVector&) from the PDB (fixed the step adds), swapped operands, const result (PDB ?B), named copy, result-local return (flips X/Z instead), const global | next: lane gap: x87 canonical operand order
- 009e5c90 CSymbolMap::LoadFromFile DIFFER(373v373): terminator store `mov byte ptr [esi+eax],0` vs retail `[eax+esi]` | tried: inline terminator helpers taking the buffer (the 0x009BB0D0 lever), size+buffer, char& accessor, ClearBuffer / AllocBuffer inlines, const long& / char& / const unsigned long& bindings | next: lane gap: SIB base/index order (GOTCHAS: 14 forms in 2026-10-27 family pass 5)

## Skipped
- none (no row holds two functions).

## Names (evidence) and label corrections
- 0x008A8AF0: catalog name = Ego_r.pdb. Slot 10 of CWorldMap's primary vtable (+0x28, called with the map number) is named
  **ForceLoad(long)** from the Ego_r CWorldMap (long) virtuals -- UNPROVEN (the static vtable slot 10 entry 0x004FB280 is an
  ICF thunk `jmp 0x0049D870`, which takes no stack argument, so the dynamic class is not CWorldMap itself). Virtual call:
  no relocation carries the name. Header debt: engine/CWorld.h and engine/CWorldMap.h flatten the vptr (local models).
- 0x0051E630: the call at 0x004C7BC0 (`mov ecx,[0x013B8A1C] ; jmp 0x0051E2E0`, result unused) is named
  **CThing::CheckUID() const** (Ego_r ?CheckUID@CThing@@IBEXXZ, protected void) -- PROPOSED from the Ego_r CThing name set
  and the debug-check context; one source only, review at landing (name_review). CThingManager's list at +0x18 is in the
  PDB's unnamed base part (descriptive member name AllThings).
- 0x00D910B0: CQ_HobbeCaveScript::KilledSideRoomHobbes (retail +0x4E; Ego_r +0x62 with the 0x14 quest-base shift of the
  landed CDrinkingHobbe row) and CScriptDef::HC_BanditCallsForHelpDelay (retail +0xCB0; Ego_r +0xD60 with the 0xB0 shift of
  agent640's PrisonExperienceForKillingKraken) replace the best attempt's invented RemoveSamples / BIR_SpeechDelay.
- Catalog labels: 0x00712900 "vfunc_1" = ?Update@CActionPlayAnimation@NTCScriptedControl@@UAEXXZ (slot 1, as the best
  attempt said); 0x00A22CA0 "VectorAngle_ComputeRotation" = ?GetAngleXZ@CRightHandedSet@@QBEMXZ; 0x00A1AD40
  "CNavigatorAStar_GetRandomNavigablePos" = ?GetPosInDirection@CNavigatorAStar@@ABE?AVC3DVector@@ABV2@ABVC2DVector@@M@Z;
  0x0049D870's manifest label GFGetPreMainMemoryUsed is the ICF-folded ?GetFrame@CWorld@@SIJXZ binding of the landed sibling.
- Respell list for callers: NAME_FIXES.tsv (15 addresses).
