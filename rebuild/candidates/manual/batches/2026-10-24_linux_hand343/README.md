# Hand chunk 343 (NUISystem::CFrontEndButton .. NUISystem::CMenuEntry): 37 of 40 byte-exact

Every manifest name in this chunk was a `vfunc_N` placeholder. The real names come from the Ego_r.pdb source order
(`ghidra_out/egor_pdb_names.tsv`), lined up against the retail TU order and the vtable slots (dumped from the exe). All
37 graded MATCH/RELOCATION_MATCH under RTM cl 13.10.3077 (`grades.txt`), and every test passes under g++ and VC7.1 (`tall.sh`, `tall.py`).

## What matched (37)

- **NUISystem::CFrontEndButton** (5): Initialise 0x54DB50, ProcessEvent 0x54DBC0, OnHovered 0x54DC30, OnUnhovered 0x54DCC0,
  OnObserverRemove 0x54DE70. These share one shape: `if (LoadAndReturnDef()->PCStyle) ...`. The slot-108 CDefPointer comes back
  through a hidden pointer and is released inline. CUIDef has its refcount in a second base (`add eax,4 ; dec`) and PCStyle at +0x221.
  ProcessEvent sits in the CObserver vtable (0x01249530 slot 1), so its `this` is the +4 subobject and the members show up at -4.
  This falls out of declaring the override over a `CComponent, CObserver` base pair. The debounce is
  `if (Time - Last < Delay) return;` (`test ah,5 ; jnp`). Hovered/unhovered call ObserveEvent/IgnoreEvent through this+4: one `add esi,4`, then reused.
- **NUISystem::CMenuEntry** (8): AddChild 0x53D150 / AddChildDynamically 0x53D170 (child->GetType() with the result unused,
  then the base call), Initialise 0x53D190, OnHovered 0x53D540, OnLeftUnclicked 0x53D600 (GoForward + ExecuteActions),
  OnRightClicked 0x53D660 (GoBackwards + ExecuteActions), OnObserverRemove 0x53D6C0, OnUnhovered 0x53D970.
  - OnHovered (0x53D540): a redefiner-list parent is scrolled to the entry. The downcast goes into a named local with its own null test, and Parent is re-read.
  - OnUnhovered (0x53D970): the hovered list at 0x13B8AD4 is a head-node list with no size member. `find` + guarded `erase` + `free` are inlined.
- **NUISystem::CKeyRedefiner** (5): OnHovered 0x557860, OnUnhovered 0x557880, OnLeftUnclicked 0x557AF0, ChangeState 0x557C10
  (embedded jump table), SetAction 0x5586A0.
  - OnUnhovered calls CClickable::**OnHovered** 0x55AEB0. This is a retail bug, reproduced as is.
  - OnLeftUnclicked: the tag CCharString and the yellow CRGBColour sit in two sibling blocks so that they share one frame slot.
    The text has two SetText overloads, and MSVC puts overloaded virtuals in the vtable in reverse declaration order.
  - SetAction: STLport `list::clear` plus `insert(end(), x)` (pos taken first; prev bound to a local). The value goes through a
    null-tested placement new (`throw()` operator new). The 28-byte definition is a POD copy (`rep movsd`).
- **NUISystem::CClickable** (1): OnRightClicked 0x55AFD0, inherited into the CKeyRedefiner vtable slot 148. The definition
  temporary outlives the ChangeState call (end of the full expression).
- **NUISystem::CList** (7): ScrollUp(bool,bool) 0x53605F / ScrollDown(bool,bool) 0x536088, SetDefaultChild(CComponent*) 0x5362AD,
  GetPositionOffset 0x53CDCA, SetNextScrollingChildPosition 0x53CDFF, SetActionOnScrollUp/Down 0x53CE97/0x53CED4.
  - The TU is `optimize("s")`.
  - The 5-arg/2-arg ScrollUp and the SetDefaultChild overloads use the same reverse-order vtable rule.
  - GetPositionOffset needs a user C2DVector copy ctor: X is copied through the FPU and Y through a register.
  - The SetAction* setters need a `void` CCountedPointer `operator=` that forwards to the out-of-line ShareData 0x53CEAE. A
    `T& operator=` keeps `mov eax,esi`.
- **NUISystem::CListArrow** (1): Update 0x54C2A0. The parent list's selection is checked against [MinToDraw, MaxToDraw] (two
  GetSelectedChild calls). The constant 1 is shared between the push and the byte store.
- **NUISystem::CFrontEndList** (1): Update 0x54C430 (6→0, 5→3 after a state change).
- **NUISystem::CFrontEndScreen** (2):
  - ProcessEvent 0x54E280 sits in the CObserver vtable slot 1. It has a 0.2 s debounce and a byte-indexed jump table, with the
    cases written in layout order: 0x1b, then 0x21/0x22, then 0x1a.
  - AddActionOnLeftClicked 0x54E4F0 (slot 71) is the CCountedPointer operator= of the landed 0x992470 family, with the member at +0x160.
- **NUISystem::CManager** (2): ClearBastardChild 0x41DED2 (STLport map walk, ReleaseData on each value and on BastardChild,
  tail-jump `clear`, size TU), SetRightStickForce 0x42D592 (+0x94).
- **NUISystem::CIMEFont** (2): SetPosition 0x404950, GetTextExtent 0x404F60. The latter reuses the landed DrawTextW model: a
  hidden-pointer counted font pointer whose inline release zeroes Data/Info, and `__ftol2` for each dimension.
- **NUISystem::CIconText** (3): SetValue(const CCharString&) 0x541E60 and SetValue(const CWideString&) 0x541F20 (game bank first,
  then the system bank; the result is built in the dead argument slot; overload slots 145/144), SetTextWindow 0x543AE0.

## Parked for Fable 5.1 (3)
- 005361d3 NUISystem::CList::SetNonScrollingChildrenStates DIFFER(221v218): entry test `sar ecx,2 ; je` (retail) vs `test ecx,0xfffffffc ; je` (lane); everything else identical | tried: ternary argument, if/else calls, `size() >= 1`, `(long)size() > 0` | next: lane gap: peephole on a `(last-first)/4 != 0` test (retail keeps the shift's flags)
- 00404d20 NUISystem::CIMEFont::SetColor DIFFER(43v47): retail copies `colour>>8` into ebx (push/pop ebx) and loads the alpha byte between the G and R stores; ours shifts eax in place | tried: 4-arg CRGBColour ctor temporary, member stores G,R,B,A, a chained `high >>= 8` local, two locals green/red | next: a COLORREF-style GetGValue/GetBValue macro form or the alpha assigned before R; else lane gap: register allocation
- 00543b10 NUISystem::CIconText::GetTextAlignment DIFFER(23v26): retail `movzx eax,al` before `shr eax,4 ; and eax,2`; lane omits the zero-extension | tried: unsigned char bitfield ternary, plain mask `(f >> 4) & 2`, unsigned long bitfields at +0x12c, `RightAligned * 2` | next: lane gap: dead zero-extension (the name is descriptive; no PDB name for slot 151)

## Skipped
None. Every row is exactly one function: the two rows with trailing jump tables (0x54E280, 0x557C10) include the table in
their length and graded MATCH.

## Label corrections
- All 40 `vfunc_N` rows have been renamed from Ego_r.pdb (see STATUS.tsv).
- 0x0055AFD0 "CKeyRedefiner::vfunc_148" is **NUISystem::CClickable::OnRightClicked**. It sits in the CClickable TU (0x55AC90..0x55AFD0, in Ego_r order Initialise, Update, OnLeftUnclicked, OnRightUnclicked, ProcessEvent, OnHovered, OnUnhovered, OnDeactivate, OnLeftClicked, OnRightClicked).
- 0x0055AEF0 "CClickable::OnHovered" (duplicate label) is CClickable::OnUnhovered (it makes IgnoreEvent calls).
- 0x0041E5F2 "GetInstance → CFrontEndManager*" is **NUISystem::CManager::GetInstance**. It caches 0x13B8710 and allocates 0xD0 bytes,
  and the object has the CObservable/CManager vtable 0x01230134: slot 2 AddObserver, 3 AddExclusiveObserver, 4 RemoveExclusiveObserver, 13 GetKeyPressed.
  0x00595582 is the real CFrontEndManager::GetInstance.
- 0x00404A80 "AddChildPrimitive" is CIME::GetInstance (it returns the static 0x13B7CD8). 0x009E2C80 "CMessageEventFilter...::operator()" is the font bank's GetFont.
- 0x0099B0F0 "CCharString::operator==" is CWideString::operator!= (used on CWideStrings, true when different). 0x0099EAE0 "_Cons_val<...>" is ~CCharString.
- 0x004291DE "_Dest_val<...COptimisedPrimitive>" is the CCountedPointer<T>::ReleaseData body. 0x0042C02F "CTCCarryable::OnKill" is the STLport map `clear`. 0x00533B50 "Container_PushElement" is the base CComponent::AddChild.
- 0x00558170 "GetMovementActionText" is CKeyRedefiner::GetActionText(CWideString&, const CCharString&) const.
