# Hand chunk 344 (NUISystem::CMenuEntry .. NUISystem::CSprite): 37 of 40 byte-exact

## What matched (37)
- **CChangingStateComponent / CComponent (catalogued as CSprite vfunc_*)**
  - 0041c5a0 ChangeState(K, float) records the state and parent update time. Overloaded virtuals take vtable slots in REVERSE declaration order, so declaring `ChangeState(K)` before `ChangeState(K, float)` puts (K, M) at slot 47 and (K) at slot 48.
  - 0041c5c0 HasState and 0052c8f0 ChangedState are plain member tests.
  - 0052ceb0 GetState, 0052cee0 GetState const and 0052cf10 GetCurrentState go through map find (0x42B402 non-const, 0x52D6B0 const). The body is written `if (it != end) return &it->second; return 0;`, with the found arm first.
  - 0052e750 SetZoom, 0052e860 SetPosition and 0052e990 SetRenderZoom snap target, initial and current values.
  - 0052e7a0 SetColour must copy the colour into a local first. Otherwise the reference is re-read after every store.
- **CObserver (catalogued as CSprite secondary slots 2 and 4)**
  - 0052d900 IsObservingEvent and 0052d940 IgnoreEvent use `std::find` over the set.
  - The tag temporary sits over the live `event` slot. That needs a same-TU `__declspec(noinline)` STLport `__find` with an unnamed `input_iterator_tag`, under `#pragma auto_inline(off)`.
  - IgnoreEvent needs a named iterator, with `end()` re-read for the test, and the inlined `erase` calls `_Rb_tree_rebalance_for_erase` 0x42A34E.
- **COk**: 0041cb9d ProcessEvent runs ChangeState(0) and then the OK callback on event 26 or 4. The 12-byte set must be modelled as a struct. The body sits unaligned at 0x41CB9D, so it needs `#pragma optimize("s", on)`.
- **CViewport (catalogued as CScrollingViewport vfunc_91)**: 0053fc90 ClearHandle uses the inline CEnginePrimitiveHandle::Clear model from the landed 0x00B1FFC0.
- **CSpellContainerList**: 00544110 Show and 00544190 HideSpellSelectionChild call GetColour (slot 14), set alpha to 0xff or 0, then call ChangeColour(c, 0.2f, false) (slot 38).
- **CSpellContainer**
  - 00545670 AcceptAssignments uses the counted-pointer `operator=`, which reads info before data.
  - 00545df0 AddChildAndSetOffsets is written as ONE statement, `GetStates()[0].Position.X = PositionOffset.X * (float)Children.size()`, so the float spills into the dead argument slot.
  - 00545e60 Initialise calls push_back, which calls `_M_insert_overflow` with a `__false_type()` temporary.
- **CScrollingComponent**: 00546fc0 Update is a switch on CurrentState, and 00547010 ChangeState chains 5->1 and 6->0.
- **CMesh**: 00550070 Update rotates by speed*dt about the axis. 00550be0 SetOrientation copies 6 dwords.
- **CMorphingSprite**: 00554150 Update calls UpdateMorph and the sprite update, trims the handle list to RenderCount-1, then zeroes the counters.
- **CScrollableList**: 005562a0 OnDeactivate calls HideList(0.2f), then IgnoreEvent(37) and IgnoreEvent(36) through the observer base.
- **CRedefinerList**: 00557080 Update runs the fade timer, then calls Refresh or ClearList.
- **CScrollBar**: 0055a5b0 OnHovered needs an inline `GetMouseCursor()` accessor so that the member loads straight into ecx.
- **CHoverable (catalogued as CMenuEntry)**
  - 0055bbf0 RemoveFromHoveredList finds and erases this component in the static list 0x13B8AD4.
  - 0055bd50 IgnoreEvent is the observer IgnoreEvent plus hover handling inside the found branch. It needs an inline primary-class helper, StopHovering(), so that `this` is converted once.
- **CMouseCursor**
  - 0055c580 Update publishes the position to GCursorPosition 0x13B8AD8.
  - 0055c810 TurnToHand and 0055c860 TurnToClosedHand read the def field +0x4c or +0x12c through a CDefPointer temporary.
- **CObservable**
  - 0055c9c0 RemoveObserver and 0055ca00 RemoveConcurrentExclusiveObserver use find, then IgnoreAllEvents, then erase.
  - 0055ca40 AddObserver and 0055ca90 AddConcurrentExclusiveObserver need a named iterator so that end() is re-read.
  - 0055cae0 IsExclusivelyObserved needs two sequential `if (...) return true;` and then `return false;`.

## Parked for Fable 5.1 (3)
- 0053dcd0 NUISystem::CMenuEntry::AddThingActionParam DIFFER(112v122): retail opens a `push ecx` frame word and stores a dead `mov dword ptr [esp+8],0` before the null test; it is never read | tried: plain list ctor, `_STLP_alloc_proxy` with by-value and const& node pointer, allocator with/without user ctors and rebind, `__stl_alloc_create`, /EHsc | next: the same dead store is in about 24 sibling lazy-list pushers (0x0053E180.., 0x005498E0.., 0x0055AA80..), so solve one and apply it to all; candidate is an unused EH-state or try-block temporary from a `new list` inside a ctor initialiser.
- 0053dd50 NUISystem::CMenuEntry::AddUIActionParam DIFFER(112v122): the same dead `push ecx` / `mov [esp+8],0` as 0053dcd0 (the +0x19c twin) | tried: the same forms as 0053dcd0 | next: the same as 0053dcd0.
- 0055cb10 NUISystem::CObservable::SetEvent DIFFER(204v195): the copied list's sentinel is loaded into eax and then copied to ebx, where retail loads ebx straight from the frame word; the second loop head also gets a 16-byte alignment pad | tried: copy-list local with an `end` local before each loop (204v195) and without one (196v195) | next: model the copy as the real STLport `list(const list&)` out-of-line 0x55CF50 with the iterator taken off `_M_node` through a reference local.

## Skipped
None.

## Label corrections
- The CSprite vfunc_11/13/15/29 rows are CComponent SetZoom/SetColour/SetPosition/SetRenderZoom.
- The CSprite vfunc_44/47/49/50/134/135 rows are CChangingStateComponent HasState/ChangeState(K,M)/ChangedState/GetCurrentState/GetState const/GetState. The code is inherited, not CSprite's own.
- CSprite secondary-vtable slots 2 and 4 (0052d900, 0052d940) are CObserver::IsObservingEvent and CObserver::IgnoreEvent.
- CMenuEntry vfunc_4 (0055bd50) is CHoverable::IgnoreEvent; CMenuEntry vfunc_133 (0055bbf0) is CHoverable::RemoveFromHoveredList.
- CScrollingViewport vfunc_91 (0053fc90) is CViewport::ClearHandle.
- 0x0042A34E, labelled "CMemoryAllocatorVariableSize::Erase", is STLport `_Rb_tree_rebalance_for_erase`.
- 0x0042D5B1, labelled "CPersistTraits::Transfer", is `map<unsigned long, CUIState>::operator[]`.
- 0x0042AC25, labelled "CActiveFile::OnReadFinished", is `~list`.
- 0x0055CF50 is the list copy constructor.
- 0x0042B402, labelled "SetMiniMapRegionExitTextOffsetX", is the non-const `map<unsigned long, CUIState>::find`.
- 0x0052DEC0 is STLport `__find` (input-iterator form).
- The landed 0x00556580 (CRedefinerList::OnDeactivate) comment calls CObserver slot 4 "SetState". It is IgnoreEvent; slot 3 is ObserveEvent.
