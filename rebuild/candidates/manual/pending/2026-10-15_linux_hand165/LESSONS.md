# Chunk 165 lessons (candidates for GOTCHAS wave 25)
- Retail CTCHeroStats float trio sits 8 below the PDB: Age +0x30, SunTan +0x34, Fatness +0x38 (Morality +0x28, Money +0x3C); the twin "SetFatness" rows are SetSunTan (+0x34) and SetFatness (+0x38).
- A def field read twice through `GetDefinitions()->P...Def` wants an inline `GetXDef()` accessor; reading it in the body swaps the esi/edi roles of this and the parent thing.
- A vector of CCharString filled by index: take `T* names = v.begin()` into a named local, or every `operator=` re-reads the frame's first pointer.
- `push_back(static_cast<Derived*>(p))` builds the reference temporary after the tests; pushing the base-pointer local stores it right after the lookup.
- `for (...) ++Abilities[i].Level;` (re-index) gives `inc dword ptr [edx]`; a reference local stores `edi+1` instead.
- `_M_start[i]` (not `*(begin()+i)`) keeps a strength-reduced byte offset in a dead argument slot for 0x2C-byte elements.
- A `switch` whose cases are `if (flag) return f(CCharString("A",-1)); return f(CCharString("B",-1));` matches: VC hoists the shared lead and merges all tails; a ternary on the literal does not.
- `if (--idx < 0) idx = size() - 1;` gives `dec ; jns`; `idx >= size()` with the unsigned STLport size gives the `jb` wrap.
- Rows at 0xC4xxxx/0xC6xxxx labelled CTCHeroStats::* are propagated labels on library objects; model with descriptive names.
