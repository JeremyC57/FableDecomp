# Chunk 847 expert retry -- NOTES (agent847)

Fifteen retries of parked rows. 5 matched (all RELOCATION_MATCH: 0x005029A0, 0x0063E590, 0x00C71F80, 0x0075E270, 0x00760A70),
10 parked in differ/ with their tests (every one a 2-8 masked-line compiler residue: scheduling slot, register tie, x87 operand pair,
or the 0x00A852B0 source-order residue). Attempt counts and every spelling family are in STATUS.tsv; every grade line is in tmp/ALL.log
(deleted at hand-back) and the probes stay in probe/.

## Matched rows

### 0x005029A0 GetRelevantBuilding (free `__fastcall`, CWorldMap TU) -- RELOCATION_MATCH 337 B, 29 attempts
- Name proven: Ego_r.pdb / FableWin.pdb `?GetRelevantBuilding@@YIPAVCThingBuilding@@AAV?$map@...@std@@W4ESimBuildingProperty@@_N2@Z`
  (Ego_r 0x004D68D0); only caller CWorldMap::PopulateRegionWithVillagers 0x00502E90. Catalog "CBuilding_FindMostSpaceForNPCs" is stale (LABELS.tsv).
- Shape: `#define FABLE_RETAIL_STLP_LIST` before the shared stlport includes (list::size()); def = `it->first->PeekPSimDefinition()` in
  the loop body; the capacity test and the best test nest as ONE flattened condition
  `if (!(work || live) || it->second.size() < (work ? def->WorkSpaces : def->LiveSpaces)) { if (!best || buildings[it->first].size() < buildings[best].size()) best = it->first; }`
  -- the separate `if (work||live) {...; continue;}` block of hand719 and 27 other spellings colour the map into the frame (DIFFER 334v337).
- Callees: CThingBuilding::PeekPSimDefinition() const 0x0082D2D0 (`QBE?AV?$CDefPointer@$$CBVCSimBuildingDef@@@@XZ`, Ego_r; two other
  spellings of the same address are bound from 0x0065f056 / 0x00917a10 -> NAME_FIXES.tsv), map::operator[] 0x0051BF90 (already bound with the
  identical spelling from 0x00502b00). Layout: CSimBuildingDef Property/WorkSpaces/LiveSpaces at retail +0x3C/+0x40/+0x44 (Ego_r - 4).

### 0x0063E590 CWorldMap::GetNavigationLayerAt(const C3DVector&, const CThing*) const -- RELOCATION_MATCH 340 B, 62 attempts
- Name proven: Ego_r.pdb `?GetNavigationLayerAt@CWorldMap@@QBEJABVC3DVector@@PBVCThing@@@Z` (shared engine/CWorldMap.h declares it).
- Shape: shared headers (C3DVector, C2DBoxI + CTORS, CWorldMap, CWorld, CThingSearchTools + FOREACHINROUGHAREA opt-in, CMap + NAVIGATIONLAYER
  opt-in, CMainGameComponent + EDITOR opt-in). Each search branch reads `CThingSearchTools* tools = &World.GetThingSearchTools();` into a local
  and the Bind3 branch names `CThingFilter_IsNot isNot(ignore)` first: this is what puts the inlined CFilter_Bind2 F1 / Bind3 F3 pointer stores in
  retail's slots (the direct call form flips them, md4). CFilter_Bind2 comes from the shared engine/CFilter_Bind2.h (reference members, same bytes);
  CFilter_Bind3 has no shared header yet (local model, header debt). All models spelled `class` with const (QBE) methods so the bound symbols are exactly the PDB ones.
- Callees/relocations: `??$ForEachInRoughArea@VCThingPhysical@@V?$CFilter_Bind2@...@@VCThingFindNavigationLayerAt@@@CThingSearchTools@@QBE_N...@Z`
  0x0063F4B0 and the Bind3 twin 0x0063F7C0 (Ego_r), `?GetNavigationLayerAt@CMap@@QBEJABVC3DVector@@@Z` 0x0081E920, `?PMainGameComponent@
  CMainGameComponent@@1PAV1@A` 0x013B86A0 -> IsEditorActive slot 9, GetMapNumberAt slot 16 / IsMapLoaded slot 10 / PeekMap slot 19, `__ftol2`,
  `?g_bMapNavigationLayers@@3_NA` 0x0137571C (DESCRIPTIVE: the bool is unnamed in the PDBs; renamed only if a better source appears).
- Header edits: hdr/README.md (two opt-in declarations). Header debt: none beyond that.

### 0x00C71F80 CAudioSource::ComputePitchScale(const SPitchProperties*, __int64) -- RELOCATION_MATCH 311 B, 43 attempts
- Name DESCRIPTIVE (libpk / GameCODA, no PDB), class and record from the landed siblings SetPitchProperties 0x00C71F10 / ApplySound 0x00C72E00.
- Shape: `pow(2.0f, wave * depth)` through <math.h>'s C++ `float pow(float,float)` overload (-> _CIpow with the double 2.0 pool constant),
  __alldiv / __allrem for the 64-bit phase. The caller 0x00C72E00 binds the record parameter as `const float*` (PBM_J); this row and the sibling
  SetPitchProperties use the struct (PBUSPitchProperties@@_J@Z): NAME_FIXES.tsv.

### 0x0075E270 CTCCameraPointScriptedSpline::OnInitialActivate() -- RELOCATION_MATCH 172 B, 63 attempts
- Name proven: Ego_r.pdb `?OnInitialActivate@CTCCameraPointScriptedSpline@@UAEXXZ` (slot of the landed class).
- Shape: the inlined InitialiseSpline(0, -1) writes the end node through a named reference `long& end = NodeToEndOn;` (keeps retail's dead -1 store);
  the key position sum reads the thing position through a raw float view `const float* pp = &ParentThing->GetPos().X; key->Pos = C3DVector(key->Pos.X + pp[0], ...)`
  (operator+ flips the X pair). Callees: CTCBase::OnInitialActivate 0x00686870, CThing::GetPos 0x004C73D0 (`?GetPos@CThing@@QBEABVC3DVector@@XZ`),
  UpdateSplines 0x0075DDF0 (private). Models: shared engine/C3DVector.h, engine/CThing.h (default path, GetPos) and engine/CTCBase.h
  (FABLE_RETAIL_CTCBASE, OnInitialActivate slot 6); only CKeyCamera and the CTCCameraPointScriptedSpline members stay local.

### 0x00760A70 NShape::CSubdivisionTriangle::GetUpDirection() -- RELOCATION_MATCH 332 B, 98 attempts
- Name proven: Ego_r.pdb `?GetUpDirection@CSubdivisionTriangle@NShape@@AAEXXZ` (private; the model declares it `private:`).
- Models: shared engine/C3DVector.h (FABLE_RETAIL_C3DVECTOR; its copy ctor and operators give the same bytes as the former local struct).
- Shape: dot product as a sequenced running sum Z, Y, X with each product wrapped `(float)(double)(...)` (identity on float products; the extra
  fold moves `pop esi` one slot -- noted on line 2). Header debt: no shared NShape headers (local CShape model: point vector +0x08, UpDirection +0x20).

## Parked rows (differ/<addr>.cpp line 2 = residue, line 3 = next idea; tests in differ/<addr>.test.cpp, all PASS g++)

- 0x00C75F80 CCombFilter::Allocate (DESCRIPTIVE module/name, GameCODA; LABELS.tsv: catalog label is a stale CQ_GuildTrainingSkillScript autoname)
  -- DIFFER(111v111) md4: the Clear() byte count is hoisted above the m_pos store/data load. 71 attempts (52 body spellings + 18 flag sets + final).
- 0x00A852B0 CVectorMap<unsigned __int64, std::list<C2DLineF>, CKeyPairCompareLess<...> >::Insert (name proven from the Ego_r CVectorMap template
  and the C2DLineF list; LABELS.tsv: stale Vector_PushBack_ControlTreeNode_With_Sort) -- DIFFER(143v143) md4: source-order residue (sentinel store
  before the x loads). 53 attempts; blocker documented on line 2/3 (no STLport copy-ctor shape evaluates x first; synthetic proof in probe/00a852b0_syn_*).
- 0x0065B142 CTCVillage::EnableVillagerDefTypes (proven: Ego_r `?EnableVillagerDefTypes@CTCVillage@@QAEX_NABVCCharString@@@Z`) -- DIFFER(386v386) md4:
  ParentThing load slot. 54 attempts. Callees respelled to the PDB names: CThing::GetPos 0x004C73D0, CTCCreatureNavigation::GetNearbyWanderPosition
  0x006CB6E0 (`QBE_NABVC3DVector@@MAAV2@_N@Z`), CTCDummyVillager::SetEnabled 0x0072F0E0, CTCDummyVillager::CreateVillager 0x0072F750 (Ego_r takes
  `CTCVillage&`; the landed caller 0x0065B349 binds the pointer form -> NAME_FIXES.tsv candidate), IS_THING_ALIVE 0x0040DCE0 / 0x004A88F0,
  bitset::test 0x00410DE0, CVectorMap LowerBound 0x0040F020, CThing::Kill 0x004C9B80, CGameDefinitionManager::Get 0x0044C6B0,
  CDefinitionManager::GetDefGlobalIndexFromName 0x009AD410.
- 0x00A22E10 CRightHandedSet::MakePerpendicular (proven, `?MakePerpendicular@CRightHandedSet@@QAEXXZ`) -- DIFFER(358v358) md2: second table-offset
  `mov esi,17Ch` one slot late. 73 attempts (Normalise x dot-form grids).
- 0x00B92300 CEngineSceneGridCell::CreateOcclusionGroup (proven) -- DIFFER(335v335) md4: prologue tie (`mov ebp,ecx` / push edi / this spill order). 77 attempts.
- 0x00F36A30 CHeteroTerrainFractal::GetFractalHeight (proven) -- DIFFER(361v361) md4: octave counter reload one slot after `fmul [edi]`. 60 attempts.
- 0x006AFE60 CTCPhysicsBase::RespondToCollision (proven) -- DIFFER(145v145) md2: velocity parameter load one slot late (alias-driven: /Oa fixes it,
  not allowed). 419 attempts (240-cell operand grid + 26 cast/order variants): the velocity slot and the Y pair exclude each other.
- 0x006A9690 CThingPlayerCreature::ResolveRelativeMovementAcceleration (proven) -- DIFFER(450v450) md2: RotateAboutZ `lea` before the angle push. 85 attempts.
- 0x00986ED0 CThingFilter_IsAICreatureMatchingTypeAndFactionNearby ctor (proven) -- DIFFER(353v353) md8: two up.X fmul pairs. 1071 attempts
  (64 textual orders are byte-identical; 729-cell member/alias/pointer grid floors at md8).
- 0x0085C7D0 NCameraModes::CMoveOnCylinder::OnInit (proven) -- DIFFER(332v332) md8: vptr/result-slot register pair of the GetFacingVector call
  (pointer view + `(float)(double)` on the y term removed the other two residues). 220 attempts.

## Labels to correct
LABELS.tsv: 0x00C75F80, 0x00A852B0, 0x005029A0 (stale autonames). 0x0081E920 is already CMap::GetNavigationLayerAt in the catalog (no entry).

## Binding checks (one symbol, one address)
- map::operator[] 0x0051BF90: same spelling as the existing binding (0x00502b00). PeekPSimDefinition 0x0082D2D0: three spellings bound, ours is Ego_r's
  (NAME_FIXES.tsv lists the two wrong ones). CreateVillager 0x0072F750 and GetNearbyWanderPosition 0x006CB6E0: the differ source uses the Ego_r
  spellings; other spellings exist from landed rows (NAME_FIXES.tsv). ForEachInRoughArea 0x0063F4B0 / 0x0063F7C0, CMap::GetNavigationLayerAt
  0x0081E920, g_bMapNavigationLayers 0x0137571C: first bindings from this chunk.

## Header debt
- No shared CFilter_Bind3 header (0x0063E590 models it locally next to the shared CFilter_Bind2); no shared NShape (CShape / CSubdivisionTriangle) headers; no shared CAudioSource / SPitchProperties header; CSimBuildingDef retail member
  offsets (Ego_r - 4) modelled locally in src/005029a0.cpp; CVectorMap / CKeyPairCompareLess have no shared header (local STLport model in differ/00a852b0.cpp).

## Not used
No `#pragma inline_depth`, no /d2 flags, no `__asm`/`naked`/`_emit`/`volatile`, no same-type reinterpret_cast. The owner-accepted identity levers used:
`(float)(double)` on float products (0x00760A70 line 2; 0x0085C7D0 differ), raw float views of C3DVector members (0x0075E270, 0x006AFE60, 0x00986ED0, 0x0085C7D0).
