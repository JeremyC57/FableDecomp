# Landing nf301 (Account A, 2026-11-23): T1 "other" class / namespace, NF298's continuation (6 rows, 21 table lines)

Hand-back: `pending/2026-11-23_nf301_handback/`; helpers (fix301.py exact-pair edits with a line-count check) under
`naming_batch/agent_tools/nf301/`. 59 entries with a verdict (1-60 less 14 / 15 / 44 / 45 / 46, plus 63, 68, 309, 427 and NF298's
entries 7 / 8): 6 done, 4 done-already, 8 done-partial, 29 skipped-port, 12 skipped-held, no rejects; no header or vtable change. The
five header entries and the 401 unreached go to NF302.

- 0x00B81BC0: member type `CPerlinNoise` into `NFractal::`. 0x00DC21F0: the stand-in parent `CQ_BanditRaidScript` (no PDB class) folded into
  the row's `NScript::CQ_OakValeBanditRaidScript` model, `ParentClass` retyped. 0x006778A0: free `__fastcall CreateFromType` -> a static
  member of the local `CReactionFrequencyTraits`. 0x006B84B0: `CScreenEffectPriorityStack` into `NScreenEffect::`. 0x00717C50: free
  `GetMessageFromTextIndex` -> a static member of the local `CSubtitledMessage`. 0x0092BCB0: view struct `CTCAvoidRegionExitView` ->
  `CTCAvoidRegionExit` (no shared header for it). Every new name in both PDB name sets and already defined by its owner.
- Proof: RELOCATION_MATCH at HEAD and new; objsec ALL_EQ under one-symbol rename maps, vtables unchanged; link: the 6 stand-ins HEAD left
  unresolved now resolve, 0 duplicates; every other exe caller already binds the PDB spelling; no landed file includes any of the rows.
- Table: 4 respelled, 17 deleted (15 stale witnesses, each witness's HEAD object compiled: 0x00487520, 0x00449970, 0x00449960, 0x00CDD360,
  0x00A821C0, 0x00961B90, 0x00D3AF50, 0x00DF2F90, 0x0051CBB0 and the global copies in 0x00F09640 / 0x00D167C0 / 0x00D16BB0, plus
  0x00F09830 WatchForContactDeath from the same object; NF298's two `CIMECalls` lines among them). Left for NF302: the GetPlayer line whose
  witness 0x0066FB80 is stale (rewitness). Left for NF299's landing: `?GFGetIME@@YAPAUCIMECalls@@XZ` (0x00404A80) is stale too, since
  0x00851860 binds `?Get@CIME@NUISystem@@SIAAV12@XZ`.
- Rulings: VC10 Dinkumware PDB spellings (ClearParticle / ClearDecal `_List_val`) where binders bind the STLport form are equivalent, not fix
  targets (tracker follow-up on the ROADMAP); a table-less global base may move into its PDB namespace in place when objsec is ALL_EQ.

## Recheck
`logs/recheck.log` (landed tree): 6 RELOCATION_MATCH PASS, 0 DIFFER, 0 RELOC_CONFLICT. 37 skipped-port / done-partial remainders queued in `naming/T3_PORT_QUEUE.tsv` (source nf301); NAME_FIXES.tsv empty.
