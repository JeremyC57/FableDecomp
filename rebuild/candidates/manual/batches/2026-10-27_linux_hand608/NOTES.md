# Hand chunk 608 (CAIStateGroup_SitForMeal .. CTCCreatureGroup): 1 of 10 byte-exact

All ten rows came in as parked misses that were already the same length as retail. Each row was regraded from its best file,
had its names re-proven (the parked spellings were wrong in 9 of 10), and was then graded with one change at a time.

## What matched (1)
- NExplosiveTrailHelpers::RandomiseThingOrientationAroundDirection 0x0080FFB0: RELOCATION_MATCH, test PASS (g++ and VC7.1),
  vland clean. The lever that won was reading the physics pointer into a named local before the virtual call
  (`CTCPhysicsBase* physics = thing.PhysicsTC; physics->SetRHSet(CRightHandedSet(up, forward));`). That loads ecx ahead of
  the 24-byte set copy, so the copy runs in eax/edx as retail does. The parked `thing.PhysicsTC->SetRHSet(...)` copied
  through three registers and loaded ecx last. The parked model was otherwise right; with the named local it matches
  whether C3DVector's copy constructor is user-defined or trivial. The kept form uses the user copy constructor, the same as the other landed rows.
  Names re-proven: GetWorldSeed const (0x0049D810; GetLocalSeed is 0x0049D800), CMainGameComponent::GetWorld() returning
  CWorld& (PDB inline), GFFloatRandom, C3DVector::Rotate, CTCPhysicsBase::SetRHSet at slot 75 (see Label corrections).

## Parked for Fable 5.1 (9)
- 00945bd0 CAIStateGroup_SitForMeal::Update DIFFER(219v219): `mov ecx,[eax+18h] ; test byte [ecx+28h],40h` v retail `mov eax,[eax+18h] ; test byte [eax+28h],40h` | tried: PDB GetCreature() reference accessor, bool HasTC (if/return, ?:), named creature pointer, const CThingAICreature& local, raw bitset expression, `== 0` | next: lane gap: register choice for a dead pointer chain
- 007f8c20 CTCAnimatingObject::AddQueuedAnimation DIFFER(233v221): the def release goes through a register (`mov esi,[eax+4] ; dec esi ; mov [eax],esi`) v retail `dec dword ptr [ecx+4] ; jne`, and esi/edi are shrink-wrapped (pushed after the GetSubDef test) | tried: engine/CDefPointer.h, a local CDefPointer whose release holds the pointee in a local, early returns (278) | next: a header variant with a signed `long IVRefCount` (the parked file's signed count gave the RMW form and 221 bytes), then the push placement
- 00844600 CCreatureAction_Read::PerformAction DIFFER(222v222): this in edi and the text bank in esi (retail esi/edi); the font is now in edx as retail | tried: PDB accessors (the win), named font reference, early return, named bank reference | next: lane gap: callee-saved register colouring
- 00864ff0 CTCCreatureHitNotification::DrawHitNotification DIFFER(227v227): loop guard `cmp ebx,[edi+10h]` v retail `mov eax,[edi+10h] ; cmp ebx,eax`; info read via eax v edx; alignment pad nop v lea | tried: real STLport templates + PDB accessors (this register fixed), reversed end() compare, no engine local | next: a guard form that keeps end() in a register
- 00881690 NScreenEffect::CScreenEffectPriorityStack::InsertRadialBlurEntry DIFFER(229v229): `mov ecx,0Ch ; lea edi,[esp+28h]` scheduled after the fadingout/vtable stores (retail before) | tried: named frames local, body-assigned CStackEntry, body-assigned param, temporary entry, stack reference first | next: lane gap: scheduling
- 00808aa0 std::__linear_insert<pair<C3DVector,long>*, ...> DIFFER(229v229): else-arm by-value copies not software-pipelined (retail loads the second dword before `sub esp,2Ch`) | tried: /Oa, /Ow, trivially copyable C3DVector (rep movsd), body-assigned C3DVector copy | next: lane gap: scheduling of argument copies
- 009ad5f0 CDefinitionManager::DecompressDefinitionData DIFFER(231v231): block offset in ebp and the next entry's first index in ebx (retail ebx/ebp); data base added in ecx (retail eax) | tried: compressed size inline, long block offset, named next-first local, if/else bound | next: lane gap: register colouring
- 0074a4e0 CTCCreatureGenerator::DoNavCheck DIFFER(234v234): creature read into eax + `lea ebx,[eax+44h]` (retail ebx, add in place); the index sits in its own frame slot | tried: index declared after PeekTC (while/for) puts it in the dead creature slot like retail but merges the zero into ebx (232), single count variable, for-init index | next: combine the post-PeekTC index with a form that stops the zero CSE
- 00813010 CTCCreatureGroup::AddNewMember DIFFER(238v238): frame 0x64 v 0x5c; retail shares the 8-byte insert result slot with the uid/find-key slot | tried: named uid (before/after PeekWorld), insert result with constructors | next: real STLport map/pair templates (stand-ins still in place)

## Skipped
none.

## Names (evidence)
Proven (PDB name set plus a second source: donor order, vtable slot, body or caller):
- Own symbols: 0x00945BD0 ?Update@CAIStateGroup_SitForMeal@@MAEXXZ (vtable slot 12); 0x007F8C20 ?AddQueuedAnimation@CTCAnimatingObject@@QAEXABVCCharString@@_N@Z;
  0x00844600 ?PerformAction@CCreatureAction_Read@@UAEXXZ; 0x00864FF0 ?DrawHitNotification@CTCCreatureHitNotification@@QBEXABVCEnginePrimitivePrepareInfo@@@Z;
  0x00881690 (params nested in CEnvironmentSpellEffectInterface; see NAME_FIXES); 0x0080FFB0; 0x009AD5F0 ?DecompressDefinitionData@CDefinitionManager@@IBEXAAVCMemoryDataInputStream@@K@Z;
  0x0074A4E0 ?DoNavCheck@CTCCreatureGenerator@@IAEXABVCThingCreatureBase@@@Z; 0x00813010 ?AddNewMember@CTCCreatureGroup@@UAE_NPBVCThingAICreature@@@Z;
  0x00808AA0 STLport __linear_insert (anonymous-namespace comparator; leaf kept as substring because the ?A0x hash is per TU).
- Callees: every entry in NAME_FIXES.tsv, plus CIEngine::AddPrimitive (CEngine/CIEngine slot 20), CGameDefinitionManager::PeekGlobalDef,
  CMainGameComponent::GetTextBank / PeekCutSceneMainFont, CPlayerGui::GetSubtitleRenderer, CCreatureActionBase::GetPCreature,
  CThing::PeekBaseDef, CThing::PeekTC<T>, CThing::GetPos, CTCCreatureNavigation::IsAbleToNavigateFromPosToPos (QBE),
  CDefinitionManager::DecompressionCache (FableWin dynamic initializer name), CAnimFilterBase (vtable 0x0122FFCC, slot 2 = deleting dtor).
Descriptive or unproven:
- CDefinitionManager::CDecompressionCache: the class key and the static member's access (private here) are not in either PDB name list.
- The ETCInterfaceType enumerator for 0x46 (SitForMeal HasTC) and EMovementType 3 / EAnimUpdateMode 1 / EWeaponTrailType 2 are casts; their enumerator names are unproven.
- 00813010 still uses the parked stand-ins for map<unsigned __int64, CGroupMemberData>, make_pair and pair's converting ctor
  (struct SMemberPair / SMemberValue / CMemberMap): these must become real STLport templates before landing.
- Filler virtual slots: CTCPhysicsBase v00..v74 (0x0080FFB0), CIEngine v00..v19 (0x00864FF0; the manifest names only some slots).
- g_pMainGameComponent / g_pPlayerGui / g_framesPerSecond keep the landed spellings (no PDB data symbols).

## Label corrections
- 0x00864FF0 catalog "CThingPhysical_RenderWeaponTrails" is CTCCreatureHitNotification::DrawHitNotification.
- 0x0074A4E0 catalog "VertexBuffer_FilterNavigableVertices" is CTCCreatureGenerator::DoNavCheck.
- 0x00808AA0 catalog "std_insertion_sort_Generic" is STLport std::__linear_insert<pair<C3DVector,long>*, pair<C3DVector,long>, CCompareTargetedPositionsForFirstPersonTargeting>.
- 0x008FCF40 manifest "CXMVPlaybackInfo" is CAIStateGroupBase::Terminate; 0x009106E0 "CVillageTaskCarryCrate::AssignCreatureToTask" is CAIStateGroup_UsesSeat::SetSeat.
- 0x007239C0 (CTCPhysicsStandard slot 75) manifest "SetFacingAnglesFromRHSet" is SetRHSet. Ego_r donor order is SetFacingAnglesFromRHSet, UpdateOldRHSet, ResetRHSet, SetRHSet, which matches slot 74 0x007238E0 .. slot 75 0x007239C0, and the slot-75 body calls UpdateOldRHSet (slot 95) and then stores the set. The slot-74 label is the right one.

## Header debt
- engine/CIVCountedPointeeBase.h declares `mutable unsigned long IVRefCount`. At 0x007F8C20 this makes VC7.1 release through a
  register, while a signed long count gives retail's `dec dword ptr [ecx+4] ; jne`. Worth checking against other CDefPointer release sites.
- engine/CAIStateGroupBase.h, engine/CTCBase.h, engine/CTCPhysicsBase.h and engine/CCreatureActionBase.h flatten the vptr into data and cannot
  host the virtual slots these rows need, so local polymorphic models are used (`// Layout:` lines in the files).
- engine/CTCCreatureGenerator.h and engine/CDefinitionManager.h keep containers as opaque bytes, so the rows use local real-template models.
- CGlobalDef::GraphicWeaponTrail sits at +0x5C in retail (Ego_r +0x60).
