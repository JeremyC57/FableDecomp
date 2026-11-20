# NF201 lessons (one line each)
- A free placeholder over a world chain (World +4 -> +0x18 -> +0x1964) ports in one grade onto CWorld.h / CEnvironment.h with FableWin's inline accessors GetEnvironment().GetSpellEffectInterpolator(): no overlay struct is needed (0x00890150, 0x0079C750).
- A bool operator whose retail body ends `xor eax,eax` / `mov eax,1` comes from `return a != b || c != d;`; the `if (..) return false; return true;` form gives `xor al,al` / `mov al,1` (0x00987D70).
- When retail expands a counted copy but keeps the default Release (zero store after the null test's join), the all-in FABLE_RETAIL_CIVCOUNTEDPOINTER is wrong: an add-only copy-only opt-in matched (0x007464A0).
- STLport no-default list::insert(iterator) / push_back() re-author as explicit instantiations on fable_containers.h (INSERT_DEFAULT_NOINLINE for the insert, INSERT_DEFAULT for push_back), the 0x009E4AA0 recipe.
- A row that calls GetDefaultVal<T> needs FABLE_RETAIL_CPERSISTCONTEXT_GETDEFAULTVAL_BODY + CPersistTraits.h (with CDataOutputStream.h first); without the body the default is a call, not retail's ctor / copy / dtor (0x00681BA0).
- `!s.IsEmpty()` gives `je`; retail's `jle` is `s.GetLength() > 0` (signed long).
- Copy the caller row's opt-in set first when a re-port of an STL COMDAT differs only in inlining: vector<C2DBoxF>(n) matched with 0x00A781A0's FABLE_RETAIL_C2DBOXF + FABLE_RETAIL_STLP_VECTOR (87v122 without).
- Compare a new pick-any COMDAT with the retail bytes at its owner address (COMDAT relocation fields zeroed in both) when the owner row is a stand-in that does not define the symbol (tools/cmpret.py).
- Probe WORKLIST binders at HEAD before porting: 3 of 4 q11772 binders and q11797's named binder were already right (stale table lines).
