# Ego_d reservation (2026-11-17, owner): 774 parked rows that Ego_d.pdb can help, for the other account on `FullDecomp-B`

Owner: "are there any parked rows that ego_d might help with? if there are put them together for a handoff for the other account
to work on".

`RESERVED.tsv` lists the rows (`UNMATCHED_ROWS.tsv` marks them `reserved (egod, parked)` / `reserved (egod, identify)`). They are
off-limits to the coordinator's lane and to any other lane until this list is released (renamed `RELEASED.tsv`). The coordinator's
naming agents work only on landed rows, so they do not touch these.

## What Ego_d adds for a parked row
`docs/journal/2026-11/EGOD_FINDINGS.md` describes the PDB. Ego_d is a VC10 `/Od` debug build of the TLC source tree
(`E:\FableTLC\Fable TLC Build Repository\BBBProjects\Fable1_5MainPC\`). It gives two things no earlier source in the repo had:

1. **The source's own named locals, parameter names, their types and scope blocks, per function.** The carved module streams keep
   each function's `S_BPREL32` / `S_REGREL32` records and `S_BLOCK32` scopes. `lane/egod_locals.py <Ego_d.pdb> <qualified name>`
   prints them; resolve a type index with `llvm-pdbutil dump -types -type-index=0x... Ego_d.pdb`. Example, 0x0040B790:

       == CUserProfileManager::GiveTutorialForCategory (debug length 1151)
         bprel -148 type 0x1d9cb this
         bprel +8 type 0xce58 category
         bprel +12 type 0x30 ignore_fading
         {
           bprel -20 type 0xb7c1 world
           bprel -16 type 0x18f7a pcomponent
           {
             bprel -36 type 0x2318 tutorial_text_tag
             ...

2. **The object file (TU) each function came from, and its order inside it:** `ghidra_out/egod_carved_functions.tsv`.

How that helps each parked residue class (`residue_class` column, from `PARKED_INDEX.tsv`; empty = parked after that index was
built, or never finished):

| residue | rows | what Ego_d gives |
|---|---:|---|
| register-tie | 95 | the real named locals and their scopes (named locals are the known lever, GOTCHAS); the TU's other functions (NF68: c2 record-id ties are an effect of the whole TU) |
| stack-slot/frame | 93 | which values were named locals and in which scope; which callees share the TU (the GOTCHAS 24l same-TU body recipe) |
| x87-order | 86 | local types (`float` vs `double`) and the named intermediates of each sum |
| incomplete-model | 59 | parameter names and types, every local, the scope outline |
| block/arm-layout | 52 | scope blocks follow the source's `if` / loop nesting where locals are declared |
| other (flags, inline-vs-call, bounds, load order, EH) | 10 | TU membership and locals |
| (no class recorded) | 237 | as incomplete-model: most are later parks or drafts without a diagnosis |

**Caveats.** Frame offsets and code generation are VC10 debug, not retail. Unnamed temporaries are not recorded. STL internals are
Dinkumware (`_Tree`, `_List_val`), not retail's STLport: spell STL from `rebuild/include/stlport/`. Local and parameter names are
source evidence, not a byte recipe: a row still has to match retail bytes under VC7.1 with the project's rules.

## The two lists
- **`EGOD_NAMED.tsv`, 632 rows** (about 686 KB by gap estimate; largest first). Parked rows with a real name that Ego_d has a
  complete function record for: Ego_d name, object file(s), debug length, how many records share the name (4 rows have 2:
  overloads, check the parameters), the number of named locals and their names, and the closest draft. Every one has a complete
  locals record; 560 have five or more named locals.
- **`EGOD_IDENTIFY.tsv`, 142 rows** (about 98 KB). Parked rows with made-up names (`_global::...`, `_unattributed::...`,
  `_gapscan::sub_...`, address-suffixed names). The nearest landed rows with a real name give the likely object file
  (`likely_object`, `named_neighbours`). Find the function in that object in `egod_carved_functions.tsv` by its order next to
  those neighbours, then confirm it by its parameters and locals against the retail body. **A name taken from Ego_d needs a second
  source** (Ego_r / FableWin names, retail strings, FSE, callers or vtable slots), and made-up names stay descriptive under the
  owner's 2026-11-10 rule only when no source has the original.

Not in the lists: about 400 other parked rows (templates and overloads Ego_d spells differently, or functions whose module stream
was not contiguous enough to keep), and the 98 rows that are not plain C++.

## Before the first row
1. Merge `FullDecomp` into `FullDecomp-B`. `FullDecomp` already has `FullDecomp-B`'s Ego_d commit (cherry-picked as 2c3014f1b6),
   so the merge only brings the newer landings, headers and this list.
2. Read the acctC reservation README (`../2026-11-13_acctC/README.md`): its "Rules that keep the hand-back landable" and the
   2026-11-14 acceptance test apply unchanged.

## Rules (short form)
- **Stay on today's headers.** Merge `FullDecomp` into `FullDecomp-B` at least once a day and before every hand-back, and
  re-verify every matched row against the merged `rebuild/include`.
- **Header changes are additions under new `#ifdef FABLE_RETAIL_*` opt-ins** against the current header, handed back as small
  diffs. Never reuse an existing opt-in name for different content; pin moved `FABLE_STATIC_ASSERT`s (`lane_merge/pin_asserts.py`)
  and prove the change over the whole tree (`lane_merge/equiv_check.py`, then `recheck_rows.py`).
- **Use a class's shared header** if it has one; no local models.
- Project rules (CLAUDE.md): byte match plus behaviour test, plain portable C++ (no `__asm` / `naked` / `_emit` in rows), PDB
  names from two sources, no retail bytes or the PDB itself committed, nothing landed by the lane itself.
- Hand back matched rows staged in pending folders with a final index (real lengths, like `../2026-11-13_acctB_FINAL/INDEX.tsv`),
  and keep a `STATUS.tsv` ledger here (one line per reserved row: OPEN / IN_PROGRESS / MATCHED_STAGED / PARKED, true length,
  staged folder). Rows renamed from Ego_d also go in a `NAME_FIXES.tsv` (address, correct_symbol, evidence, wrong_spellings_seen).

## Files
- `RESERVED.tsv`: kind (`named` / `identify`), address, module, name, length estimate, residue class, best grade on record.
- `EGOD_NAMED.tsv`, `EGOD_IDENTIFY.tsv`: the two lists above.
- `lane/egod_locals.py`: the locals printer (`--tsv` gives one summary line per name).
