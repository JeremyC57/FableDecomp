# Chunk 843 notes

0 of 6 rows matched. 4 rows parked in differ/ (source + `.test.cpp`, both tests pass under g++ and VC7.1). 2 rows were not attempted:
the budget went to the four parks. All six names were proven from two sources. Every attempt is in STATUS.tsv. Skeletons for the two
rows not attempted are in skel/.

## 0x00B2FD60 CEngineCamera::SetupGamut (was the placeholder _global::CPlane_InitializeFromCorners) -- DIFFER(1802v1812), md 213
- Name: Ego_r.pdb and FableWin.pdb `?SetupGamut@CEngineCamera@@AAEXXZ` (private). Ego_r address order RefreshD3DTransforms / SetupGamut /
  ClipSphere is the same as retail 0x00B2FC50 / 0x00B2FD60 / 0x00B30480. The only caller is SetCameraState at 0x00B3110F. Listed in
  LABELS.tsv and NAME_FIXES.tsv.
- Shape: the screen origin and the four far-plane corners (one `C3DVector corners[4]`, each set with `.Initialise(x, y, FarZPlane)`)
  go through the screen-to-view and view-to-world transforms. LookVector = InverseViewTransform.TransformVector(0,0,1), then
  AccurateNormalise. Four cross products of corner differences give the plane normals, and the planes are stored as
  `FrustrumPlanes[i].Initialise(n, n.Dot(origin))`.
- Callees: C3DVector::AccurateNormalise 0x00A14440 (see NAME_FIXES), CPlane::Initialise(const C3DVector&, float) 0x00A42140.
- Residue: retail keeps corner 0 wholly in x87 registers and re-stores each corner's Z (`mov [esp+38h],edx` at 0x00B2FF32 and others).
  The top and right normal slots are swapped. LookVector is stored through esi before `lea ecx`.
- Header debt: CEngineCamera's own members and the two forceinline transform helpers live in the GAMUT opt-in (hdr/README.md).

## 0x00A86110 CIPhysicsFacesSegmentBase::CalculateDividingPlane -- DIFFER(1967v1967), md 26, size exact
- Name: given in the brief (PDB mangled, protected static __fastcall); the class has no engine header, so it is modelled in the row
  with only this static (it needs no other member).
- Shape: start from the first vertex of the first face. Track the min / max over every vertex of every face (pointer loop over a copied
  `points[3]`), centre = (min + max) * 0.5. Sum the face centroids relative to the centre (copy then `-=`, (b + a + b) * 0.333333f),
  scale by 1/count, add the centre back, and return CPlane(axis normal, dot(total, normal)).
- Descriptive name: the file-static axis table `DividingPlaneAxes[3]` (0x01398658) has no PDB name.
- Residue: the axes-table address uses retail `lea ecx,[eax*4+tbl]`. The Y/Z scale operand order differs (a named scale fixes it but
  costs md 22 / 1969 bytes). There is a loop-align pad and one `pop ebx` scheduled elsewhere.

## 0x00D22F30 NScript::CQ_BreakSiegeScript::CKG_Sheriff::Main -- DIFFER(3335v3399), md 200
- Name: slot 1 of the entity vtable 0x012CAF1C that CKG_Sheriff::Alloc 0x00D1DF20 stores (OnPersist 0x00D21010 is slot 4 of the same
  table); `?Main@CKG_Sheriff@CQ_BreakSiegeScript@NScript@@EAEXXZ` in Ego_r.pdb and FableWin.pdb. The Codex pass-13 ROW.json is
  UNSTARTED (no prior source).
- Model: the landed CDisplacedTrader::Main 0x00D1FD30 (same script) and CEndGameScythe::Main 0x00D2ED20 (cast map, RunCutsceneMacro,
  STB operator=). ParentClass fields: BanditDemandsKnown +0x4A, ChiefRefusesDemands +0x4B, BanditLeaderFree +0x4C, BanditSiegeStart +0x4D,
  EndCutsceneDone +0x54.
- Strings: "TEXT_QST_018_GATEGUARD_QUESTION_OPEN_GATES" (yes / no question), "TEXT_QST_018_SHERIFF_ON_HIT",
  CS_BREAKSIEGE_GATESOPEN / _GATESOPENNOMEN (cast HERO / SHERIFF / LEADER), "TEXT_QUEST_BREAK_SIEGE_OBJECTIVE_03",
  "TEXT_QST_018_SHERIFF_ESCAPE_1".
- Result: the source is RELOCATION_MATCH under the diagnostic /d2inlS15 (probe s3d) and md 200 at the default budget. Every implicit
  MovieBase dtor is called as the COMDAT 0x006E7B80 where retail expands it, so the exit pads stop tail-merging. This is the documented
  caller-size budget (brief, GOTCHAS chunks 805 / 822). It is parked, and the differ source carries no /d2 flag.
- Levers tried: do { } while (0) round the SAY block, the answer wait, both, and the cut-scene if / else (chunks 821 / 834); own `{}`
  scopes; MOVIEBASE_FORCEINLINE (extra vptr store, as chunk 812 found).

## 0x00DE9690 NScript::CQ_RansomVictimScript::CRansomHelper::Main -- DIFFER(3428v3412), md 128
- Name: slot 1 of the entity vtable 0x012DD1E4 that CRansomHelper::Alloc 0x00DE1D00 stores; Ego_r.pdb and FableWin.pdb. The Codex
  ROW.json is UNSTARTED.
- Model: the landed CVictimsBrotherHelper::Main 0x00DEB280 (same script; movie ctor / dtor COMDATs 0x006E7B60 / 0x006E7B80).
  Fields: RescuedVictim +0x4B, RansommerDied +0x4C, RansomHelperStarted +0x4D, RansomHelperSpoken +0x4E, RansomHelpersAttacked +0x50,
  ToldGuardToWaitOutside +0x51.
- Callees: PlayCutscene 0x00DE6730, OpenDoor (slot 425), SetThingPersistent (477), SetIsPushableByHero (844, CScriptThing by value),
  IsDistanceBetweenThingsUnder 0x00CBE2FF, IsDistanceFromThingToPositionUnder 0x00CBE4B7 (declared in the row: no shared header has
  it), Me.GetHomePos (slot 7).
- Descriptive local names: HitTimer, GoingHome.
- Residue (documented blocker, the CIsThingAlive copy-inline case, GOTCHAS chunks 810 / 816): retail calls the CScriptThing copy
  0x004ABE90 inside the expanded CIsThingAlive ctor, expands the first by-value SetIsPushableByHero(Me) copy, and calls the second.
  - With IMPLICIT_COPY (the parked source), only the predicate copy is wrong.
  - Without it (r3, md 160), both top-level copies are called.
  - /d2inlS15 changes neither, so this is not the budget. Something tried that is not in GOTCHAS: the /d2inlS15 meter on both copy
    models.

## Not attempted (no compiled source)
- 0x005253F0 `?LoadGameState@CThingManager@@QAEXAAVCPersistContext@@@Z`: Ego_r.pdb + FableWin.pdb; the sole caller is
  CWorld::LoadGameStateInternal 0x004A21F0 (call at 0x004A2AAF). Skeleton: skel/005253f0.txt.
- 0x00DD4AD0 `?Main@CPrisonerAccomplice@CQ_PrisonEscapeRescueMotherScript@NScript@@EAEXXZ`: slot 1 of the vtable 0x012DBEF0 that
  CPrisonerAccomplice::Alloc 0x00DD49A0 stores; Ego_r.pdb + FableWin.pdb. Codex ROW.json UNSTARTED.
  - Notes for the next agent: the CTimer ctor and dtor are expanded inline (RegisterTimer / DeregisterTimer over pDSTGame at each exit,
    the "per-exit ~CTimer split" blocker class). It calls ParentClass.MakePrisonerComment 0x00DD5860, reads PMasterData +0xE8 (long) /
    +0xEC / +0xEE, and uses movie COMDATs 0x006E7B60 / 0x006E7B80.
  - Skeleton: skel/00dd4ad0.txt; compact listing tools bin/ann.py + bin/cmp.py.

## Labels, names, bindings
- LABELS.tsv: 0x00B2FD60 -> CEngineCamera::SetupGamut.
- NAME_FIXES.tsv:
  - 0x00A14440 is ?AccurateNormalise@C3DVector@@QAEAAV1@XZ (landed rows bind it as two different Normalise spellings).
  - 0x008AC970 is the STLport ~vector<CScriptThing> (PDB spelling already bound by 0x00E4B430; eight other spellings bind the same
    address).
- One symbol, one address: every callee the parked rows bind was checked against rebuild/oracles/reloc_bindings.tsv. Apart from the
  0x008AC970 duplicate spellings above, none is bound at a different address.

## Header debt
Only the opt-in overlay in hdr/ (see hdr/README.md). The two Mains use repo headers unchanged.
