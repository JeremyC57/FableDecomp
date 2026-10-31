# Hand chunk 638 (CTCBuyableHouse .. CTCShop): 2 of 10 byte-exact

All ten rows were parked same-length misses from the 2026-10-30/31 retry sweep. Each best attempt was regraded first and
reproduced its recorded residue; every change after that was one lever per grade. Probes are in `probe/`, grade logs in `tmp/`.
Grades were tagged with QTAG=a638_<addr>_<tag>.

## What matched (2)
- **CGameScriptInterface** 0x008A8890 `GetAllThingsWithDefName` (RELOCATION_MATCH 317v317). The function collects the script
  things of every thing built from the named def. It now uses the model of the landed sibling `GetAllThingsWithDefNameByDistanceFrom`
  0x008A9C40, which is the same body plus a sort. What matched: the same-TU stand-in for the search template takes the filter's
  ADDRESS. The filter is built in the dead defName slot, and it is now stored before `lea ecx,[esp+18h]` (&found), as in retail.
  The parked stand-in read `filter.DefIndex` by value, which let VC7.1 hoist the lea. The source is then respelled to the PDB,
  with no byte change:
  - explicit `std::vector<CScriptThing>` / `std::vector<CThing*>` specialisations, giving the real reserve / _M_insert_overflow symbols;
  - the const template member `GetAllThings<CThing, vector<CThing*>, CThingFilter_HasDefGlobalIndex>`;
  - `CThing::GetScriptThing() const` and `CDefinitionManager::GetDefGlobalIndexFromName const`;
  - engine/CWorld.h (`PThingSearchTools_ptr`) and engine/CGameScriptInterface.h (FABLE_RETAIL_NSCRIPT).
- **CWorldMap** 0x004FF920 `Update` (RELOCATION_MATCH 319v319). The per-frame world-map update. There are two levers:
  1. PDB respell. ForEachInRoughArea is a const template that returns bool and takes the callback as `const F&`. Update is
     virtual (UAE; the old leaf matched no symbol and graded the largest function). UpdateDestroyedThemeAreas is protected.
  2. The callback is the temporary `&CThingPhysical_SetAsMoving`, bound to the const reference in each call, and the search
     template has a noinline body in the same TU (retail instantiates it from the header). VC7.1 then knows the call only reads
     the callback word, and hoists the store into the loop preheader, as in retail.

  Without the body, a named local stores before the guard and a loop-body local or temporary stores every pass from ebx. The
  callback's name is single-source: the only `bool __fastcall(CThingPhysical&)` in the PDB name sets, in a ?A0x namespace.

## Parked for Fable 5.1 (8)
- 006c06d0 CTCBuyableHouse::GetRentMoneyPos DIFFER(312v312): retail `fld [esp+30h] ; fmul st(2)` for the normalised Y, ours `fld st(1) ; fmul [esp+30h]` | tried: PDB respell (private GetSignPostPos(bool, C3DVector&, CRightHandedSet*), C2DVector::Rotate), PDB-shaped Normalise on a named copy (336), named `y = Y*r` (same) plus the sweep's r*Y / operator* helper / member-wise result / const copy | next: lane gap: x87 canonical operand order
- 00a112f0 CMatrix3x4::TransformPlane DIFFER(316v316): frame 0x24 v 0x1c; retail keeps the normal copy, the rotated temporary and the renormalised normal in one slot (z copied through eax), x spilled to the dead plane slot | tried: implicit copy ctor (350), normal = Rotate(normal) + separate newNormal (340/374), normal = Rotate(normal).AccurateNormalise() (352/350), Rotate by value (312/314) | next: model CMatrix3x4 operator* / the real C3DVector operators from the PDB (Cross/GetScaled family) and retest slot sharing
- 007175d0 NTCScriptedControl::CActionSummonerLightningAttackTarget::Update DIFFER(318v318): retail block-2 creature in ebx (ebx pushed at the head), ours ebp with a shrink-wrapped `push ebx` | tried: PDB respell (IsModeActive const, CModeBase& GetCurrentMode const, IsPerformingAnyAction const), creature reference in block 1 / block 2, same-TU LowerBound stand-in (all same) | next: lane gap: register colouring tie
- 007729a0 CTCShop::CalcIsStallLike DIFFER(319v319): list node edi / &item esi in retail, swapped in ours | tried: item pointer declared before the node, same-TU GetPItem body with CIntelligentPointer<CThing> over CBaseObject* (both same) | next: lane gap: register colouring tie (same residue as 006061b0)
- 006d8830 CTCHeroExperience::AddSuccessfulMultistrikeHit DIFFER(321v321): `fst [esp+10h]` of |damage| scheduled after UpdateComboMultiplier's `push 1` (retail before) | tried: PDB access/const respell (GetWorld() CWorld& protected const, private helpers), strength-first product, <math.h> float fabs, same-TU UpdateComboMultiplier stand-in (all same) | next: lane gap: store/push scheduling
- 006061b0 CTCInventoryMap::FrameUpdate DIFFER(322v322): list node edi / card pointer esi in retail, swapped in ours (retail pushes edi before the empty-list test) | tried: node/item declaration order (same); CWorld::GetFrame and access respelled | next: lane gap: register colouring tie
- 00a1abf0 CNavigatorAStar::IsNewDestinationGoingToCauseRestart DIFFER(324v324): `mov eax,[esi+78h] ; cmp edi,eax` after the reach-radius `fadd` (retail before) | tried: agent633's GetNodeAt respell plus the interface slots GetNavigationLevelAt / PeekNavigatorRadius, private IsEqual / protected FindNearestNavigableNode (same); askedTo local (329); named bool (same); `range` local (same) | next: lane gap: x87/integer scheduling
- 00773950 CTCShop::IsOpeningHours DIFFER(324v324): retail `lea ecx,[esp+18h] ; lea esi,[eax+44h] ; push ecx`, ours `lea esi ; lea eax ; push eax` | tried: same-TU LowerBound stand-in, CInterfaceMap::Find inline member, GetPVillage member respell, const IsOpeningHours (PDB QBE), GetInterface out-pointer form, ternary GetTC (all same) | next: lane gap: register colouring tie

## Skipped
None.

## Label corrections
- 0x00A112F0 "Matrix_TransformPlane" = Ego_r.pdb `?TransformPlane@CMatrix3x4@@QBEXAAVCPlane@@@Z` (as the best attempt found); its
  callee 0x00A14440 is `C3DVector::AccurateNormalise` (landed), not Normalise.
- 0x006D8830 "Experience_ProcessMultiHitCombo" = FableWin.pdb `?AddSuccessfulMultistrikeHit@CTCHeroExperience@@QAEXABVCHitParameters@@@Z`.
- 0x007175D0 "vfunc_1" = `?Update@CActionSummonerLightningAttackTarget@NTCScriptedControl@@UAEXXZ` (vtable 0x0126586C slot 1; PDB name set).
- 0x006061B0 "vfunc_9" = `?FrameUpdate@CTCInventoryMap@@UAEXXZ`.
- 0x004FF920's interface slot 6 call in 00a1abf0 is `PeekNavigatorRadius(bool) const`; the only `float(bool) const` slot in the
  external-interface name set (single-source).

- 0x008ADBA0 catalog label `GetAllThings<CThing, vector<CThing*>, CFilter_Bind2<CThingFilter_InMap, unary_negate<CThingFilter_InArea>>>`
  is propagated: the body called from 0x008A8890 / 0x008A9C40 is the CThingFilter_HasDefGlobalIndex instance. vland flags this as
  WARN RELOC_NAME, the only warning left; the landing should relabel the address.

## Names: proven vs descriptive
- Proven (PDB name set + caller/RTTI): every callee in the two matched sources, except as listed below. The full list of
  respelled addresses is in NAME_FIXES.tsv.
- Descriptive / single-source:
  - `g_bShowPassability` 0x013B8A78. There is no PDB name. It is the established spelling; the console toggle at 0x004FAFE0
    flips it.
  - `CThingPhysical_SetAsMoving` 0x004FB260. Single-source PDB name in a per-TU ?A0x namespace.
  - The stand-in callees inside same-TU stand-in bodies: `FindThings_GetAllThingsWithDefGlobalIndex` (as the landed sibling) and
    `GetFirstThingInRoughArea_Stub`. These are compiled but never graded.

## Header debt
- engine/CWorldMap.h flattens the vptr. 0x004FF920 therefore uses a local CWorldMap with the slot names of the landed IsPosPassable
  family (slots 0-10). Update sits after slot 10 in the model, but is slot 27 in retail (vtable 0x01244AEC).
- The CMap slot names 1-12 are manifest labels with unproven parameter lists. None of them is called.
