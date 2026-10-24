# Chunk 371 -- lessons from the authoring agent (fold into GOTCHAS at landing)

## Same-TU stand-ins
- **The growth member itself (0x4ADB50).** A `vector<T>::_M_insert_overflow` whose three `__false_type()` temporaries
  sit on the live `__atend` argument slot (`lea ecx,[esp+28h]` = &atend) matches first try. It needs same-TU
  `auto_inline(off)` bodies for its tag-taking callees (`__uninitialized_copy`, `__uninitialized_fill_n`,
  `__destroy_aux`) that never touch the tag. Declared-only callees give the tag its own slot.
  - This is the likely fix for the parked size-TU twin 0x493FF8.
- **A read-only callee (0x4AF610).** VC7.1 uses a same-TU callee body for loop-invariant hoisting. With the read-only
  `Compare` 0x411570 body above the function, the searched CCharString's data pointer is loaded once before the list
  walk. With the callee declared only, VC reloads it every pass.
- **Escape analysis (0x4B4A10).** A local container that retail keeps in registers across element destructor calls
  (start/finish in edi/ebp around the `~CCharString` loop) needs two things:
  - STLport's by-value `~vector() { _Destroy(_M_start, _M_finish); }`, which caches finish;
  - a same-TU stand-in for the function the container is passed to (`ActivateMultipleQuests` 0x4B4260, the same TU in
    retail) that never keeps its address. That caches start: VC then knows the local does not escape.
- **Tags on a live value slot (0x4ADCB0).** The `push_back` -> `_M_insert_overflow(_M_finish, x, __true_type(), 1,
  true)` tag shares the live value slot (`lea ecx,[esp+24h] ; push ecx ; lea edx,[esp+28h] ; push edx`) once the POD
  growth COMDAT 0x48D0D0 has a tag-free same-TU body.

## Codegen shapes
- **lea vs push eax for a returned temporary (0x4AD3D0).** `set.insert(table.GetString(def))` pushes GetString's eax;
  retail re-forms the temporary's address with `lea`.
  - That comes from an inline `CDefString::GetString() const { return g_DefStringTable.GetString(TablePos); }`
    together with the inline STLport `set::insert -> _M_t.insert_unique`.
  - A named string local also gives `lea`, but lets the dropped pair share the CDefString frame word (frame 0x48
    against 0x4C).
- **Nested records (0x4B8260).** A three-CCharString record whose fill loop keeps the count in the `push ecx` frame word
  and re-bases this/other (`lea ebx,[esi+4]` / `lea edi,[ebp+4]`, then `+4`) is
  `pair<CCharString, pair<CCharString, CCharString> >` with a declared copy ctor. A flat struct keeps the count in a
  register (69 v 81).
- **boost::checked_delete (0x4ADD30, 0x4AE080).**
  - How to spot it: `mov ecx,[ecx] ; jmp F` plus `mov edx,[esp+4] ; mov eax,ecx ; mov ecx,[eax] ; mov [eax],edx ; call
    F ; ret 4` next to F = scoped_ptr dtor/reset, so F = `boost::checked_delete<T>`.
  - How to emit it: /Gr plus `template void boost::checked_delete<T>(T*);` on the inline template.
  - How to name T: from the CWorld member the call sites touch, and the `new T` constructor beside them.
- **CCountedPointer<T>::DeleteData (0x4AE510).** The address stored into a count block by `Reset`
  (`mov [eax+4], offset F`) is `CCountedPointer<T>::DeleteData`. Write it as `static void __fastcall DeleteData(void*
  p) { delete (T*)p; }` with an explicit member instantiation. A `const` def pointer needs `Release() const` with a
  mutable count.
- **list::erase on a polymorphic element (0x4B7CA0).** `push 0 ; call [eax]` is the explicit `~T()` of a polymorphic
  element in STLport `_Destroy`. VC drops `deallocate`'s `if (p)` on a node that has already been dereferenced, so the
  verbatim STLport source matches.
- **Singleton constructor (0x4AE2A0).** A protected inline `CTBaseSingleton()` base leaves only the derived vtable
  store. STLport `_Rb_tree()` stores the 0 header first, then `malloc(sizeof node)`, then count 0, colour, root and
  left/right = header.
- **CQuestManager forwarders (0x4B4A10).** `bool` arguments forwarded to a callee push the full argument dword (no
  movzx). The bool result parked in the dead first-argument slot across a destructor loop means all three callee-saved
  registers are taken by the cached container words.

## Process
- finish.py takes module::name from line 1 up to the first `(`. Template arguments containing `::` in the function
  name (`F<list<T>::iterator>`) split wrongly, so put them in the parameter list.

Review at landing:
- **Descriptive names (no PDB identity found):**
  - 0x4ADCB0 `CopyIfFlagSet` (caller CWorld::LoadHSP 0x4A0940; the tested byte is +0xD4 of the listed thing).
  - 0x4B8260's `SNewQuestRecord` typedef (record vector parsed up to "END_NEW_QUEST").
  - 0x4B27F0 named `GetQuestMasterDataPersistScript` by its use in LoadMasterScriptDataFromGameState; its twin
    `GetQuestScriptPersistScript` has the same shape.
- **Same-TU stand-in bodies compiled above graded functions** (each header comment says so):
  - 0x4ADB50 (three STLport COMDATs);
  - 0x4ADCB0 (0x48D0D0);
  - 0x4AF610 (Compare 0x411570, a copy of the landed body);
  - 0x4B4A10 (0x433530 `_M_insert_overflow`, 0x4B4260 `ActivateMultipleQuests`).
- **Local models next to engine headers that carry PDB offsets:**
  - 0x4A08D0 `CWorld_RetailLayout` (engine/CWorld.h has the Dinkumware-debug offsets);
  - the CQuestManager rows use local retail layouts, as the landed Update 0x4B4490 does.
- **Already-landed sources to fix:**
  - 0x0041A410 is sloppy (`struct S`, leaf GetAutoSaveFileName).
  - The CWorld ConvertWorld* 3-argument twins 0x49D550 / 0x49D6B0 use a local `struct CWorld`.
  - The landed `GFToScreen` 0x4935F8 is really the CSubtitleRenderer::CEmbeddedGraphic copy constructor.
  - The landed `unattributed_ScalarDeletingDestructorCTBaseSingletonDerived00_004ae3b0` is
    CSkeletalMorphResourceManager's scalar deleting destructor (vtable 0x01238F84 slot 0). Its +4 member is the
    CachedMorphs map, not a list, so 0x4AE200 is that map's destructor.
- **Grade counts:** no row needed more than 4 grades. 0x493FF8 used 4 and is parked with a concrete next step (the
  0x4ADB50 stand-in recipe). 0x4978A0 is parked after 2 grades as scheduling-only.
