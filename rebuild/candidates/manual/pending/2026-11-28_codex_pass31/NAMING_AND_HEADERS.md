# Naming and headers: how to get both right the first time (Codex pass 30; names: docs/NAMING_FIRST_TIME.md overrides this file where they differ)

A row lands only if it matches the bytes, uses the shared headers, and names everything the way the original source did. A
byte match with a wrong or made-up name is sent back. This page is the procedure. The binding rule texts are in
`docs/EXTRA_RULES.md` ("names right the first time" 2026-10-28, "stand-in names" 2026-11-05, and the 2026-11-20 / 2026-11-21 pick-any
sections). They override anything here that disagrees. Section 9 is new in pass 29: the checks the coordinator's name gate runs on every row, and the lessons since pass 28.

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
| Ego_r.pdb (release PDB of the same engine) | `names/pdb/egor_pdb_names.tsv` (address, mangled name), `names/pdb/egor_obj_functions.tsv` (object file, order, length, mangled name) | exact mangled spellings; the object (TU) each function came from |
| FableWin.pdb (editor build) | `names/pdb/fablewin_pdb_names.tsv`, `names/pdb/fablewin_obj_functions.tsv` | the same, for a second build; holds many inlines Ego_r folds away |
| Ego_r.pdb / FableWin.pdb procedure records (new in pass 27) | `rows/<addr>/ego_r_proc.txt`, `rows/<addr>/fablewin_proc.txt` (96 rows); `names/pdb/Ego_r.modules.txt`, `names/pdb/FableWin.modules.txt`; the PDBs themselves via the links in README_CODEX.md (`tools/repack4k.py`, then `llvm-pdbutil`; `tools/pdb_proc.py` for any other function) | the function's full signature (`type = ...`), parameter and local names with their types, `S_CALLSITEINFO` signatures of indirect / virtual calls. Both are VS 2010 (cl 16.00.40219) builds (Ego_r game release, FableWin editor debug): code sizes, frame offsets and registers are not retail's, and a local Ego_r lacks was optimised away (Ego_d decides locals) |
| Ego_d.pdb (VC10 /Od debug build of the TLC source) | `rows/<addr>/egod_locals.txt`, `egod_lines.txt`, `egod_tu.txt`; `names/pdb/egod_carved_functions.tsv`, `names/pdb/egod_carved_names.tsv`, `names/pdb/egod_pdb_func_names.tsv` | local / parameter names and types, scopes, per-line sizes, TU order |
| Class layouts | `names/pdb/struct_layouts_egor.tsv`, `names/pdb/fable_types.h`, `names/pdb/pdb_class_keys.tsv` | 3,807 classes with real member names, offsets, and class vs struct keys |
| RTTI | `names/pdb/rtti-vtables.tsv` | vtable addresses and slot order (a virtual call's slot names the method) |
| What retail binds | `names/state/reloc_bindings.tsv` (symbol, retail target, referencing site) | the spelling each retail address is already bound to by landed rows |
| Settled spellings | `names/answers/ESTABLISHED_SPELLINGS.tsv` | spellings the project has proven |
| Known wrong spellings | `names/state/NAME_FIXES_QUEUE.tsv` | landed spellings already proven wrong, with the right one; never copy the wrong one |
| Landed rows | `names/hints/catalog_names.tsv`, `names/hints/landed_sources.tsv`, `names/hints/manifest_names.tsv`, `names/hints/labels/` | hints only (see 3) |
| FSE | `names/pdb/fse_api_manifest.json` | 933 reversed script-API functions |

The PDB addresses are in THEIR builds' images, not Fable.exe's. Match a PDB function to a Fable.exe row by name, object file, TU order
(`egod_tu.txt`, `*_obj_functions.tsv` order), size, callers and strings, never by address.

## 3. Hints are not evidence
- Manifest and catalog labels such as `vfunc_0`, `sub_XXXXXXXX`, `FUN_`, `_global::Class_Method`, `_gapscan`, `CUnproven*`, `*_v3`,
  and anything with an address in it are placeholders. Most small-function labels were propagated across byte-identical bodies and
  are wrong.
- `rows/<addr>/ROW.md` flags a NAME CONFLICT where the draft's `// Leaf:` and the manifest label disagree (7 rows in pass 27).
  Settle those first.
- A landed row's spelling can be wrong. Check `names/state/NAME_FIXES_QUEUE.tsv` for the address before reusing it.

## 4. Procedure for one row
1. Start from `rows/<addr>/ROW.md`: Ego_d name, mangled candidates from Ego_r and FableWin, their procedure records
   (`ego_r_proc.txt` / `fablewin_proc.txt`: signature and parameter types), queued fixes, headers.
2. Fix the row's own symbol from two sources: e.g. Ego_r + FableWin mangled names, or one PDB name plus the vtable slot, the sole
   caller's argument types, a string, or the TU position. Write it as the full mangled `// Leaf:` line.
3. For each callee: look up its retail address in `names/state/reloc_bindings.tsv`. If the bound spelling is right, use it exactly. If it
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
- A class with no header: add `include/engine/<Class>.h` from `names/pdb/struct_layouts_egor.tsv` (all members, real names, a
  `FABLE_STATIC_ASSERT` on the size), not just the members you need.
- Header changes are add-only: a new `#ifdef FABLE_RETAIL_<CLASS>_<WHAT>` block with a comment naming the row and the evidence; the
  default path is unchanged and an existing opt-in is never edited. A `FABLE_STATIC_ASSERT` whose line moves is pinned with
  `tools/lane_merge/pin_asserts.py` (VC7.1 slot order can depend on the typedef name). Hand every change back as an old_text /
  new_text pair in `hdr/PATCHES.json`, applied in order to the package's `include/`, then regrade every row on the final tree.
- Inline devices in a header (FABLE_FORCEINLINE bodies, `#pragma optimize` scaffolding) follow `docs/EXTRA_RULES.md`. Size-pragma
  scaffolding is allowed only to give a pick-any copy its owner's body, under the three conditions written there.

## 6. Pick-any copies (why pass 25 landed 2 of 5 matches; the pass 26 landing checked 482 copies)
Your object also emits COMDAT copies of inline functions it uses: STL members, small accessors, ctors / dtors. When
`names/state/reloc_bindings.tsv` maps such a symbol to another retail address, your copy must be byte-identical to Fable.exe at that address
(relocation fields masked). Otherwise the final link can pick your copy and break the other function. Check every exported function
COMDAT this way before you report a row matched. Pass 25 lost three matches to this: a `/Os` row emitted size-mode copies of
`Clear` / `HasTC` / `LowerBound`, and two spell rows emitted STL copies that called a copy constructor retail inlines. See
`lessons/codex_pass25_HELD_pickany.md`. Usual fixes: the same opt-ins as the owner row, a declaration-only opt-in when the row
does not expand the inline, or the size-pragma scaffolding above.
Pass 26 did this check on every row: 475 copies equalled Fable.exe at the table's address, the 7 that did not were all stale table lines
(each equal to retail at the corrected address Codex handed back), and two rows (0x00EB5750, 0x009F3DA0) stayed HELD_PICKANY.

## 7. Pass 26 landing lessons (`lessons/codex_pass26_landing_README.md`: 95 of 100 landed, 327 header patches)
- **Retail-asm helpers need their evidence lines.** A row that calls a shared retail-asm helper (`engine/GFFloatToLongFloor.h`,
  `GFFloatToLongNear`, `GFFPUSetDoublePrecision`, `GFFPUSetRoundingNearest`, ...) needs `// Retail-asm-helper: <name>` and, for each
  site, `// Retail-asm: 0x<VA> `<instruction>` ...` citing the retail instruction there (0x prefix, a mnemonic of the helper: `fistp`,
  `fnstcw` / `fldcw`). Without them `crawl/purity.py` grades the row `naked_asm`; 0x00A9DC20 (three `fistp` sites) and 0x00417747 (two
  fnstcw / fldcw pairs) had to be fixed at landing. `docs/ASM_RULES.md` is the rule.
- **The catalog name comes from the verified leaf.** Where the parked label is a placeholder, a VC10 Ego_d template name (`_Copy_if`
  for STLport `std::copy_if`) or a mangled string, say the leaf-derived `module::name` in NOTES.md. 44 rows were renamed at landing.
- **Header patches: unique, short old_text.** Make every `old_text` unique in its file and as short as possible: one landing (nf214)
  had inserted a block between a pass's two anchor lines, and that patch had to be re-anchored by hand. When a patch moves a pinned
  `FABLE_STATIC_ASSERT_AT(<old line>, ...)`, give the re-pinned assert lines yourself, including the second copy of a duplicated assert
  expression (CNavigatorBase.h was mis-pinned to the first block's lines; CEngineTextureDecal's `AT(12)` belonged to line 11).
- **Strong same-TU copies are kept and reported.** When your object carries a strong definition of another function of the same TU
  (equal bytes, the nf96a / nf218 TU-mate kind), keep it and report it in STATUS.tsv with the owner's retail address and whether it
  equals retail there. A whole extra function that matches retail is worth reporting as such: pass 26 found FractureShape
  0x00763060 (inside 0x007634E0's TU, 1146 B; now queued as its own row) and SetCameraState 0x00B30B50 (inside 0x00B31160's; not
  landed yet).
- **BINDING_EVIDENCE.json / PICKANY_CORRECTED.txt worked well: keep doing it.** Where `names/state/reloc_bindings.tsv` has a stale line
  (a pick-any copy differs from Fable.exe at the table's address but equals it at the right one), hand back the corrected address
  with the evidence. Pass 26's seven pick-any NEQs were all stale table lines of this kind, and the landing queued every correction.
- Not landed in pass 26 (IN_PROGRESS / HELD_PICKANY; `lessons/p26_open_notes/`): 0x00BAA870, 0x005F59C7, 0x00723E50, 0x00EB5750,
  0x009F3DA0. Their NOTES.md files show what held them.

## 8. Pass 27 landing lessons (`lessons/codex_pass27_landing_README.md`: all 100 landed, 354 header patches)
- **Gate EVERY edit to an existing header line.** Two pass-27 patches changed a shared inline for every file that includes the header,
  not just for the rows that needed it:
  - an unconditional `FABLE_RETAIL_ASSERT_ARM(index != 0);` in `CBankFile::GetEntryDataType` stopped 12 landed rows compiling (they do
    not include `fable_assert.h`), and no row needed it;
  - an unconditional declaration-only `CTCPerceiveThing::IsAwareOfThing` changed the bytes of 0x0091B3A0 and 0x009578A0.
  Both are now gated under the one row's opt-in. Before handing back, run `tools/lane_merge/equiv_check.py` (HEAD include v your final
  include, every landed row via `names/hints/landed_sources.tsv`) and recheck every landed row whose preprocessed text changes on your tree. A
  differing row that you did not mean to touch means a patch is missing its `#ifdef`.
- **`// Flags:` is read by the landing.** A row compiled with extra flags (`/TC /MD /Gr /Ob1` on 0x00C95580, `/d2inlT<N>`, `/GS`,
  `/Os`) must carry exactly those flags on its `// Flags:` line; `default` or an empty line means `/O2 /Oy /W3` and nothing else.
- **Inline devices need their `// Inline-budget:` line and an ablation.** 16 pass-27 rows used `#pragma optimize` / `inline_depth` /
  `auto_inline`, `__declspec(noinline)` or FORCEINLINE opt-ins without that line (`lessons/codex_pass27_inline_devices_without_marker.txt`).
  For each device, say in the line what it does and what each flag you tried first gave (per-row flags come before source devices:
  0x00A90AE0's `/d2inlS50..300` fixed one copy and broke three others, so its header `__forceinline` stayed). A size pragma that only
  restores the original TU's `/Os` default is scaffolding; mark it `// Fidelity: scaffolding, not retail` (EXTRA_RULES 2026-11-20).
- **A bare `__forceinline` in a header must be inside `#if defined(_MSC_VER)`** with a plain `inline` twin, or spelled
  `FABLE_FORCEINLINE` in a header that includes `fable_compiler.h` (stlport/fable_containers.h includes it only conditionally).
- **Binding conflicts are evidence: report them.** Codex was right that IsEventButtonHeld / IsEventButtonReleased were bound swapped:
  0x00687B30 tests every Ego_r EInputEvent `IE_*_HELD` code, 0x00687BD0 every `IE_*_RELEASED` one. The landing swapped them across 70
  files. When your row needs a spelling the table binds elsewhere, hand back NAME_FIXES.tsv with two sources rather than following the
  table.
- **Second spellings are held.** 54 of the lander's new reloc_bindings lines would have given an address a second name (mostly Codex's
  PDB-exact access / const forms beside older landed ones). They are held until the older binders are respelled, so cite them in
  NAME_FIXES.tsv instead of relying on them.
- **Two grading views of one TU are pair debt, not an error** (0x00628000 / 0x00627BE0): say so in STATUS.tsv and name the file to
  build once.

## 9. Pass 29: the coordinator's name / header gate, and the lessons since pass 28
Every row is run through `name_gate.py` before it lands; a copy is in `tools/reference/name_gate.py`. It runs in the coordinator's lane
and is not standalone, but you can do each check by hand. It compiles the row and reads every name the object uses.

**What it checks:**

1. **The row's own symbol** must be the full mangled `// Leaf:` symbol, with class, access, const and calling convention spelled.
   A short or plain leaf fails. The graded function must BE that symbol.
2. **Every relocation's symbol, at the retail address it lands on:** each call, global, vftable and string.
   - The gate adds your spelling to the spellings landed rows already bind at that address (`names/state/reloc_bindings.tsv`,
     `names/state/self_symbols.tsv`) and asks what the naming tracker would say.
   - **OK:** one spelling, and it is a PDB name (Ego_r / FableWin / Ego_d) or a std / compiler / C-library name. Also OK: the address
     was already open and your spelling is its PDB name.
   - **FAIL:**
     - a stand-in (`struct T`, `_<addr>`, `FUN_`, `DAT_`, `Unproven`, `vfunc`);
     - a second spelling at an address that already has another, unless yours is the PDB name;
     - a non-PDB spelling not marked `// Descriptive:`.
   - **DEBT (allowed):** reusing a non-PDB spelling landed rows already bind where no PDB name exists. If you can find the PDB name
     from two sources, use it and add a NAME_FIXES.tsv line instead.
   - `names/state/OPEN.tsv` lists every address the tracker counts open and why. Look up your callees there.
3. **Headers.**
   - **SHARED_HEADER_AVAILABLE:** the row or its test defines a class that has an `include/` header instead of including it.
   - **LOCAL_CLASS:** the row defines any other class or struct body. Add the class's header instead (section 5). A type retail
     really defined inside that TU (evidence: the PDB module lists it, or an anonymous namespace) is the only exception; cite the
     evidence.
4. **Code rules:**
   - NOT_GENUINE: `__asm`, `naked`, `_emit`;
   - BARE_FORCEINLINE: use `FABLE_FORCEINLINE`;
   - BAKED_STRING / BAKED_ADDRESS: a retail string or data address typed in as an integer (`names/state/data_vas.tsv` lists the data
     addresses).

**Lessons since pass 28:**
- **A vftable your object emits must name retail's functions slot by slot** (nf425). Giving a class its real virtuals makes VC7.1
  emit `??_7<C>@@6B@`, and every slot binds a symbol.
  - If the class model lacks an override retail has, the slot binds the base function. Typical cases: the class's own `Initialise`,
    `FinishAction`, `IsToFaceTarget` overrides on CCreatureAction / CCombatAction classes.
  - Count WRONG slots, not just agreeing ones. `tools/reference/vtw425.py` shows the method: read retail's dword at each index, and
    resolve each slot's symbol through `reloc_bindings.tsv` / `self_symbols.tsv`.
  - Report the result in STATUS.tsv.
- **The FableWin /Od body is the best statement-order source** (AUTH26, 0x006C0B20). `tools/fwdis.py FableWin.exe <va>` disassembles
  it, and `tools/fwsym.py FableWin.exe names/pdb/FableWin.publics.txt <call targets>` names every callee in the exact PDB spelling. Its
  pieces line up with Ego_d's line table. Use the FableWin address from `names/pdb/fablewin_pdb_names.tsv`.
- **A masked operand is not proven by the grade.** The grader masks relocated operands, so a wrong float constant, string or global
  still grades RELOCATION_MATCH. Pin each one in the behaviour test (AUTH26: a 20.0f mutant of a 400.0f compare graded the same).
- **Template instances get their full PDB spelling.** A pick-any template copy in your object (e.g. `CThingSearchTools::GetAllThings`
  with its filter type) is bound by the gate like any callee: use the spelling FableWin / Ego_r publics give, not a stand-in class.
- **Inline flags.** `/d2inlS<N>` needs no `/Og`. Report the smallest N that matches; nf424 r4 swept 17-40 and landed 20.
- **Do not fit a device to one row's residue.** Row 0x00723E50's last two instructions are a scheduler-cycle effect. Four rounds of
  hypotheses consistent with Ego_d did not move them (`rows/00723e50/auth26/TERMINAL.md`). A device that fixes only them
  (a dead tuple, a fake dependence) is not reconstruction. Report the residue instead.
