# Hand chunk 646 (CCreatureActionBase .. NUISystem::CList): 3 of 10 byte-exact

Ten parked rows from the parked-row retry sweep (all same-length misses). Every row got a real attempt with at least two new
levers (most got four to twelve probes). Three now grade RELOCATION_MATCH. Their tests pass under both g++ and VC7.1.

## What matched (3)
- **NPlayerGui::CDrawNPCName::Draw (0x006572A0)**: draws the NPC name centred at the top of the screen and, while the screenshot
  message shows, TEXT_GUI_MENU_SCREENSHOT_TAKEN. The fix was the font call's real signature. Slot 1 is
  `virtual C2DExtentsF GetStringExtents(const wchar_t*, bool) const`, the same slot the landed CFontBank::GetVStringExtents
  0x00437210 calls. It is reached through the PDB inline `CFontBank::GetStringWidth(const CWideString&)`. The `push 0` before
  the GetAsCharArray call is the bool argument, not a GetString(0) argument. The 8-byte non-POD return then shares the
  render-target dimensions' slot as in retail. Also respelled: CPlayerGui::Draw2DText(const CFontBank&),
  CMainGameComponent::PeekCutSceneMainFont / GetTextBank / GetCurrentWorldFrame, NGameText::CDataBank::GetTextBySymbol const,
  and the CDrawBase members.
- **NTCScriptedControl::CActionTalkBase::GetNextCameraTagIndex (0x00712E40)**: picks the next conversation camera tag. The fix
  was to write `rand() % 3` in each branch (`switch (rand() % 3)` and `if (rand() % 3 == 0) return current_index;`). VC7.1
  hoists the identical call above the `current_index == -1` test and only then schedules the current_index load before
  `or esi,-1`. That also put the DefaultCameraTags base load in edx/eax as in retail. The static DefaultCameraTags is an
  STLport vector indexed with `*(begin()+n)`.
- **CCreatureActionBase::InitialiseActionEvents (0x006946C0)**: for each embedded animation event whose first word names a
  registered action event, it allocates a copy and keeps it when SetDataFromString accepts the name. The fix was same-TU
  bodies for BOTH callees that take the event:
  - CCountedPointer<CActionEventBase>::Reset, as a noinline explicit specialisation carrying the shared header's Reset body
    (retail calls 0x00695A00 out of line);
  - the ActionEvents push_back stand-in.

  With only one of them visible, the inline ~CCountedPointer still loaded eax, copied it to edi and re-compared. With both,
  it gives retail's `mov edi,[Info] ; dec [edi] ; jne`. The shared engine/CCountedPointer.h now replaces the old local
  count-block model. The PDB inline `CWorld::GetGameCamera()` gives the camera chain's ecx/eax roles.

## Parked for Fable 5.1 (7)
- 00868e50 GetArmourWeightsFromThing DIFFER(456v456): `fld [edi+60h] ; fadd [eax]` v retail `fld [eax] ; fadd [edi+60h]` in total_weight += weighting | tried: k = 0..7 unused named locals at function entry, loop body and armour block (24 grades, all inert); (double) sum; += (double)w; (void)total_weight; (void)modifier->Weighting; const float& / float accessor on both uses; const float* weighting | next: lane gap: x87 commutation (the synthetic rule says the larger displacement is loaded first, retail does the opposite; try an alias of total_weight through a by-value pair member or a different multimap insert shape)
- 00649e8b NPlayerGui::CDrawFishingGame::Draw DIFFER(469v469): the two implicit CCharString temporaries share [ebp-0xc] (retail -0xc / -0x10), frame 0x14 v 0x18; colour B/A constant stores before the quest-info `mov ecx,[eax+1c8h]` | tried: the landed sibling model (0x0064A060: visible LowerBound + IS_THING_ALIVE template), which fixed the hero/key/strain slot; explicit CCharString temporaries (474); inline GetText member; non-const GetTextBySymbol; named symbol pointer; colour call temporary; quest-info reference; no optimize("s") (492); early return | next: a scope that keeps the first char temporary alive across the second GetTextBySymbol (e.g. both texts fetched in one helper or one full-expression)
- 009c8c00 NGameText::CDataBank::NotifyEntryAdded DIFFER(469v469): scratch registers rotate eax->ecx->edx at every pushed-address / reload site (push_back args, ReadWithSrcChunkOverflow, ReadNullTerminatedString, final data reload); NarratorIndex stored before the push | tried: same-TU push_back bodies for both buffers; real CIVCountedPointer copy ctor/dtor (493); k-sweep 0..7 | next: model CMemoryDataInputStream with its real virtuals (ReadNullTerminatedString is UAE) instead of the derived reader view; same-TU stand-ins for the stream ctor/dtor and ReadWithSrcChunkOverflow
- 00697490 CGameCameraManager::IsPosCameraInsideThing DIFFER(475v475): `mov eax,[esp+5ch]` (pos) after the last axis `fstp [esp+14h]` (retail before) | tried: PDB signature (kept), operator/= with inner reciprocal, member-wise offset, pos alias, offset before the normalise (486) / before the scale (480), float along (483) | next: lane gap: scheduling (single load/store swap)
- 008539c0 NCameraModes::CScriptModuleUseLamp::OnInitLeaf DIFFER(475v475): AxisY temporary `mov edx,[esp+1ch]` before `fstp [esp+20h]`; the unsigned side store before `test eax,eax` | tried: named delta, implicit copy ctor, inline (float)(rotated % 2), user operator= (495), stand-ins for GetHelperPoint / GetPWorldSeed (inert) and GFROR13 (473) | next: lane gap: scheduling (twin 0x00853670 has the same residue)
- 00687730 CMessageEventManager::AddMessage DIFFER(477v477): GetTC key homed in the dead def slot [esp+0x74], retail the dead pos slot [esp+0x78]; register renames follow | tried: PDB reference signature (kept), same-TU LowerBound body, pos copied member-wise, pos alias, user operator= | next: the faction read through a different accessor (CTCFactionMember via a PDB inline GetFaction / Peek) so that the key's scope changes
- 00537776 NUISystem::CList::CanMove DIFFER(480v480): frame 0x3c v 0x48, because retail keeps top, the ConvertX / GetOffset hidden results and the margin in separate slots | tried: named converted / scroll locals in a block (97 lines, same frame); a second named copy of top | next: rebuild the model with the PDB names (GetUIScale is NUISystem::CManager::GetUIScale const returning C2DVector; CanMove is a UAE virtual) and named C2DVector results per call site

## Skipped
None.

## Label corrections
- 0x009C8C00 catalog "NGameText::CDataBank::vfunc_12" is `?NotifyEntryAdded@CDataBank@NGameText@@MAEXKKABV?$CArray@D@@@Z`
  (protected virtual, vtable 0x0129B4D4 slot 12). The differ now spells both NotifyEntryAdded overrides protected.
- 0x00712E40 catalog class "CActionTalkBase" is `NTCScriptedControl::CActionTalkBase` (protected IAE).
- 0x006572A0 is `NPlayerGui::CDrawNPCName::Draw` and 0x00649E8B is `NPlayerGui::CDrawFishingGame::Draw` (namespace missing in the catalog).
- 0x00697490: the PDB signature takes `const CThing&`, not a pointer.
- 0x00687730: the PDB signature takes `const CMessageEventDef&, const C3DVector&`, not pointers.

## Names: proven vs descriptive
- Proven (Ego_r/FableWin name set plus a second source, objdump-checked): Draw, CPlayerGui::Draw2DText,
  CWideString::GetLength / GetAsCharArray, CDisplayManager::GetRenderTargetDimensions, GFAXToVX / GFAYToVY,
  NGameText::CDataBank::GetTextBySymbol, CMainGameComponent::PeekCutSceneMainFont / GetTextBank (inline),
  CFontBank::GetStringWidth (inline) / GetStringExtents (slot 1), GetNextCameraTagIndex, DefaultCameraTags,
  InitialiseActionEvents, CBaseIntelligentPointer::GetPItem (CBaseObject*), CThing::GetWorld (IAE), CWorld::GetGameCamera,
  NAnimationEvents::CManager::GetEventNames / GetEventTimeInSeconds, CMainGameComponent::IsEditorActive (slot 9),
  CAIGameCameraBase GetCameraManager (slot 9, Ego_r UAE name on CGameCameraManager), NCreatureAction::CActionEventBase
  Alloc / SetDataFromString (landed sibling slot order).
- Probable (one PDB name plus body shape): CMainGameComponent::GetCurrentWorldFrame 0x004167C8 (QBEJXZ; its body is a tail
  `jmp` to CWorld::GetFrame 0x0049D870); NAnimationEvents::GetGameEventManager 0x006F90B0 (the Game/Sound twins have the same
  shape; the game one fits action events).
- Descriptive (no PDB data names): g_pDisplayManager 0x013B8390 (the most common landed spelling), the camera-tag flag
  0x01380C30 and side 0x013BAD00 (g_UseConversationCameraTags / g_ConversationCameraSide), g_FramesPerSecond 0x01375550, the
  slot 7 animation-index virtual of CCreatureActionBase (GetAnimationIndex), and the RegisteredActionEvents map view
  (CRegisteredActionEventMap::Find 0x00696340, which is the STLport `_Rb_tree::_M_find` of
  map<CCharString, CCountedPointer<NCreatureAction::CActionEventBase> > CCreatureActionBase::RegisteredActionEvents).

## Header debt
- 0x006946C0 uses a local CCreatureActionBase view because engine/CCreatureActionBase.h flattens the vptr and this row calls
  virtual slots 6 and 7. The map is modelled as a one-word node view, not a real STLport map.
- 0x006572A0 and 0x00649E8B use local CDrawBase / CDrawNPCName / CDrawFishingGame / CPlayerGui views. No engine header exists
  for CDrawBase. engine/CPlayerGui.h is a flattened PDB layout (PDrawQuestInfo is at retail +0x1c8, PDB +0x1d0).
