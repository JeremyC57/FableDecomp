# Naming agent FAM09: the 399 `_static_init::g_*` placeholder rows

The user, 2026-11-11: naming goes one family per agent. Your family: every landed row whose catalog name is `_static_init::g_<Type>_<addr>`
(ROWS.tsv in your directory: address, catalog name, length; 220 g_CCharString, 28 g_cos, 26 g_sin, 18 g_CWideString, 17 g_scaled,
15 g_tree, 9 g_list, and a few CStatic / one-offs). Each is a compiler-generated `??__E` dynamic initializer of one static-storage
object; the placeholder is the OBJECT's name (`g_<Type>_<object address>`). Give each object its real name and respell its row.

**Model:** agent NF22 already did this for some rows. Read rebuild/src/compiled/01/21/staticinit_STHEADTURNH_0121eba0.cpp (was
g_CCharString_013bace4): catalog `_static_init::ST_HEAD_TURN_H`, leaf `??__EST_HEAD_TURN_H@@YAXXZ`, a `// Names (...)` comment citing
the sources, and the definition respelled with the real name and type qualifiers (`extern const CCharString ST_HEAD_TURN_H(...)`).
Grep rebuild/src/compiled for `Names (agentNF22)` for more examples, and keep that file-name / comment / catalog convention.

**Name sources (two needed, exact spelling):**
1. FableWin.pdb: ghidra_out/_fablewin_api.tsv rows `` `dynamic_initializer_for_'NAME'' `` (2,187; namespaced ones carry their
   namespace, e.g. `NStreamingFontBankGlobals::`, and `` `anonymous_namespace' ``), also `dynamic_atexit_destructor_for_`.
2. Ego_r.pdb: ghidra_out/egor_pdb_names.tsv `` `dynamic initializer for 'NAME'' `` (714), ghidra_out/egor_signatures.tsv,
   ghidra_out/egor_srcfiles.tsv (source file per address).
3. Third source (private, never committed, never copied into the repo): the Fable Anniversary Xbox 360 "Test" PDB, repacked for
   llvm-pdbutil at $S/anniv_dbg/test4k.pdb. Pre-dumped: $S/anniv_dbg/publics.txt (S_PUB32 with full mangled names and types, e.g.
   `?ST_HEAD_TURN_H@@3VCCharString@@B` = const CCharString) and $S/anniv_dbg/modules.txt (module list). Per-module symbols with
   source lines: `llvm-pdbutil dump -symbols -modi <N> $S/anniv_dbg/test4k.pdb` (find N in modules.txt; Fable code is in unity
   modules `Unity_*EtAl.cpp` with FableLib / FableEngine3D / ... object paths). Usage rule (docs/re_reference/FABLE_ANNIVERSARY.md):
   it confirms only when it equals FableWin or Ego_r; TLC wins on a difference; cite it as "Anniv360 Test PDB (third source)".
   It IS the best source for the object's type/qualifiers (const, namespace) when FableWin/Ego_r agree on the name.
**Mapping retail rows to names:** FableWin/Ego_r addresses are other builds, so map by evidence: (a) the CCharString / CWideString
literal the initializer passes (read it from the exe: `python3 $S/w17/xdis.py <addr> <len>` and the string at the pushed address;
the exe is "$S/dl/game/Fable The Lost Chapters/Fable.exe") -- very often the object's own name, but prove it against the PDB lists;
(b) the order of initializers inside one translation unit: retail kept the TU order, so a run of unnamed rows between named neighbours
(already-landed staticinit rows) maps onto the same run in FableWin's / Ego_r's address order for the same source file; (c) the
users of the object (`python3 $S/w23/xref.py <object va>`: which landed functions read it, e.g. a sin/cos table read by one class).
The g_cos / g_sin / g_scaled float rows and g_tree / g_list container rows need (b)/(c). A row you cannot name from two sources stays
as is (list it in NOTES.md with what you found); never invent a name for a global that has a real one somewhere.

**What a rename touches:** the row's source (definition name + comment), its leaf, RENAMES.tsv section 1 (module `_static_init`,
name = the object's name, exactly as the landed ST_HEAD_TURN_H row), and any landed row that declares the object under its old
placeholder name (`grep -rn "g_CCharString_013bace4" rebuild/src rebuild/tests`): respell those callers too (rows/<addr>.cpp, and
RENAMES.tsv section 2). Each changed row must still grade MATCH / RELOCATION_MATCH, and its test must still PASS (update the test
when it names the object).

Repo: /home/user/FableDecomp (read-only for you). Scratch: $S = /tmp/claude-0/-home-user-FableDecomp/40effb6c-294b-5111-9a88-ca0c32f095ec/scratchpad.
Your folder is $S/w22/agentFAM09/. Read CLAUDE.md and tools/decomp_pipeline/linux_lane/lane_tools/chunk_pool/EXTRA_RULES.md first.
The repo is committed and current (HEAD).

## What to do for each entry
1. **Verify the claim.** An entry is a CLAIM. Check it against two sources: the PDB name sets (ghidra_out/egor_pdb_names.tsv,
   fablewin_pdb_names.tsv, _fablewin_api.tsv), the callee body, vtable slots or the binding evidence. A claim that does not hold goes to
   REJECTED.tsv with the reason.
2. **Find every landed binder of the wrong spelling.** Grep the retail target in rebuild/oracles/reloc_bindings.tsv (symbol, target,
   first caller), and grep the wrong spelling's pieces in rebuild/src/compiled. reloc_bindings lists only the FIRST caller of each symbol,
   so the source grep is what finds the rest.
3. **Respell each binder's declaration or call** to the correct symbol, using the shared header declaration where one exists, with an
   `#ifdef` opt-in if the header lacks it. Grade every changed row: it must stay MATCH or RELOCATION_MATCH. Run its test, and keep the
   catalog PassPattern.
4. **When the wrong name is a landed row's own name** (all 399 rows here), respell that row too and list it in
   RENAMES.tsv section 1 (module = class, name = method only; never `Class::Method` in the name column).
5. **Rows reached through stand-in classes** (address-suffixed classes, `_unattributed`, `Row00c...`): respell them only if the correct
   class is proven and a shared header exists. Otherwise record the gap.

## Hard rules
- Never run git in any form, not even read-only commands. Never write under the repo. Never land anything.
- Every changed row must still grade MATCH or RELOCATION_MATCH.
- No `__asm` / `naked` / `_emit` / `register` / bare `__forceinline` in rows or tests. Use `volatile` only where retail used it.
- Use shared headers over local models, and add no NEW local models of classes that have a header.
- Header changes go in $S/w22/agentFAM09/hdr/<path>: a full copy of the CURRENT repo header with `#ifdef` additions only. Grade with
  FABLE_HDR_OVERLAY=$S/w22/agentFAM09/hdr.
- Use exact PDB names: one address, one name, and no made-up names.
- Do not touch the addresses in BUSY.txt (every RESERVED row) or any pending/*/RESERVED.tsv address.
- Run at most one compile at a time (another agent may compile in parallel), inside `timeout 240`. Wine cl can hang on `#error`.
- Make at most 10 grades per row, counting failed compiles.
- Every Bash call must exit 0: end probe, grep and diff pipelines with `; true` (user rule). A grading script prints its result as text.

## Lane
Every Bash call: `export S=...; source $S/wine/env.sh; export S; cd /home/user/FableDecomp`.
Grade: `timeout 240 python3 $S/tools/q.py <addr> <file.cpp> '<full mangled leaf>' [--flags ...]`, taking the flags from the row's
`// Flags:` line. The oracle comes from the catalog. Then `objdump -dr $S/wine/q/<addr>/<addr>.obj` shows which symbol each relocation
binds. Binding pairs: tools/decomp_pipeline/reloc_identity.py `check()`.

## Hand-back (in $S/w22/agentFAM09/)
- rows/<addr>.cpp for every changed row, plus rows/<addr>_test.cpp if its test changed.
- hdr/ and hdr/README.md.
- RENAMES.tsv: section 1 header exactly `address\tmodule\tname\tmangled_leaf\told_catalog_name\told_leaf\tevidence`, for rows whose own
  name changes. Then a `# Section 2` line, followed by caller respellings as address, old_symbol, new_symbol.
- STATUS.tsv: address, change, grade, test, grades_used, note.
- APPLIED.tsv: each queue entry with done / partial / rejected and the count of binders respelled.
- REJECTED.tsv, NOTES.md, LESSONS.md (one line each).

Report briefly at the end: entries applied and rejected, rows changed, rows left and why, header opt-ins, and any rule slip.
