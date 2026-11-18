# Name-fix agent NF69 (2026-11-17, round 7): apply the names Ego_d.pdb settled (40 entries, 33 rows)

Owner: "Continue the naming fix queue with two agents until complete", "Keep going until queue is done" and "add another agent to
speed up naming queue completion". Round 7 has fixed about 1,600 rows. Another account carved Ego_d.pdb (a VC10 debug PDB of the
same game) and answered several questions earlier agents left open. Its write-up is landed at 2c3014f1b6:
`docs/journal/2026-11/EGOD_FINDINGS.md` (read all of it first), with two new lookup tables `ghidra_out/egod_carved_names.tsv` and
`ghidra_out/egod_carved_functions.tsv` (function -> Ego_d object file). NF67 and NF68 run beside you on other rows.

**Ego_d is VC10 + Dinkumware.** Game class, member, static, enum and function names carry over to retail. Its STL internals
(`_Tree`, `_Tree_val`, `_List_val`) do NOT: spell every STL instance from the repo's STLport headers (`rebuild/include/stlport/*`,
`fable_map.h`) the way landed tree rows already do (grep the catalog for `std::_Rb_tree<` rows and copy their form). A whole
`std::map<K, V, less<K>, allocator<...> >` type mangles the same in both, so the decorated names of the four static maps below are
right as given.

## Your slice
Files in `$S/w22/agentNF69/`:
- `ENTRIES.tsv`: 40 entries (`q10819`..`q10858`, tag `egod1117`, already appended to
  `rebuild/candidates/manual/naming/NAME_FIXES_QUEUE.tsv`): entry, tag, address, correct symbol (or a description where the
  spelling comes from STLport), evidence, wrong / current spelling.
- `WORKLIST.tsv`: landed rows that bind an entry address under another spelling (from rebuild/oracles/reloc_bindings.tsv at
  2c3014f1b6): entry, target, binder_row, bound_symbol. 44 lines.
- `rows.txt`: the 33 rows you may change. All are locked as NF69 already.

The groups:
1. **Hit locations** (`NHitLocations::CHitLocation`): the static map `RegisteredHitLocationCodelets` 0x013BAF2C (private), its
   dynamic initializer 0x0121F4F0 and atexit destructor 0x01229F30, `StatRegisterClass` 0x008162F0, the tree `_M_insert` 0x00817D70
   and `insert_unique` 0x00818000, the reader `ProcessHitLocationCode` 0x00815E80, and the four codelets 0x008F5D00 / 0x008F5CF0 /
   0x008F5D40 / 0x008F5E00 (`NHitLocationCodelets::HLC*`, `__fastcall (CThing&, const CHitParameters&)`). Only 0x008F5CF0 is a
   catalog row (as `_unattributed::CallWith_008f5cf0`); its self name becomes `NHitLocationCodelets::HLCAutoKill`.
2. **Combat sequences** (`CCombatSequenceBase`): the three protected static maps 0x013BAFA8 / 0x013BAFB4 / 0x013BAFC0, their
   initializers 0x0121F680 / 0x0121F6C0 / 0x0121F700 and atexit destructors 0x0122A0A0 / 0x0122A100 / 0x0122A160,
   `StatRegisterClass` 0x008B08B0 (catalogued as CScreenEffectPriorityStack::Process), the three tree pairs
   (0x008B29A0 + 0x008B2DD0 IsValid, 0x008B2A60 + 0x008B2E90 OnStart, 0x008B2B20 + 0x008B2F50 OnStop), the readers
   CCombatSequenceGeneric::IsValid / OnStart / OnStop (0x008B07E0 / 0x008B0830 / 0x008B0870), the caller
   CCombatManager::RegisterCombatSequences 0x008B0D10 (binds 0x008B08B0 as `?PostRegister@CCombatSequenceMap@@QAEXXZ`), and the
   nine handlers in `NCombatSequenceIsValidModules` / `NCombatSequenceOnStartModules` (none is a catalog row; respell them in
   0x008B08B0's binding).
   - Check the enums `EHitLocationCodelet`, `ECombatSequenceIsValidType`, `ECombatSequenceOnStartModuleType`,
     `ECombatSequenceOnStopModuleType` and the static members against `rebuild/include/engine/` headers (CHitLocation.h,
     CCombatSequenceBase.h or wherever they live; `ghidra_out/struct_layouts_egor.tsv` and `fable_types.h` have the classes). Add
     the static members and typedefs as `#ifdef` opt-ins in your overlay if the headers lack them.
3. **CTCTargetingPlayer destructor** 0x004E37BF (catalog name right; its binding uses stand-ins `CDefMember_004e1aaf`,
   `CDefBase_004da8b7`). Section 4a of the findings maps the body onto Ego_r's layout. Port it onto engine/CTCTargetingPlayer.h
   with the real `vector<NPlayerTargeting::CTargetPoint>` members and the real base destructor, then respell its other binders:
   0x004E37A3 (binds it as `CTCTargetingPlayer_Destruct`), 0x004E0080 / 0x004E1023 / 0x0047EEE4 (bind 0x004DA8B7 under stand-ins).
   Prove from the base chain in the headers (and Ego_r) which class 0x004DA8B7 destroys: the catalog says CTCTargeted; Ego_r has
   `??1CTCTargeted@@UAE@XZ`. 0x004E1AAF's spelling comes from STLport `~vector`.
4. **0x00CD3A16** is `map<int, CCharString>::operator[]` (catalog says `long`): re-author under the given spelling, so its call to
   0x00CD36C2 binds the `int` insert.
5. **0x009C8890** (NGameText::CDataBank::GetPGroupEntry) was blocked on CGroupEntry's base. Findings section 5 settles it: CGroupEntry
   is a class with public base CResource at offset 0. Add that as an opt-in in your overlay of engine/NGameText_CDataBank.h and port
   the row. If it needs the default path of that header changed, mark the row `blocked/header` and write the exact change in NOTES.md
   (the coordinator makes default-path changes).

Do not touch the questions the findings leave open (the identical destructor pairs, the set<long> finds, libpk).

## Per row
1. Read the entry, the evidence and the binder lines in WORKLIST.tsv.
2. Port the row onto the shared headers (no local model of a class that has a header), with `#ifdef` opt-ins in your overlay for
   members the headers lack. The wrong binding must become the entry's spelling.
3. Grade until RELOCATION_MATCH / MATCH with the test PASS: **up to 15 grades per row.** Take the closest rows first and carry
   each to a terminal grade before starting the next. A row with one draft grade at hand-back is not finished.
4. The tree rows and the four map initializer / destructor rows share one shape each: prove the edit by hand on one or two, then
   script it for the rest (NF44's `$S/w22/agentNF44/work/scratch.py` shows the method).
5. If a row's self name changes, re-author it under the PDB spelling and put it in RENAMES section 1 (address, module, name,
   mangled_leaf, old_catalog_name, old_leaf, evidence). `module` + `name` is the catalog name; give the real class / namespace and
   member (no `Class::` prefix in `name`). For STL instances use the module spelling the catalog already uses for that container
   form. For a leaf-only change on a row whose catalog name is right, repeat the old catalog name.

**Every row must end `ported`, `not-a-binder` or `blocked`.** `carry` is not allowed. A row already right is `not-a-binder` (say
why). A blocked row needs the grade count and the exact compiler error or DIFFER.

## Also carry the lessons
These sections of `$S/w22/agentNF44/BRIEF.md` apply unchanged; read them: "Lessons of earlier passes", "Disk", "Round-3 /
Round-2 / Round-1 lessons", "Per entry", "Hard rules", "Lane", "Hand-back". Then read the newest LESSONS.md files:
`$S/w22/agentNF68/LESSONS.md`, `$S/w22/agentNF64/LESSONS.md` and `$S/w22/agentNF41/LESSONS.md`. The tool scripts in
`$S/w22/agentNF68/` (g.sh, vl.sh, pay.py, fpay.py, st.py, claim.sh) are a working set: copy and repoint them to your directory.

The main lessons, repeated here:
- No new local model of a class that has a shared header (that includes renaming a stand-in to the real class name, and including a
  header under a renaming macro). No cast addresses. No `__asm`, `naked`, `_emit`, and no `#error` probes in rows.
- Overlay headers carry `#ifdef` opt-ins only, with macro names specific to the member (`FABLE_RETAIL_<CLASS>_<MEMBER>`); a body
  edit must be token-identical without the opt-in, and you must say so in hdr/README.md. NF67 and NF68 may add opt-ins to the same
  headers.
- Work only in `$S/w22/agentNF69/`, using absolute paths. Grade in your private `S=<your dir>/pS`. Never run git. Never write under
  /home/user/FableDecomp.
- Run one compile at a time, always inside `timeout`. Every Bash call must exit 0 (end `grep -c` / `diff` loops with `; true`).
- Write any file or note argument that contains backticks with a quoted heredoc (`<<'EOF'`), never inside double quotes.
- If you change a row that EH funclets include, regrade those funclets.
- Count before you report: every number comes from your files.
- Claim any extra row before changing it: `mkdir $S/w22/nflocks/<addr> 2>/dev/null && echo NF69 > $S/w22/nflocks/<addr>/owner; true`.
  If it is held by another agent, record it `deferred: held by <owner>`.

## Hand-back (in $S/w22/agentNF69/)
- rows/; hdr/ and hdr/README.md;
- RENAMES.tsv (section 1 header line exactly as in NF44's brief, then `# Section 2`);
- STATUS.tsv (one line per row in rows.txt, with its terminal status);
- APPLIED.tsv (one line per entry: done, done-already, or blocked with the rows left);
- NAME_FIXES.tsv (address, correct_symbol, evidence, wrong_spellings_seen) for proven fixes outside rows.txt;
- REJECTED.tsv (any entry the evidence does not hold up for, with why), NOTES.md (mark descriptive names) and LESSONS.md;
- recheck_final.log: one vland run over every changed row.

Report briefly: rows ported, not-a-binder, blocked and deferred; entries done and blocked; header opt-ins; rule slips.
