# Chunk 413 -- lessons from the authoring agent (fold into GOTCHAS at landing)

- STLport `map::operator[]` needs two inline-return layers (0x9EFCF0, 0x9F1470):
  - `_STLP_DEFAULT_CONSTRUCTED(_Tp)` as `__default_constructed((_Tp*)0)` (`return _Tp();`). A bare `_Tp()` reuses the
    ctor's eax; retail re-derives `lea edx,[esp+N]`.
  - `map::insert(pos, v)` as an inline wrapper over `_M_t.insert_unique`. A direct call reads the result iterator
    through eax; retail reads it from the frame.
  - The iterator also needs a user copy ctor, which builds the by-value argument in place:
    `push ecx ; mov eax,esp ; mov [eax],esi`.
- A vector default-constructed in a ctor and then `push_back`ed must use the `_Vector_base` + `_STLP_alloc_proxy`
  model (0x9EBB30). With plain zero member inits VC folds `finish == end_of_storage` and emits only the overflow call;
  retail re-reads `[this+4]`.
- Retail can home the `push_back` tag in the slot that still holds x: both `lea` operands point at the first argument
  slot (0x9EBB30, 0x9EBCB0). That needs a same-TU noinline `_M_insert_overflow` stand-in. When x is loaded into a
  register before the pushes, the slot is dead and a declared callee is enough (0x9EFDF0).
- An element copy that calls the out-of-line `CBase()` DEFAULT ctor (no argument) is a user copy ctor that omits the
  base from its init list (`CConsoleCommandLine(const CConsoleCommandLine& o) : CurrentEntry(o.CurrentEntry), ...`).
  An implicit copy ctor only stores the vptr (0x9EEB00).
- `CGenericVar() { Type = NONE; }` puts the (dead) Type store after the ValString ctor call; a body assignment, not an
  init-list member (0x9EA6C0).
- The Lionhead `IsValidPosition((__int64)x)` guard (`x >= 0 && x <= 0x7fffffff`) on an unsigned 32-bit sum compiles
  to `xor ecx,ecx` (hi word) then `jl/jg/test;jb` and `test;jg/jl/cmp 7fffffffh;ja`. In
  CGameEventDataPackage::GetData 0x9F18F0 it guards the wrap branch, and a failure returns before Length is touched.
- VC7.1 emits an unreferenced anonymous-namespace function. Its symbol `?Name@?A0x<hash>@@...` carries a per-file
  hash, so write the Leaf as the `?Name@` prefix (0x9F1C80).
- STLport `list::size()` (distance) compared with a `long` member compares unsigned (`jbe`). `advance(it, size())`
  with an unsigned distance keeps its dead backward loop behind a `je` (0x9EAC80).
- A brace-initialised struct in a `for`-init (`for (It it = { p }; ...)`) is a VC7.1 syntax error; declare and assign
  before the loop (0x9EB690).
- `CCodeSectionManager()` stores only its own vtable over an inline protected `CTBaseSingleton<T>()`, and its maps
  zero the header proxy before `allocate(1)` in the `_Rb_tree_base` body (0x9F0320).
- Earlier rows:
  - Accessor chains (`GetAPosShift = GetRPosShift + GetNoRBits`, ...) reproduce the retail B+G+R add order
    (0x9E3F60).
  - An RAII buffer holder explains a `free` after the result copy (0x9E9E30).
  - Direct member stores keep the retail store order where an inline `Open()` reorders them (0x9E93C0).
  - Two named locals hoist both `GFGetSystemManager` calls (0x9E23F0).

Review at landing:
- Landed 0x9E5120 "CSymbolMap::Lookup" is really GetSymbolSLONG, and landed 0x9E51C0 "FindSymbol" is really
  SymbolExists. The real `FindSymbol(name, long&)` is this chunk's 0x9E5170.
- The landed CConsoleVariable::GetHelp 0x9EA3C0 and CConsoleCommandBase::GetHelp 0x9EA7D0 describe vtable slot 3 as
  "the name by value". ConsoleCommandList / ConsoleVariableList compare slot 3 with "command" / "variable", so slot 3
  is the type string (GetTypeAsString). This is a comment-only fix; the bytes are unaffected.
- Local models sit next to engine headers:
  - CJoystick (0x9E4420): CJoystick.h has a 12-byte list.
  - CConsole (0x9EAC80, 0x9EB220, 0x9EB690, 0x9EB8E0): no header; it follows the landed `CConsole_Retail` offsets.
  - CGameEventDataPackage, CCodeSectionManager and CConsoleCommandParameters: no headers.
- 0x9EAC80 is named ScrollToHistoryBeginning from PDB behaviour fit only; confirm with an xref if one is available.
- 0x9F1470's catalog label "CCodeSectionManager_RegisterSection" duplicates the landed RegisterSection 0x9F05A0.
- 0x9E24B0 GFRecalibrateFastSubFrameTimer is a retail_asm candidate (inline rdtsc). Only the owner-graded form can
  land it.
- No row needed more than 4 grades; 0x9E3F60 and 0x9F18F0 were parked at the budget.
