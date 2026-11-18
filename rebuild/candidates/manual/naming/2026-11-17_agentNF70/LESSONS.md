# NF70 lessons (round 7 name fixes)

1. **Grade a respelled self row under its new leaf.** If you don't, q.py warns "leaf matches no symbol" and grades the largest
   function in the object. That can be a different function, which gives a bogus DIFFER (0x006994F0 graded 112v108 until the
   leaf was given). For the same reason, write the new `// Leaf:` line into the row: pay.py hands it to vland. `setleaf.py`
   does it.
2. **Check what the baseline grade picked.** When the landed catalog leaf is a bare word, the baseline can grade the wrong
   function, as it did at 0x006C9EE0 (a 9-byte ctor). Look at the `# ` lines before trusting OTHER_CHANGED.
3. **"Renaming a stand-in to the real class name" covers whole-class renames inside a row.** First check `rebuild/include`
   (engine/, libpk/, stlport/) for the target class. For example, CEventTriggerRef lives in libpk/CBankProcessor.h. If the
   class has a header, the row has to move onto it, or be blocked. A clean RM + PASS rename draft is not enough.
4. **A row needing only a respell often has a header opt-in waiting for it.** Search the headers for the target member
   before you edit the local model. Examples:
   - CPersistContext `TRANSFERVECTOR_NAMED_BODY` (0x006C9EE0)
   - CThingCreatureBase `SETPITEMTOUNSHEATHE` (0x00662450)
   - CThingGameObject `LEVELSCRIPT` + CThingPhysical `CONSTRUCTFROMPARAMS` (0x008388D0)
   - CStaticMapBankStateBlock `_CLASS` (0x00B430A0)
   - STLport `FABLE_RETAIL_STLP_FIND` (0x0053C8F8 / 0x0053C91C)

   Each of these was a one-grade port.
5. **Retail CThing-chain headers need their chain macros.** engine/CThingBuilding.h only derives from CThing when
   `FABLE_RETAIL_CTHINGGAMEOBJECT` and `FABLE_RETAIL_CTHINGPHYSICAL` are defined. Without them, `IsDead` / the CThing* conversion
   fail (SRC_FAIL).
6. **CIntelligentPointer assignment needs `FABLE_RETAIL_CINTELLIGENTPOINTER_ASSIGN`.** Without it, the inline copy skips the
   out-of-line GetPItem call (DIFFER 63v74 at 0x00662450).
7. **Check the callee's access in local STLport models.** When you give a local model STLport's template arguments, also give
   the callee STLport's access. `_M_insert_overflow` is protected (IAE), and the landed binders of the same callee use that
   spelling. bnd.py shows it.
8. **Anonymous-namespace helpers get a per-TU hash** (`?A0x........`). A self row and its caller in different TUs can never
   agree on it. Flag such pairs to the coordinator rather than chasing the hash.
9. **Run the final recheck in the background with a longer timeout.** vl.sh is capped at 900 s, and a large final run
   can be slow. A copy (`work/vlf.sh`, 5400 s) ran in the background while the docs were written. It finished in about
   70 s for 88 rows.
10. **Do not overlay fable_string.h.** The session's permission check refused it. Write the exact opt-in into NOTES.md and
    block the entry `blocked/header`.
