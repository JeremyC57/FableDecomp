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
