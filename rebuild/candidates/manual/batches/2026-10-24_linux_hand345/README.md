# Hand chunk 345 (NUISystem::CSprite .. SetStaticCast<NUISystem): 36 of 40 byte-exact

## What matched (36)

- **NUISystem::CComponent (the 9 "CSprite vfunc_N" rows; vtable 0x0122F5D4 slots resolved to CComponent virtuals by slot + Ego_r.pdb name + retail layout = PDB - 0x30)**
  - 0x0052EBB0 ChangeZoom / 0x0052EC60 ChangeColour: twins of the landed ChangePosition 0x0052E9C0 (`ZoomTime = time; if (time > 0) {Target = z; Initial = current; Elapsed = 0;} else {all three = z}`); the PDB third parameter is `bool`.
  - 0x00530690 CanScroll: the largest-magnitude child amount; needs `(float)fabs(amount) > (float)fabs(result)` (the casts give retail's `fxch st(1) ; fxch st(1)` pair, a double compare drops it) and the landed HasChild iterator-object vector.
  - 0x00530740 IsPCStyle: `return LoadAndReturnDef()->PCStyle;` with the temporary CDefPointer's null-tested release; the CDefVirtual model needs a slot-0 filler so Destroy is `call [edx+4]`.
  - 0x00531B40 PositionChildDying: a real same-TU `__declspec(noinline) __fastcall` SGI `__find` body (unnamed input_iterator_tag parameter, under `#pragma auto_inline(off)`) homes the tag in the live value argument slot; the set erase is `Rebalance_for_erase` + free + `--count`.
  - 0x00531E50 AddActionIAmParamOf: inline STLport list push_back; the iterator needs a declared copy constructor and a `create_node` helper, else the edi push is shrink-wrapped.
  - 0x00531E90 SetDeletion: by-value CDeletion assigned to +0xd4; the callee destroys the argument (`lea ecx,[esp+8] ; call ~list`).
  - 0x00533BA0 AddChildFront: `Children.insert(Children.begin(), child)` (out-of-line insert 0x00535B10).
- **NUISystem::CText / CTextContainer / CTextSlider**
  - 0x0054F370 / 0x0054F410 SetValue(char / wide, lookUpTag, keepSize): tag from the main game component's text bank, else the new front end's; the bank must be read through an inline const getter (`GetGameTextDataBank()->GetTagFromCRC(text.GetCRC())`) to place its load between the inlined string reads and Calc; overloaded virtuals are laid out in reverse declaration order (slots 143/144/145).
  - 0x0054FFB0 SetTextWindow: plain box copy into +0x178. 0x0054FFF0 GetAlignment: inline `bool` accessors `(flags & mask) != 0` keep the `movzx eax,al` before the folded `shr ; and`.
  - 0x00546C90 CTextContainer::AddChild: only children whose GetType() is text get SetTextWindow(Window) and the base AddChild (the base call is inside the if).
  - 0x00548760 CTextSlider::OnActivate: StartObserving(0x19) on the observer base at +4, then CFrontEndManager::GetInstance()->AddObserver(this).
- **0x00720050 CCompositeBlendEntry(key, desc)**: key store + CCompositeBlendDesc copy ctor 0x005BA48A.
- **0x00546A50 NUISystem::CZoomingComponent::Update(float)** (vtable slot 1, Ego_r.pdb Update): base CChangingStateComponent::Update, then `Panning = false` when `(float)fabs(Target - Position) <= 1e-4f` on both axes (`test ah,41h ; jp` is `<=`).
- **0x00C4AFD0 PKCBufferedReader::ConvertOffsetToAbsolute(offset, origin)**: switch 0 = current (first block * block size + position), 1 = end (size), 2 = start; `absolute < 0` fails; then the absolute seek 0x00C4AA40.
- **0x00C31A70 PKSerialiseCParser::ValidateWriter()** (`// Flags: /EHsc`): the assertion macro (string built in a nested block, EH state 0, log sink, trace temporary) of the landed seek 0x00C4AA40, then `return m_pWriter`.
- **Plain constructors** (all `ret` / `ret 4` with an unread argument as the landed CTargetAndThing_0078e700(long unused)):
  - 0x0078E3A0 CTCDivineWrath::PerEffectInfo, 0x007939D0 CTCUnholyPower::PerEffectInfo (Ego_r.pdb nested structs {C3DVector Position; CIntelligentPointer<CThing> Effect;}; C3DVector's empty ctor emits nothing).
  - 0x009A7670 CBankInfo (Ego_r.pdb, four CWideString paths + RegisteredBank = 0), 0x009A9900 / 0x009AA210 CBankInfo-shaped address-suffixed twins.
  - 0x0050F090 / 0x005102F0 CTwoLayerPolyMap_<addr>: the two CLayer members (+0x16, +0x841E, ctor 0x0050EE40) of the packed map whose operator= is landed at 0x004FA860.
  - 0x0049CFE0 {CCharString, 0}, 0x005B1A7B {CWideString, 0} (`and dword ptr [esi+4],0` = `#pragma optimize("s", on)`), 0x00527DE0 {CCharString, CWideString}, 0x009F0900 {CCharString, CCharString}: descriptive, address-suffixed (no callers, no PDB-unique owner).
- **0x004294E2 std::_Rb_global<bool>::_M_decrement** (MATCH, `#pragma optimize("s", on)`): the verbatim STLport 4.6 body, size TU next to the landed _Rotate_left 0x0042951B.
- **SExpSpendUndo** (CTCInventoryExperience size TU, Ego_r.pdb names, 4-byte STLport lists): 0x0056E93D SaveBefore, 0x0056F597 SaveAfter (push_back 0x0042B687 out of line), 0x00579C35 copy ctor (memberwise), 0x0056F4BA RevertExperiencePoints (`#pragma optimize("y", off)` + `("s", on)`; back() must be STLport's `iterator tmp = end(); --tmp; return *tmp;` so the last node is fixed before each GetStatExperience call).
- **0x00BFD0B0 CPatchTesselationEdgeStrip::SaveVerts<CFanBaseVertex>**: the landed CEngineWaterBackgroundSubPatch::Save stream/compressor/CArray model; the vertex pointer must be a named local (`T* data = verts.Data;`) to land in edi.
- **0x0083FBC0 LHNetworkLib::LHHttpHeaders::Reset()**: `HeaderString = ""; HdrDocumentHeader.clear();` (assign 0x005693B0, vector<string>::erase(first, last) 0x00841FC0).

## Parked for Fable 5.1 (3)

- 0x005305a0 NUISystem::CComponent::ChangeColourDelta DIFFER(105v104): retail builds Colour + delta in place in the push-ecx frame (byte copy of B/A, R/G summed directly); ours copies the operator+ result once more (dword copy + shr extracts) | tried: ctor-sum (89v104), memberwise copy + operator+ (107), += / operator+ (105), copy ctor + += (107), B,G,R,A += order (111) | next: named local `CRGBColour colour(Colour); colour += delta; ChangeColour(colour, time, flags);` with a memberwise copy ctor
- 0x0041cb10 NUISystem::CYesNo::ProcessEvent DIFFER(67v70): ours `mov eax,[esi-4] ; lea ecx,[esi-4]` vs retail `lea ecx,[esi-4] ; mov eax,[ecx]` for ChangeState(0), and ours cross-jumps the two callback calls through esi (retail calls YesCallback through eax and falls into `cmp edi,5`) | tried: separate ifs, named callback locals, `event == X && cb` form with static_cast to the primary base | next: call ChangeState through an explicit `CComponent* self = static_cast<CComponent*>(this);` local and give the callbacks an inline `Call()` wrapper; else lane gap: cross-jumping
- 0x0084e810 NCameraModes::CBase::AddSubComponent DIFFER(192v176): the replace search / erase block now matches; ours re-reads the by-value CCountedPointer words from the stack (null test, push_back copy, destructor) where retail keeps Data in ebp from entry and Info in esi for the copy and the release | tried: `find != end()` / `end() != find`, `component->` vs `component.Data->`, named `found` local (fixed the search block) | next: a named `CSubComponentBase* sub = component.Data;` local for the test and GetType, and a copy-constructing push_back over `const CCountedPointer&` that reads both words once

## Skipped

- 0x00462fac NUISystem::CUILocaleGraphicsDef vfunc (row length 56): wrong bounds, two functions. 0x00462FAC is a 5-byte thunk `jmp 0x00462FB1`; 0x00462FB1 is a separate 51-byte body ending `ret 4` (a CUILocaleGraphicsDef::operator= shape: base operator= 0x00431ED0, two member operator= at +0x3c / +0x48, a dword copy at +0x54). 0x00462FB1 is absent from the manifest. Correct rows: 0x00462FAC len 5 (thunk) and 0x00462FB1 len 51 (as the landed CBrainDef_Copy 0x00462B10 + CBrainDef_operator 0x00462B15 thunk/body split).

## Label corrections

- 0x0052EBB0 / 0x0052EC60 / 0x005305A0 / 0x00530690 / 0x00530740 / 0x00531B40 / 0x00531E50 / 0x00531E90 / 0x00533BA0 "CSprite::vfunc_N" are NUISystem::CComponent::ChangeZoom / ChangeColour / ChangeColourDelta / CanScroll / IsPCStyle / PositionChildDying / AddActionIAmParamOf / SetDeletion / AddChildFront.
- 0x00720050 "CUIStateDef::vfunc_25" comes from a bogus vtable (the "vtable" bytes are UTF-16 text): it is CCompositeBlendEntry::CCompositeBlendEntry(unsigned long, const CCompositeBlendDesc&).
- 0x00546A50 "CZoomingComponent::vfunc_1" is CZoomingComponent::Update(float); 0x0041CB10 "CYesNo::vfunc_1" is CYesNo::ProcessEvent(EEvent).
- 0x004294E2 "RBTree::GetMaxNode" is std::_Rb_global<bool>::_M_decrement.
- 0x0049CFE0 / 0x005B1A7B "PerEffectInfo" are not PerEffectInfo (string + long records); 0x0078E3A0 / 0x007939D0 are CTCDivineWrath::PerEffectInfo / CTCUnholyPower::PerEffectInfo.
- 0x0050F090 / 0x005102F0 "QuestData" are the two-CLayer poly map constructors; 0x00527DE0 / 0x009F0900 "QuestData" are string-pair constructors (only 0x009F0900 has QuestData's layout, not its module).
- 0x009A7670 / 0x009A9900 / 0x009AA210 "SExpSpendUndo" are CBankInfo constructors.
- 0x0084E810 "ScriptEventList::InsertEvent" is NCameraModes::CBase::AddSubComponent (FableWin.pdb, as the landed SetUpPhysics callers bind it).
- 0x0083FBC0 "SetStaticCast<NUISystem::CComponent>" is LHNetworkLib::LHHttpHeaders::Reset.
- Callee labels: 0x00535800 is list<CComponent*>::operator= (labelled splice), 0x00534F20 SGI __find, 0x0042A34E _Rebalance_for_erase, 0x0042B687 list<long>::push_back (landed as "LinkedList_PushFront"; it inserts before the header), 0x00841FC0 vector<string>::erase(first, last) ("vector_move_and_cleanup"), 0x0084F010 __find_if with CIsOfType ("StdVector_LinearSearch"), 0x006D6510 CTCHeroExperience::GetStatExperience ("CTCInventoryAbilities::GetAbilityLevel"), 0x0050EE40 CLayer::CLayer ("CLayer::SetAllPolys").
