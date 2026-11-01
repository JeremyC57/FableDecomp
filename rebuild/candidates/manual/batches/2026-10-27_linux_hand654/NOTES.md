# Hand chunk 654 (CAIStateGroup_MinionPutOutFires .. CEnginePrimitiveManagerText): 3 of 10 byte-exact

## What matched (3)
- **_global / std::vector** -- 0x00A6BCD0 `vector<CIVCountedPointer<CManagedTextureData> >::_M_fill_insert` (catalog
  "Vector_InsertN_DetailPlacementGrid"): in-place slide-and-fill or `_M_insert_overflow` (protected, IAE, as the landed
  0x00A6BA60 leaf). Shape: the shared `engine/CIVCountedPointeeBase.h` (FABLE_RETAIL_DEF_CLASS_BASE) with AddRef/Release, the
  release-and-zero dtor and a REAL inline `operator=` (the 0x00AF2010 model): VC7.1 then knows `__x_copy` is not clobbered
  and stops reloading it in the tail (a declared-only operator= gave 285v276).
- **CTCVillage** -- 0x0065A8EE `IsTimeToGoToSchool(CThingAICreature*, const CWorld&, bool)`: is the child's school start
  (scaled by the world time step) later than the walk/run time for distance*5 capped at 70? Shapes: the PDB out-parameter
  `CThing::GetTC<T>(T**)` (T::StatGetInterfaceType, HasTC through the out-of-line bitset<274>::test) gives
  `mov eax,[eax+4] ; mov ecx,eax`; start/end in an inner block let the IS_THING_ALIVE temporary, the GetTC key and `start`
  share the dead `child` slot; two block-scope CCharStrings in an if/else give `push -1 ; je ; push A ; jmp ; push B ; call`;
  `if (start > r) return false; return true;` gives `test ah,41h ; jne`; `#pragma optimize("s", on)` TU.
- **CThingCreatureDef** -- 0x00670D50 `Validate(const CValidateDefinitionInitBase&, const CCharString&) const`: release build
  keeps only the side effects of the graphic-mesh, DEFAULT-animation, right-hand dummy object and focal helper-point checks.
  Shape: `AnimExists("DEFAULT", CAnimFilterNoGroup())` -- the IMPLICIT const char* -> CCharString conversion temporary is
  built before the argument pushes (cleanup bit 1, the frame slot above the filter, and the filter's inline base dtor keeps
  its 0x0122FFCC store); an explicit `CCharString("DEFAULT")` builds the filter first. The type-5 bank through a named
  `CMeshDataBank*`; GetSubDef is the landed 0x0042B0A2 body as a same-TU stand-in (as 0x004BFC50) so the def pointer does not
  escape (header CDefPointer release `dec [edi+4]`, no zero store). Retail headers: CSubDefClassBase/CDefPointer/
  CCountedPointer/CDefinitionManager pattern; real members PrimaryMeleeWeaponDef +0xd8, Graphic (CEngineGraphic) +0x100.

## Parked for Fable 5.1 (7)
- 007b8b10 CTCTavernTable::GetEatingPoint DIFFER(408v417): &pointIndex lives in ebp in retail v ebx here, the startIndex/this web swaps (ebx v ebp), retail copies the interface-map start `mov eax,[..+44h] ; mov esi,eax` and re-loads ParentThing through edx at the back edge | tried: 25 forms -- landed CVectorMap inline LowerBound, PDB GetTC<T>(T**) with the ternary pick, SetActionPointAsFree(long), CCharString::GetLength, cached end, signed compare (p12) and same-TU GetFood (p18) each flip the registers to retail's but change other code | next: lane gap: register allocation tie (try p12+p18 combined with the CTCTavernTable header layout)
- 00935e40 CAIStateGroup_MinionPutOutFires::PutOutFire DIFFER(476v468): retail loads the interface search start through eax then `mov esi,eax` (ours straight into esi) and builds the PlayAnimation zeros as `xor ecx,ecx ; push ecx` (ours push/mov immediates) | tried: 16 forms -- PDB GetCreature()/AIAction_PerformAction signatures, begin/count/cursor variants of the search, the landed 0x0091E750 call shape (which matches the zero register there) | next: the sibling 0x009363D0 (MinionWatchFires::Update) has identical code: solve both together; lane gap: constant-in-register choice
- 0078f8f0 CTCDivineWrath::CancelForCutscene DIFFER(506v505): retail loads the caster (ParentThing) into esi and spills it to [esp+18h] before FinishCurrentAction; ours calls Finish through a fresh ParentThing read and loads the caster after | tried: 19 forms -- STLport _Rb_tree iterator with begin/end in the for-init, same-TU StopSound member inlined, GetInterface(&abilities, 0xa7); every caster-first form gives the caster ebx and drops `mov ebx,1` (499v505) | next: lane gap: spill/scheduling of the caster load
- 00d38d20 CQ_FireHeartScript::RunVillagerComments DIFFER(529v526): retail lays the natural-end exit copy (inline ~vector free + DeregisterTimer) right after the final re-test and the merged return copy after it; ours the other way round (same ten exits, same two merged groups); plus `lea ebp,[ebx+eax]` v `mov ebp,[esi+110h] ; add ebp,ebx` | tried: 21 forms + 3 flags -- shared NScript headers (CScriptThing/CGameScriptInterface/CScriptBase/CTimer inline bodies), real STLport vector, loop forms (for(;;) top/bottom breaks), final-test polarity/bare call, inner blocks, operator!= | next: a CFG lever that decides which merged exit is laid first (GOTCHAS b19/I-42 family)
- 00bc7660 CEnginePrimitiveManagerText::Render2DPrimitive DIFFER(695v692): retail tests the frame-data pointer in ecx and keeps it across the LastFrameStamp store and into the not-loaded BeginLoading (both failed tests threaded, `mov ecx,edi` after the create), ours tests in eax and reloads [esi+8]; the copy tail's zero register at the null test (ours) v in the frame-data dtor | tried: 16 forms -- FableWin `bool CGraphicInfoFrame::GetTexture(const CTexture**, mip, flags) const` (residue 86 -> 38), TouchFrame member, IsInitialised, operator T*, nested/goto/inverted forms, counted-pointer zero order, named frame copy | next: lane gap: register allocation of the frame-data pointer (or the real CGraphicInfoFrame/CFrameData headers once written)
- 0095ef80 CAIStateGroup_OpinionReactionRoot::IsRunnable DIFFER(1536v1528): frame 0x38 v 0x30 -- retail shares one dword (sources list / GetTC key / the uninitialised opinion pointer) at +0x20 and puts the CIntelligentPointerConstThing in the candidate slot, ours keeps them apart; squared-distance sum dx2+dz2+dy2 (retail) v dz2+dy2+dx2; the reaction-table lea | tried: 13 forms -- PDB GetTC<T>(T**) for all five component reads (residue 92 -> 57), all six sum orders, GetSquaredMagnitude through operator-, block scopes for the source pick / opinion / pointer / flags | next: the engine/CAIStateGroup_OpinionReactionRoot.h header and real CTCOpinionTarget accessors; then slot packing
- 00533d30 NUISystem::CComponent::GetSize DIFFER(1723v1750): retail lays both centred arms first and both non-centred arms after (cross-jump of the maximum.Y stores), keeps `first` in edi + frame and Start in ebx, and merges the `first == -1` zero return into the final free/epilogue | tried: 12 forms -- minimum seeded in two statements + `maximum = minimum + GetSize()` through C2DVector::operator+ (frame 0x9c -> 0x94, residue 429 -> 344), first/i/count scopes and types, extern Find, inverted arm conditions (identical code) | next: the shared exit (single return of a result) and the maximum.Y cross-jump

## Skipped
- none

## Label corrections
- 0x00A6BCD0 catalog "Vector_InsertN_DetailPlacementGrid" is `std::vector<CIVCountedPointer<CManagedTextureData> >::_M_fill_insert`.
- 0x007B8590 manifest "GetFood" is `CTCTavernTable::GetTankard` (GetFood is 0x007B85C0).
- 0x00CBE583 (prior source "GetDistanceBetweenThings") is `GetSquaredDistanceBetweenThings` (no fsqrt; labels_sweep1 already right): the 0x00D38D20 constant 25.0f is 5 squared.
- 0x00A24280 / 0x00A242B0 catalog "GetAbilityLevel" are `CMeshDataBank::PeekStaticMesh` / `PeekAnimatedMesh` (wave-1 decision; coverage.tsv still stale).
- 0x00A2EA20 is `C3DMeshStats::HasHelperPoint` (one landed row declares it HasDummyObject; Ego_r order HasHelperPoint 0xB8E300, HasDummyObject 0xB8E340).
- 0x005DC2E0 (FUN_/CAnimationDef_FindAnimationByName) is `CAnimationSet::AnimExists(const CCharString&, const CAnimFilterBase&) const`.
- 0x00686A70 catalog "GetDisplayEngine" is `CTCBase::GetWorld() const` (protected).

## Names
- Proven (two sources): the ten row symbols; CTCVillage::GetSchoolTimes(float&, float&) const; CTCVillageMember::GetPVillage;
  bitset<274>::test; IS_THING_ALIVE<CThingBuilding*>; CMainGameComponent::ConstantFPS/GetConstantFPS; CMeshDataBank::Peek*;
  C3DMeshStats::HasHelperPoint/HasDummyObject; C3DMeshInfo::GetMeshStats; CAnimationSet::AnimExists; CCRC::Calc;
  CCharString::GetLength/GetAsCharArray/GetCRC; CDefClassBase::GetSubDef<CAppearanceDef>; CTimer {TimerIndex};
  GetSquaredDistanceBetweenThings; CTCUserActionPointBase::SetActionPointAsFree(long); CTCTavernTable::GetFood(long).
- Descriptive: TCI_* enumerators, AI_ACTION_*/MOVEMENT_TYPE_* enums, the CTCInterfaceA7 fields (really CTCSpecialAbilities
  AbilityMutexLock +0xC / CurrentAbility +0x10), CValidateDefinitionInitBase +8 holder (CValidateResources, retail-only),
  locals; globals 0x013BAC30/34/48 keep the bound placeholder spellings g_CCharString_013bac30/34/48 (three competing
  descriptive spellings bound, none PDB-proven).

## Header debt
- engine/CThingAICreature.h carries Ego_r offsets (WorkBuilding retail +0x170, PDB +0x198).
- engine/CAnimFilterNoGroup.h is a flattened generated stub (no vtables); rows model CAnimFilterBase/NoGroup locally.
- No engine header for CThingCreatureDef, CAppearanceDef (+0x34 Animation), CEngineGraphic, CGraphicInfoFrame/CFrameData,
  CValidateDefinitionInitBase (Ego_r shows 8 bytes; retail reads +8).
- fable_string.h CCharString lacks the copy ctor and the defaulted length; rows define CCharString locally.
- vland (read-only, tmp/vland.log): 3 RELOCATION_MATCH + PASS, no RELOC_IDENTITY; WARN SHARED_HEADER_AVAILABLE for CTCBase
  (0x0065A8EE; engine/CTCBase.h has no GetWorld and a plain-void* vptr) and C3DMeshInfo / CMeshDataBank (0x00670D50; the
  generated headers are flattened data-only stubs without GetMeshStats / PeekStaticMesh / PeekAnimatedMesh, so the rows
  declare the callee classes locally). Two RELOC_NAME warnings on 0x00A6BCD0 (catalog labels of 0x00A6AFD0 / 0x00A6B340
  are descriptive; our STLport spellings are the template instances).
