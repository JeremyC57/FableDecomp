# Chunk 618 lessons

## GOTCHAS one-liners
- (0071ba60) In a switch, `return GFLimitReturn(difference = expr, 0.0f, 1.0f);` with a block-local `float difference` stores
  the by-reference literal temporaries first and puts the difference in a slot shared across cases. A named
  low/high/difference trio, or the plain expression argument, gives other slots.
- (00849700) For CreateThingAt-style helpers copy the landed sibling's shape (0x00849380): the defIndex local, then
  `Create(defIndex, at->GetPos(), owner->GetPlayer(), 0, 0, name)`. A template GetPItem cast wrapper, or the sibling's
  GetInterface body on a const path, recolours the whole function (+0x10..+0x1a bytes). Change one thing at a time on the
  parked source.
- (00849700) GetInterface bodies: the ternary-select form and the if-form load the component into different registers. Retail
  used both in one function (ternary non-const, if-form const).
- (005c71aa) If retail's early `return` jumps past a local vector's `free` test, the local must be non-escaping. Give the
  out-of-line `vector::resize(n, x)` (0x004428F0) a same-TU `__declspec(noinline)` body; with only a declaration the
  address escapes and the free test stays on the return path (cf. 008c12d0).
- (00542780) The PDB `C2DVector::Initialise(float, float)` evaluates its arguments right-to-left (height first).
  `Size = C2DVector(w, h)` gives width-first loads and a shared conversion slot but a materialised temporary. No form
  tried matches retail's width/height load pair.
- (0047f30f family) Dropping `optimize("s")` does not move the ChangePosition push ecx / fsubp swap. It stays a lane gap.

## Review at landing
- 0x00849700: respell PeekComponent (0x004C7A70) to the PDB const-reference form, and move CTCInventoryItem / CSubtitleRenderer /
  CPlayerGui / CWorld to the engine headers if they still byte-match (vland warns SHARED_HEADER_AVAILABLE).
- Landed sibling 0x00849380: respell CreateThingObject to `?Create@CThingObject@@SIPAV1@...` and GetMaxNumberItems to
  GetMaxNumberOfItems (NAME_FIXES.tsv).
- Labels: fix 0x005D9030, 0x005D9050, 0x005BF2ED and 0x006668A0 in ghidra_out (NAME_FIXES.tsv).
- Resolve the 0x0041E5F2 CManager vs CFrontEndManager GetInstance conflict once, fleet-wide.
- vland on 0x0071BA60 (switch row): RELOCATION_MATCH PASS with no RELOC_JUMPTABLE / RELOC_IDENTITY warnings.
