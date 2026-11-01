# Hand chunk 652 (CTCActionUseScriptedHook .. NShape::CShape): 4 of 10 byte-exact

All four matches are RELOCATION_MATCH under q.py, and vland grades all four `RELOCATION_MATCH PASS`. No RELOC_IDENTITY errors were reported. vland's remaining warnings are SHARED_HEADER_AVAILABLE (header debt, listed below) and RELOC_NAME label mismatches (listed under Label corrections).

## What matched (4)
- **CCreatureAction_GiveMoneyToThing::PerformAction (0x00845040)**
  - What it does: takes up to Amount gold from the creature's hero stats and gives it to the receiving thing. It posts an entity "received money" event, and logs a deed when the creature is the player character giving to a wife or villager.
  - Shape: this is the landed CCreatureAction shared model (0x00849E80) with three changes. The same-TU LowerBound bodies are removed. `GetTC` uses the one-ternary store `*out = ((it == end || T::type < it->first) ? end : it)->second`. `HasTC` is `__forceinline`, matching the 501 landed HasTC rows; plain inline runs out of VC7.1's inline budget.
- **NShape::CShape::OnSerialise (0x00761C00)**
  - What it does: persists "Shape[i]", which is the type name (".Type"), the point count and every point (X and Y are stored relative to the integer offset).
  - Shape: Transfer<T> is the PDB member template, three-argument plus a forwarding two-argument form with `GetDefaultVal<T>()`. The real landed `Transfer<float>` body (0x00410620) sits in the same TU as a noinline explicit specialisation. VC7.1 can then see that the 0.0f default temporaries are only read, and hoists their three stores out of the loop as retail does.
  - Names: the two type names are the file's anonymous-namespace `CLOSED_STRING_NAME` / `LINE_STRING_NAME`. OnSerialise is virtual (vtable 0x0126A208 slot 2).
- **CTCActionUseScriptedHook::GlobalUpdate(bool) (0x0072A090)**
  - What it does: consumes the pending confirmation message. Otherwise it teleports the hero through the connected entrance (door sounds) and starts a spline scripted camera mode when the camera track's point is a track point.
  - Shape, part 1 (the prior's three register residues all came from stand-ins):
    - The engine CCountedPointer keeps the mode Data in esi.
    - The GetTC ternary loads the component straight into ecx.
    - FableWin.pdb inline `CWorld::GetGameCamera()` and `CWorld::GetMessageEventManager()` fix the camera vcall registers and remove a dead `add eax,60h`.
  - Shape, part 2 (real types replacing invented ones):
    - The camera module is `CCountedPointer<NCameraModes::CScriptModuleBase>` holding `new CScriptModuleSpline(0, -1)`, not an invented CScriptModuleCameraTrack.
    - The track is set through `CScriptModuleBase::SetPCameraPoint(CThing&)` (the COMDAT at 0x0072A4A0).
    - The point test is the inline template `CTCCameraPointDefinitionBase::GetPointType<CTCCameraPointTrack>`.
- **CTCInventoryMap::PanMap (0x005FB140)**
  - What it does: pans the zooming map. Once it has moved, it selects the region whose world-map offset is nearest the map centre (within 128 units).
  - Shape: the real STLport 4.6 `_Rb_tree` / `map` text, as the landed 0x00613630 operator++ carries it. The default-constructed iterator (`_M_node = 0`) is the best-region iterator, and `!=` is the free operator on `_Rb_tree_base_iterator`. The prior's 873v877 loop allocation residue disappeared with no other change.

## Parked for Fable 5.1 (6)
- 0x009C4F20 CFileInstaller::ProcessAlways DIFFER(490v490): retail re-reads PCompletedRead after the null test (`cmp [ebp+74h],esi ; je ; mov ecx,[ebp+74h]`); ours keeps it in a register | tried: 24 probes (const& views, loop forms, real STLport list/set, ProcessFileReadCompletion same-TU, member order); the member with the vo-latile qualifier is RELOCATION_MATCH (probe/009c4f20_diagV2.cpp) | next: owner decision on the qualifier (the landed 0x009C42F0 already carries it), else lane gap: purity rule
- 0x00E36750 CV_ArcheryCompetitionScript::DoCountDown DIFFER(541v536): retail keeps the post-wait exit and the wait-loop return block right after the post-wait test; VC7.1 here sinks both to the end (block placement only) | tried: 19 probes (for/while/for(;;) timer loops, named locals, bool loop variable, out-of-line GetTime, /Ob1 /Og- /Os /GL, QFE 4035) | next: the real quest-004 countdown source shape (Aeon Lua port / FSE) and an out-of-line ScriptedThingBase dtor
- 0x007280C0 CTCRandomAppearanceMorph::UpdateMorphSet DIFFER(795v802): def/flag frame slots swapped, def base kept in edi with dec flags feeding jne, and the morph loops differ (150 lines, prior 236) | tried: 18 probes (engine CIVCountedPointee AddRef/Release, local-pointee CDefPointer dtor, out-of-line vector dtor 0x005BC0D3, && flag) | next: model the real CRandomAppearanceMorph::CMorphSet (three CArray members) and CTCAppearanceMorphBase::CMeshMorphSet with real STLport containers
- 0x005FD5E0 CTCInventoryMap::ConstructRegionDescriptionQuests DIFFER(975v982): retail moves the whole non-core subtree (IsVignette, vignette / optional / second core leaves) after the `ret 0Ch` and keeps the shared FindChild/SetText/AddChild tail right after the core block; VC7.1 here lays the subtree out in source order | tried: 14 probes (`!IsCore()` nesting, bool local, `continue`, tail duplicated per leaf / per arm, goto merge, /Oy-) | next: lane gap: block layout (no source form found that makes VC7.1 treat the IsCore()==false arm as out of line)
- 0x006276F0 CTCQuickAccessMenu::GetItemsListUI DIFFER(1244v1260): retail keeps the others-vector finish in edi and saves ebx late; here every vector word lives in memory | tried: 12 probes; the same file plus an invented same-TU body for ConstructItemCategoryList is RELOCATION_MATCH (probe/006276f0_p05.cpp) | next: reconstruct CTCQuickAccessMenu::ConstructItemCategoryList 0x00626E40 (~1.5 KB, same retail TU, just before this function) and compile it ahead of GetItemsListUI
- 0x00E96110 CV_HauntedHouseScript::CM_QuestControl::Main DIFFER(1360v1356): retail promotes 0 into ebx for the whole body and keeps the timer index in edi; ours gives the timer index ebx | tried: 17 probes; with ~CTimer out of line everywhere the zero lands in ebx as retail (p11, 121 lines), but retail inlines ~CTimer at the two exits inside the decay loop | next: find the inline expansion that spends VC7.1's per-function budget before the main exit in retail

## Skipped
- none

## Label corrections
- 0x006BBF40 is `CRegion::GetWorldMapOffset` (CRegionDef +0xAC = Ego_r +0xB8 less 12). 0x006BBF70 (+0xB4) is GetNameGraphicOffset. rebuild/manifest/functions.tsv and the landed CTCInventoryMap_SetPanMode_005fb8f0.cpp call 0x006BBF40 GetNameGraphicOffset; vland's label (GetWorldMapOffset) is right.
- 0x00487DC0 label `CPlayer::GetHeroCreature` → `CPlayer::GetPControlledCreature` (FableWin.pdb, 65 bindings).
- 0x0072A4A0 label `_unattributed::AssignTarget_0072a4a0` → `NCameraModes::CScriptModuleBase::SetPCameraPoint(CThing&)`.
- 0x005ED2C0 label `_global::CCountedPointer_Release_005ed2c0` → `CCountedPointer<NCameraModes::CScriptModuleBase>::~CCountedPointer`. Its body is the destructor, not ReleaseData: it zeroes Data before Info.
- 0x0066BD00: the bindings disagree; the right name is `CTCSoundPlayer::PlayAttachedCriteria(const CCharString&, bool, bool, bool, long)`, not the `CTCSound::PlaySound` spellings.
- 0x00852710 is `NCameraModes::CScriptModuleSpline::CScriptModuleSpline(long, long)`. The prior's invented CScriptModuleCameraTrack is wrong.
- 0x004C7590 (UpdateMorphSet callee): the bindings disagree (UpdateGraphicAppearance / SetGraphicAppearanceChanged). Unresolved.

## Names: proven vs descriptive
- **Proven (two sources: PDB symbol + retail use or binding):**
  - Every leaf symbol (objdump-confirmed against Ego_r/FableWin) and every callee in NAME_FIXES.tsv.
  - CTCActionUseScriptedHook members (Ego_r).
  - CV_HauntedHouseScript UndeadBar / GuiCounter / CellarDoorOpen / InHouse and CScriptDef HH_UpperBarLimit / HH_BarDecrementAmount / HH_BarDecrementInterval / HH_ClearedHHRenown (Ego_r, with the retail offset shift).
  - CMessageEvent::ExtraData; CQuestCardDef::IsVignette; CQuestManager::ActiveQuestCards; CTCInventoryMap pmap_real / MapList / MapRegions.
  - CLOSED_STRING_NAME / LINE_STRING_NAME (Ego_r initializer names + retail initializers).
  - FableWin inline accessors: CWorld::GetGameCamera, CWorld::GetMessageEventManager, CTCQuestCard::IsVignette, CBasicString<char>::operator==, CThing::GetPos / GetPlayerNumber / IsDead.
- **Descriptive (no PDB name proven):**
  - Virtual slot names on NUISystem::CComponent / CList / CZoomingComponent (Pan / GetZoomCenter / GetZoom are PDB names, but their slot positions are not verified).
  - CThing padding and the ThingType byte at +0x10 (00845040).
  - CTCPhysicsBase slots other than 63 (GetFacingAngleXY).
  - CCreatureAction_GiveMoneyToThing enum values (casts only).
  - The UpdateMorphSet CRetailVector stand-ins (differ only).

## Names and evidence
- The anonymous-namespace hash in `?CLOSED_STRING_NAME@?A0x...` differs between our object (?A0xab7262e7) and retail. Bind the retail TU's hash at landing.
- 0x0143E90C uses the landed convention `extern CScriptDef* pGlobalScriptDef` (Ego_r `pGlobalScriptDef` is a CDefPointer<CScriptDef>; its first word).
- 0x00411570 is `CBasicString<char>::Compare` (FableWin `?Compare@?$CBasicString@D@@KIJPBD0@Z`, protected static fastcall). CCharString's equality is `CBasicString<char>::operator==` over `CCharStringData::Data` (Ego_r layout). That form also produces retail's separate length-mismatch `xor al,al` (005fd5e0 p05).

## Header debt
- engine/CTCBase.h, CWorld.h, CGameEvent.h, CAIGameCameraBase.h, CGameCameraManager.h, CPlayer.h, CPlayerManager.h, CTCPhysicsBase.h and CRegion.h are flattened (a `void* __vftable` member, `#pragma pack(1)`, templates as byte arrays). They cannot carry the virtual GlobalUpdate (UAE) or typed STLport/CCountedPointer members, so 0072a090 / 005fb140 / 005fd5e0 / 006276f0 keep local classes. vland reports these as SHARED_HEADER_AVAILABLE warnings.
- engine/CQuestManager.h / CTCQuestCard.h flatten ActiveQuestCards (list) and PDef (CDefPointer).
- There is no shared STLport `map` / `list` / `vector` header. 005fb140 / 005fd5e0 / 006276f0 carry the landed STLport 4.6 text inline (from 0x00613630 / 0x00618DA0 / 0x00618100 / 0x0059400C).
- 007280c0 still uses the invented CRetailVector morph-set stand-ins (differ only).
