# Chunk 486 -- lessons from the authoring agent (fold into GOTCHAS at landing)

- A counted loop that becomes a down-counter (`mov [esp+x],n ; ... dec ; jne`) while the base is reloaded every pass
  means the source computed the count once into a local: `long n = arr.Size(); for (i = 0; i < n; ++i)`. The inline
  `i < arr.Size()` re-divides the pointer difference on every pass (0x0051E800 GetLevelSaveStats).
- A `movzx eax,bl` index next to a strength-reduced `ebp += 0x14` counter in the same loop comes from a `long` loop
  variable passed to an accessor that takes `unsigned char` (Ego_r `E` argument, e.g. CThingManager::GetTypeList); the
  Used test goes through the untruncated index (0x0051EA30).
- STLport `pop_heap(first, last, comp)` that has not been inlined is `push comp ; push 0 ; call __pop_heap_aux`
  (/Gr, `ret 4`). The `0` is the `_Tp*` value-type tag (0x004C42B0). Spell it pop_heap / `__pop_heap_aux`, not the
  PDB's Dinkumware `_Pop_heap_0`.
- An out-of-line `_Pow_int<float>` (`??$_Pow_int@M@@YIMMH@Z`, y in ecx, `fdivr` by 1.0f) is the VC7.1 <math.h> template
  verbatim in a /Gr TU; its caller is the cdecl `pow(float, int)` inline (0x00534430 / 0x005349B0).
- `new (p) T(v)` in a /Gr TU is `test ecx,ecx ; je ; push edx ; call T::T(const T&) ; ret`. Spell it
  `std::_Construct<T, T>` (STLport). Ego_r's `_Construct<T, const T&>` is a different library (0x0051B410).
- A thiscall `ret 4` that mallocs a 0x14-byte node, tests `node+8` and copy-constructs a 12-byte value is STLport
  `list<T>::_M_create_node`. To pick T, use the copy shape (out-of-line member copy ctor + plain dword copies = implicit copy)
  and the PDB list element sets (0x004B8AC0).
- Leaf mangling: when a template's `_Tp*` argument has the same type as an earlier parameter, it back-references as `0`
  (`...@00V...`), not `PAV12@`. A wrong Leaf silently grades the largest function in the TU (0x004C2C00 matched anyway,
  but the Leaf was fixed after the objdump check).

Review at landing:
- 0x004C7AA0 / 0x004C7AB0 (landed as "CViewBase::GetDisplayEngine") are CThing::GetWorld / PeekWorld. Re-check their names
  against 0x004C7AE0 CThing::GetPWorldSeed.
- The landed 0x004C2B90 uses global-namespace spellings for STLport helpers (`__unguarded_linear_insert` and friends);
  they belong in namespace std.
- The landed 0x00533720 comment names 0x0052CCA0 "CDraggableInto"; it is NUISystem::CChangingStateComponent's copy ctor.
- 0x004C7590 is bound under three different names by landed rows (CTCLook 0x00752200 UpdateGraphicAppearance,
  0x0077C740 SetGraphicAppearanceChanged, CTCLight 0x00640BB0 ForwardToGraphicAppearance). Neither PDB names it; pick
  one spelling.
- Local base models next to existing engine headers (vland SHARED_HEADER_AVAILABLE):
  - CObserver: 0052cca0, 00545840, 00546660, 00546c20, 00548780.
  - CClass: 00527300, 0052ace0.
  - CRegion: 0051b410.
  - CQuestManager: 004b8ac0.

  The headers are flat Ego_r data models (vptr as data, no ctors), so they cannot express these rows. Header debt is in
  NOTES.md.
- engine/CWorldMap.h carries Ego_r offsets (Maps +0x24 vs retail +0x20, see 004fc170).
- Catalog rows that hold several functions: 004ddaa4 (3 functions), 004e0adc (4 functions). Re-split the catalog.
- No row needed more than 4 grades. 0051ea30 stopped at 3 (register allocation only).
