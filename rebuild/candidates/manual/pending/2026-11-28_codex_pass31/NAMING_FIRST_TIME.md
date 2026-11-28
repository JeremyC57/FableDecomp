# Naming right the first time (Codex pass 30)

The owner's rule: every function, callee, global, class, member and script name is as close to retail as evidence allows, in the exact PDB spelling, one retail address = one name. Pass 30 adds: **your rows leave no new disagreement behind: any landed spelling your names contradict is fixed in the same pass, so nothing has to be queued.** This file overrides older naming text in docs/ where they differ, except docs/EXTRA_RULES.md's hard rules.

## Why (what pass 29 cost)
Pass 29's 184 rows matched, but the names did not land with them:
- **159 NAME_FIXES lines.** These are landed spellings Codex proved wrong but left for us. A naming agent spent an hour on them: 47 rows landed, and 100 lines are still blocked.
  - 31 of the blocked lines are landed rows whose self is a made-up stand-in.
  - About 15 more are landed rows that model the callee on a stand-in local class, so a respell there needs a port.
  - 9 sit behind holds.
- **116 landed addresses read as name disagreements** after the landing, until those fixes land.
- **7 rows backed out over names or a vftable:**
  - two callees against registered best guesses;
  - `_expf` against `?exp@@YAMM@Z`;
  - two anonymous-namespace globals against named globals;
  - two best guesses against held stand-in binders;
  - a 25-slot `??_7` where retail has 31.
- **5 rows were missing their own `// Retail-asm:` line.** 10 had no catalog name, and their Leaf had to be supplied.
- **11 registered best guesses were wrong.** Codex's callers proved the real names (9 from the PDBs, 2 from the GameCODA SDK). Those were our errors and Codex was right, but the proof should come with the row, not after it.

## 1. Where the names are (`names/README.md`)
| Folder | What | Use it as |
|---|---|---|
| `names/answers/NAME_ANSWERS.tsv` | one line per retail address the landed code names (64,151), with a verdict and the spelling to use | **the answer key** |
| `names/answers/descriptive_names.tsv` | the 2,017 registered best guesses, each with its basis | answers (verdict REGISTERED_GUESS) |
| `names/answers/ESTABLISHED_SPELLINGS.tsv` | 40 data spellings proven by hand (GGame, GSystemManager, ...) | answers |
| `names/pdb/` | Ego_r / FableWin / Ego_d names, publics, signatures, layouts, modules, source maps, RTTI slots, FSE manifest, library signatures | evidence for anything UNSETTLED |
| `names/state/` | binding table, landed selves, open queues, `LOCKS.tsv` | which landed rows your names touch, and which you may not edit |
| `names/hints/` | manifest / catalog labels, Ghidra label files, landed source paths | **not evidence**: never copy a name from here without a source in `pdb/` |

`rows/<addr>/NAMES_TO_USE.tsv` is the answer key cut to the row. It has one line for the row itself (SELF) and one for each retail call, jump and data target the row's retail bytes reference. Look up anything the row adds beyond these (an inline callee's own callees, a vftable's slots) in `NAME_ANSWERS.tsv` by address.

## 2. Verdicts
**PROVEN means:** exactly one landed spelling, and that spelling is a real PDB / std name. The tracker matches by name, not by body, so a PROVEN line is strong but not infallible. When your row's own evidence (its Ego_d record, an Ego_r / FableWin call-site signature, or FableWin's /Od call target) shows the call goes to a different function, that evidence wins. Spell what it says, put both names and the evidence in `NAME_CONFLICTS.tsv`, and respell the landed binders, because you created a new disagreement (section 4).

- **PROVEN:** one spelling, in a PDB name set, or a std / CRT / compiler / high-confidence library-signature name. Spell `use_this_spelling` exactly.
- **PROVEN_RTTI:** retail RTTI puts the address in a vftable slot whose PDB name is known. Spell it exactly.
- **PROVEN_LANDED_WRONG:** the PDB spelling is known, but the landed rows in `landed_wrong` spell it otherwise. Spell the PDB name, then do section 4 for those rows.
- **REGISTERED_GUESS:** a best guess (owner rule 2026-11-25). Spell it exactly. Replace it only with an original name proven from a PDB, the GameCODA SDK, FSE or retail strings. When you replace one, that is section 4 for every landed row that uses the guess, plus a REGISTRY line (section 5).
- **FOLDED:** several PDB names for one body the linker folded. Every listed name is true; use the one the calling function's source used, from its Ego_d / Ego_r call-site record.
- **UNSETTLED / NOT_NAMED_YET:** nothing proven yet; the hint spellings are only hints. Settle it before you write the code (section 3).
  - 106 of these are vftable names the tracker counted as compiler names. Either their class contradicts retail RTTI (`NParticleEngine::CAEnvironmentInteraction`, not `CAEnvironmentInteraction`), or their base qualifier is a stand-in (`CComponentBase3_Retail`). The `basis` column says which.

## 3. Settling an UNSETTLED name (two sources)
1. **Functions:** the mangled name in `pdb/egor_pdb_names.tsv`, `pdb/fablewin_pdb_names.tsv` and `pdb/egod_carved_names.tsv`, found by class and method. Confirm the signature with the row's `ego_r_proc.txt` / `fablewin_proc.txt` and the call-site records. In FableWin's /Od body, `tools/fwsym.py` names call targets in exact PDB spelling.
2. **Data:** `pdb/Ego_r.publics.txt` / `pdb/FableWin.publics.txt`, plus the use site (the type it is read or written as).
3. **Library code:** GameCODA SDK symbols, `pdb/library_signatures.tsv` / `vendor_signatures.tsv` (confidence high), and CRT names from VC7.1 `msvcrt.lib`.
4. **No original name anywhere:** a best guess.
   - Use retail's own style for that class and module, and say what the function does.
   - Never use `vfunc` / `sub_` / `Unproven` / `_Retail` / `_Methods` / an address suffix.
   - Never put a guess over a proven name.
   - Register it in `DESCRIPTIVE.tsv` (section 5) with its basis: `one-source:<pdb|sdk|layout|caller>` or `style:<class/module>`.

Write every name you settle into `NEW_NAMES.tsv`: address, mangled symbol, the two sources.

## 4. Same-pass respell (the no-queue rule)
**Your rows must never leave a new disagreement behind.** When you give an address a different spelling from the one the landed tree agrees on, you fix those landed rows in this pass. That covers three cases:
- an UNSETTLED address with one landed spelling, which you settle to another name;
- a REGISTERED_GUESS you replace with a proven name;
- a landed self you prove wrong.

This is required: without it your row lands as a split, and the fix becomes a queue line.

PROVEN_LANDED_WRONG targets are different. The split already exists, and spelling the PDB name puts your row on the right side of it.
- Fixing those landed rows is welcome when it is cheap: a spelling-only change, on rows you are already touching.
- It is not required. These 100 rows reach 465 such targets, carried by up to 442 landed rows, and the tracker already lists every one of them.
- Matching your rows comes first.

For each landed row you fix:
1. **Find every landed row with the old spelling.**
   - Start with `landed_binders` (NAMES_TO_USE.tsv / NAME_ANSWERS.tsv) and `names/state/reloc_bindings.tsv`.
   - The sources and tests of every landed row that binds one of these rows' non-proven targets are in `landed/<addr>/` (`landed/INDEX.tsv` gives their catalog paths, flags and locks), as they stand at repo commit @@BASE_COMMIT@@.
   - Grep them for the old spelling: the binding table misses callers. For a landed row not shipped, give its address in NAME_CONFLICTS.tsv.
2. **Check the lock.** If a row is in `names/state/LOCKS.tsv` (another lane holds it) or in `names/state/T3_PORT_QUEUE.tsv` (held), do not edit it. Write it to `NAME_CONFLICTS.tsv` instead. These are the only lines that queue.
3. **Otherwise return a RESPELL unit:** `respell/<landed addr>/<addr>.cpp` (the `landed/` source, changed only as far as the spelling needs) and `<addr>_test.cpp`. Grade it on your final include tree like any row. Also add a `BINDINGS.tsv` line: action, old symbol -> new symbol, target, witness <landed addr>.
   - **A spelling-only change** (a declaration or callee name) must keep the bytes: same grade, and the object differs only in that symbol.
   - **A landed row that models the callee on a stand-in local class or a made-up free function** needs a port onto the shared header, `include/engine/<Class>.h`, with an add-only opt-in if one is missing.
     - Regrade the port like any row: MATCH / RELOCATION_MATCH with its test, and the pick-any check.
     - A port that does not match goes to `NAME_CONFLICTS.tsv` with its best text. It is not a reason to keep the wrong spelling in your own row.
4. **Self respells move every binder.** When the landed self changes, every landed binder of the old spelling moves with it, in the same unit.

`NAME_CONFLICTS.tsv` lines say exactly why the fix could not be done in this pass: a lock, a hold, a failed port, or a ruling needed. Pass 29's NAME_FIXES.tsv format is retired.

## 5. Your own rows
- **Leaf:** every row starts with `// Leaf: <exact mangled self>`. LABELS.tsv gives address, mangled name and catalog name.
- **Catalog name:** demangle the leaf. Never put a calling convention in the class (`__thiscall CFoo`), and never leave `class ` / `struct ` keywords inside template arguments.
- **One address, one name.** Byte-identical bodies at two addresses are two functions unless NAME_ANSWERS says FOLDED.
- **Wide characters:** `unsigned short` (mangled `G`), never `_W`, and no `/Zc:wchar_t` (owner, 2026-10-29). A PDB `_W` spelling maps to `G`.
- **Anonymous namespaces:** VC7.1 hashes them per translation unit (`?A0x<8 hex>@`), so the hash differs between TUs and the tools ignore it. Put a symbol in an anonymous namespace only when the PDB shows it in one (Ego_d / FableWin `_anon_` or `?A0x`). Never move a name that NAME_ANSWERS lists as a named global into one (pass 29 back-outs 0x00A19430, 0x009A2000).
- **Two original names for one address** (the C-linkage CRT `_expf` against the SDK's `?exp@@YAMM@Z`): spell what NAME_ANSWERS says. If it is UNSETTLED, put both, with their sources, in `NAME_CONFLICTS.tsv` and spell the one the calling TU's own header declares (back-out 0x00C72200).
- **vftables:** a `??_7` your object emits has at least retail's slot count (`pdb/rtti-vtables.tsv`; rule r459a). Each slot names its NAME_ANSWERS spelling, and the class is the retail RTTI class (back-out 0x00625160).
- **Retail-asm helpers:** a row that calls a `// Retail-asm-helper:` header carries its own `// Retail-asm:` line, giving the 0x address and mnemonic of the helper's instructions in this function (docs/ASM_RULES.md; pass 29: 5 rows).
- **Registry:** best guesses you register go in `DESCRIPTIVE.tsv` (address, symbol, basis, note). Guesses of ours you prove wrong go in `REGISTRY_REMOVE.tsv`, each with its proof.

## 6. Self-check before you return a row
`tools/reference/name_gate.py` is the coordinator's gate (it runs in our lane; read it as a checklist). For every row and RESPELL unit:
- the Leaf is exact and in LABELS.tsv;
- every relocation's spelling is PROVEN, PROVEN_RTTI, REGISTERED_GUESS, FOLDED-listed, or in your NEW_NAMES.tsv / DESCRIPTIVE.tsv;
- no relocation spells an address two ways across your rows plus the landed tree, after your RESPELL units;
- no row-local class that has a shared header, and no typed-in retail address.
