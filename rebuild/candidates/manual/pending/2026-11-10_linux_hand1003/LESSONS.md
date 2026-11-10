# chunk 1003 lessons (CTCInventoryClothing)

- A key copied into a temp, stored right before the call (`mov [ebp-14h],ebx` just before `call map::operator[]`, 0x005B69D3), with the
  component staying in ebx = a NAMED `CComponent*` local passed as the key plus STLport's operator[] body visible
  (`FABLE_RETAIL_STLP_MAP`; still called out of line, 0x0042D1B7). A `CText*` key converted at the call makes a temp too, but the
  allocator ranks the text above the iterator (edi/ebx swap); a `*it` key passes the list node's address (RefreshDescriptions 0x005B690B).
- c2prio.py is a compile, not a grade: use it to try register-allocation variants (who gets edi vs ebx) and only grade the winner
  (0x005B690B: three probes, the fourth grade matched).
- q.py copies the source into its own work dir: a relative `#include "x.inc"` is not found and cl under Wine SPINS (no SRC_FAIL) until the
  timeout. Inline every prelude before grading (0x005B48A2 lost one grade to this).
- `push 1` in one path and a register in the other for the same callee argument = the call is written in both if/else arms
  (ConstructClothingListPC's menu-entry Create, 0x005B6A66; ConstructScreen's ConstructClothingList(.., true/false), 0x005B76D1).
- A result homed differently on the empty-input path = `if (items.empty()) return 0;` written early, not a null-initialised result
  (ConstructClothingListPC 0x005B6A66).
- Count loops over `partLists[i].size()` must index the outer array each time; a reference local strength-reduces to a count-down loop
  (ConstructScreen 0x005B76D1, 4198v4210 -> match).
- Argument evaluation order / register choice around a CCharString temporary passed by const& (`CThingObject::Create(.., CCharString(""))`):
  build it as a named local in its own `{ }` block around the call, as landed 0x00846410 (OnInitialActivate 0x005B3AEF, 427v422 -> match).
- A retail bitfield test-and-set on the created thing (`test al,1 ; jne ; or al,2 ; mov [ecx],al`, 0x005B3C2F) = `if (!thing->IsDead())
  thing->SetAsCreatedByOwnerThingCreation(true);` with both inline.
- Same-TU `GetSubDef<T>` / `GetDef<T>` / `_List_base(const allocator&)` bodies under `auto_inline(off)` (the ConstructScreenPC devices) carry
  over unchanged to OnItemSelected 0x005B48A2 and OnInitialActivate 0x005B3AEF: in-place `dec [reg+4] ; jne` releases and the list's
  allocator temporary on the dead parameter byte ([ebp+0Bh], 0x005B491F).
- A `ret 4` with a run of tiny `mov eax,ecx ; ... ; ret 4` stubs after it inside one catalog row = a split: DrawPreparePrimitivesForRendering
  0x005B9641 really ends at 0x005B9BF0 (0x5AF of the 1,867 catalog bytes).
