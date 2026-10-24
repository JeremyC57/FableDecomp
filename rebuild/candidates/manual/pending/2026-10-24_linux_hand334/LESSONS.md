# Chunk 334 -- lessons from the authoring agent (fold into GOTCHAS at landing)
- An inline `IsValidIndex()` negated into an early `return false` places the fail block between the two range tests; this matches retail (00c44430).
- A named bool result plus a `const` frame local drops the `xor eax,eax` pre-clear (008af1e0).
- A float local holding a divisor turns two divides into one reciprocal multiply. To keep two `fdiv`s, write `(float)g_framesPerSecond` at each use (008af5f0).
- For a return-true path that retail emits as two forward jumps to `xor al,al`, write the guard as `!(a<x) && !(b<y)` (008af5f0).
- Positive nested ifs share one fail tail; the early-return form duplicates it (008afc40, 00401fe0).
- Read through inline reference-returning accessors instead of a reference local to keep the `lea` late (009cc640).
- Write HasTC as `if (bits & mask) return true; return false;`; the `!= 0` form did not match (0080e530).
- In ForEachIf, a `__forceinline` `&&` filter keeps the filter inline under the template (0080e530).
- VC7.1 reads an unnamed temporary iterator late and a named one early. Pick the form by where retail loads the slot (008af1e0 / 008af500).
- A CVectorMap LowerBound under `#pragma optimize("s", on)` needs `int count` declared before `first` (0065e412).
- A 4-byte CRGBColour with a `Set(R,G,B,A)`-based copy ctor gives retail's `mov ax,[slot+2]` + al/ah byte stores, where the implicit copy gives one dword move (004c5890, parked at 130v132).
- Lane: `#include` of a probe-local header (e.g. "cq.h") made q.py hang to its 200 s timeout. Inline the model into each source instead.
- VC7.1: write `> >` in nested template argument lists, since `>>` is SRC_FAIL. wchar_t is not built in, so use `unsigned short` for wide literals.

Review at landing:
- 00401fe0 EULA::DoEULAThings replaces an already-landed asm_bake oracle at the same address; retire the oracle.
- 00abc440 uses a local C3DMeshFileTrianglesChunk model (list at +0x2C) because the engine header has the donor layout; check the header.
- The CombatDialogueManager rows (008af1e0/5f0/780/c40) use local models of CThingCreatureBase / CTCTalk / CCombatDialogueDef. Retail CCombatDialogueDef offsets are the PDB offsets minus 4. Consider a shared header.
- The Coin rows use a local Mass / CIntelligentPointer (pack(1)) model; check it against the engine headers.
- Five catalog labels were wrong; see NOTES.md "Label corrections". The CVectorMap value types do not fit for 0065e412, 00819fd0, 0086a8d0, 009b2190 and 009b2230.
- Five rows are parked (004c5890, 006b93d0, 008af500, 008af6e0, 008ef200). Each used its 4 grading attempts; the best source is in differ/.
