# Chunk 611 -- lessons from the authoring agent (fold into GOTCHAS at landing)

- Split a product with a call result into `float f = (float)call(...); f *= m;`: the compound form schedules an unrelated
  integer load/test before the `fmul [m]` as retail (0x00C74BB0); the one-expression product emits the fmul first.
- VC7.1 rewrites an inline `f * K / d` into `f / d * K` (`fdiv ; fmul K`); a named `float w = f * K;` then `w / d` keeps
  retail's `fmul K ; fdiv d` (0x00C74BB0).
- A double sin/cos result used both in a float formula and in float vector maths: explicit `(float)s` casts (or a float-typed
  Square helper) make VC7.1 CSE the conversion and spill s as a float dword, adding a frame word; leaving s double with no
  casts spills it as retail's qword and fixed the whole frame (0x007BBC9F, 94 -> 28 diff lines).
- A base virtual called from a derived member with a direct `call` is a qualified call: 0x0087BA90 calls
  `CBase::GetObservedThingCurrentMeshHeight()` (CBase slot 15; CScriptedControl overrides it). Check RTTI slots before
  declaring such a callee non-virtual.
- Substring `// Leaf:` lines hid wrong own symbols again: OnInit/OnDie of CScriptedControl are virtual (UAE), InitMovement
  is protected virtual (MAE), GetNearestNode is const. Always use the full mangled leaf.
- A map key that retail copies to a frame word before `operator[]` is the derived-to-base pointer conversion temporary
  (`map<CComponent*, long>[textPtr]` with `CText* textPtr`); no invented by-value helper is needed (0x005B690B).
- `if (x < y) return true; return false;` after an early `return false` merges both false exits into one `xor al,al` and
  puts fnstsw after the dead x87 pops; `return x < y;` keeps retail's separate `mov eax,1 / xor eax,eax` exits (0x00860AD0).
- Residues no lever moved (park after 2-3 grades): equal-use colouring of an STL list loop (0x005B690B), vtable-load register
  rotation (0x0087BA90), copy load/store interleaving (0x00860AD0), def-pointer CSE across a && temporary (0x007805D0), and
  the STLport insert_overflow / fill_insert register swaps whose helpers take register arguments (LTCG family).

Review at landing:
- 0x00C74BB0 matches as the descriptive CEffectFilter::SetHighPass(long, float, unsigned long); the landed caller 0x00C783F0
  binds `?SetPeak@CEffectFilter@@QAEXMMK@Z` (wrong first-parameter type). Respell the caller (NAME_FIXES.tsv) or pick one
  descriptive name for both; vland's RELOC_ALIAS warning is the expected _CIpow import thunk.
- NAME_FIXES.tsv: the landed GetNearestNode 0x00860A50 (QAE -> QBE) and OnDie 0x0087BBF0 (QAE -> UAE) are wrong in their own
  leaf symbols; the push_back/fill callers bind vector members without std::allocator; list/map callee spellings in the
  CComponent family.
- The parked 0x0087BA90 / 0x005B690B / 0x007BBC9F differs keep filler virtual slots (vNN / SlotNNN); only the named slots are proven.
- Rows above 6 grades this round: 00860AD0 (9), 007BBC9F (9), 005B690B (7).
