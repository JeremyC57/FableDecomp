# Chunk 77 -- lessons from the authoring agent (fold into GOTCHAS at landing)

- `resize(Size(), 0)` where retail stores the zero temporary only after re-reading the member: the count must be a named
  local (else the lane stores the temporary early and puts the zero in eax).
- CRGBColour stores at +2, +1, then A, then +0 = a `(r, g, b, a)` constructor assigning R, G, B, A in its BODY; an init list
  stores in declaration order (B, G, R, A).
- Duplicated `push flag ; add eax,anchor` arms joining into one call = an if/else of two whole statements (merged by
  `optimize("s")`); a ternary argument pushes the flag once after the join.
- A shared `mov al,1` after the last `je` = nested ifs with a single trailing return; flat early returns end in `sete`/`setne`.
- A hoisted `xor eax,eax` used as `cmp [mem],al`, or `cmp byte ptr [..],0` tests = `#pragma optimize("s", on)`; an implicit
  copy constructor copying every scalar through eax with `movsd` for a 12-byte POD needs `optimize("s")` too.
- A callee whose result the caller tests with `test al,al` must be declared `bool` (`int` gives `test eax,eax`).
- Retail layouts differing from the PDB headers: CStringParser 0x28 bytes (comment list one word, ErrorCallback +0x18,
  CursorPos +0x1C); CSubtitleRenderer DisplayFlags +0x7C, cutscene bools from +0xDC; CBankFile 0x164 bytes; CSteeringAgent
  lists 4 bytes each; CPlayerGuiDef controller graphics 0x3C below the PDB offsets; CStopWatch matches the PDB exactly.

Review at landing: parked 00b1b5e0 CSprite::GetHeight inlines an `__asm` fistp rounding helper in retail -- not
reproducible in plain C++ under byte purity; do not retry. 00a5f290 is the "24i" shape: retry with the STLport
`_Vector_base` model (hand75 LESSONS). 009ba390 keeps `this` in ecx across a call (LTCG). Label corrections in NOTES.md
(0x00438010 is CGameUI::Draw2DText, 0x0099EAE0 is ~CCharString, 0x00463871 the CStringParser comment-list copy constructor).
