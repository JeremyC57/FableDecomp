# agentNF15 lessons (one line each)

- A same-TU stand-in body for a member template (GetDef<T>, GetSubDef<T>) becomes an explicit specialisation `template <> __declspec(noinline) bool CDefinitionManager::GetDef<T>(...) const`. The mangled name is the retail one and the code it emits is unchanged.
- Keep a non-const pointee with `CDefNonConst<T>` (work/defnc.py). A custom out-slot struct with no destructor becomes a `template<> class CDefPointer<const T>` specialisation, not a new class.
- `struct` keys mangle as `U` and `class` keys as `V`. PDB classes must be declared `class` in a row (CMainGameComponent was a struct in several rows), or the binding fails while the bytes still match.
- When a row-local base gains virtuals, delete its explicit `void* __vftable` member, otherwise the layout shifts by 4. An otherwise empty CTCBase stand-in needs a `virtual void v00()` placeholder so no this-adjust appears.
- A private static (CThing::PCurrentThingManager, CMainGameComponent::ConstantFPS) is reached through its inline Ego_r accessor (`GetPCurrentThingManager()`, `GetConstantFPS()`); the reloc then binds the static's own name.
- Inline accessors can reorder VC7.1 loads. In 00799e30 reading members directly kept retail order. In 007233b0 the opposite was needed: `GetParentThing()` puts the thing pointer in ebx early, as retail does. Try both before giving up.
- The `CInterfaceMap { SEntry* first, *last; LowerBound(const long*) }` stand-in maps 1:1 onto the explicit specialisation `CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<...> >` (pair value_type, `_M_start/_M_finish`). With the caller's key as `ETCInterfaceType type = (ETCInterfaceType)x; LowerBound(type)`, the bytes are identical (work/lb.py; 26 rows, all first try).
- Tie any regex respelling of a local (`long key = x;`) to the use it serves (`LowerBound(&key)` within a few lines), or it rewrites unrelated locals with the same name (005a30a1 `defIndex`).
- A struct nested in a namespace (NCreatureMode) cannot hold an explicit specialisation of a global template. Close the namespace, specialise, then reopen it (008df3a0).
- An ICF-folded empty body (0x007EAB90 `ret 4`) can carry several retail names. Bind the one Ego_r gives the call site (CTCActionUseBase::OnActionUse) and leave rows whose call site is the other name (007edfd0).
- Overlay headers drift: other agents land header changes while you work. Re-copy the current repo header and re-apply your additions before hand-back (CTCBase.h gained FABLE_RETAIL_CTCWIFE_MARRY mid-session).
- Use `cp -f` over `rm`: the harness refuses `rm $VAR/...` paths, and an overwrite gives the same fresh copy.
