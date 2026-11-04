# Chunk 764 -- 25 parked rows (297-342 bytes): 8 matched, 17 parked

## Results
| address | name | status | grades | grade / residue |
|---|---|---|---|---|
| 00750110 | CTCChest::DisplayLockedMessage | matched | 2 | RELOCATION_MATCH |
| 006d7b80 | CTCHeroExperience::GetAbilityPossibleLevelUpAmount | matched | 13 | RELOCATION_MATCH |
| 00859660 | CCombatActionBase::ResetMeleeCombatAbilityEffects | matched | 12 | RELOCATION_MATCH |
| 00849f70 | CCreatureAction_PickUpThingAtTable::PerformAction | matched | 5 | RELOCATION_MATCH |
| 0064d92a | NPlayerGui::CDrawQuickAccessIcons::ToggleCharge | matched | 4 | RELOCATION_MATCH |
| 00b451d0 | CEngineLightingManager::CalculateFogTransformMatrix | matched | 4 | RELOCATION_MATCH |
| 005be284 | NInventory::CTCInventoryBase::SetNumberOfSelectedItemOwned | matched | 3 | RELOCATION_MATCH |
| 006c7370 | CTCHeroOpinionDeedLog::OnSerialise (was "CDeedManager::TransferState") | matched | 7 | RELOCATION_MATCH |
| 0083b6b0 | CThingVillage::DrawPreparePrimitivesForRendering | parked | 15 | 308v309: interpolant copied to a frame word (retail x87) / add order |
| 0083dc80 | CThingPhysicalSwitch::DrawPreparePrimitivesForRendering | parked | 2 | 308v309: as 0x0083B6B0 (same template, ported in one step) |
| 00837b30 | CThingTrackNode::DrawPreparePrimitivesForRendering | parked | 2 | 313v314: as 0x0083B6B0 |
| 007bc004 | NThingShot::CTCShotDefinitionNymphFun::InitMovement | parked | 24 | 343v342 dl=5: temporaries' frame slots |
| 00746ec0 | CTCStealth::GetPositionToMoveToForStabOn | parked | 13 | 326v326 dl=10: frame slots (position / direction words) |
| 00407cd0 | CUserProfileManager::RedefineControlPassive | parked | 11 | 309v310 |
| 0096dbc0 | CCreatureAction_ScreamerIntoAggressive::Initialise | parked | 9 | 331v331 dl=28: three-register rotation + two frame words swapped |
| 00ed9bb0 | NScript::CV_TalentlessBardScript::ProcessSongsClothes | parked | 8 | 298v306 dl=1: retail's dead entry store `mov [esp+4],0` |
| 00f35c60 | NFractal::CNoise::PerlinNoise | parked | 8 | 312v316 |
| 0089da70 | CGameScriptInterface::EntitySetAlpha | parked | 6 | 314v314 dl=43: loop registers |
| 005ebfa0 | CTCTavernGame::CameraUseCameraPoint | parked | 5 | 321v329; NOT land-ready (fabricated same-TU FindThingsWithScriptName stand-in) |
| 0070d780 | CAnimateThing::AddCombinationAnim | parked | 4 | 305v305: ebp caching of InternalAlphas._M_start |
| 00753380 | CTCLook::FrameUpdateHeadInterested | parked | 4 | 298v297: block layout |
| 009d5bf0 | CBankFileAsync::HandleFailedAllocation | parked | 3 | 305v314: this spill / registers |
| 00a76770 | CNavQuadTreeNode::GetLeafNodeAt | parked | 3 | 318v321: QuadTreeLevel CSE / tail calls |
| 00969e70 | CAIStateGroup_SleepOnFloor::IsRunnable | parked | 2 | 324v311: old local model; the position's x87 caching and box build order |
| 00bcfb00 | CEnginePrimitiveManagerAnimatedMeshesBase::RenderPrimitiveBlocksIndexBuffer | parked | 2 | 329v336: register/frame allocation around the expanded DrawBuffersIndexed |

Budget note: rows below 10 grades ran out of session budget (the trio 0x0083B6B0 got 15, the rest were sorted by residue size); every parked row's next idea is below.

vland (read-only, `vland_result.txt`): WINS 8, all RELOCATION_MATCH + behaviour PASS. Warnings only:
- 005be284 SHARED_HEADER_AVAILABLE CCategory: engine/CCategory.h is a global-namespace generated stand-in; the PDB class is NInventory::CCategory, so the row keeps a one-method local NInventory::CCategory (the right mangled name).
- 00750110: SHARED_HEADER_AVAILABLE for local stand-ins (CDefinitionManager, CThing, CMainGameComponent, CSubtitleRenderer, CPlayerGui, CTCBase); RELOC_NAME g_pPlayerGui -> 0x013B8790 labelled CTBaseSingletonNoDefaultAlloc<CPlayerGui>::PClass.
- 00859660 RELOC_NAME: 0x004C6140 RemoveBulletTimeSlowdownMultiplier (PDB) vs label CTimeMultiplierController_004c6140::RemoveRequest (see NAME_FIXES).
- 006c7370 RELOC_NAME: the local std::__copy stand-in vs the label std::__copy<_Rb_tree_iterator<pair<long const,COpinionDeed*>>, back_insert_iterator<vector<pair<long,COpinionDeed*>>>, int> (this label confirms the call is STLport __copy).

## What fixed each match (GOTCHAS material)
- **00750110**: the landed sibling 0x0074EE90 model (CDefPointer / CSubDefClassBase headers, same-TU GetSubDef body, PeekComponent().PeekCutSceneMainFont(), const member).
- **006d7b80**: retail headers (CThing GetTC, the CTCHeroExperience / CTCInventoryAbilities overlays) and a while loop with two induction variables (++levels; ++nextLevel), which VC7.1 folds into nextLevel+levels-1.
- **00859660**: retail headers + overlays (CCombatActionBase members, CCreatureActionBase accessors, CWorld::GetEnvironment, haste / bullet-time / environment declarations), a same-TU noinline GetDef<T> body, and the PDB names.
- **00849f70**: retail headers (CThing GetTC, PeekBaseDef), a same-TU `__declspec(noinline)` GetSubDef<CCarryableDef> body, and `!thing->IsDead()`, not `IsAlive()`.
- **0064d92a**: retail headers with FABLE_RETAIL_CVECTORMAP_FORCEINLINE_FIND in the size TU, a same-TU noinline IS_THING_ALIVE<CThingPlayerCreature*> body (as the siblings 0x0064B668 / 0x0064D6A8), and a named CCharString.
- **00b451d0**: the PDB signature (C4DVector by value) and the landed sibling SetupDefaultFogStates' 12-float ctor + inline PreMultiply (a copy of *this = rep movsd), moved into overlays.
- **005be284**: the old local model gave a global esi/edi swap. The retail-header port fixed it in one grade:
  - `PeekTC(TCI_INVENTORY / _WEAPONS / _CLOTHING, &inv)` on the hero;
  - CTCBase::PeekPlayerManager, the const GetMainPlayer and PeekPControlledCreature;
  - a same-TU noinline IS_THING_ALIVE<const CThingPlayerCreature*>;
  - the PDB const GetNumberOfItemsOfTypeInInventory.
- **006c7370**: the identity comes from retail vtable 0x0125EA48 slot 1 (RTTI CTCHeroOpinionDeedLog) and the Ego_r members, which are the five transfer strings. Three source changes closed the residue:
  - `if (context.IsSaving())`: the inline predicate puts the save body on the fall-through, as retail does (the open `Mode==1||Mode==3` laid the else first);
  - the save copy is `std::copy(begin, end, back_insert_iterator(records))`, an inline wrapper over STLport `__copy(first, last, result, const input_iterator_tag&, int*)` 0x006C9B70. It is __fastcall: the hidden back_insert_iterator return goes in ecx, the tag reference in edx, and the by-value tree iterators are copy-constructed in place. The wrapper's by-value parameters make VC7.1 evaluate both iterators before the pushes;
  - `records.reserve(map.size())`: the inline size() puts the count in eax, which fixed the whole eax/ecx/edx rotation (dl 17 -> 0).

GOTCHAS one-liners:
- A CPersistContext save/load row whose `Mode==1||Mode==3` test lays the load branch first wants the inline `IsSaving()`.
- An STLport range copy into a vector is the inline `std::copy` -> fastcall `__copy(..., const input_iterator_tag&, int*)`. Retail pushes 0 and &container, builds the iterators in their arg slots, and passes ecx = hidden return, edx = tag.
- A three-register (eax/ecx/edx) rotation in a reserve(count) / push sequence: read the count through an inline accessor (`map.size()`), not the member.
- The same-TU noinline IS_THING_ALIVE<T> / GetSubDef<T> / GetDef<T> bodies let VC7.1 know the reference does not escape, and give retail's register and frame sharing.
- `!thing->IsDead()` gives `test byte [..+0x91],1`; `IsAlive()` gives not/test.
- CCountedPointer(T*) declared without a body gives retail's out-of-line ctor call (FABLE_RETAIL_CCOUNTEDPOINTER_CALLED_CTOR; 0x0096DBC0 length fix).
- Writing the byte conversion `(unsigned char)(alpha*255.0f)` at each call lets VC7.1 CSE it into a dead argument slot (0x0089DA70 length fix).
- `v *= FastInvSqrt(v.GetLengthSquared())` with operator*=(const float&) and a plain-inline `X*X+(Y*Y+Z*Z)` gives retail's `fstp [r]; fxch; fmul [r]` (0x00746EC0).
- In a DrawPreparePrimitivesForRendering static-mesh row, passing `info.Current.WFInterpolate` straight into the interpolation (no named float) fixes the length to within one byte. A named `float t` hoists the fld to the statement start (296 v 309).

## Parked residues and next ideas
- **0083b6b0 / 0083dc80 / 00837b30** (one template, one byte short; shared C3DVector / CRightHandedSet / CRGBColour / CEnginePrimitiveThemeChannelDesc headers):
  - Residue: retail loads the interpolant into x87 after the tint bytes (`fld [info+4]`, then `fmul st(2)` / `fmul st(1)`, `fstp st(0)` after the Y add) and adds/stores X, Y, then Z. Ours copies the by-value float parameter of operator* into a frame word and adds Z, Y, X. Frame 0x90 v 0x80; UniqueID low is preloaded into ebx in retail.
  - Tried: a named float (296), a `const float&`, `L + (P-L)*t`, rvalue forms, and an overlay operator+ built on += (fixed the multiply phase only, so reverted).
  - Next: CTCPhysicsBase::GetPosition(float) const is out of line at 0x0049D150 (same Z,Y,X order as ours), so retail did not inline it here. Look for a landed static-mesh sibling, or a C3DVector lerp-like inline in FableWin.
- **007bc004** (dl=5): retail gives the RandomiseShotDirection temp, the CRightHandedSet temp and the velocity temp disjoint frame slots (0x5c) and copies the result through eax. Next: scopes / lifetimes that keep the temporaries unshared.
- **00746ec0** (dl=10, same length): frame slots only. Retail puts position at +0x10..+0x18 with Z sharing the dead direction.Z word; ours puts direction.X there.
- **0096dbc0** (dl=28, same length): a three-register rotation (retail this=ebp, &PTarget=ebx, temp=esi) and the mode-manager / new-result words swapped. Next: write each exit out in full (the cross-jump lever).
- **00ed9bb0** (dl=1): only retail's dead entry store `mov [esp+4],0` into the CCharString temp slot is missing.
- **0089da70** (dl=43, same length): the loop's start/count/item registers (retail start eax, count/item ecx with `mov edx,ecx`).
- **0070d780** (same length): retail keeps InternalAlphas._M_start in ebp from after Init to the final free.
- **005ebfa0**: not land-ready. The draft's same-TU FindThingsWithScriptName body is a fabricated stand-in (0x005ED860 is GetAllThings<...> with a ThingSearchMethods filter), and the landed callee names differ (see NAME_FIXES). Needs a real port.
- **00969e70**: old local model (eight GetInterface variants). Retail loads pos.X / pos.Y once and keeps them in x87 (`fld st(1); fsub 20`), builds the box X0, Y0, X1, Y1 and keeps the zero in edi. A copied C3DVector local copies via integers (350). Next: a CThing / CAIStateGroupBase retail-header port with C2DBoxF, as the landed AI rough-area siblings 0x0091B010 / 0x00977B10.
- **00bcfb00**: the PDB signature DrawBuffersIndexed(unsigned long, unsigned long, CIndexBuffer&, unsigned long, unsigned long, EPrimitiveType) cut dl 50 -> 40. Retail keeps the shader manager in ebx and the index buffer in ebp and homes the three counts; ours homes the manager. Next: port to the shared CShaderRenderManager.h / EPrimitiveType.h.
- **00407cd0 / 00f35c60 / 00753380 / 009d5bf0 / 00a76770**: see each `differ/<addr>.cpp` line 2. These were unchanged by this session's levers.

## Header overlays (hdr/engine; all new sub-macros, hdrcheck2 default=same for every file)
- Used by matched rows:
  - CTCBase.h FABLE_RETAIL_CTCBASE_PEEKPLAYERMANAGER (0x00686D50);
  - NInventory_CTCInventoryBase.h FABLE_RETAIL_CTCINVENTORYBASE_SELECTEDOWNED (GetSelectedCategory, const GetNumberOfItemsOfTypeInInventory, protected SetNumberOfSelectedItemOwned);
  - CTCCarrying.h _PUTTHING;
  - CMatrix3x4.h _PREMULTIPLY;
  - CEngineCamera.h _NOSCALEVIEWTRANSFORM;
  - CEngineLightingManager.h _FOGTRANSFORM;
  - CTCHeroExperience.h _STATEXPERIENCE;
  - CTCInventoryAbilities.h _LEVELUP;
  - CCombatActionBase.h _INITIALISE / _MELEE;
  - CCreatureActionBase.h _ACCESSORS;
  - CTCHaste.h _UNSET;
  - CBulletTimeManager.h _REMOVESLOWDOWN;
  - CEnvironment.h _DEFAULTTHEME;
  - CWorld.h _ENVIRONMENT / _MESSAGEEVENTMANAGER;
  - CTCCreatureModeManager.h _ADDMODEPOINTER.
- Used by parked rows only:
  - CCountedPointer.h _CALLED_CTOR (0096dbc0);
  - CTCPhysicsBase.h _APPLYACCELERATION, CRightHandedSet.h _CTOR, CWorldMap.h _HEIGHTZ (007bc004).
- No landed row defines any of the new macros (SIBLINGS.tsv).

Helpers: none. No row needed an impossible-list instruction.
