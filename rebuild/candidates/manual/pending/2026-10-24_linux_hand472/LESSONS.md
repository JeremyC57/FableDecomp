# Chunk 472 -- lessons from the authoring agent (fold into GOTCHAS at landing)

- VC7.1 inlines `_M_insert_overflow` at one push_back and calls it out of line at a later one in the SAME function when the
  per-function inline budget runs out: write both as plain STLport `push_back` (only push_back `__forceinline`) and the
  compiler reproduces retail's split by itself (CUserProfileManager ctor 0x0040CEC0: in-loop inline, post-loop call 0x004124C0).
- A by-value C2DExtentsI argument built in place (`sub esp,8 ; mov eax,esp ; mov [eax],..`) needs a user copy constructor
  (the PDB has `??0C2DExtentsI@@QAE@ABV0@@Z`); without it VC pushes the two words and homes the source locals (0x0040CEC0).
- The PDB has no CDisplayModeDesc::operator== / C2DExtentsI::operator==: retail's display-mode compares are field-wise
  (`it->Dimensions.X != prev.Dimensions.X || ...`), which keeps `cmp [ecx],edx` with folded offsets (0x0040CEC0).
- Zero stores / `push` scheduling around a deque or vector member in a ctor follows the real STLport structure: model
  `_Vector_base(n, allocator_type())` with an `_STLP_alloc_proxy` end-of-storage and `_Deque_base` with two `_STLP_alloc_proxy`
  members + `__stl_alloc_create`; flat containers put `push 0`/`push 0C8h` one store early (0x0040CEC0).
- `GFGetSystemManager()->PDisplayManager->F(simple args)` through a CCountedPointer `operator->` evaluates the object
  expression BEFORE the simple arguments (call GF, then push), but AFTER by-value class arguments built in place; a raw
  pointer member pushes the arguments first. A named `CSystemManager* system = GFGetSystemManager();` gives the folded
  `mov ecx,[eax+60h]` (0x0040CEC0, as the landed ClampAntialiasing 0x00409140).
- A pointer local that retail zeroes early (`xor ebx,ebx` before an unrelated call) is declared and initialised before
  that call, not at the point of the `if` that assigns it (PCLoad 0x0047DC69).
- Free callbacks of the in-game menu TU (PCLoad 0x0047DC69) are a frame-pointer size TU: `optimize("y", off)` +
  `optimize("s", on)`, the same pragmas as the landed CloseInGameMenuPC 0x0047DBB8.
- The biased /Os frame `lea ebp,[esp-N]` comes from `#pragma optimize("s", on)` ALONE; adding `("y", off)` turns it
  into `mov ebp,esp` (GFLogMemoryUse 0x00415E85).
- VC7.1 has no NRVO; retail's "result temp, ~tag, delete bank, ~title" order is an `auto_ptr` declared between the two
  strings and `reset(new T)` (GetWindowTitle 0x004023F0).
- Nesting a function's success path inside `if (ok) { ... return true; }` moves the shared failure block (MessageBox)
  and return to the end (GFConfigDetection 0x004017B0).
- An inline wide `operator==` written as an if/return pair keeps the bool in bl; a ternary widens the && arm to
  `mov ebx,1`; STLport `char_traits::length` must be an index loop (`xor eax,eax ; inc eax ; cmp word [eax*2+lit],0`),
  and both inline levels need `__forceinline` (0x00402130).
- Literal temporaries passed through `const CCharString&` (implicit conversion) take frame slots in source order ahead
  of call-result temps; an explicit `CCharString("")` argument is constructed after the later arguments are pushed, an
  implicit / default one before (GFParseCommandLine 0x004138D0).
- Retail has TWO `""`: the TU's own literal (0x0122D70E) and the string library's shared empty string 0x0129AAF4 returned
  by the inline `CCharString::GetString()`; model the latter as `extern const char g_emptyLiteral_0129aaf4[]` or one
  literal symbol binds two retail addresses (vland rejects it) (0x00402130, 0x004138D0).
- STLport 4.6 heap: `sort_heap -> pop_heap -> __pop_heap_aux -> __pop_heap` all inline (the implicit pair copy too),
  `__adjust_heap` out of line (0x00993130).

Review at landing:
- 00405220 (parked): an earlier session swept 19 source forms with sweep.py beyond the 4-grade budget.
- 00402130 and 004138d0 were regraded a fifth time after binding GetString's `""` to 0x0129AAF4 (no code change; still
  RELOCATION_MATCH); seven line-1 comments (004117c0, 00411b90, 00411db0, 00472c2a, 0047c030, 0047c140, 0047ccd0) were
  shortened to `std::vector::<name>(...)` with the element type in the description, and every `_STL` became `std`; all 35
  were regraded after these edits.
- Local base models next to existing engine headers: CTCInGameMenu (0047dc69, retail layout is PDB -0x20 past the
  vectors), CGamePlayerInterface uses engine/CGamePlayerInterface.h (00446d10); CUserProfileManager has no engine header
  (local model with PDB names, 12-byte vectors, 0x28-byte deque).
- Already-landed source errors: see NOTES.md "Errors noticed in ALREADY-LANDED sources" (GetIconsDef file name,
  PeekWaterHasType, $E2, the 0x009E1DA0 GFVXToAX/GFAXToVX disagreement, SavedConfirmed's placeholder types).
- No catalog row of this chunk holds two functions.
