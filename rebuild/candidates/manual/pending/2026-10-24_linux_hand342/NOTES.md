# Hand chunk 342 (NParticleEngine::CPSCUpdateNormal .. NUISystem::CEditBox): 38 of 40 byte-exact

All 38 sources grade MATCH (4) or RELOCATION_MATCH (34) under the RTM cl 13.10.3077 lane (`grades.txt`). Every behaviour
test prints `<addr>_TEST PASS` under g++ (`tall.sh`) and VC7.1 (`tall.py`, `tall_vc.txt`).

## What matched (38)

- **NParticleEngine**
  - 0x00B0F8E0 CPSCUpdateNormal::GetParticleLifeSecs(float&). A nested float ternary assigned through the reference
    gives retail's per-arm `fld ; fstp` with a tail-duplicated `mov al,1 ; ret`. if/return arms give integer moves.
  - 0x00AF15B0 CParticleDataBank::Close(). Erases the emitter-info vector, then calls CBankFile::Close. It is a port of
    the landed CLipSyncDataBank::Close 0x00A42DA0 (move, destroy range, finish = kept).
- **NPlayerGui**
  - 0x00643FC8 CDrawAbilityButtons::GetSpecialMoveIndexForButton. Putting `GetMainPlayer()->PlayerInterface` into a named
    local orders the call before the argument push.
  - 0x00644FFC CDrawAbilityButtons::ClearPrimitivesIfNecessary. Two inline handle Clear() bodies, then
    `for_each(mem_fun_ref(&CEnginePrimitiveHandle::Clear))`.
  - 0x00644660 CDrawMiniMap::ClearPrimitives and 0x00644FE5 CQuestInfoElement::Clear. A `for_each` over the handle
    vector, under `#pragma optimize("s", on)` (the /Os module with no frame pointer).
  - 0x00644D0E CDrawQuestInfo::ClearPrimitivesIfNecessary. A list walk calling element->Clear(), a for_each, then the
    base call.
  - 0x00657550 CDrawQuickAccessIconsPC::ClearPrimitivesIfNecessary. Declare `Clear` only: an inline body is emitted
    and becomes the largest symbol.
- **CCodeSectionManager** 0x009F0500 GetHandleFromLongName(CCharString). A map `find` whose end() is read after the call.
- **NResourceDirectoryNames**
  - 0x0041A430 GetMyDocumentsDir. Needs `/GS` and dllimport SHGetFolderPathW.
  - 0x0041A540 GetSaveDir. Same-TU GetUserDataDir / GetSaveDirWithoutTrailingBlackslash helpers reproduce the early
    temporary destruction.
- **NScreenEffect::CScreenEffectPriorityStack**
  - The three deleting destructors 0x00882720 / 0x008826F0 / 0x00882750. A user dtor `{ TheStack = 0; }` plus a `new`
    factory. VC drops the dead derived vtable store.
  - ResetToDefault 0x008809F0 / 0x00880A10 / 0x00880EC0. Use component stores through a member reference, chained `=`
    (it stores right to left) and inline `Initialise` forms; temporaries build frame copies.
  - 0x00880E00 CScreenFilterProcess::Process. The `Interpolate` result assigned back to the current parameters.
- **Audio / reader** (manifest labels were wrong, see below)
  - 0x00C69FB0 reverb ProcessCombs. A memset, then four comb Blend/Process calls with a wrapping input index.
  - 0x00C3A9E0 reader Read. Clamp, ReadBlock, and Reset on failure.
- **NShape::CTrack** 0x00761740 IsPosInShape. `if (f()) return d < 0.25f; else return false;` keeps the separate
  `xor al,al` exit.
- **NStreamingFontBankGlobals** 0x0122A610 `_$E2`. A global array of glyph records whose inline counted-pointer dtors
  give the atexit thunk's reverse loop.
- **NTCScriptedControl** (CActionBase {vptr, PTCScript +4, Activated +8, Terminated +9})
  - 0x00712240 CActionWait::Update. Pre-increment the counter, then `NoFramesToWait > 0 && counter > NoFramesToWait`.
  - 0x00713660 CActionStopScripting::Update. `if (PTCScript->GetNoActions() <= 1) PTCScript->ClearActions(); else
    Terminated = true;`. The signed `long` count gives `jg`. An inline CTCScriptedControl::ClearActions() member, the
    InterruptControl front-action/clear idiom without the quest call, keeps the script in edi and forms `lea esi` late.
  - 0x00713CD0 CActionWaitForThingToFinishPerformingTasks::Update. Refresh, IsAlive, the const GetInterface 0x1F
    (CTCScriptedControl), and Locked / queue-empty checks.
  - 0x00714CC0 CActionTurnToFacePosition::Update. GetInterface 0x43 (CTCLook): LookAtPos(&Position, 0x21), then
    IsPosInViewRange.
  - 0x00714D30 CActionTalkBase::OnAdd (slot 3). Read the parent thing into a local before the GetThing() compare. An
    inline `CCountedPointer::Get()` (not `.Data`) keeps the conversation pointer in esi for the CActionTalkedTo ctor.
    The inline 8-byte CScriptConversation ctor (CBaseObject ctor + vtable 0x01265520) goes through Reset.
  - 0x00714F60 CActionOverheadTalkToThing::DisplayText (slot 6). `ScrollText = GetCurrentText().GetLength() >= 25` is
    stored (`setge`) before the temporary dies. Short lines go to the CTCEmoteIcon (0x45).
  - 0x007152F0 CActionFireProjectileWeapon::Update. STLport `pop_front()` = `erase(begin())` into
    `erase(const_iterator)`: the iterator is converted in its push slot, and the CurrentActionID store stays before the
    push.
  - 0x007173D0 CActionTalkToThing::Update. Profile IsSubtitlingEnabled() ? !DisplayingText : a type-0x11 message
    event. Put the message manager in a named local so the filter's 0x11 store lands after the push.
  - 0x00717920 CActionTalkToThing::OnActivate (slot 2). `AddedDuringCutscene && g_pPlayerGui CutsceneSkipped (+0xF7)`,
    else the base call as a tail `jmp`.
- **NThingShot** 0x007BF350 CTCShotDefinitionBase::OnRemove (not CTCShotDefinitionThrow). Calls
  CTCObjectAugmentations::StopTrailEffect, then the landed IS_THING_ALIVE(PEffect) idiom and `Kill(true)`.
- **QuadricOptimiserInternals** 0x00AC7A90 CVertexPairContraction::ContainsAllVertices(const CIndexList&). Retail never
  reloads ecx for HasVertex (LTCG). The real HasVertex body sits in the same TU under `auto_inline(off)`, and VC7.1 then
  keeps ecx too. A static `_M_increment(node)` returning the next node fixes the eax/esi pairing of the tree walk.
- **NUISystem**
  - 0x00533B30 CComponent::GetCurrentState = `&States[0]` (the map `operator[]`; the literal key is a frame temporary).
  - 0x00558D90 CDraggableInto::ProcessEvent. `this` = the CObserver subobject at +4. ExecuteActions is primary slot
    131 through this-4.
  - 0x00540180 CEditBox::SetText. LeftMost = 0; Text = text; CursorPos = text.GetLength().
  - 0x00540D70 / 0x00540DA0 CEditBox::AddActionOnEnter / AddActionOnEscape. `list.push_back(action)`. The iterator
    needs a user copy ctor so end() is built in its push slot.

## Parked for Fable 5.1 (1)

- 00c69a70 CReverbProcessor_00c6a460::AllocateMemory DIFFER(240v236): retail emits `mov ecx,ebx ; shl ecx,2` before the delay-line length/position stores and re-reads the buffer after them; ours emits `lea ecx,[ebx*4]` after an early buffer re-read | tried: the base form, `m_length*sizeof(float)` inside Initialise, a precomputed byte-count local, a Clear() helper with `<<2` (worse, 198v236) | next: a signed length type, or a byte count derived from a pointer difference

## Skipped

- 00644263 NPlayerGui::CDrawMoney "vfunc_0" (catalog length 75). The row holds THREE 25-byte (0x19) functions with no
  padding between them:
  - 0x00644263 CDrawMoney::ClearPrimitivesIfNecessary (vtable 0x0125A544 slot 0)
  - 0x0064427C CDrawRenown::ClearPrimitivesIfNecessary (vtable 0x0125A550 slot 0)
  - 0x00644295 CDrawMorality::ClearPrimitivesIfNecessary (vtable 0x0125A55C slot 0)

  Each one is `cmp byte [ecx+18h],0 ; jne ; mov eax,[ecx+4] ; mov ecx,[eax+2CCh/2D4h/2DCh] ; test ; je ; jmp [vptr+4]`,
  ending in its own `ret`. The correct length for 0x00644263 is 25 bytes. The other two need their own catalog rows.

## Label corrections

| Address | Manifest label | Correct identity and evidence |
|---|---|---|
| 0x009F0500 | NQuestManager::GetAnimationEntry | CCodeSectionManager::GetHandleFromLongName(CCharString): the LongNameToHandle map at +0x10, as the landed RegisterSection. |
| 0x00C69A70, 0x00C69FB0 | NScript::CQ_GuildTrainingSkillScript | The audio reverb processor (0x00C6xxxx DSP module; set-up caller 0x00C6A460): AllocateMemory and ProcessCombs. |
| 0x00C3A9E0 | NScript::CV_MurderTwistScript::Read | A stream reader's Read(void*, unsigned long): ReadBlock 0x00C4B890, Reset 0x00C3A9C0. |
| 0x008809F0 / 0x00880A10 / 0x00880EC0 | vfunc_1 | ResetToDefault. Slot 3 (0x00880E00) is Process; slot 0 is the `??_G` deleting destructor. |
| NPlayerGui vfuncs | vfunc_* | Slot 0 = ClearPrimitivesIfNecessary; CDrawMiniMap slot 1 = ClearPrimitives; CQuestInfoElement slot 2 = Clear; 0x00643FC8 slot 3 = GetSpecialMoveIndexForButton (it reads ControllerButton at retail +0x40; PDB +0x44). |
| NTCScriptedControl vfuncs | vfunc_* | Slot 1 = Update, 2 = OnActivate, 3 = OnAdd, 4 = GetActionName, 5 = OnCleanup, 6 = DisplayText. So 0x00717920 = CActionTalkToThing::OnActivate, 0x00714D30 = CActionTalkBase::OnAdd, 0x00714F60 = CActionOverheadTalkToThing::DisplayText. |
| 0x00719280 | remove_if<...CConvFilterToCP...> | The action list's clear() (byte twin; already noted by the landed 0x00713850). |
| 0x007BF350 | NThingShot::CTCShotDefinitionThrow::vfunc_2 | NThingShot::CTCShotDefinitionBase::OnRemove: CTCBase slot 2 = OnRemove, and the body is slot 2 of the base vtable 0x0126E1A4 and of every derived shot vtable. |
| 0x007664E0 | CTCKraken::EndHeadEffects | CTCObjectAugmentations::StopTrailEffect (byte twin): it kills ProjectileTrailEffect at +0x65. |
| 0x00AC7A90 | NUISystem::CList_SetDefaultChildTreeWalk (forge autoname) | QuadricOptimiserInternals::CVertexPairContraction::ContainsAllVertices(const CIndexList&). |
| 0x00AC7990 | CList::SetDefaultChild | CVertexPairContraction::HasVertex(unsigned long). |
| 0x00533B30 | CComponent vfunc_50 | NUISystem::CComponent::GetCurrentState. |
| 0x0042D5B1 | CPersistTraits::Transfer | map<unsigned long, CUIState>::operator[]. |
| 0x00558D90 | CDraggableInto vfunc_1 | NUISystem::CDraggableInto::ProcessEvent: CObserver-subobject vtable 0x0124B2B0 slot 1. |
| 0x00540180 / 0x00540D70 / 0x00540DA0 | CEditBox vfunc_143 / 148 / 149 | SetText, AddActionOnEnter, AddActionOnEscape. |
| 0x005355C0 | _List_val<CCountedPointer<CSpawnedFuncBase>>::_Buynode | Used here as list<CCountedPointer<CAction>>::insert(iterator, const T&). |
| 0x00692620 | CWorld::DrawGetEnvironment | CCreatureActionBase::GetActionID (`mov eax,[ecx+28h]`). |
| 0x0099A380 | CBaseIntelligentPointer ctor | Also the CBaseObject ctor (byte twin). |
| 0x0041A4C0 (landed) | CUserProfileManager::GetAutoSaveCheckPointFileName | Really NResourceDirectoryNames::GetUserDataDir. It also binds 0x0041A430 as "CUserProfileManager::GetDocumentsFolder", which is NResourceDirectoryNames::GetMyDocumentsDir. |
