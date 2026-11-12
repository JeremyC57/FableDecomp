# Lessons (agentNF28)
- reloc_bindings.tsv is stale: several queue entries were already applied in the repo, so grade the repo row before editing it.
- A newly declared `template<class T> IS_THING_ALIVE(const T&)` beats a non-template `(const CBase&)` overload for derived arguments (exact match beats a derived-to-base conversion), which silently moves inline calls out of line; give the inline overload the exact type.
- A header template ctor that needs `T : CBaseObject` cannot be used with a TU's local CThing model; deriving the TU's wrapper from `CIntelligentPointer<T>` keeps the mangled type without touching the ctor.
- A const member-pointer global is optimized away (OBJDUMP_ERR); a non-const global forces the inline body to be emitted.
- Retail STLport containers are 12 bytes against 16 in Ego_r: count the containers before a member to map an Ego_r offset to retail (CDefinitionManager::InstantiatedDefs +0xBC becomes +0xA4).
- When a row drops its local model for a shared header, also delete local redefinitions such as CBaseClassNonCopyable or _D3DDISPLAYMODE, which otherwise fail to compile.
- When a script asserts that an old spelling is gone from a row, it also matches the comments in that row; reword them.
- A header opt-in that drags in fable_containers / fable_string clashes with a row's own std::vector or CCharString model; add a declaration-only opt-in in the header's default branch instead of keeping a local shell.
