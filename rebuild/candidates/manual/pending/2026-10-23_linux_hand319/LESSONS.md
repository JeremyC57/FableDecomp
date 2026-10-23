# Chunk 319 -- lessons from the authoring agent (fold into GOTCHAS at landing)

- A predicate holder with no virtual table -> needs `__declspec(novtable)`, or the stack vptr store appears (0x00DA4E30).
- Ctor/dtor inline versus out of line -> decided by the per-function inline budget in source order; a `__forceinline` accessor bypasses the budget (0x00E96660, 0x00D22880; the opposite case is parked at 0x00E03F70).
- A virtual GetPos called on a temporary -> retail hoists it before the temp's construction (0x00D2A180).
- Helpers that take a reference -> keep the virtual calls virtual; by-value or concrete forms devirtualise (0x00D2A180).
- A named local inside a nested block -> lets VC7.1 overlay earlier temporaries' stack slots; this sets the frame size (0x00DAA260, 0x00E37E40 0x40->0x3c).
- `&&` conditions that hold temporaries -> split them into nested ifs so the temps die in retail order (0x0068DD00).
- GameCalls slot 11 is IsLevelLoaded and slot 12 is IsRegionLoaded -> do not swap them (0x00CF8A70).
- ReadEBOOL into a bool -> `value != 0` (0x006F9F90).
- `buttons |= a | b` -> separate `buttons |= a; buttons |= b;` statements (0x0068B2E0).
- An empty tag struct temp -> needs a user ctor `T() {}`; otherwise value-initialisation stores 0 (0x006FA4E0).
- The STLport erase/clear inline -> must be `__forceinline` under the inline budget (0x006FA4E0).
- HasTC -> return the raw `unsigned long` mask; returning bool adds shr/test (0x0068F470).
- A derived pointer passed to `IS_THING_ALIVE(CThing* const&)` -> creates a converted temp, so the original stays in a register (0x0068F470).
- A template `operator=` calling ShareData in an /Os TU -> must be `__forceinline` (0x0053B91E).
- The push_back argument -> pass an rvalue (`GetPtr()`) to get retail's temp copy; a local raw pointer for counted-pointer calls keeps the value in ebx (0x0053B91E).
- Retail promotes the constant 0 into ebx for the whole body (cmp cl,bl; colour bytes from bl; bool spilled) -> no source form found yet; `== false`, int colour args and ?: did not help (0x00E96110, 0x0053B91E).

Review at landing:
- More than 4 grades: 0x0068DD00 and 0x00688360 (8 each, both still DIFFER).
- Local base models used next to existing engine headers: CInputProcessGameBase (0x0068AE30/0x0068B2E0/0x0068B7C0 and the parked input rows), CKeyboard (0x00AB64E0).
- Errors in already-landed sources: none noticed.
- Catalog rows holding several functions: none. The odd-address rows 0x0053B91E / 0x004184BD / 0x00428494 are one function each.
- 16 rows are first-pass stubs only and need full work: 004184bd 00428494 005430f0 006a40d0 0079f970 0082afa0 008dcc80 008de080 008de7a0 008e8060 009f57a0 00a276d0 00a495f0 00be6880 00bfa2a0 00bfad90.
