# Chunk 477 -- lessons from the authoring agent (fold into GOTCHAS at landing)

- A local STLport list passed by reference to a same-TU template, with its sentinel kept in a register afterwards (no `_M_node = 0` pre-store, no reload, the argument slot recycled), is LTCG mod-ref. VC7.1 without /GL must keep the escaped list in memory. Passing a temporary `end()` iterator instead reproduces the size (819v819), which confirms the gap (0x006FDA20).
- A polymorphic class with an `unsigned __int64` member whose first data member sits at +4 needs a 4-byte polymorphic base. MSVC pads the class's own vfptr to 8 when the class is 8-aligned, which shifts every member by 4 (0x006FDA20).
- `uint64 > 0` → `ja` (high) / `jbe` (low) with `mov eax,1 / xor eax,eax`; `!= 0` gives a different pair (0x006FDA20: `GoingIntoBuilding = BuildingUID > 0`).
- STLport 4.6 `list::insert` (`__n = pos; __p = __n->_M_prev`, then four link stores) → retail's single `mov ecx,[n+4]` load. The 4.5 form reloads `pos->_M_prev` (0x006FDA20).
- The operand order `previous == start` (the local on the left) → `cmp [mem],reg`. The reverse order gives `cmp reg,[mem]` (0x007345D0).
- The same-TU noinline `_M_insert_overflow` (`#pragma auto_inline(off)` + `__declspec(noinline)`) → VC keeps push_back's finish pointers and temporaries enregistered across the overflow call (0x007167E0, 0x007345D0).
- `_Rb_tree_base(const allocator&)` allocating the header in its own constructor, before the derived `_M_node_count(0)` → `[hdr]=0 ; call malloc ; [hdr]=eax ; [count]=0` (0x0070A570).
- The tree increment as a static `_Rb_global<bool>::_M_increment(node)` returning the node → retail's out-of-place iterator advance. The in-place `operator++` keeps the node in memory (0x0070A570).
- /Os-module COMDAT copies (0x005BB346 vector<CCompositeBlendDesc>(const vector&), 0x006CB010 push_back) → model them as declared-only members of a stand-in vector class. Inlining them never matches (0x0070A570).
- Natural VC7.1 inlining of `push_back(make_pair(a, b))` → the make_pair copy of an inner vector expands inline while the copy inside push_back's `_Construct` stays an out-of-line call. Don't force either side (0x00707B10).
- Angle helpers (`NormaliseRevolutions` / `atan2 * 1/2pi`) need `__forceinline`. A 3D heading temporary sets the frame size, and casting atan2's double to float before the scale gives `fmul dword` (0x00702E00).
- x87: `fnstsw ; test ah,41h ; jnp` = `!(a <= b)`; `test ah,41h ; jne` = `>`; `test ah,5 ; jp` = `<` (0x00702E00, 0x00752B00).
- Retail keeping a caller-saved register (edx) live across a same-TU call is LTCG-only. Park it after one grade (0x00752B00).
- A same-TU `GetSubDef<T>` body → the CDefPointer stays in edi across the lookup (0x00733B70).
- A named float divisor lets VC7.1 rewrite `index / count` as `index * (1/count)`. Keep `(float)count` inline in each expression (0x0074E140).
- `Reset(new T(a + b, ...))` with string temporaries → VC's conditional-temporary flag word (bits 1/2, 4/8) around each new-expression (0x006FAA90).
- wchar_t is not built-in in the lane's VC7.1, so wide literals are typed `const unsigned short*` (0x006FAA90).
- `__rotate_adaptive` needs `__forceinline` copy/copy_backward to inline the buffer moves like retail (0x00710DC0).
- `list(const list&)` → `insert(begin(), first, last)` with its `__false_type` homed in a dead argument slot (0x00706B50).
- Tests: in VC7.1 a `using namespace` + `for` + declaration combination can spin cl, so use qualified names. Use `int`/`unsigned int`, not `long`, for size checks on the 64-bit g++ host.

Review at landing:
- 0x007167E0 and 0x007345D0 each needed the full 4 grades. Check that both still grade RELOCATION_MATCH in the landing tree.
- Local retail models sit next to existing engine headers. engine/CGameCameraBase.h follows the debug PDB's 12-byte lists, so its offsets are wrong from +0xA8 on; retail has 4-byte lists, BuildingUID +0xB8 and size 0xC8. engine/CTCCombat.h offsets diverge from retail after +0x2C. The CTCLook model follows the landed CentreHead (4 below the PDB).
- Rename the landed caller 0x007530D0's callee note "UpdateHeadDirection" to CTCLook::SetHeadLooking (PDB).
- Identities are unconfirmed for 0x006E8EE0 (FindNearestIndexToPoint as a __fastcall free function) and 0x00702E00 (descriptive CThingHeadingGauge::GetHeadingAlignment; the catalog's CViewBase label is wrong).
- 0x00663340 is labelled GetHeroCombatDef here, which conflicts with a chunk-261 label. Reconcile before landing.
- In the 0x00739550 CIntelligentPointer<T>, T = CThing is descriptive (every instantiation shares vtable 0x01237F1C).
- 0x006FE7C0's catalog label ("CThingSearchTools rough-area" family) should become the Ego_r.pdb GetAllThingsInRoughArea<...> instantiation.
