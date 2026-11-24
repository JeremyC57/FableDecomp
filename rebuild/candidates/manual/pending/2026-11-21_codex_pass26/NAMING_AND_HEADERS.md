# Naming and headers: how to get both right the first time (Codex pass 26)

A row lands only if it matches the bytes, uses the shared headers, and names everything the way the original source did. A
byte match with a wrong or made-up name is sent back. This page is the procedure. The binding rule texts are in
`docs/EXTRA_RULES.md` ("names right the first time" 2026-10-28, "stand-in names" 2026-11-05, and the 2026-11-20 / 2026-11-21 pick-any
sections). They override anything here that disagrees.

## 1. What has to be named
- The row's own function: class, namespace, method, const-ness, access, virtual / static, calling convention, return and parameter
  types. All of it is visible in the mangled `// Leaf:` symbol your object emits; confirm it with `objdump -t` or `dumpbin /symbols`.
- Every callee, global, vtable and string the row references. The linker binds them by mangled name, so a wrong spelling is a
  different symbol.
- Its locals and parameters (Ego_d gives them; see 4).
- Any class, member or enum you add to a header.

## 2. Sources, strongest first (use two for every name)
| Source | File in this package | What it gives |
|---|---|---|
| Ego_r.pdb (release PDB of the same engine) | `names/egor_pdb_names.tsv` (address, mangled name), `names/egor_obj_functions.tsv` (object file, order, length, mangled name) | exact mangled spellings; the object (TU) each function came from |
| FableWin.pdb (editor build) | `names/fablewin_pdb_names.tsv`, `names/fablewin_obj_functions.tsv` | the same, for a second build; holds many inlines Ego_r folds away |
| Ego_d.pdb (VC10 /Od debug build of the TLC source) | `rows/<addr>/egod_locals.txt`, `egod_lines.txt`, `egod_tu.txt`; `names/egod_carved_functions.tsv`, `names/egod_carved_names.tsv`, `names/egod_pdb_func_names.tsv` | local / parameter names and types, scopes, per-line sizes, TU order |
| Class layouts | `names/struct_layouts_egor.tsv`, `names/fable_types.h`, `names/pdb_class_keys.tsv` | 3,807 classes with real member names, offsets, and class vs struct keys |
| RTTI | `names/rtti-vtables.tsv` | vtable addresses and slot order (a virtual call's slot names the method) |
| What retail binds | `names/reloc_bindings.tsv` (symbol, retail target, referencing site) | the spelling each retail address is already bound to by landed rows |
| Settled spellings | `names/ESTABLISHED_SPELLINGS.tsv` | spellings the project has proven |
| Known wrong spellings | `names/NAME_FIXES_QUEUE.tsv` | landed spellings already proven wrong, with the right one; never copy the wrong one |
| Landed rows | `names/catalog_names.tsv`, `names/landed_sources.tsv`, `names/manifest_names.tsv`, `names/labels/` | hints only (see 3) |
| FSE | `names/fse_api_manifest.json` | 933 reversed script-API functions |

The PDB addresses are in THEIR builds' images, not Fable.exe's. Match a PDB function to a Fable.exe row by name, object file, TU order
(`egod_tu.txt`, `*_obj_functions.tsv` order), size, callers and strings, never by address.

## 3. Hints are not evidence
- Manifest and catalog labels such as `vfunc_0`, `sub_XXXXXXXX`, `FUN_`, `_global::Class_Method`, `_gapscan`, `CUnproven*`, `*_v3`,
  and anything with an address in it are placeholders. Most small-function labels were propagated across byte-identical bodies and
  are wrong.
- `rows/<addr>/ROW.md` flags a NAME CONFLICT where the draft's `// Leaf:` and the manifest label disagree (13 rows). Settle those
  first.
- A landed row's spelling can be wrong. Check `names/NAME_FIXES_QUEUE.tsv` for the address before reusing it.

## 4. Procedure for one row
1. Start from `rows/<addr>/ROW.md`: Ego_d name, mangled candidates from both release PDBs, queued fixes, headers.
2. Fix the row's own symbol from two sources: e.g. Ego_r + FableWin mangled names, or one PDB name plus the vtable slot, the sole
   caller's argument types, a string, or the TU position. Write it as the full mangled `// Leaf:` line.
3. For each callee: look up its retail address in `names/reloc_bindings.tsv`. If the bound spelling is right, use it exactly. If it
   is wrong, use the right one and add a NAME_FIXES.tsv line. Never bind a name to an address that already has another proven name.
4. Locals and parameters: take Ego_d's names and types (`egod_locals.txt`). A local Ego_d does not list was one expression in the
   source; an extra local contradicts Ego_d and cannot land.
5. No name anywhere after a real search (both PDBs, Ego_d, FSE, strings, the sibling sources)? Then use an accurate descriptive name in
   Lionhead style. Say what the function does (`CEngineMap_PeekWaterHasType` for a free stand-in of a member), use one spelling for
   one address, and mark it `// Descriptive:` with the search in NOTES.md. Never `T`, `Slot`, `Helper`, address-only names, or a name
   that belongs to another function.
6. STL instances: no PDB lists them. Use the STLport 4.6 spelling the package's `include/stlport` headers produce.

## 5. Headers
- `include/engine/<Class>.h` is the one model of each class: real member names and offsets from Ego_r, plus `FABLE_RETAIL_*`
  opt-in macros for retail-only inline bodies. Model `this` and every callee's class by including that header and defining the
  opt-ins the row needs. A row-local `struct`/`class` for a class that has a header is rejected.
- Find an opt-in before writing one. `ROW.md` lists each relevant header's macros; `grep -rn "FABLE_RETAIL_<CLASS>" include/`
  shows the comment above each opt-in (which row added it and why). Landed same-TU neighbours (`rows/<addr>/neighbours/`) show
  the defines and pragmas the original TU needed: copy their `#define` block first.
- A class with no header: add `include/engine/<Class>.h` from `names/struct_layouts_egor.tsv` (all members, real names, a
  `FABLE_STATIC_ASSERT` on the size), not just the members you need.
- Header changes are add-only: a new `#ifdef FABLE_RETAIL_<CLASS>_<WHAT>` block with a comment naming the row and the evidence; the
  default path is unchanged and an existing opt-in is never edited. A `FABLE_STATIC_ASSERT` whose line moves is pinned with
  `tools/lane_merge/pin_asserts.py` (VC7.1 slot order can depend on the typedef name). Hand every change back as an old_text /
  new_text pair in `hdr/PATCHES.json`, applied in order to the package's `include/`, then regrade every row on the final tree.
- Inline devices in a header (FABLE_FORCEINLINE bodies, `#pragma optimize` scaffolding) follow `docs/EXTRA_RULES.md`. Size-pragma
  scaffolding is allowed only to give a pick-any copy its owner's body, under the three conditions written there.

## 6. Pick-any copies (why pass 25 landed 2 of 5 matches)
Your object also emits COMDAT copies of inline functions it uses: STL members, small accessors, ctors / dtors. When
`names/reloc_bindings.tsv` maps such a symbol to another retail address, your copy must be byte-identical to Fable.exe at that address
(relocation fields masked). Otherwise the final link can pick your copy and break the other function. Check every exported function
COMDAT this way before you report a row matched. Pass 25 lost three matches to this: a `/Os` row emitted size-mode copies of
`Clear` / `HasTC` / `LowerBound`, and two spell rows emitted STL copies that called a copy constructor retail inlines. See
`lessons/codex_pass25_HELD_pickany.md`. Usual fixes: the same opt-ins as the owner row, a declaration-only opt-in when the row
does not expand the inline, or the size-pragma scaffolding above.
