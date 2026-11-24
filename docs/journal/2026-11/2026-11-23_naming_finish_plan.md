# Plan to finish naming (Account A, 2026-11-23)

Owner request: "have the fable advisor plan the rest of the steps to finish naming as fast and accurately as possible". A read-only
planning agent on the Fable model wrote it from the files. Its counts come from re-running `naming_status.py`'s classification in memory
and joining `ghidra_out/labels_rtti_port.tsv`, the PDB name sets and `rebuild/oracles/self_symbols.tsv`. The coordinator checked the
claims marked (v) before recording them.

## Corrections to the earlier picture

- **`agree_std` (9,267) is overstated (v).** `naming_status.is_std()` returns true for any name that does not start with `?`. So 1,270
  made-up C names (`_Construct`, `@VecDelDtor@12`), 276 flattened C names whose class is in a PDB, and 213 rows with an RTTI PDB name they
  do not spell all count as done.
- **`agree_pdb` has conflicts.** 88 non-std PDB names sit on 222 addresses, and 8 rows disagree with RTTI (e.g. 0x00865E20, 0x0088E080).
- **The RTTI file is one line per address by construction (v).** `tools/rtti_port_all.py` drops any retail address that gets two
  different names from two vftables (its ICF guard) and ports only slots whose donor name belongs to the class itself. So the fold guard
  lives in the generator, not in a "no address carries two names" check on its output; the latter is empty.
  - RTTI agrees with 2,646 of 2,654 `agree_pdb` rows.
- **Family sizes in OPEN.tsv T3:** CCountedPointer 1,072, `T` 954 (+179 hidden in agree_unverified), vector 708, _Rb_tree 420.
- **STLport-only reclassification:** 911 rows, not 748.
- **About 2,500 non-done addresses have no landed row.** Only their callers can be fixed until the row lands.
- **Unusable as sources:** the FSE manifest has no addresses to join, and `ghidra_out/egod_pdb_names.tsv` has 24 lines (Ego_d effectively
  unused).

## Routes, in run order

| # | Route | Addresses | Tool | Identity source | Main risk |
|---|---|---:|---|---|---|
| 0 | Tracker fixes, no code | reclassifies ~6,000 | script | - | none; makes later numbers real |
| 1 | Access / const script, widened | ~2,090 | script (`access_fix.py`) | unique PDB symbol with the same `?name@scope@@`; RTTI for 622 | a virtual emitting a vftable; binders that must move with the row |
| 2 | RTTI-named, label agrees (flatten residue) | 2,481 | script (`flatten_pilot.py`) | RTTI slot plus the label's class and member | header opt-ins; acceptance well under 86% |
| 3 | Signature respell, unique target | ~1,530 | script first, agents for the rejects | unique PDB symbol; RTTI for 503 | stand-in parameter types turn into ports |
| 4 | Compiler-generated shapes (`??_G` / `??_E`, `Dest_val`, `Alloc_*`) | 2,705 | scripted census, then family agents | class from the label plus the vftable slot | propagated labels (e.g. one scalar-deleting-destructor name on 235 addresses) |
| 5 | Template families (CCountedPointer, `T`, vector, _Rb_tree, PKCTRList) | ~3,700 | family agents with the header lane | typed callers, Ego_r layouts | unknown pointee |
| 6 | Rulings and identity research | ~700 | agents / owner | per case | - |
| 7 | Non-PDB residue | 5,157 | class-level judgment | a second source per class | calling a stand-in "descriptive" |

- **Route 0, three changes to `naming_status.py`:**
  - add an RTTI column and status;
  - treat a non-std name on more than one address as a stand-in. That moves 3,158 `agree_unverified` rows (576 names, e.g.
    `AutoTinyCountedHandle::Reset` on 341 addresses) and 923 `agree_std` rows, and puts 155 `agree_pdb` rows up for audit;
  - extend `STANDIN` to the `??1T@@` / `@S@@` forms, tighten `is_std`, and apply the STLport equivalence (911 rows).
- **Route 1:**
  - sources: T2 258, plus `agree_unverified` rows with RTTI and an access-only difference (618), plus non-RTTI unique-PDB access-only rows
    (995), plus split_unknown (215);
  - for a non-RTTI row that turns virtual, both PDB name sets must agree.
- **Route 3:** the 503 RTTI same-name signature differences include many `GetSizeofClass` `QAEHXZ` -> `UAEKXZ` rows (one template edit).
- **Route 6:** hold the 250 rows where RTTI contradicts the label; they are not mechanical. Also split_pdb_multi 96, the 222
  multi-claimed addresses, `*_CLAIMED` 385 and overloads 446.
- **Route 7 (`agree_unverified` has 5,157 unique names with no RTTI and no PDB candidate):** judge them per class, not per row. That is
  530 classes; the classes with 20 or more addresses cover 2,297.
  - Third-party code (DirectShow `CBase*` / `CPosPassThru`, `CEAX*`, about 450) is checked against vendor source.
  - Audio / PKC classes absent from both PDBs get descriptive names, marked in NOTES.
  - For the 815 whose class is in a PDB but whose member is not, check a full Ego_d.pdb export before ruling.

## Agents

- Keep them off T1 batches: 1,830 of the 1,994 T1 addresses turned out to be ports.
- Use them for one family or one header each (routes 4-5), for route 3's script rejects, and for route 6.
- Each brief is a pre-computed ENTRIES list:
  - target symbol, evidence source and binder list for each entry;
  - an expected in-place yield;
  - a stop rule: hand back after 20 consecutive ports.

## Safeguards for every scaled route

- **Proof:** keep the five-step proof (HEAD / new compile, objsec ALL_EQ, the graded symbol equals the PDB symbol, a passing grade,
  binders that agree).
- **Identity:**
  - keep the claim guard (catalog, Leaf, definition, file name, table);
  - never rename onto a PDB symbol another address holds;
  - virtual targets need RTTI;
  - non-virtual targets need a unique member in both PDB sets, or one PDB plus a call site;
  - trust only MATCH / RELOCATION_MATCH self symbols.
- **Kill:** stop a route when acceptance falls below about 60%, or when any accepted row fails its landing recheck.
- **Audit:**
  - check 1 landed row in 20 against a donor call site;
  - add a planted wrong-name control to each batch;
  - run a full-tree regrade before bulk header changes (ROADMAP line 85).

## Effort (the planner's assumptions)

- **Routes 0-3:** about 6,100 addresses. With acceptance falling from 86% to about 50%, that is 3-5 coordinator days, landing roughly
  3,500-4,500.
- **Routes 4-7:** about 12,000 addresses plus the script rejects. At 47 rows per agent-hour that is about 250-300 agent-hours, or 2-3
  weeks with 3-4 family agents, if deriving the pointee type from typed callers works.
- **Floor:** if it does not, about 2,000-3,000 addresses stay as documented descriptive names.

## Route 0 done (coordinator, same day)

`naming_status.py` changes:

- **`is_std`.** A C name (no `?`) counts as done only when it is one of these:
  - a proven library symbol (the `symbol` column of `library_signatures.tsv` / `vendor_signatures.tsv`);
  - an import-table name (`__imp_...`);
  - SDK GUID data (`_GUID_` / `_IID_` / `_CLSID_` ...).

  1,514 rows leave agree_std, among them `_$E1` / `_$E2` (the compiler's placeholder for `dynamic initializer for 'X'`), flattened STL
  helpers and made-up C names.
- **Shared names.** A non-PDB, non-std name that is the only spelling of several addresses becomes a stand-in (T3, fix
  `shared name (n addresses)`): 3,836 rows. Compiler-generated `??_` thunks are exempt. PDB names on several addresses are listed in
  `status/SHARED_PDB.tsv` (108 lines) for audit.
- **`STANDIN`** also matches the placeholder classes `S` and the `??0T@@` / `??1T@@` forms; neither class exists in either PDB.
- **New `rtti` column in OPEN.tsv:** 793 open rows carry an RTTI name, and none contradicts the tracker's pdb_spelling.
- **Not done here:** the STLport equivalence (911). Its two-spelling rows are real link mismatches, such as a symbol with and without its
  allocator argument, so they need the NF301 equivalence rules rather than a status flip.

Result (64,076 addresses):

| Bucket | Before route 0 | After route 0 |
|---|---:|---:|
| agree_pdb | 22,205 | 22,205 |
| agree_std | 9,267 | 7,731 |
| agree_unverified | 13,006 | 10,673 |
| standin | 6,972 | 10,841 |

The other buckets are unchanged. Done is 29,936 (46.7%), down from 49.1%: the old count included names that were never real.

## Progress on the routes (coordinator, same evening)

| Batch | Route | Rows | Acceptance | Notes |
|---|---|---:|---:|---|
| access 1 | 1 (T2) | 170 | 88% | |
| access 2 | 1 widened | 188 | 84% | |
| access 3 | 3a | 140 | 77% | `--virtual --ret`: RTTI-confirmed virtuals take their vptr from a leading vtbl member / pad |
| access 4 | 3a | 72 | 91% | full-Leaf rows added to the pool |
| alias 1 | 2b | 29 | 24% | **stopped** under the 60% rule: alias bodies use stand-in parameter types and members the real model names otherwise (ports, queued) |
| binders 1 | T1 | 16 callers | 73% | caller-side access / const |

- **agree_pdb:** 22,205 after route 0, 22,769 now.
- **New tooling:**
  - `land_scripted.py` lands any run directory: reservation check, recheck on the landed tree, symbol check, batch folder, queue lines,
    and table changes for binder runs;
  - `alias_header.py`;
  - `access_fix.py --wide --virtual --ret --alias --binders`.
- **Route 4 (deleting destructors, 1,997 `scalar_deleting_destructor` and 274 `vector_deleting_destructor` rows) needs a retail identity
  source first.** `tools/rtti_port_all.py` walks every retail / donor vftable pair, but its `class_of()` cannot parse `??_G` / `??_E`
  names, so it never ported them; that is why the RTTI file has no deleting destructors. A dtor-only port with a `??_G` / `??_E`-aware
  class filter is being run on the lane's binaries (evidence only, never committed).

## Route 4: identity of the deleting destructors (coordinator, same evening)

- **New retail identity source: `ghidra_out/labels_rtti_port_dtors.tsv`** (1,538 addresses, from `tools/rtti_port_dtors.py`).
  - It is the RTTI vftable walk of `rtti_port_all.py`, restricted to `??_G` / `??_E` slot names. The stock `class_of()` could not parse
    those names, so they had never been ported.
  - Same guards: matched vtables only, own-class names only, and an address given two names is dropped (none was).
  - Before using them, the coordinator checked that the lane's binaries reproduce the committed `labels_rtti_port.tsv` exactly (6,653
    lines).
- **Joined with the landed rows:**
  - 1,417 rows name the same class in their label: two sources.
  - **106 name another class** (e.g. 15 "copies of `CAnimFilterBase`'s destructor" are 15 different derived classes, which RTTI separates).
    They are queued as `route4-identity` in T3_PORT_QUEUE.tsv.
- **What VC7.1 emits (lane compiler test, `$S/dtor/`):** `??_GX@@UAEPAXI@Z` is the real body, a COMDAT emitted wherever X's vftable is.
  `??_EX@@UAEPAXI@Z` is only a weak external aliasing it, unless the TU uses `new X[]`. So a retail scalar body is `??_G`. For 585 of the
  1,430 two-source rows the donor PDB spells the slot `??_E` (a different toolchain), and the exact `??_G` spelling is in no PDB set.
  **Owner ruling needed:** may these take `??_G<Class>` on the VC7.1 evidence?
- **Emitting `??_GX` from C++ is a port, not a rename.** The TU has to emit X's vftable. The landed precedent (0x00429A45) uses an inline
  virtual destructor plus a made-up non-inline helper `void* MakeOne() { return new X; }`. **Owner ruling needed** on that construct: a
  made-up global function in every such row, colliding at a full link if two rows share the helper name. The alternative is a construct
  that leaves only COMDATs.

### Rulings (owner asked the coordinator to decide with the advisor) and their effect

- **Spelling:** `??_G` / `??_E` per address. The class comes from the RTTI slot, the kind from the retail body, the tail from the donor
  name. 910 of the 1,538 `??_G` spellings are in a PDB set and 628 are not (counted with the donor tail kept). The earlier "585, a
  different toolchain" figure and explanation are withdrawn.
- **Emission:** none at scale. Every forcing construct also defines a fake `??0X` and a truncated `??_7X`, while with an out-of-line ctor and
  dtor `new X` emits neither (lane test). Emission belongs to each class's ctor port.
- **The landed precedent:** 539 rows contain the `MakeOne` construct, each with the same strong `?MakeOne@@YAPAXXZ`, which is debt.
- **The tracker reads both RTTI files:**
  - **rtti_conflict (99):** a row whose only spelling contradicts RTTI is open, including 12 that counted as agree_pdb.
  - **rtti_identified (4,842):** a row whose RTTI name it does not spell yet is identified, with emission open. This takes 4,112 of the
    old agree_unverified rows.
  - `??_G` / `??_E` of one class and tail agree.
- **Now** (64,076 addresses):

  | Bucket | Addresses |
  |---|---:|
  | agree_pdb | 22,757 |
  | agree_std | 7,644 |
  | rtti_identified | 4,842 |
  | rtti_conflict | 99 |
  | agree_unverified | 6,216 |
  | standin | 10,093 |
  | split_fixable / split_unknown | 4,766 / 7,186 |
