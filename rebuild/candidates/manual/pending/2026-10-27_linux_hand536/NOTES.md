# Chunk 536 retry (18 rows parked DIFFER before) -- notes

Toolset: VC7.1 RTM 13.10.3077 (Wine lane). Five rows RELOCATION_MATCH. Thirteen stay parked in differ/: twelve from the
earlier retry, plus 0070d580, which this pass took from 512v504 to 504v504. vland dry run: 5 wins, no RELOC_CONFLICT,
RELOC_DUPLICATE or RELOC_IDENTITY. Its warnings are SHARED_HEADER_AVAILABLE (see "Header debt") and one RELOC_NAME,
where the catalog label for 0x007FC800 is wrong (below).

## Proven names (two sources each, exact PDB spelling, checked in the objects)
- 0x0054F6E0 `??1CText@NUISystem@@UAE@XZ`. FableWin.pdb, and the three CText vtables it stores:
  - 0x01249CCC `??_7CText@NUISystem@@6BCUIInterface@1@@`
  - 0x01249CA4 `...6BCObserver@1@@`
  - 0x01249C9C `...6BCActionParam@1@@`
- Callees of 0x0054F6E0:
  - 0x00530EC0 `?LoadAndReturnDef@CComponent@NUISystem@@UBE?AV?$CDefPointer@$$CBVCUIDef@NUISystem@@@@XZ`
  - 0x009E2B40 `?ReleaseFont@CFontManager@@QAEXABVCCharString@@@Z`
  - 0x0099A2E0 `??1CBaseClass@@UAE@XZ`
- 0x008C9A40 AttachEffectToStaff, an anonymous-namespace `__fastcall` free function (FableWin.pdb name set).
  - Callee 0x00669580 `??$GetSubDef@VCSpecialEffectsDef@@@CDefClassBase@@QBE_NPAV?$CDefPointer@$$CBVCSpecialEffectsDef@@@@@Z`.
  - Callee 0x0040F020 `CVectorMap<ETCInterfaceType, CTCBase*>::LowerBound`, spelled in NAME_FIXES.tsv. The STLport base vector is 12 bytes.
- 0x00924520 `?FinishAction@CCreatureAction_KrakenLowAttackLoop@?A0x...@@UAEXXZ`. The class is in an anonymous namespace (Ego_r name set, vtable slot 4).
  - Callee 0x007FC800 `?EndAttackBeam@CTCKraken@@QAEXXZ`. It is in the PDB name set and the bound spelling agrees. The catalog label `Dtor_LinkedList_KillAllThingsBase_007fc800` is wrong.
- 0x00B812F0 `?Render@CEngineClothSimulationStaticMesh@@QAEXABVC3DPrimitive2@@ABVCEngineSubPrimitiveStaticMesh@@W4ERenderPassType@@@Z`.
  - Callee 0x00AE1600 `CParticleProgram::PrepareForRendering` (PDB spelling; it was unbound before).
- 0x00B9DFC0 `?Update@CEngineInternalPrimitive2DStaticMesh@@UAEXABVCEnginePrimitive@@K@Z`.
  - Callees: 0x00A24280 `CMeshDataBank::PeekStaticMesh` and 0x00A24260 `PeekMeshInfo` (FableWin exact).
- Parked rows, real names (the oracle labels in brackets were wrong):
  - 0x0049ED80 `?ConsoleSetFreeCameraLookVector@CWorld@@KIXAAV1@ABV?$vector@...` [_global]
  - 0x00910DF0 `CAIStateGroup_MinionGuardHeardYellForHelp::Update`
  - 0x00929020 `?IsRunnable@CAIStateGroup_BanditPlayDice@@MAE_NXZ`
  - 0x00935C40 `?GoToWell@CAIStateGroup_MinionPutOutFires@@AAE_NXZ`
  - 0x0093AB70 `?GetPNearCampfire@CAIStateGroup_BanditSitRoundCampfire@@ABEPAVCThingObject@@XZ`
  - 0x00947110 `?FindPrettyThingInShop@CAIStateGroup_WanderAroundShops@@AAEPAVCThing@@PAV2@@Z` [CAIStateGroup_WanderAroundShops_FindRandomNPC]
  - 0x00B43720 `?PeekThemeId@CEngineMap@@QBEJJJJ@Z` [PeekLandscapeHeight]
  - 0x00B43870 `?PeekThemeBlend@CEngineMap@@QBEEJJJ@Z` [PeekLandscapeHeight]
  - 0x00B9E120 `?Initialise@CEngineInternalPrimitive2DStaticMesh@@QAEXXZ`
  - 0x0077C960 `?DoMovement@CTCHighlightItem@@IAEXXZ`
  - 0x00815420 `?GetBoundingSphere@CTCHitLocations@@QBEXAAVC3DVector@@AAM_N@Z`
  - 0x00C11D00 `??0CSoundEventManager@@QAE@PAUCAudioOwner@@MMMMMPAUCSteeringWorldEntry@@@Z` [CTCHero::Initialize]. Its vtable 0x012B2748 is `??_7CSoundEventManager@@6B@`.
  - 0x0070D580 `?SetMainAnim@CAnimateThing@@QAEXABVCAnimInit@@JJ@Z`
- Callees in 0x0070D580 (Ego_r and FableWin name sets, with the caller's argument types):
  - 0x0070D100 `?GetAllCombinationAnims@CAnimateThing@@QAEXAAV?$vector@PAVCAnimSet@@V?$allocator@PAVCAnimSet@@@std@@@std@@@Z`
  - 0x0070D1A0 `?Init@CAnimSet@@AAEXAAVCAnimateThing@@ABVCAnimInit@@M@Z`. Private, so CAnimateThing is a friend.
  - 0x00711A10 `?push_back@?$list@VCAnimSet@@V?$allocator@VCAnimSet@@@std@@@std@@QAEXABVCAnimSet@@@Z`. One argument, so this is the out-of-line push_back and not insert.
  - 0x0070B8A0 `?clear@?$_List_base@VCAnimSet@@V?$allocator@VCAnimSet@@@std@@@std@@QAEXXZ`. STLport 4.6 puts the out-of-line clear in _List_base.
- Inline CAnimSet members in 0x0070D580 (PDB name set). Retail inlines all of them, and they are what closes the row:
  - `SetAsFinished(long)` (QAEXJ@Z). The modified by-value parameter is the frame word retail stores before the Finished test.
  - `IsOwnedByMainAnim() const`
  - `IsValid() const`
- Other callees: 0x008FCF90 GetPlayerManager, 0x008FCFD0 GetPWorldSeed, 0x0097D6F0 / 0x0097E380 / 0x0097DB70, 0x00910CF0,
  0x00493B5B, 0x00A01B50, 0x00910B20 (UAE) / 0x00910C30, 0x008302F0 GetFOV, 0x00449970 GetMainPlayer, 0x007EE540 GetPVillage,
  0x0049D870 `?GetFrame@CWorld@@SIJXZ`, and 0x00936100 GetPBestLeafThing<...>. Their spellings and evidence are in NAME_FIXES.tsv.

## Probable names
- 0x004B8050 `std::_List_base<CIntelligentPointer<CThing> >::clear`. Callers 0x0093AB70 and landed 0x00618100 use it. The
  allocator template argument is not verified: the landed model uses one-parameter list templates, and the STLport 4.6
  spelling would carry `V?$allocator@...@std@@`.

## Descriptive names (nothing proves a real name)
- 0x0129BA3C, 1e-4f, declared `extern const float g_epsilon_0129ba3c` (an existing bound spelling).
  - In 0x0070D580 it must be a variable: `1.0f - eps` is computed at run time (`fld 1.0 ; fsub [0x129BA3C]`). A literal folds the subtraction. Landed 0x00ADEBC0 also copies it with integer moves.
  - Data symbols are not in the PDB name sets, and 49 different spellings are bound to it.
- 0x01436E98 `g_pEngineResourceManager_01436e98` and 0x01436E80 `g_pPrimitiveRenderer_01436e80`: the bound descriptive spellings.
- 0x00C11D00 support types: `CVtable_012b4030` and `CVtable_012b401c` (landed ctors 0x00C26E90 / 0x00C26A30), and `CAudioFilter00c25b10`.

## Parked rows (line 2 of each differ/ source)
- 0049ed80 DIFFER(310v310): x87 operand order in the cross product. All 64 operator^ orders were tried (lane gap).
- 0070d580 DIFFER(504v504): register roles only.
  - The vector end is loaded through eax and copied to ebp; retail loads ebp directly.
  - IsValid's `Anims[1]` reads through edx; retail uses ecx.
  - Earlier progress: the STLport 4.6 iterator model fixed the for_each argument construction; `if (init.Anim != 0)` fixed the shared tail; the inline SetAsFinished/IsValid fixed the loop and the size CSE; storing the flag before ChangeAlphaBy fixed the tail order.
  - The run took 8 multi-variant grades against a budget of 4, because every grade converged (534 -> 504).
- 0077c960 DIFFER(502v502): the bounding-box centre sum loads MaxCoord.Z first.
- 00815420 DIFFER(498v498): retail keeps begin() in ebx for the first element.
- 00910df0 DIFFER(500v500): the inlined GetPOriginator uses `xor eax,0xf ; test al,0xf` (no source form gives it), plus the brain load register.
- 00929020 DIFFER(509v507): `mov eax,[eax+4] ; mov ecx,eax` where retail has `mov ecx,[eax+4]` (2 bytes).
- 00935c40 DIFFER(499v502): frame slot order. A same-TU stand-in for GetPBestLeafThing gets IPA-folded.
- 0093ab70 DIFFER(511v504): register roles in the campfire loop, and frame 0x2c v 0x30.
- 00947110 DIFFER(356v354): the pushed item temporary gets its own slot, and HasTC tests with `shr/test cl` where retail uses `test dword`.
- 00b43720 / 00b43870 DIFFER(313v324): the mapY spill slot.
- 00b9e120 DIFFER(365v365): the registers of the WorldPos copy.
- 00c11d00 DIFFER(507v507): vptr-store scheduling (lane gap).

## Review at landing
- Anonymous-namespace hashes:
  - 0x00924520: the two ctors 0x009255E0 / 0x00925680 carry this TU's `?A0x...` hash. Their bound spellings lack the namespace.
  - 0x008C9A40: same issue for AttachEffectToStaff.
  - 0x0070D580 (parked): `CMultiplyAnimSetAlphaBy` and its for_each 0x007100A0 are `?A0x...`.
- Both matched rows rely on `_Use` devices to emit the anonymous-namespace body:
  - `KrakenLowAttackLoop_FinishAction_Use`
  - `AttachEffectToStaff_Use`
- 0x00B812F0: the 0x0143B934 fixup is `g_EngineInterpolationInfo` (0x0143B8E0) + 0x54 (the InterpolationSetList element field). It binds through the landed FrameStart spelling.
- 0x004B8050: the allocator argument of the clear spelling (see Probable).
- 0x0129BA3C: one descriptive spelling chosen out of 49.
- 0x00C11D00 (parked): the CVtable_* / CAudioDevice cast debt.

## Header debt
- 0x00B9DFC0: engine/CEngineInternalPrimitive2DStaticMesh.h flattens the vptr into a data member. Deriving from it with the virtual Update adds a second vptr, so `this` is a local polymorphic model with the header's offsets.
- 0x00B812F0: a local CEngineClothSimulationMeshBase / CEnginePrimitiveRenderer model.
  - Retail CArray and map members are 8 bytes shorter: ClothRenderer is at +0x2F0, where the PDB has +0x2F4.
  - vland warns SHARED_HEADER_AVAILABLE for C3DMeshInfo, CEngineInternalPrimitiveBase and CEnginePrimitiveRenderer.
- 0x0054F6E0: retail CText is the Ego_r.pdb layout minus 0x30 (landed ctor 0x0054F5C0). vland warns for CIVCountedPointeeBase and CObserver.
- 0x008C9A40 / 0x00924520: CThing is modelled locally.
  - Retail is the PDB layout minus 0x10 after Components, because of the 12-byte STLport CVectorMap.
  - vland warns for CDefinitionManager and CCreatureActionBase. CCreatureActionBase is a 0xA8 opaque polymorphic base.
- 0x0070D580 (parked): no shared header exists for CAnimateThing, CAnimSet or CAnimInit. The local model uses the retail sizes:
  - CAnimSet 0x54
  - CCompleteAnimation 0x60
  - 4-byte STLport lists
