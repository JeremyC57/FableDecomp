# Chunk 2043 -- lessons from the authoring agent (fold into GOTCHAS at landing)

- c1 evaluates every argument that calls an inline / body-visible function (vector operator[], begin(), GFToScreen, C2DVector temps,
  an ftol nested in operator[]'s index) before the outer call's pushes; a declared-only callee in the same argument list (ScaleAlpha)
  stays in place and is a memory barrier, so the hoisted loads land before it. When retail shows such loads AFTER the declared call,
  give that callee its real same-TU body (out of line): c2 then knows it writes no memory and schedules the hoisted lookups after it
  (0x0064ED4F: CDrawBase::ScaleAlpha 0x00643D5D; 2356v2314 -> RELOCATION_MATCH in one grade). Check this before rewriting vector
  accesses as raw pointer reads (the landed CDrawPerceivers::Draw 0x0064499C used a raw `Start[frame]` model for the same symptom).
- `long d = it->X; ... if (d < 0) { if (d * 20 < -128) a = 0; else a = (unsigned char)(it->X * 20 + 128); }` gives retail's
  `mov eax,[slot] ; cmp eax,ebx ; jge ; lea eax,[eax+eax*4] ; shl eax,2` + an 8-bit `mov al,[node+8] ; imul dl ; add al,80h` arm;
  `d` in the arm reuses al, `it->X` in the condition compares memory and reloads for the multiply (0x0064EE29).
- A C2DVector operator* on a def member (`def.V * t`) materialises `&def.V` (`add eax,824h ; fmul [eax]`); the member-by-member
  `C2DVector(def.V.X * t, def.V.Y * t)` gives retail's `fmul [eax+824h] / [eax+828h]` (0x0064EE62).
- NPlayerGui::GFAnchorToTopLeft is the identity and retail keeps an uncalled out-of-line copy at 0x00643C94 (right before
  GFAnchorToTopRight); every caller expands it (the temp-then-copy of a by-value C2DVector, 0x0064EDE2).

Review at landing:
- 0x00643D0C reloc binding `?GFAPositionToScreen@@YIXPAVC2DVector@@PBV1@@Z` (row 0x00658020) is wrong: NAME_FIXES.tsv.
- The row defines CDrawBase::ScaleAlpha's body (same-TU function, retail 0x00643D5D, byte-identical); the landed 0x00643D5D source
  is a free-function stand-in (`Cons_val_CEntry`) and could be re-authored with this body.
- 0x00643BE2 is probably NPlayerGui::GFToScreen(const C2DVector&), catalogued as NUISystem::CComponent::CanMove.
- The row's own object also emits CDrawBase::ScaleAlpha, CEnginePrimitiveHandle::Clear and the inline COMDATs; only the Leaf lands.
