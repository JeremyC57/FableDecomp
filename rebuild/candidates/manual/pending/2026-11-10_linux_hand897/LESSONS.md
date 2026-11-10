# agent897 lessons (GOTCHAS-ready)

- 0x00976E20: an explicit instantiation `template bool CThing::GetTC<T>(T**);` of the shared forceinline GetTC (FABLE_RETAIL_CTHING_GETTC_EXPANDED) emits the retail out-of-line COMDAT exactly; CThing.h's "Fable.exe has no out-of-line copy" comment is wrong.
- 0x00976E70: a 5-byte `jmp ~Base` destructor is the IMPLICIT dtor emitted by a qualified `p->X::~X()` call in a helper (landed 0x008C36D0 pattern); a user-written `~X() {}` keeps the vtable store (11v5).
- 0x0083AD90: a 7-byte `mov [ecx],vftable ; ret` with no `mov eax,ecx` is a non-virtual empty dtor of a polymorphic base (NHeroStatModifierEvent::CBase), not an inline ctor.
- 0x0083ADF0: reverse_iterator operator== gives VC7.1's `xor/cmp/sete dl/mov al,dl` only when the iterator is a POD one-pointer struct with a MEMBER operator== compared by value; STLport _Rb_tree_iterator, user-ctor iterators or raw pointers give the 18-byte sub/neg/sbb/inc form.
- 0x0083ADA0..0x0083AE20: a run of tiny STLport map COMDATs (operator*, end, iterator ctors, _S_value, _Select1st) all come out of ONE placeholder map model with explicit instantiations; list symbols with a dummy leaf first, then fill each leaf from objdump.
- 0x00F29FF0: a def's virtual Copy is `*this = *static_cast<const T*>(other)` and VC7.1 turns it into `jmp ??4T` (5 bytes); gap-scan extents that swallow the following operator= are EXTENT_WRONG.
- 0x00C0E2C0 / 0x00C0E310: libpk CString SpanIncluding / SpanExcluding are MFC strcore with _MBCS: `_tcsspn`/`_tcscspn` as static inline wrappers over dllimport `_mbsspn`/`_mbscspn`, then `Left((int)n)`.
- 0x00DDE6F0: script OnPredicateFail with `mov ecx,[ecx+4]; mov eax,[ecx]; jmp [eax+54Ch]` is `Game->RemoveAllQuestInfoElements()` (CGameScriptInterfaceBase slot 339).
- 0x00EE94C0: NScript script-class members sit at Ego_r offset - 0x14 (LastFishingLevel Ego_r +0x58 -> retail +0x44); entity classes (CEntityScriptBase-derived, e.g. 0x00E8BDE0 / 0x00F00C60) are unshifted.
- 0x005D7081: the PC experience page is the console twin 0x005D6748 with PC component names, mouse-movement observe/ignore pairs and 303x53 sliders; the last residue was only the order of the two experience null-inits (both before the first GetTC).
