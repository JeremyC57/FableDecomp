# agentNF5 lessons (name-fix wave, 2026-11-10)

1. **Source wchar spelling from the owner decision, not the PDBs.** Both PDBs are VS2010-era builds, so their `_W`
   (native `wchar_t`) is that compiler's default. TLC is built without `/Zc:wchar_t` and the wide text stays
   `unsigned short` (owner, 2026-10-29). So every `PB_W` / `_W` queue spelling has a `PBG` / `G` twin that is the right
   TLC name. Reject the `_W` form, apply the `G` twin, and add no `/Zc:wchar_t` flag.
2. **STLport members are named from STLport, not from the PDBs.** Ego_r and FableWin list Dinkumware members
   (`_Tidy`, `_Buy`, `_Vector_iterator` erase). Retail's own TU uses STLport 4.6 forms, so they are not the spelling to use:
   - `resize(size_type, const T&)` by const reference;
   - `erase(T*, T*)`;
   - `_M_fill_insert`.

   The second source for those is a RELOCATION_MATCH binder row plus the STLport source.
3. **The shared STLport model often matches better with the body visible than declaration-only.**
   - 0x005F4B30: with STLport's own `push_back` body visible, VC7.1 still calls it out of line, but now it knows the
     null-def padding temporary is not captured. The temporary's release then folds away, as in retail.
   - With `FABLE_RETAIL_STLP_VECTOR_PUSHBACK_OUTOFLINE` (declaration only), the release stays in and the row is
     DIFFER(300v277).

   Try the plain header before an out-of-line opt-in.
4. **A tail jump that forwards a stack argument needs a named local.** At 0x005BF2ED, folding
   `Find(...)->IsSpaceInCategoryForItem(arg)` into one expression makes VC7.1 pre-push `arg` and `call`, giving
   DIFFER(36v32). Writing `CCategory* category = Find(...); return category->IsSpaceInCategoryForItem(arg);` gives
   retail's `pop esi; mov ecx,eax; jmp`.
5. **`__fastcall` on a static member works under VC7.1** (the mangled name is `SI...`). A `push ecx` right after
   `mov ecx,[esp+8]` is not the convention failing: VC is pre-pushing the argument for a later call.
6. **A ctor that calls the grandparent ctor means the middle ctor was expanded inline.** At 0x00F336F0,
   `??0CConversation` calls `??0CDefClassBase@@AAE@XZ` (0x0042D9E0) directly, with no CSubDefClassBase vtable store.
   CSubDefClassBase's ctor was inline in that TU and its vtable store was dead. The out-of-line copy (0x0044C0C0) is
   for the other TUs. Model this with opt-ins on the headers; do not reparent the class.
7. **Under Wine, a header `#error` can hang `cl.exe` until the grader timeout.** It does not fail fast. When a
   grade "hangs", run `g++ -fsyntax-only -I<overlay> -Irebuild/include row.cpp` first. The missing opt-in define shows in
   about a second (CDefString.h needs `FABLE_RETAIL_CDEFSTRINGTABLE` + `FABLE_RETAIL_CCHARSTRING`).
8. **Self-row leaf selection.** When a row's out-of-line helper COMDAT has the same bytes, the grader's leaf
   (from the catalog name) can pick the wrong symbol. An example is the `CSmallVector` dtor beside `~CThingComponentSet`.
   Always pin `// Leaf:` to the new spelling, then check that the `SELF` symbol is the queued one.
9. **Batch transforms need guards.** For the families of the same call spelled wrong in many rows:
   - Regexes must skip `//` lines.
   - Restore the original access after inserting a `public:`.
   - Compare each row's full binding set against the pre-change scan (NEWBAD). A row whose test already failed the
     same way before the change is kept only as `PREFAIL_TEST` and listed as needing a portable test.
10. **The anonymous-namespace hash is not reproducible.** `?A0xacdf2dc5` is the hash VC7.1 derived from the original
    TU. A rebuilt TU gets a different `?A0x...`, so a queue spelling that includes it cannot be bound byte-for-byte by
    any row (0x008E8A10). Name the class, and let the coordinator decide how such symbols are compared.
11. **Two labels at one CRT address are not a rename.** `__chkstk` and `__alloca_probe` are one routine in
    chkstk.obj. Its grade is `toolchain_library`.
