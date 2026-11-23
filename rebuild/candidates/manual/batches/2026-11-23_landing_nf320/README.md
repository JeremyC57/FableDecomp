# Landing nf320 (Account A, 2026-11-23): 61 alias rows onto the real member with the PDB signature, 4 add-only header opt-ins

Hand-back: `pending/2026-11-23_nf320_handback/`; helpers under `naming_batch/agent_tools/nf320/`. Entries: 144 alias rows with stand-in
parameter types, plus the 23 CCreatureAction Clone header ports from NF317. Outcome: 61 done, 25 skipped-held, 81 skipped-port.

- **Landed: 61 rows.**
  - **57 rows** move `X_Methods::M` onto `X::M` of engine/X.h, with the PDB parameter and return types, under class-model switches that
    landed rows already define (no `_PASS` switch).
  - **4 rows** through new add-only declaration opt-ins: `FABLE_RETAIL_CENEMYDEF_TRANSFER_DECL` and the matching ones for
    CExpressionSubDef, CGoldDef and CAIStateGroup_MinionReactToCriminal (InitStateGroup). Both PDB sets have each member, and each
    overrides a base virtual at the same slot. The moved asserts are pinned with `FABLE_STATIC_ASSERT_AT`.
  - No test changed.
- **Proof:**
  - **Per row:** precheck; vland HEAD and new under ONE work path (no `?A0x` drift); objsec with exactly one DEF rename, equal UND / DEF
    sets, no `??_7` / `??_R`, and every non-debug section equal; `.drectve` equal or the libcp exception (18 rows); the graded symbol is the
    PDB symbol; a link of HEAD and new with 0 duplicates and identical unresolved sets.
  - **equiv_check:** 57,146 landed rows and tests, 0 differing. The planted control flags the default-branch includers.
  - **At landing:** the four header originals equal HEAD. The recheck covers the 61 rows plus every direct includer of the four headers (78
    rows), all PASS, and every row defines its PDB symbol.
  - **0x009949F0** also renames its pick-any `operator->` COMDAT onto the FableWin name `boost::scoped_ptr<CCompressorZlib>` (NF304 Q1).
- **Rulings on NF320's questions:**
  - **Q1 (Clone pilot 0x0084C6C0, held):** a Clone TU emitting the class vftable is faithful, because retail's Clone constructs a copy. It
    lands only when the emitted `??_7` matches retail slot for slot. Slot 10's GetAnimExtraFlags override is missing from the header, so
    add it as an add-only declaration first. Its `??_G` / `??_E` pick-any is fine under the deleting-destructor ruling.
  - **Q2 (held):** a move that ADDS unreferenced pick-any COMDATs fails "COMDAT sets SAME", like NF318 Q5's drop (0x00B9D060, 0x004FC130,
    0x004FC150, and 0x00BCEA40 by the same answer).
  - **Q3 (allowed):** the retail RTTI placement plus ONE PDB set are two sources. RTTI is retail evidence, like a retail call site. The 6
    FableWin-only rows go to the next pass.
- **Queued:**
  - T3 `nf320` lines for the ports and holds;
  - NAME_FIXES_QUEUE: 0x00463BDB defines `CCombatAbilityFlourishCounterAttackDef::Copy`, while RTTI names
    `?Copy@CCombatAbilityBlockUnarmedAttackDef@@UAEXPBVCDefClassBase@@@Z` there.
- **Agent slips** (none reached the repo): one `rm -f` of a variable path inside its own folder (before switching to clean.py), a
  `sleep 0`, and one foreground `tail --pid` wait.

## Recheck
`logs/recheck.log` (landed tree, 78 rows = 61 rows + header includers): 78 PASS.
