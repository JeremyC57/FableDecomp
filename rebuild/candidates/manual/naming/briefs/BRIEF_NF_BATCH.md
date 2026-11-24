# Naming batch agent brief (tracker batches T1 / T2), consolidated 2026-11-23

This is the standing brief for the naming-tracker batch agents (NF28x and later). It replaces the scratch-only chain (NF214_BRIEF.md,
NF278 / NF282 / NF283 briefs) that the 2026-11-23 container restart wiped. Each agent's own `BRIEF.md` is a short prefix (scope, order,
new rulings) plus a pointer here; where the prefix and this file disagree, the prefix wins.

Owner (2026-11-22): "make it easier to track our naming progress ... and batch items that are easier to work together".
Owner (2026-11-23): "continue where the naming queue batches left off before reset".

## Paths
- `S=/tmp/claude-0/-home-user-FableDecomp/5221bbec-fb18-575f-8649-a32d96bb1c2d/scratchpad`; `source $S/wine/env.sh` in every Bash call
  (Wine prefix, `FABLE_VC71` = cl 13.10.3077, `FABLE_EXE` = retail Fable.exe, `/run/user/0`, a persistent wineserver).
- Your folder: `$S/w22/agentNF<nn>/` (rows/, hdr/, orig/, evidence/, work/, tools/). Locks: `$S/w22/nflocks/<addr>/owner`.
- Shared toolkit (repo, read-only for you; copy into your tools/ if you change it): `tools/decomp_pipeline/linux_lane/lane_tools/naming_batch/`:
  `lock.sh <AGENT> <addr> [release]`, `clean.py <your folder> <paths...>`. Write every other helper you need (precheck, objsec /
  cmpobj object diff, link check, vtable map, census, xsites) into YOUR `tools/` and hand it back: the coordinator commits useful ones
  to the toolkit so the next reset does not lose them.
- Donor debug builds are on the lane since 2026-11-23: `$S/pdb/debug tlc/` (FableWin.exe/.pdb, Ego_d.exe/.pdb, ego_r.exe, Ego_r.pdb).
  Read them for evidence (call order, call counts, which overload a caller binds); NEVER copy, commit or quote their bytes or the PDBs.
- PDB name sets in the repo: `ghidra_out/egor_pdb_names.tsv`, `ghidra_out/fablewin_pdb_names.tsv`; per-object function lists
  `ghidra_out/egor_obj_functions.tsv`, `ghidra_out/fablewin_obj_functions.tsv`; types `ghidra_out/struct_layouts_egor.tsv`.
- Tracker: `tools/decomp_pipeline/naming_status.py`, output `rebuild/candidates/manual/naming/status/` (SUMMARY.md, OPEN.tsv columns:
  target status tier fix klass n_spellings pdb_spelling self_leaf landed catalog source spellings). Table: `rebuild/oracles/reloc_bindings.tsv`
  (symbol, target, witness). Catalog: `rebuild/build_candidates.ps1` (find a row's files BY ADDRESS there, never by a remembered path).
- Queues: `rebuild/candidates/manual/naming/NAME_FIXES_QUEUE.tsv`, `rebuild/candidates/manual/naming/T3_PORT_QUEUE.tsv` (ports: not yours).

## Reading list (in full, before your first compile)
This file; your BRIEF.md prefix; CLAUDE.md; docs/pipeline/ASM_RULES.md; tools/decomp_pipeline/linux_lane/lane_tools/chunk_pool/EXTRA_RULES.md;
docs/COORDINATION.md; docs/pipeline/GOTCHAS.md (all of it); the landing READMEs of the batches before you
(`rebuild/candidates/manual/batches/2026-11-22_landing_nf281_282/`, `_nf283/`, `_nf284/`, `_nf285/`) and the hand-backs they point to
(`rebuild/candidates/manual/pending/<date>_nf<NNN>_handback/`, e.g. `2026-11-23_nf296_handback/`: NOTES.md, LESSONS.md, STATUS.tsv, evidence/).

## Hard rules
- Never write into /home/user/FableDecomp; never run git, not even read-only. Write only inside your folder.
- NEVER any sleep and NO foreground waits or links: launch grades / links / `cl /Zs` in the background (`nohup ... &`, save the PID),
  check them with `kill -0 <pid>` in a LATER call. Never `pgrep -f`, `pkill`, `killall`, `wineserver -k`; kill only PIDs you started.
- Every grade has `timeout 900` ON ITS OWN LAUNCH LINE (no outer timeout around a batch).
- Every source a grade compiles (unchanged HEAD sources and tests included) has its own passing precheck in an EARLIER call, after its
  lock is yours. Precheck = clang `-fsyntax-only` (write your own `tools/pre.sh`; `NOWCHAR=1` / `-Xclang -fno-wchar` matches VC7.1's
  default wchar_t and is accepted). Ruling D1: when clang fails ONLY inside toolchain headers (VC7.1 CRT / STLport / `<cwchar>` /
  iostream / `_assert` / `short wchar_t`) or on a shared-header FABLE_STATIC_ASSERT typedef redefinition, a VC7.1 syntax-only compile
  (`cl /Zs` with the row's flags and includes, background, `timeout 900`) passing in an earlier call counts. Record which check each source
  passed in work/pre_*.log. Precheck a row's landed test BEFORE you claim its lock.
- Claim a lock with `lock.sh` before ANY compile of that row (controls included); claim only rows you will compile; a lock held by
  another agent means skip that address and note it. Release every lock you no longer need (`lock.sh <AGENT> <addr> release`).
- NEVER `rm` / `rm -f` / `rm -rf` any variable path, not even a symlink in your own folder: use `clean.py`.
- Quote every heredoc delimiter (`<<'EOF'`). Absolute paths in every precheck.
- No `__asm`, `naked`, `_emit`; `volatile` only where retail used it. Any edit to an existing SHARED HEADER line goes under an opt-in macro
  with an equiv_check proof (`tools/decomp_pipeline/linux_lane/lane_tools/lane_merge/equiv_check.py old_tree new_tree out.tsv`, with S
  pointing INTO your folder, plus a planted-token control that must flag). Add-only opt-ins: declarations only, both PDB name sets agree,
  0 differing includers.
- No retail byte dumps (or donor-build bytes) in hand-back files. Names from two sources (both PDB name sets, or one PDB plus the retail
  call site / donor call); one retail address, one name, in the exact PDB spelling. Census every new symbol NAME (grep the landed sources
  and objects) before handing back: a same-TU helper under the new name with another body is a conflict.
- Stay off: other agents' entries and locks; the Codex pass 28 reserved rows (`pending/2026-11-22_codex_pass28/RESERVED.tsv`); held rows
  0x00B4D370 / 0x00BD3070, 0x00890250, 0x0091C080, 0x0052C7E0, 0x00A254A0, 0x00ADC190 / 0x00ADC250 / 0x00ADC4E0 / 0x00ADCB90 /
  0x00ADCE60, 0x00BE8E60 / 0x00BFBD80 / 0x00BE6090 / 0x00BFB590; everything in T3_PORT_QUEUE.tsv (unless your prefix sends you there to re-read).
- Disk is a fixed allowance: keep work/ small, delete build dirs with clean.py when done.

## What a fix is
An entry is an address whose PDB spelling (`pdb_spelling`) differs from what some landed row binds or defines. Fix in place, in each
row's OWN local model, the smallest change that makes the row bind / define the PDB spelling with the object bytes unchanged:
- T1 access / T2: `private:` / `protected:` labels, `virtual`, `const`, an existing `// Leaf:` line. Wider fixes are allowed when
  byte-identical, evidence-backed and recorded per row (ruling R1 examples: a const slot the PDB-const body forces; `virtual` on a
  non-polymorphic local model with the leading pad shrunk to keep every offset; a return type both PDBs give; a `friend` naming the real class).
- T1 standin_alias: a caller binds a stand-in alias (`Foo_<addr>`, `...Methods` / `_Retail` class, `struct T`, `Native::`): declare the
  real member (PDB signature) in the caller's local model, or use the shared header / an existing opt-in. The CCountedPointer DeleteData form
  is the landed `FABLE_RETAIL_CCOUNTEDPOINTER_DELETEDATA_DECL_ONLY` + `engine/CCountedPointer.h` hybrid (NF282).
- A stand-in that is a DIFFERENT function, or needs a port (stand-in class or parameter types, a new class header, a table change, a
  same-TU strong copy that already carries the PDB spelling, `mutable`): REJECTED.tsv or STATUS `skipped-port` with the reason (T3).
- If a vtable appears or changes, stop on that address and hand it back as a decision.

## Proof per changed row
- objsec / cmpobj HEAD v new object: every non-debug section, bytes and relocation lists equal under the respelled names (ALL_EQ),
  `.drectve` included; both function COMDAT sets SAME; vtmap of any emitted table unchanged.
- Link check: HEAD leaves the wrong spelling unresolved where the new set resolves it; never leave unresolved a reference HEAD resolved;
  no new duplicate definition with a different body.
- Recheck every changed row through vland (`python3 tools/decomp_pipeline/linux_lane/lane_tools/recheck.py <dir> <bare lowercase addrs>`
  then `nohup env LANDVERIFY_WORK=<your work dir> timeout 900 python3 -u tools/decomp_pipeline/linux_lane/lane_tools/vland.py
  <dir>/payload.json <dir>/oracle.tsv --recheck-landed > ... &` with FABLE_ROOT pointing at a copy of the repo tree that has your rows,
  or hand the recheck to the coordinator with a note). The oracle file holds retail bytes: keep it in work/, never in a hand-back file.
- E1: a row whose own precheck passes but whose unchanged landed test fails clang only in VC7.1 headers may be handed back with that noted
  (the landing recheck compiles the test with VC7.1).

## Hand-back (after about 60 addresses, or sooner when context gets heavy)
NOTES.md, LESSONS.md, STATUS.tsv (one line per ENTRIES address reached: done / done-partial / skipped-held / skipped-port / rejected +
reason), APPLIED.tsv, REJECTED.tsv, RENAMES.tsv (self renames + caller respellings), TABLE_CHANGES.tsv (action symbol target witness note;
actions delete / add / replace `old -> new` / rewitness), NAME_FIXES.tsv (address correct_symbol evidence wrong_spellings_seen), rows/
(`<addr>.cpp`, `<addr>_test.cpp`), hdr/ + orig/ for any header change, evidence/, recheck_final.log, rows.txt (one bare lowercase address
per changed row), tools/ (every helper you wrote), and the decisions you need. Keep every file current as you go: a restart can stop you.

## Standing rulings (accumulated)
- NF281 D2 / NF283 D3: a self move that would duplicate another row's strong same-TU copy is T3. NF281 D4: 0x0052C7E0 held until
  0x005367F3 is fixed (RELOC_CONFLICT). D1 (NF281): a `friend` naming a stand-in is held; a friend naming the real class lands.
- NF282 D1: the hybrid CCountedPointer form is accepted; D2: binder / owner self names are T3 ports; D3: pinned `// Leaf:` lines stay.
- NF284 D1: 0x00B94F90's self row is a T3 port with 0x0067F650 / 0x00A110D0 / 0x00A99FE0 / 0x00A9CB20; D2: standalone tests with stub
  definitions under stand-in names stay; D3: one address bound under two stand-in spellings is one entry, fix both.
- NF285 E2: in-place-shaped entries may be pulled first, but open every binder before classifying; record the order in STATUS.
- Identical-body equal-copy LNK4006 from folds is accepted; a RELOC_CONFLICT (one symbol, two targets) is held.
- NF291 Q4: a flat branch marked "GENERATED ... do not edit by hand" may take an add-only declaration opt-in (gen_class_headers.py keeps
  any header with a `FABLE_RETAIL_` mark, so a regenerate cannot drop it), with the usual equiv_check and planted control.
- NF289 / NF291 Q1: a table-only fix whose witness fails the clang precheck: compile the witness's HEAD object (D1 / D1b) and read what it binds.
- NF301: a tracker PDB spelling in the VC10 Dinkumware form (`_List_val`, `_Vector_val`, `_Tree_val`, ...) where the binders bind the
  STLport form is `done-equivalent`, not a fix target; a table-less global base may move into its PDB namespace in place when objsec is ALL_EQ.
- NF299 Q1 / Q2: a `friend` naming the real class after an alias-pattern `#undef` lands (NF281 D1); a full mangled `// Leaf:` replacing a
  bare one lands when the recheck payload follows it.
- NF299 Q3: `virtual` on a local base model (the PDB member is virtual, the binder calls it qualified) is allowed under R1 when the binder's
  TU emits no vftable / RTTI COMDAT for that class (no new `??_7` / `??_R` symbol), every member offset is kept (leading pad shrunk where
  the model was non-polymorphic) and objsec is ALL_EQ with the COMDAT sets SAME; anything else stops and is handed back.
- NF299 Q4: entries whose every binder binds through a class or scope found in neither PDB name set (or the wrong class) may be marked
  `skipped-port (census)` from a census script without opening each binder; commit the script in tools/, open 1 in 10 of them (and every
  entry where a binder includes a shared header that already declares the PDB member) and record the sample in STATUS.
- NF300 Q1: including an existing shared enum header above a local model and replacing a literal by its enumerator lands when objsec is
  ALL_EQ and the preprocessed diff shows only the enum, the parameter type and the literal changing.
- NF300 Q2: re-scoping a local model of a nested or namespaced PDB type (global `struct T` -> `NFoo::T` / `COuter::T`) is in place (T1)
  when the TU emits no vftable / RTTI for it, none of the TU's own definitions change name, and objsec is ALL_EQ; otherwise a port.
- NF300 D1 extension: a clang failure only on MSVC-only syntax in an UNCHANGED landed source (e.g. the `unsigned int(...)` functional
  cast) is covered by D1: a passing `cl /Zs` in an earlier call is the precheck.
- NF302 Q2: a binder respelled onto the PDB name whose owner row still defines a stand-in lands (unresolved before and after); queue the
  owner in NAME_FIXES. Q5: an owner's own const / access self fix is in place when objsec is ALL_EQ and every landed binder of the old
  spelling moves in the same landing (NF282 D2 is about re-scoping a self, not this). Q1: giving a flat shared-header class a base it lacks is a
  layout change, so a port.
- NF303 Q1: `virtual` on a local base that also flips the binder's own override leaf QAE -> UAE (the PDB spelling) is allowed per row under
  NF299 Q3 + NF302 Q5, recorded in RENAMES section 1. Q2: a member-template declaration that adds a `/alternatename` line to `.drectve` is
  T3 (a `.drectve` change is T3). Q4: held entries are taken once their locks are free, after re-reading the landed rows.
- NF304 Q6: an owner's own PDB-signature self fix (bytes equal) lands when every landed binder of the old self spelling moves in the same
  landing (NF302 Q5 widened; NF282 D2 covers re-scoping a self onto a made-up owner). Q1: a re-scope whose only renamed definitions are
  pick-any copies of the type's implicit members or local inline getters, now in the PDB spelling, lands when each copy's body equals the
  landed PDB definition or no landed strong definition of that symbol exists; otherwise T3. Q2: a local `long` member / parameter retyped to
  the shared 4-byte enum lands under NF300 Q1.
- NF305 Q1: a respell that moves another call onto a DIFFERENT wrong form is a port (a binder may stay unresolved only on the PDB name).
  Q2: `cl /Zs` counts as the precheck of a CHANGED source only when the same clang error (outside the toolchain headers) also fires on
  its unchanged HEAD source; record both logs. Q3: a native `wchar_t` (`_W`) need is a compile-flag change: T3. Q5: a `friend` the retail
  access forces, naming the real class, lands under R1.
- NF306 Q2: a tiny stub target (GOTCHAS NF30 shape) is not named on one-step-indirect donor evidence (a callee the retail build inlined);
  hold it as identity research. Q3: a binder-only fix where the self row already carries the PDB Leaf lands.
- NF307 D1: NF304 Q1 covers renamed pick-any copies only when the new names are the PDB names; a re-scope that renames getters of a
  stand-in class onto other made-up names is held (port the stand-in). D3: a literal passed as the PDB enum (`(E)0xC` or an OR of
  enumerators) lands as one unit with the selves retyped to the enum and every binder moving (NF304 Q6). D5: a binder may bind a PDB name
  that a pre-existing TU emitter also defines strongly; queue the emitter for the nf86 / nf88 pick-any scaffolding.
- NF308 Q2: a re-scope that renames an emitted vftable (`??_7...@6B<base>@@` included) is a port, not a per-case decision. Q3: a table line
  carries the spelling its witness object binds; a Dinkumware `U?$char_traits` line whose witness binds the STLport `V` form is respelled
  after compiling the witness. Q4: an owner free function moving into its PDB namespace (all binders with it, catalog Module updated by the
  coordinator) lands in place.
- NF310 D1: a binder may land alone on the PDB name when it resolves to byte-equal pick-any COMDATs other rows emit, while the strong self
  would collide (LNK2005); queue the self for the nf86 / nf88 pick-any form. Helpers: copy nf310's nf.py (clean.py, not rmtree / unlink).
- NF311 Q5 / Q6: NF299 Q3 also covers a `virtual` destructor on a local member model and the access label the PDB gives the override
  (e.g. MAE), under the same conditions. Stale witnesses can hide behind access / const / pointer-v-reference differences: compile the
  witness rather than screening identifiers, and read full spellings from the table (ENTRIES cuts them at 160 characters).
- NF309 Q2: an `#ifdef` opt-in giving an existing shared-header declaration its PDB access (`#ifdef` new / `#else` old) plus a friend naming
  the real class lands with equiv_check and a planted control. Q3: a self fix that forces const onto same-TU stand-ins is T3. Q4: a
  table-only delete of a stale line, proved from the witness's object, counts as `done`. Q5: nested types only a shared header can declare
  are header-lane / T3 work. Screen TARGETS as well as binders against stay-off (T3_PORT_QUEUE column 3 lists unit targets too), and keep
  the changed rows' locks until the landing.
- NF313 Q2: the call-site edits a pointer -> reference fix needs (`&x` -> `x`, `p` -> `*p`) are part of it. Q3: a row whose own self is a
  virtual override (local `typedef long E` models) may retype that self only when every landed vftable and binder of the old self spelling
  moves in the same landing; otherwise a port. Never pipe a payload build or a grade into `head` (broken pipe).
- NF312 D3: dropping a made-up `__fastcall` from a self and its callee is an in-place self fix when objsec is ALL_EQ and no landed row binds
  the old spelling. D5: an object's UND symbol proves the binding for a rewitness whatever the row's grade. Write a rewitness's witness
  column as the NEW bare address only (not `old -> new`).
- NF316 Q3: a binder named only in a T3_PORT_QUEUE reason column is not reserved; an address in column 2 or listed in a unit's column 3 is.
- Coordinator, 2026-11-23 (flatten switch route; scripted coordinator route only, NOT the T1 agent standard above, which keeps
  `.drectve` in ALL_EQ):
  - Scope: a flattened row moved onto its class header under an existing landed class-model switch (`flatten_pilot.py --switch`).
  - The move may add exactly `/DEFAULTLIB:"libcp"` to `.drectve`, under these conditions:
    - every other section is ALL_EQ;
    - the new `.drectve` tokens are HEAD's plus that one token;
    - the undefined-symbol sets are equal;
    - the grade is MATCH / RELOCATION_MATCH PASS.
  - Why: NF303 Q2 covers `/alternatename`, which changes symbol resolution. libcp only extends the library search, comes from the shared
    header the header rule requires, and is already carried by about 17% of landed objects. It is not a claim that retail linked libcp.
  - Virtual targets also need the RTTI vftable placement (`labels_rtti_port.tsv`) to name the same symbol at that address.
- Owner question 2026-11-23, decided with the advisor: deleting destructors.
  - **Spelling.** A deleting destructor takes `??_G<Class>` or `??_E<Class>` per address:
    - the CLASS comes from the retail RTTI vftable slot (`ghidra_out/labels_rtti_port_dtors.tsv`);
    - the KIND comes from the retail body (scalar = `??_G`, vector = `??_E`), which the grade enforces;
    - the tail (access, virtuality) comes from the donor name.

    A donor `??_E` only means some donor TU array-news the class, so a retail scalar body is `??_G` even when no PDB set spells it. VC7.1
    defines `??_G` and makes `??_E` a weak alias of it unless the TU uses `new X[]` (lane test, `$S/dtor/`).
  - **Emission.** No forcing construct at scale. A helper that constructs X (`MakeOne`, an explicit template instance, a `selectany`
    pointer) also defines `??0X@@QAE@XZ` and `??_7X@@6B@` from a throwaway model: a fake constructor and a truncated vftable under real
    PDB-shaped names. With the constructor and destructor declared out of line, `new X` emits neither (lane test). So `??_GX` belongs to the
    TU that compiles X's constructor, and its emission is part of each class's ctor port.
  - **The 539 landed `MakeOne` rows are debt.** Each defines the same strong `?MakeOne@@YAPAXXZ` (a full-link duplicate) plus a fake
    constructor and vftable. Do not copy that pattern.
- NF317:
  - **Q1:** const overloads added to local inline container stand-ins, whose pick-any helper COMDATs then carry renamed stand-in symbols,
    are T3 (NF309 Q3).
  - **Q2:** const on a force-inline lookup or a virtual-slot call of the local model, which the PDB-const body forces and which emits no
    symbol, lands under R1.
  - **Q3:** a fix that grows a stand-in vftable is a port onto the class header, never a COMDAT vftable change.
  - **Q4:** when retail's bytes show an int / byte return width and the PDB says bool / void, the retail code wins. Record it as a PDB
    revision difference and do not respell.
  - **Gotcha:** HEAD and new must be compiled at ONE path, or `?A0x` anonymous-namespace hashes differ and objsec reports false diffs.
- NF318:
  - **Q1:** the coordinator's libcp `.drectve` exception covers every move of a row onto its class's shared header under an existing
    class-model switch (the alias route as well as the flatten switch route), with the same conditions.
  - **Q2:** a class-model switch no landed row defines yet stays out of these routes. Hold the row.
  - **Q3:** an enum return written as a literal cast to the PDB enum is the NF307 D3 form, and allowed.
  - **Q4:** respelling a callee during a move needs the callee's own second source; otherwise it is a T3 unit.
  - **Q5:** a move that drops a COMDAT (even an unreferenced made-up one) fails "COMDAT sets SAME". Hold it.
- NF319:
  - **Q1:** triaging all entries by script first and then working the feasible ones meets the stop rule's intent.
  - **Q2:** the libcp rows of header moves fall under NF318 Q1.
  - **Q3:** adding a declaration to a class written on one line edits that existing line, so it is a class-model change (hold).
  - **Landed precedent:** `FABLE_RETAIL_<CLASS>_RTTI_DECLS` add-only opt-ins, each declaration overriding a base virtual at the RTTI slot
    (clang `override` probe plus a made-up-name control), with `FABLE_STATIC_ASSERT_AT` pins.
- NF320:
  - **Q1:** a TU that faithfully emits a class vftable (a Clone constructs a copy) may land only when the emitted `??_7` matches retail
    slot for slot. Missing overrides are added as add-only declarations first, and `??_G` / `??_E` pick-any is fine.
  - **Q2:** a move that ADDS unreferenced pick-any COMDATs fails "COMDAT sets SAME" (the mirror of NF318 Q5). Hold it.
  - **Q3:** the retail RTTI placement plus ONE PDB name set count as two sources.
- NF321:
  - **Q1:** a move whose only symbol change is a new UND `__fltused` is allowed, as a narrow exception shaped like the libcp one. VC7.1
    emits that marker for any float-typed expression, so a PDB float parameter requires it. Conditions:
    - symdiff shows the UND delta is exactly `__fltused`;
    - an already-landed MATCH row's object carries it;
    - the callee has its own second source (NF318 Q4).
  - **Q2:** an out-of-line copy of an in-class header inline whose scaffolding adds a helper falls under NF320 Q2 (hold), unless
    coff_info shows no COMDAT or symbol the HEAD object lacks. The whole-unit route (self plus every landed copy, with a census) stays open.
  - **Q3:** a port that drops a same-TU strong stand-in definition (a faked CCharString constructor) is held in the naming lane.
    Queue it as a re-author row, with a link check and the real constructor's owner defining the PDB symbol.
- NF322:
  - **Q1:** widening an existing `_PASS<n>` class-model guard with `|| defined(FABLE_RETAIL_<CLASS>)` is in effect an opt-in, because no
    TU defines the new macro. Conditions:
    - the region holds only the class model;
    - every other `_PASS<n>` use under rebuild/include is checked;
    - the model is proved by the compiler-emitted `??_7` probe against retail (declaration order alone is not proof);
    - equiv_check plus a planted control inside the block.
  - **Q2:** a new base header is part of a class-model port when the base is in rtti_bases.tsv, `gen_class_headers.py` produces its flat
    header from the PDB layout, and it takes the opt-in model, probe and asserts and lands with or before its rows. A class absent from
    the layout TSV is a separate authoring unit.
  - **Q3:** NF318 Q1 covers libcp on header-route rows.
- **Table replaces (coordinator, 2026-11-24):** a `replace W -> P` is only safe when no landed row still references W. The table keeps one
  witness per line, so check with `ref_scan.py` (compile every landed row naming the identifier) before applying. Where another row still
  references W, move it in the same landing, or use `add` plus a `rewitness` of the old line.
- **Identity claims (coordinator, 2026-11-24):** when another address claims the PDB name:
  - a catalog label only, proven wrong: proceed, with a NAME_FIXES line;
  - a different symbol (an overload): not a conflict;
  - a landed self defining the SAME symbol: hold both as one unit, because landing one would create a second strong definition.
- NF323 (full text in `batches/2026-11-24_landing_nf323/README.md`):
  - **Q1:** a brand-new class header takes the opt-in form. gen_class_headers.py's flat output is the default branch, and the polymorphic
    model goes under `FABLE_RETAIL_<CLASS>_VIRTUAL_MODEL`. Never an unguarded polymorphic default.
  - **Q2:** an out-of-line base copy-constructor call needs two sources, `??0<Base>@@QAE@ABV0@@Z` in the PDB sets plus a landed row or
    a donor call; then an add-only declaration.
  - **Q3:** NF309 Q2 covers PDB access on a virtual. The binders define the same opt-in and land as one unit.
  - **Q6:** a Clone TU's UND set may grow by exactly what its emitted `??_7` / `??_G` reference (under NF320 Q1).
- NF324 (`batches/2026-11-24_landing_nf324_b1/README.md`):
  - **Q3:** same-width integer, `char` -> `bool` and pointer -> reference retypes onto the PDB parameter types are accepted when proven
    byte-equal.
  - **Q7:** RTTI-evidenced declarations that no landed row defines may stay in an opt-in block; they emit nothing.
- NF325:
  - **Q1:** `friend` under an add-only opt-in is allowed (NF309 Q2).
  - **Q2:** base slots may be declared `= 0` from retail `_purecall` plus the derived classes' override spellings, under the opt-in, with
    the vftable probe.
  - **Q3:** a new shared header lands even though local models of the class elsewhere then get SHARED_HEADER_AVAILABLE warnings.
- NF327:
  - **Q1:** a TU that adds `??1` destructors, or an out-of-line base destructor call that retail inlines, is held (NF320 Q2).
  - **Q2:** a unit whose binders are not ALL_EQ under the one rename (their own emitted vftable moves) is held whole.
  - **Q3:** keeping HEAD's existing helper class unchanged is not new scaffolding.
- NF326:
  - **Q1:** the pick-any whole-unit form is allowed, for this unit only:
    - each self defines its member `inline` on the shared header and emits it pick-any through ONE per-TU (`?A0x`) helper in the landed
      nf86 / nf88 form;
    - every landed copy is respelled to the same COMDAT bytes;
    - the unit lands whole or not at all, and the helpers are recorded as scaffolding debt.
    Later units of this kind (~CScriptThing / copy constructor, NF321 Q2) come back with the same proof set: the selfproof, the copyproof,
    the object COMDAT census, and a full link of the census objects.
  - **Q2:** a copy that still differs from retail (0x00DF6010's GetDataString) does not hold the self; queue it as debt.
  - **Q3:** mixed accessor / local spelling in copies is fine when the bytes are equal. The accessor form's two extra COMDATs per object
    are forbidden (NF320 Q2).
- **Equiv-check scope (coordinator, 2026-11-24):** an equiv_check over the reverse include closure of the changed headers is accepted,
  provided the closure follows quoted includes and folded `../src/compiled` TUs and the planted control is cross-checked. NF325's full
  57,146-row run found nothing beyond its closure run except path-only `__FILE__` lines.
- **Paths are per session.** The S path at the top names one session's scratchpad. Each agent's BRIEF.md prefix gives the current S and the
  donor-build path. Wine tools need `unset TMPDIR TMP`: a TMPDIR inside the scratchpad makes cl segfault.
- NF326 Q4 / NF328 (`batches/2026-11-24_landing_nf328/README.md`) and NF331 (`_nf331/`): CScriptThing member units.
  - **Form:** every strong definer of a member that retail expands inline becomes `inline` (pick-any). If nothing else emits the member
    out of line, the self gets ONE `?A0x` helper: a qualified explicit destructor call, a by-value copy return, or an assignment.
  - **Helper device:** `#pragma inline_depth(0)` over that one helper function, restored right after it, below the row's own code, with
    an `// Inline-budget: scaffolding, not retail` line. Ruled per unit; it is debt.
  - **Differing copies:** pre-existing copies that differ from retail are queued debt (T3 one line per copy), not a hold, provided the
    change adds or alters none (NF326 Q2 / NF328 D1).
  - **Census holes:** Codex-reserved rows may be compiled read-only for a census.
- NF329 / NF330 (`batches/2026-11-24_landing_nf329_b3/README.md`, `_nf330_b*`): flat-header ports onto generated
  `FABLE_RETAIL_<CLASS>_VIRTUAL_MODEL` opt-in derived models.
  - **New slot:** a member on a NEW slot is proven when the retail vftable slot holds the row's own address (D-a).
  - **Packing:** the generator's `#pragma pack(push, 1)` with explicit pads and every offset asserted is the convention, not authored
    packing (D-g).
  - **Base selection:** `// Needs` lines may select a landed base switch (D-d).
  - **Base-model `_RTTI_DECLS` blocks:** allowed (NF324 b5 / b6 form) only when the BASE class's own emitted `??_7` matches retail slot for
    slot with the block on. A derived-class probe is not enough (NF330 D1 / b6).
  - **Non-public destructor:** declared where the retail slot's `??_G` tail and both PDB sets give it (NF330 D3).
  - **HEAD entries:** a row whose landed source carries a retail byte-dump comment needs its HEAD entry rebuilt from the repo before
    `land_scripted.py` (D-f).
  - **T3 lines:** the coordinator lands rows it sent an agent into from T3 with `land_scripted.py --release-t3 <tag>`.
- NF340 (pick-any all-copies audit, `naming/audits/2026-11-24_pk_all_nf340/SUMMARY.md`; advisor decision, owner delegated):
  - **Checker:** `lane_tools/pk_all/pk_all.py <obj> <row-address>` sits ALONGSIDE `pickany_vs_retail.py`. It finds each copy's retail address
    from the leaf's own relocations, so it also sees `?A0x` and stand-in-named copies. Run both on every row you change.
  - **Gate:**
    - **Row change:** compare HEAD with new. A leaf-called NEQ copy that is new, or that turns NEQ, holds the row. A copy that was already
      NEQ is debt; list it in NOTES.
    - **New row:** any leaf-called NEQ copy holds it.
      Exception (advisor decision, owner delegated, 2026-11-24; EXTRA_RULES 2026-11-24 pick-any entry): the row lands as debt when ALL hold: (1) the NEQ copy equals a landed row's HEAD copy at object level (bytes, relocation symbols, COMDAT selection); (2) no fix is on record (measured copy-EQ texts all move the leaf); (3) every other leaf-called copy is EQ under both checkers on A's object; (4) the row carries a `// Pick-any:` note and the copy's T3 line lists it as an emitter. Where a fix exists the row still holds and takes it (0x00839940). First use: 0x00ED8680 (batches/2026-11-24_landing_acctB_b24d).
      Scope (advisor decision, owner delegated, 2026-11-24): NF318 Q5 and NF320 Q2 ("COMDAT sets SAME") gate MOVES (renames, header moves). A copy repair changes the COMDAT set on purpose and is gated by the drop test instead: no reloc_bindings line witnessed by the row names a dropped symbol, no landed object is left undefined, and every dropped EQ copy with a retail address keeps an EQ definer elsewhere (nf341_r2, nf346, nf347). A declaration-only opt-in additionally lands only where the new external reference resolves to a landed EQ row defining that exact symbol (nf347: 0x00D58490 lands; 0x00EA59E0 / 0x00E8EF80 wait for the 0x00CD3D2E owner respell).
    - **CompilerFlags change** (`/d2inl*`, `/Os`, `/O1`, `/GS`): it must pass this gate, because the flag both breaks and repairs copies.
  - **Debt:** it is queued per copy (T3 `nf340`, column 2 = the copy's retail address). The fix is the emitting header text or local model.
- NF348 (advisor decision, owner delegated, 2026-11-24):
  - **D1:** a port whose object gains a CCountedPointer accessor COMDAT (`operator->`) is the accessor form NF326 Q3 forbids under
    NF320 Q2, even when that COMDAT equals an already-landed copy; hold it and retry with the local spelling (NF326 Q3 allows it when
    the bytes are equal). A sibling landed before NF320 is no precedent.
  - **D2:** "both PDB name sets agree" (add-only opt-ins) is met when one PDB names the member and the other is SILENT (no rival
    unplaced member fits the body), with the retail placement as the second source (line 52; NF320 Q3): 0x00E1AD30 InitialiseVariables
    (FableWin + retail Main's first call + object order; Ego_r carries no Main / DoMission / EndMission either).
- NF352 (advisor decision, owner delegated, 2026-11-24; batches nf352_r1 / b1k):
  - **Reach flips:** an already-NEQ copy whose leaf reach flips n -> y under the change counts as a newly leaf-reached NEQ copy and holds
    the row. pkcmp does not test this; compare pk_all's reach column HEAD v new (agent_tools/nf352/reachflip_coordinator.py). A flip
    onto an EQ copy is fine. A retro scan of NF341 / NF346 / NF349's gate pairs found none.
  - **Q1b:** a size-body opt-in may cover a further copy outside the brief's addresses when it meets the same conditions (owner sets
    `optimize("s", on)`, no CompilerFlags entry, pragscan clean) and the retail caller-callee shape needs it (Reset 59 B calls ReleaseData 44 B).
  - **Q2:** condition (a) "one COMDAT changes" is per opt-in; with two opt-ins, exactly their two COMDATs may change if a run with one
    opt-in alone isolates the first.
  - **Q3:** no second-header opt-in whose device does not reach retail (DeleteData<CAction> 12 B v 21): that is the forbidden second device.
    Queue it diagnosis-only.
- NF361 / NF362 (advisor decision, owner delegated, 2026-11-24):
  - **Census labels:** NF358's "shared-body" label was wrong for the /O2 GetDef emitters. Their copies come from a row-local
    `template<class T> GetDef(long, ...)`, so a define-only fix fails (C2995). Check where a copy is defined before choosing an opt-in.
  - **SIZE_BODY blocks** (NF349 / NF352 / NF357) re-mode the rest of a TU that sets its own pragmas, and move the leaf wherever the leaf
    inlines the function (GOTCHAS 2026-11-24). A 101 B GetDef in a size TU takes `FABLE_RETAIL_CDEFPOINTER_SETDYNAMICCAST_REREAD` alone,
    an existing opt-in.
  - **Include moves** stay byte-neutral only (NF357 Q2). NF362's P2 moved `??0CCPPointerInfo` (26 -> 25 B) and is rejected.
  - **P1 sibling block** (Reset for y-off size TUs) is allowed with these conditions:
    - its own macro name, add-only, with `#error` if defined together with the RESET macro;
    - pragscan: the state before the block equals the restored state after it;
    - exactly one COMDAT changes, and the copy equals the owner object;
    - whole-catalog equiv_check plus a planted control.
  - **NF361 swap** (a row drops its local GetDef template for GLOBAL_BODY + REREAD + SIZE_BODY): allowed as a copy repair gated by the drop
    test, if the helper COMDATs equal those of an already-landed shared-body row. Rows whose leaf moves are skipped.
  - **B's rows:** agents stay off them, even read-only; the coordinator rechecks them from the repo.
