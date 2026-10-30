# Wave 3 lessons (class-model migration, 2026-10-30)

1. **The def-chain copy members stay declared out of line (wave 2 open item 3: settled as far as RTM 3077 allows).**
   - Experiment: a temporary guard dropped the CDefClassBase / CParentDefClassBase copy declarations, so every member of the
     chain was implicit, over typed PDCB members (`CDefString { long TablePos; }`, `CVectorMap<K,V,C> : std::vector<pair<K,V> >
     { C Compare; bool Dirty; }` with `vector::operator=` declared only). The container models sit under `#pragma pack(push, 8)`:
     most rows include the chain inside their own `pack(1)`, and a packed CVectorMap is 14 bytes, so the 0x39 size assert
     fails. Sources and results: `logs/copy_experiment/`.
   - **/Os callers:** 39 of 39 tested rows that call PDCB::operator= 0x00431ED0 stay RELOCATION_MATCH on the implicit chain.
     That is 38 rows that already include the shared header, plus CSoundThemeDef 0x0045C54E, also with a fully implicit
     derived operator= reached through Copy. At /Os the two models give the same bytes.
   - **Standalone bodies (/O2):**
     - 0x00431ED0 matches as the implicit member.
     - 0x00431F60 (the 6-dword CDefPointeeBase / CResource copy) is MATCH as the implicit member.
     - 0x00431F10 does not match (102 vs 77 bytes): an implicit CDefClassBase::operator= inlines the 0x00431F60 copy, and
       retail calls it. The landed row needs an out-of-line `CDefPointeeBase::operator=` declaration.
     - Under /Os, 0x00431F10 does call 0x00431F60, but its bitfield merges come out as read-modify-write, which is not
       retail's code.
   - **/O2 callers refute the implicit model.**
     - 0x00434370 CEngineDef (implicit derived operator=, scalar members only): 153 vs 123 bytes.
     - 0x00625350 CConfigOptionsDefaultsDef: 145 vs 115.
     - RTM 3077 inlines the implicit 0x00431ED0 into both, and retail calls it.
   - **The /O2 expander goes the opposite way.** Retail 0x00432E60 CFrontEndDef (vector member) inlines 0x00431ED0 and
     0x00431F10, and calls 0x00431F60 and 0x00431F90. None of these models gets the expansion; each gives 81 bytes that call
     0x00431ED0:
     - the implicit chain, with an implicit or an explicit derived operator=;
     - explicit non-inline 0x00431ED0 / 0x00431F10 defined earlier in the same TU (same-TU auto-inlining);
     - explicit `inline` definitions;
     - `#pragma inline_depth(1|2|3)` (depth 1 makes 0x00431F10 match but then inlines it into 0x00431ED0);
     - `optimize` s/t/g;
     - the QFE 4035 compiler.
   - **Rule.** No declaration of the shared members reproduces both the /O2 calls and the /O2 expansions, because retail's
     inline choices run against RTM's heuristic. So the shared headers keep the out-of-line declarations. Every one of
     the 108 landed callers models the call that way, shared or row-local, including the 3 /O2 ones. The 5 expanders (0x00432C70, 0x00432E60, 0x00434450,
     0x004345A0, 0x0067C980) keep their row-local `__forceinline` chain as recorded residue. This is now in the header
     comments (CParentDefClassBase.h, CDefClassBase.h).
   - **`FABLE_PDCB_TYPED_MEMBERS` includers keep the declared members.** A typed-member row brings its own CDefString /
     CVectorMap definitions, inside a pack(8) region when the row is in a pack(1) region.
   - **Wave 1 open item 1** (the CScriptThing copy, 0x004ABE90, inline at one site and called at another in 00f05f30) has
     the same shape: RTM 3077 cannot express retail's mixed inlining with one declaration. It was not re-tested this wave.
     Keep the out-of-line copy in engine/CScriptThing.h. Model the inline site with the row-local `InlineCopy` tag (the
     landed 00f05f30 pattern).
2. **A copy ctor that leaves out its base initializer depended on the stand-in.**
   - The row-local `class CBaseClass { virtual ~CBaseClass(); };` has an implicit inline default ctor. So a user-written
     copy ctor of a CBaseClass subclass with no base initializer (`X(const X& o) : Members(o.Members) { }`)
     default-constructed the base inline. The compiler then removed that store, because the derived vtable store kills it.
   - On engine/CBaseClass.h, the default ctor is the out-of-line 0x0099A2D0, and the same source becomes a call: DIFFER on
     5 of 644 rows (00a5e7f0, 00ab3b40, 00ab4450, 00ab4490, 00dcf920).
   - Fix: the implicit base copy `: CBaseClass(o), ...` (`scripts/mig_bc_copyinit.py`). That made all 5 SAME, as wave 2
     lesson 1 predicts.
   - Run the script on every BC batch before mcheck: it changes nothing where the base is already copied.
3. **Dtor-only and inline-dtor CBaseClass stand-ins are byte-neutral on the header.**
   - 461 b638a797 rows plus the 5 above, and 178 inline-dtor rows (77992de8 96, 62d3d4ae 82), come back SAME.
   - The inline `virtual ~CBaseClass() { }` becoming the out-of-line 0x0099A2E0 never mattered: no such row destroys a
     CBaseClass subobject in a body the row emits.
   - 194 exact-shape rows: 194 SAME.
4. **RELOC_RENAMED that is not a rename.** 6 rows differ from HEAD only in names the compiler makes up; the bytes are the same:
   - 4 have `$L` jump-table labels shifted by the header's line count (0072acf0, 007ae3d0, 007c89f0, 007c5db0).
   - 2 have a changed anonymous-namespace hash (`?A0x4eb0b7e3` -> `?A0x92c0829b` in 008091a0, `?A0xbc671d89` ->
     `?A0x601728f1` in 00870fc0). The hash follows the source text.
   - Neither hash is in rebuild/oracles/reloc_bindings.tsv, so the bindings are unchanged. vland grades them (ROWS.tsv).
5. **Lane cost.**
   - mcheck -j 3 runs at about 2 s a row (two compiles each).
   - vland --recheck-landed runs at about 10 s a row (6 rows a minute) on the shared 4-core lane while chunk 582's agent runs.
   - A 420-row regrade batch therefore takes about 70 minutes; plan wave 4 at 2 batches per 800 rows.
