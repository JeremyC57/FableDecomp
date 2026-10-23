# Chunk 324 -- lessons from the authoring agent (fold into GOTCHAS at landing)
- Mechanical field-fill functions (00562710, 96 CCharString operator= stores + byte flags): generate the statements from the disassembly (push literal / add ecx,off -> `Vec[off/16].Field = "..."`) in retail order; matched first try.
- STLport std::string copy ctor inlined around the out-of-line range init 0x00567D00: the init-list form `: m_pStart(0),... { InitialiseRange(o.m_pStart, o.m_pFinish, tag()); }` puts the zero stores BEFORE the source loads; retail loads first. Named `first`/`last` locals read before the zero stores fix the order, and a NAMED tag local with a user ctor (not a `T()` temp) restores the frame (00563480: 0x240 -> 0x238). Residue still open: eax/edx letter swap.
- An empty tag passed as `forward_iterator_tag()` WITHOUT a user ctor is value-initialised: it emits a `mov byte [slot], bl` store; give the tag a user ctor or name it.
- A local array of vectors (`CScoreVector scores[39]`) gives retail's `vector constructor iterator` call (0x004038C0) and the inline reverse dtor loop with a memory counter; the element dtor must call an inline `DestroyRange(first, last)` helper (finish cached as a parameter) or the loop reloads _M_finish each step (00563480).
- STLport reverse(first,last) on pointers is an out-of-line `__fastcall __reverse(first, last, const random_access_iterator_tag&)` (0x005671C0); give the tag a user ctor so no byte store is emitted.
- `__fastcall` helpers with a hidden result in ecx may take only ONE stack arg (ret 4): a pushed arg left on the stack after the call belongs to a LATER call (0x0055F090 in 00560d00) -- account pushes per call before reading frame offsets.
- In VC7.1 a `for (T i ...)` variable did not free its slot for a later temp even when wrapped in `{}` (00587bff, 0047e196 inert) -- the extra frame word is not a for-scope issue there.
Review at landing:
- 0077d5b0 has no draft (lane gap: /GS per-return epilogue); differ file is a triage note only.
- 00560d00, 006c6990, 006c6e80, 0057c4f1 are first drafts (1-3 grades each); worth a deeper pass.
