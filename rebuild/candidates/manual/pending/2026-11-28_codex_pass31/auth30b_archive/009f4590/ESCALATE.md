# 0x009F3DA0 CSurface::CopyToMemoryB8G8R8A8 and 0x009F4590 CSurface::SaveAsTGA -- ESCALATE (AUTH30, 2026-11-27; one file for the pair)

## Measured (our gates; Codex's drafts; 009f4590 with its two overlay opt-ins and /Gr /GS)
- Bytes: RELOCATION_MATCH 582/582 and 864/864.
- GetWidth / GetHeight copies (24 B, NEQ at 0x00415BD0 / 0x00415BF0, unreached): byte- and relocation-identical in both rows to landed
  0x009F3B00's HEAD copies (head/009f3b00.obj). They were already NEQ at HEAD: debt, not a hold.
- 009f4590's `??1CAFile@@UAE@XZ` (a 5-byte jmp, NEQ at 0x0099A300, unreached): 0x0099A300's body is ~CBaseClassNonCopyable's
  (it stores 0x01231710; NAME_ANSWERS: FOLDED). The ??1CAFile line is a table alias, so the copy has no owner there (nf181): not a hold.
- `??_GCSurface@@UAEPAXI@Z`, the only blocker: reached through the emitted `??_7CSurface@@6B@` (1/1 slot agrees, 0x0041C9C0).
  - Both rows emit the identical 58-byte body (cmpbody), which expands Uninitialise.
  - Retail 0x0041C9C0 is 36 B and calls Uninitialise 0x009F2E20; the landed CRenderTarget row 0x004979A0 emits that 36-byte EQ copy
    at HEAD (vtchk: body36 EQ, relocs EQ).
  - So ours is a NEW, different copy of a landed function (rule 2026-11-20): held.
- Why no single-TU source closes it: retail's leaf expands Uninitialise twice (the explicit call and the destructor's, 0x009F3FA8..0x009F3FDD),
  so its body is visible. With it visible, VC7.1 also expands it in the compiler-generated ??_G. Retail's 36-byte ??_G comes from an
  early TU where Uninitialise was only declared, and it won the link.
- Codex's probes (moving or inline-marking Uninitialise / GetDimensions, PCH creator/consumer splits, both front-end inline
  profiles) give either the exact leaf or the exact helpers, never both. I have no hypothesis outside that list, so no further grades.

## Still to do if the ruling clears it
- Codex's tests model CSurface / CPixelFormat / C2DBoxI locally (name_gate FAIL 6), so they need rewriting.
- Then purity and the Ego_d review.

## Question
Does a vftable-reached compiler-generated ??_G copy that differs only because this TU sees an inline body (owner copy EQ at HEAD from
another TU) hold the row? Or is it link-order debt, since the HEAD owner copy exists and an address-ordered link picks it first?
Grades: c0 each (2).
