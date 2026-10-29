# Chunk 561 lessons (GOTCHAS one-liners)

- CPersistContext::Transfer<T>: the RTTI of the text-arm wrapper vtable (vtable-4 -> COL -> TD+8) names T; the catalog's "Transfer<signed char>" is propagated across all instantiations.
- Transfer<T> shape: switch (Mode) with `default: __assume(0)` (bare `jmp [eax*4+table]`), cases written 4,3,2,1,0; CPersistTraits<T> without a constructor, block-scoped in each binary arm (function scope puts it in the key's slot).
- Inline setters `if (UseNewProfileValues) XNewProfile = v; X = v; ApplyValues(); if (save) Save();` called as SetX(XNewProfile, false) drop the guarded self-store and keep bool copies in al; plain `X = XNewProfile` rotates al/cl/dl (0x0040CCA0).
- STL containers in a hand model of an STLport class: use `unsigned long[n]`, not `unsigned char[n]`, or the following members lose 4-byte alignment.
- A switch whose accepted cases each have their own `result = x; break;` keeps a direct dword jump table; one shared body for all the labels gives a byte-index table (0x0041CD93).
- An inlined store-only helper (CFadeInFadeOutBase::Open) compiles the same out of line in any store order but not inline; order the stores until the inlined schedule matches (0x00434CD0).
- A by-value CRGBColour built in its argument slot (`push ecx ; mov eax,esp ; mov [eax+2],bl ...`) needs CRGBColour's user copy constructor; without one VC7.1 builds a frame temporary and copies it (0x004A2F10).
- VC7.1 declines to inline the CThing::GetTC<T>(T**) template inside a large caller; `__forceinline` restores retail's inline LowerBound sequence and register set (0x004A2F10).
- A derived destructor with one vtable store before the tail jump to ~CBaseObject means CClass's destructor is implicit; a user-declared empty `virtual ~CClass() {}` adds a CClass vtable store (0x004C8310).
- STLport bitset::reset(pos) with exceptions off is `if (pos >= N) { puts("bitset"); abort(); }`, and abort is noreturn: the cold block sits after the epilogue and ends in `jmp abort` (MSVCR71 import thunks 0x00BFEAF2 puts / 0x00BFEAEC abort).
- CDefPointer / CIVCountedPointer releases with `dec [ecx+4] ; jne` read the pointer once into a local (`T* p = Object; if (p) { if (--p->RefCount == 0) p->slot1(); Object = 0; }`); re-reading the member gives a second load.
- Over-spanned catalog rows (the row runs on into a thunk or the next function): grade the true extent with relocations masked, then park it as DIFFER(<true>v<row>) with "re-split" (0x00405A60, 0x0044BB00).

## Review at landing

- rebuild/oracles/reloc_bindings.tsv rows with target `-` make reloc_identity.bindings_table throw after a handful of entries. The table stays partial, so RELOC_CONFLICT is effectively off for the rest of the dry run (seen as RELOC_CHECK_ERROR on the first row only). Either fix the loader (skip non-hex targets) or clean the rows. agent561/tools/vland_wrap.py does the former for a dry run.
- finish.py derives module/name by splitting line 1 on every `::`, which breaks for a template argument with a scope (004ab640 `Transfer<CWorld::ESaveType>`).
