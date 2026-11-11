# Chunk H40j lessons (GOTCHAS-ready one-liners)

- 0x00E40E80: if retail has extra `ret` epilogue copies with an expanded `~seh_me` (0x00E446EA, 0x00E447AB) and a call at the natural exit (0x00E4452C), drop FABLE_RETAIL_CSCRIPTGAMERESOURCEOBJECTSCRIPTEDTHINGBASE_OUTOFLINE_DTOR. The implicit STB dtor lets VC7.1 expand or call it per exit, which gave RELOCATION_MATCH. The same drop changes nothing at 0x00E3BB70, 0x00D13580, 0x00E4B5B0, 0x00DB97A0, 0x00E33EA0 or 0x00D0F640, and is worse at 0x00D973D0 (190 -> 207). Try it once per row; it is not a default.
- 0x00E3BB70: an in-body `#pragma inline_depth(0)` / `#pragma inline_depth()` pair around ONE movie declaration forces a call to its ctor at that site only (retail 0x006E7B60 at 0x00E3C8BE). A second pair around that scope's closing `}` forces a call to the dtor at the scope end (retail 0x006E7B80 at 0x00E3CAAB). This took the row from 595 to 198 lines.
- 0x00E4B5B0: a `long answer = 0;` initialiser is what gives retail's `xor ebp,ebp` before the BC_PAY question (0x00E4C0AC).
- 0x00E4B5B0: a timer that retail destroys on [esp+10h] at the function exit (0x00E4DD1F) is a function-scope local, not one inside a `{ }` block.
- 0x00D973D0: a lone `seh_me.Release();` statement explains retail's extra Release calls (0x00D97F28 after the let-out loop, 0x00D9979C before the keep-money loop).
- 0x00D973D0: write a loop as `for (;;) { if (x.IsDead()) break; ... }` when retail builds the loop-test string once, before the loop (0x00D98530).
- 0x00D973D0: `if (!IsActiveThreadTerminating()) continue; return _Fail_Val.Return();` at a loop bottom gives retail's `je top ; jmp exit` (0x00D98BA0).
- 0x00D13580: use `for (;;) { if (T) return; ... NSF; }` with no pre-loop T check when retail's pre-loop check and bottom check both reach one in-place exit (0x00D13F24).
- 0x00E5FD40: a row-local `inline CScriptThing::~CScriptThing() { }` after the includes lets retail's first temporaries expand (0x00E60216, 0x00E604C1). Pair it with `!IsActiveThreadTerminating() && true` in the main loop condition to bring the other sites back to calls (1057 -> 163 lines).
- 0x00DB97A0: a cutscene outro that ends in `break;`, followed by a post-loop `if (IsActiveThreadTerminating()) return _Fail_Val.Return();`, gives retail's T test after ~item (0x00DBB2E1). Pair it with a main loop `for (;; NSF) { if (T || false) return ...; }` (248 -> 125 lines).
- 0x00CF68C0: an explicit `CCountedPointer<Movie>` dtor specialization fails with C2908 if it comes after the movie header instantiates it. Forward-declare the movie class and put the specialization before that include.
- 0x00E20600: a `#pragma inline_depth(1)` around an out-of-class TEMPLATE member definition (`CPredicate_And<P1,P2>::~CPredicate_And`) does not stop its member dtors from expanding. It does work around a plain inline ctor (0x00E33EA0).
- Inline trace tip (tools/inl.sh): `sNone` means the site took the always-inline path (tiny c1 or forced), so the T74 budget does not apply to it.
