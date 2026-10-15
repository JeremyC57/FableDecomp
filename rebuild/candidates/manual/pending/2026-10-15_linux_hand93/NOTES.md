# Hand chunk 93 (CTCInventoryBase .. CTCInventoryExperience): 39 of 40 byte-exact

All 39 src/ rows graded MATCH/RELOCATION_MATCH (gall.sh -> grades.txt) and every behaviour test prints PASS under g++ (tall.sh)
and the lane's VC7.1 cl.exe (tall.py). Retail CTCInventoryBase has no engine header: every CTCInventoryBase row uses a local
view with the Ego_r.pdb names at the retail offsets (PDB offset minus 4 bytes per preceding std::vector: Components, Categories,
MenuDisplayListItem, MenuDisplayListCategory, ConfiscatedItems), i.e. InventorySelectionState +0x1C, Categories +0x20 (0x2C-byte
CCategory), InventoryOpen +0x58, SelectedCategoryIndex +0x70, Dying +0x88, CategoryMoving +0x89, CategoryMovement +0x8C,
ConfiscatedItems +0xAC (0x14-byte CItem), pBuildingThing +0xB8, X/Z rotation gains +0x114/+0x11C. The CTCInventoryClothing rows
use a local view too (the header's PDB offsets are 0x18..0x24 high): ClothingMenuEntries +0x134, MorphPrimitiveHandles +0x14C,
PMannequin +0x158, UseMannequin +0x15C, LoadedFromSaveGame +0x15D.

## What matched (39)
- CTCInventoryBase (24):
  - 005bcea4 UpdateInventoryAnimXAxis / 005bcef8 UpdateInventoryAnimZAxis -- ease the X/Z rotation gain toward the right stick
    while its length exceeds 0.01. Lesson: `sqrtf(a*a + b*b)` multiplies `b*b` FIRST (the right operand of `+` is evaluated
    first), so retail's `y*y` then `x*x` is the source `x*x + y*y`; `gain += (x - gain) * 0.1f` gives the `lea eax,[esi+N]`
    triple use; frame pointer = optimize("y",off)+("s",on).
  - 005bd1eb OnInitialActivate, 005bf8cb InitialiseInventoryWithItems, 005bf91a ReturnAllConfiscatedItems -- size() re-read index
    loops (`push 2Ch/14h ; pop reg ; idiv`, unsigned `jb`). Lesson (0x005BF91A): a trailing `erase(begin(), end())` whose bounds
    are read through the vector register (`push [esi+4] ; mov ecx,esi ; push [esi]`) is an inline vector member (`clear()`);
    written at the call site the lane re-addresses `[edi+0B0h]` from `this`.
  - 005bdb68 GetCategoryNameWithIndex -- `name = Categories[i].GetCategoryName()`: the by-value CWideString temporary lands in the
    dead index slot; `Categories.first` must be read into a NAMED local to be loaded before the hidden-pointer push.
  - 005bdc77 GetInventoryGuildValidity -- out-pointer local zeroed first, HasTC filter TEMPORARY built in the call (its store
    lands after the pushes), two-exit bool.
  - 005bdcb4 HighlightNextItem / 005bdcdc HighlightPreviousItem, 005bdd04 SelectHighlightedItem, 005bdd39 GlobalUpdate,
    005bf3f1 GetItemOfTypeInInventory -- plain guards; `fld 1.0 ; fld m ; fucompp ; test ah,44h ; jp` is `m == 1.0f`;
    `idx >= 0 && (unsigned long)idx < size()` gives `jl` + `jae`.
  - 005bdd8c ProcessInventoryInputUp / 005bddb8 ProcessInventoryInputDown -- `sub eax,0 ; je ; dec eax ; jne` = two-case switch,
    case 1 laid out first, both arms end in vtable tail jumps (slots 28/29).
  - 005be8d3 GetBoughtPrice, 005bf3c6 RemoveItemFromInventory -- find-category forwarders. Lesson: a chained
    `Find(...)->Method(arg)` pre-pushes `arg` before evaluating Find's arguments; retail's sibling `jmp` needs the found pointer
    in a named local (`CCategory* c = Find(...); return c->GetBoughtPrice(t);`), and RemoveItem's pass-through pushes between
    the two calls need the category index in a named local computed first.
  - 005bf482 GetAllItems, 005bfa44 GetAllNonEmptyInventoryItems, 005bfa69 GetAllInventoryItemsOfType -- rotated pointer walks.
  - 005bfa8e ConfiscateAllItems, 005bfac6 ConfiscateAllItemsOfType, 005bfc90 GetBestFishWeight -- 12-byte vector local with an
    inline zeroing constructor and out-of-line destructor (no EH in this module); the float result is a named local homed in
    the dead argument slot.
  - 0061a040 GetSelectedCategoryThing -- GetCategoryWithIndex(GetSelectedCategoryIndex())->GetAssociatedThing() (all out of line).
  - 004e7d4d GetEmptyItemText (base slot 76) -- `return g_EmptyWideString;` copy-constructed into the hidden result (needs "s":
    the speed form keeps the hidden pointer in esi).
- CTCInventoryClothing (7) + CTCInventoryWeapons (1):
  - 005b34d5 / 005c2e2a ProcessButtonAReleased (slot 58) -- `bool selected = false; SelectHighlightedItem(selected);` frame byte.
  - 005b369a OnSerialise -- the "InGameSave" save arm transfers the bool PARAMETER by reference (`lea eax,[ebp+0Ch]`) with the
    `false` default homed in the dead context slot's high byte; the load arm uses the 2-argument out-of-line Transfer overload.
  - 005b40e6 RemoveAllClothing -- implicit CCharString conversion temporary of a const-ref argument, index in a named local.
  - 005b4771 AddClothingMenuEntry, 005b4a6e OnItemDeselected, 005b4e7c OpenInventory, 005b6a2d CloseInventory -- straight
    member sequences; the UseMannequin clear written before `if (PMannequin)` schedules between the test and the branch.
- Misattributed rows (7; catalog labels propagated, real identities below):
  - 008081b0 / 00927450 SGI `__pop_heap` for CIntelligentPointer<CThing> / <const CThing> -- /Gr `__fastcall`, speed TU, the
    element copy built in the argument area by a `__forceinline` user copy constructor (base ctor, vtable store, SetPItem(GetPItem())),
    `*result = *first` through a `__forceinline` operator=, the by-value `value` destroyed by this callee.
  - 009e2560 AppendLongToString -- `__fastcall (void*, CCharString&, const long&)`: `text += GFIntToCharString(value)`.
  - 00c21e20 FindInChildThenSelf -- `p = m_child.Find(key); if (!p) p = Find(key); return p;` (slot 17, key by reference).
  - 00c28610 CHandleBasedMemoryPool::FreeHandle -- both stores to the slot survive because the slot is a UNION (header pointer /
    free link) written through one reference.
  - 00c2c7b0 CRegistryLayer::Add -- parent->Add(item, a, b); m_items.Add(item); return item.
  - 004c6140 time-multiplier controller RemoveRequest -- map erase by key reference, `if (size() == 0) flag = true`, recompute.

## Parked for Fable 5.1 (1)
- 005bd5db CTCInventoryBase::GetCategoryWithAssociationThing DIFFER(84v85): retail keeps the stride offset in ebx and spills the count to [ebp-4] (`lea ecx,[ebx+eax]`, `cmp edi,[ebp-4]`), the lane keeps the count in ebx and spills the offset (`add ecx,[ebp-4]`, `add dword ptr [ebp-4],2Ch`) | tried: for+break with post `if (i < count)`, while loop with reversed declaration order, `&&` loop condition with `first + i`, outer `if (count > 0)` | next: lane gap: register allocation (callee-saved choice between the compiler's strength-reduced offset and a user local); maybe the original wrote an inline `GetNumberOfCategories()`/index helper -- try one form with the count behind an inline accessor taking it by reference

## Skipped
- none (no row spans two functions; the int3 tails of 00927450, 009e2560, 00c21e20, 00c28610, 00c2c7b0, 004c6140 are padding).

## Label corrections
- 005bcef8 "UpdateInventoryAnimXAxis" -> CTCInventoryBase::UpdateInventoryAnimZAxis (writes +0x11C from the stick's Y).
- 005bdcdc "HighlightNextItem" -> CTCInventoryBase::HighlightPreviousItem (tail-calls CCategory::HighlightPreviousItem 0x005BD0F7).
- 005bdd8c "ProcessInventoryInputDown" -> slot 38 "up" handler (SelectPreviousCategory / HighlightPreviousItem); named ProcessInventoryInputUp.
- 005be8d3 "GetItemDescription" -> CTCInventoryBase::GetBoughtPrice(long) (ends in CCategory::GetBoughtPrice).
- 005bf3c6 "GetItemDescription" -> CTCInventoryBase::RemoveItemFromInventory(long, long) (vtable slot 79, +0x13C).
- 005bfa44 "GetAllInventoryItemsOfType" -> GetAllNonEmptyInventoryItems(vector<CItem>&, bool) (forwards to CCategory 0x005BF97D).
- 0061a040 "GetItemDescription" -> CTCInventoryBase::GetSelectedCategoryThing (unreferenced).
- 008081b0 / 00927450 "CTCInventoryBase::GetItemDescription" -> SGI __pop_heap<CIntelligentPointer<CThing>*, ..., CDoNotUseBinaryPredicate>
  and <CIntelligentPointer<const CThing>*, ..., bool(__fastcall*)(...)>.
- 009e2560 "CTCInventoryBase::GetItemDescription" -> a __fastcall `text += GFIntToCharString(value)` helper (71 callers).
- 00c21e20 "CTCInventoryBase::ProcessButtonAReleased" -> a child-then-self virtual lookup in the 0x00C1 module.
- 00c28610 "CTCInventoryBase::GetCategoryWithIndex" -> CHandleBasedMemoryPool free-handle push (only caller FreeAllocation 0x00C28690).
- 00c2c7b0 "CTCInventoryClothing::AddClothing" -> a layered registration Add (caller 0x00C383D0 is the next layer up).
- 004c6140 "CTCInventoryExperience::ProcessButtonBReleased" -> the bullet-time / time-multiplier controller's RemoveRequest.
- 004e7d4d "CTCInventoryClothing::vfunc_76" -> CTCInventoryBase's own slot-76 default (present in ten inventory vtables).
- 005c2e2a "CTCInventoryClothing::ProcessButtonAReleased" -> CTCInventoryWeapons::ProcessButtonAReleased (slot 58 of 0x01253FF4).
- Callee labels: 0x004AEF50 is CPersistContext::IsSaving (already landed as such); 0x005B432C ("Unwind@00fbdab0") is the
  ClothingMenuEntries clear; 0x004E4108 ("CTCCarryable::OnKill") is a 12-byte map clear; 0x005C08C0/0x005C08D3 ("_Umove") are SGI
  find_if over Categories by GlobalDefIndex; 0x005D97B0 is GetInventoryCategory(long) -> category def index.
