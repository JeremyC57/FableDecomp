# Naming pass, wave 14 (one agent, after the chunk pair 652/653; two chunk agents share the lane)

Owner rule 2026-11-01, in force for every decision: consider how retail did it and recreate it faithfully. The decisions are
resolved in `rebuild/candidates/manual/naming/2026-11-01_wave13/DECISIONS_W13_RESOLVED.tsv`; canonical globals and members in
`naming/ESTABLISHED_SPELLINGS.tsv`. Rules, mechanics and hand-back as `briefs/BRIEF_W13.md` (read it, the wave 13 README, DEFERRED.tsv
and the wave 13 `scripts/` -- compose13.py, fam_glob.py, fam_gptr.py, hand13/ -- which this wave reuses as compose14.py etc.).

## Scope, in this order (name-only unless the decision says re-author; every family rechecked, one commit per family)
1. GetWorld@CAIStateGroupBase 0x008FCFC0: the 48 non-PDB callers (`runs/` of wave 13 lists them via famrep.py), each modelled as derived
   from CAIStateGroupBase, `GetWorld().` at the use sites; rows whose bytes move -> DEFERRED with the diff.
2. VIRTUAL_SCREEN_WIDTH / HEIGHT 0x013961E8 / 0x013961F0: 19 + 18 callers to `extern double ...;` -- ONLY if the coordinator has recorded the
   PDB data-symbol confirmation in ESTABLISHED_SPELLINGS.tsv (the names come from the Codex read of Ego_r data symbols, which this repo
   cannot re-verify); otherwise DEFERRED. Same gate for PMainGameComponent 0x013B86A0 (DECISIONS_W13_RESOLVED, GetCurrentWorldFrame row).
3. GetCurrentWorldFrame 0x004167C8 (15 callers, `g_pMainGameComponent->GetCurrentWorldFrame()`); bind 0x0049D870 as `?GetFrame@CWorld@@SIJXZ`.
4. vector<CCharString> dtor 0x00414EA0: header-based family (54 rows); byte-moving rows -> DEFERRED with the diff.
5. 0x0070C770: CIVCountedPointeeBase slot 1 is Delete (respell the invented Validate).
6. IS_THING_ALIVE<const CThingPhysical> 0x006961F0 callers: prove the +0x14 member type per caller from struct_layouts_egor.tsv, then respell.
7. The wave 13 DEFERRED items that need per-row modelling (IsPerformingAnyAction, GetWorld@CThing IAE, Initialise@CCombatActionBase UAE,
   LowerBound CVectorMap models) and the remaining QUEUE_W13 rows (c651 rows included: NAME_FIXES_QUEUE prefix c651, the CInputEvent /
   CInputManager / CProcessedInput const + enum return-type respells).
8. Anonymous-namespace re-authoring: the 0x0097ABF0 comparator family (12 rows) and CIsImposterHit (3) onto the PDB spelling with the
   retail hash; these may move bytes -- re-author until byte-exact or park with the diff (chunk-style, keep probe logs).
Not in this wave (queued elsewhere): the CDefString implicit-conversion callers (re-author, retry row set), 0x0094BB20 (migration wave 10),
0x00B677D0 (re-author pass).
