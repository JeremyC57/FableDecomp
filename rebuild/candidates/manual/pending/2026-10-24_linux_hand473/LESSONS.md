# Chunk 473 -- lessons from the authoring agent (fold into GOTCHAS at landing)

- **Out-of-line callee homes an address-taken local**: when a local is passed by address to an out-of-line callee
  (`TransferBinaryIn(stream, &count)`), VC7.1 keeps it in memory for the whole function. A same-TU noinline stand-in
  body (`#pragma auto_inline(off)`) lets the compiler see that the pointer is not captured, so the local stays in a
  register and is homed only around the call (005079c0, count in ebp).
- **GetDef stand-in**: a same-TU noinline `CGameDefinitionManager::GetDef<T>(index, CDefPointer<T>&)` stand-in keeps
  the def object in edi across the following `def->Field` read instead of reloading it (004fe7f0).
- **Aligned frame from a pair insert**: an STLport `pair` with user ctors, returned through a hidden pointer from
  `set::insert` inside loops, makes VC7.1 build an `and esp,-8` frame with an ebp frame pointer. This is right for
  004fe7f0. The same construction is wrong for 004b2ca0, where retail has a plain esp frame, under both RTM and QFE.
  That row is parked.
- **POD pair returned in registers**: a POD pair returned from a member uses eax:edx when the pair itself has no ctor,
  even if its member has one (004b2ca0 probe).
- **Lea-addressed named string arguments**: an inline helper that takes `const char*` and builds a named CCharString
  gives the lea-addressed temporary that is destroyed at once (`parser->SkipPastText("END_REGION")`, 005079c0).
- **Inline `CWideString == L""`**: write it as an explicit goto layout: the wcscmp arm for a null block, then the
  size + MemCmp_Unsigned16 arm, falling into the empty arm. An if / else-if form merges the wcscmp test, and a
  forceinline bool IsEmpty gives neg/sbb (004a05c0).
- **Load order before argument evaluation**: put a vtable-call object and a manager pointer in named locals
  (`CGameScriptInterface* gsi = ...; CPlayerManager* players = ...;`) so that retail loads them before evaluating the
  arguments (004a0940).
- **`operator delete` under /Gr**: declare it `void __cdecl operator delete(void*)` in a /Gr TU. Without `__cdecl` it
  becomes fastcall, conflicting with the implicit declaration ("redefinition", SRC_FAIL) (004a0940).
- **Free functions in the CWorld TU are fastcall (/Gr)**: 0048cc60 and 004a0940 both need `// Flags: /Gr`.
- **Line 1 of a source**: template args containing `::` before the first `(` break finish.py's module/name parse.
  Write `_STL::vector::_M_insert_overflow(CActivateQuestInfo* pos, ...)` (004bb2e0, 0051a430).
- **One-line class bodies after a struct-to-class conversion**: a scripted conversion that inserts `public:` on its own
  line breaks a one-line `class X { ... };`. Put `public:` inline (SRC_FAIL found and fixed in 004a05c0).

Review at landing:
- Every source in src/ writes STLport as `_STL`; they were authored before the 2026-10-25 `std` rule. Convert them to
  `std` and regrade. 0048cc60 and 004a0940 share the noinline `__find_if` symbol; after conversion it must still be
  identical in both.
- The landed CPlayer::LocateCharacterStartHolySite 0x488B20 models `__find_if` with non-namespaced STL structs, so its
  callee symbol differs from the 0048cc60 row's. Re-point it once 0048cc60 lands.
- Local models sit next to existing engine headers: CWorld (004a05c0, 004a0940, 0049e080), CWorldMap / CRegion
  (005079c0, 004fe7f0), CMap / CGameMap (004fe7f0), CQuestManager (004bb2e0, differ/004b2ca0), CStringParser and
  CPersistContext (005079c0, 004c88e0, 004c8a30). CRegion is modelled differently in 005079c0 (Name +0x18) and
  004fe7f0 (map list); unify them on engine/CRegion.h.
- 0051af80: the `// Leaf: ?erase@?$_Rb_tree@` prefix matches two symbols; the range overload is the larger one. Pin
  the full mangled name at landing.
- The landed 0x5BD8A0 source calls 0x9C9070 by another name; it is CDataBank::GetPTextEntry.
- The landed UninitializeDaySection 0x4B3260 / UninitializeNightSection 0x4B35E0 call 0x4B2CA0 "BuildLoadSections";
  the PDB name is CQuestManager::GetActiveQuestSectionNames.
- No row needed more than 4 grades. 004a0940's 4th grade and 004a05c0's two extra regrades only re-verified sources
  after the struct-to-class retype and the comment edits.
