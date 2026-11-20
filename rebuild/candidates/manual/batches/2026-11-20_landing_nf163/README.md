# Landing nf163 (Account A, 2026-11-20): NScript quest script names, CQ_/CV_ N-Z (10 rows; 0x00DCF6E0 held)

Agent NF163 (round 10, finished in a resume session after a container restart) handed back 45 entries (43 done, 2 done-already),
11 changed rows. Its files are in `naming/2026-11-20_agentNF163/`.

- 10 rows land. 0x00DABAC0 CQ_NewOakValeIntroScript::Main moves onto fable_string.h / CEntityScriptBinding.h /
  CQ_NewOakValeIntroScript.h. That serves the 33 Alloc entries, whose 16 rows already carried the PDB CNOVI_* names; its
  defined-symbol set equals HEAD's. Self renames (7): the ArcheryOpenState Transfer / TransferBinaryIn / TransferOut family
  (0x00CDCC20 / 0x00CDCD00 / 0x00CDCDB0), CDarkwoodTrader ??_G 0x00E016F0, CV_TempleOfLightScript::WatchForQuestInfoDisplayConditions
  0x00EDBD10, CQ_NewOakValeIntroScript::AddBadDeed 0x00DAEA70 (the PDB nested enum EBadDeeds), CNOVI_Bully::GivenTeddy 0x00DBCD00.
- Headers (`orig` equals HEAD): CQ_TraderEscortScript.h (DARKWOODTRADER_CTOR, declaration only) and CQ_NewOakValeIntroScript.h
  (ENTITIES3: CNOVI_AffairWoman / BookTrader / Barrel / CreatedBeetle and COVI_DeadFather from Ego_r layouts, five size asserts).
  pin_asserts line-pinned the existing asserts (2 + 12); with no opt-in defined the headers are token-identical to their bases.
- Held: 0x00DCF6E0 (q4134, the CTeamSpawn ??_G re-author, RELOCATION_MATCH). Its object emits ??0CTeamSpawn pick-any, which
  clashes with the strong copy in landed 0x00DCF700 (`logs/link_held_00dcf6e0_teamspawn.log`). That clash does not exist at
  HEAD, so under the hold rule the row stays at HEAD. The re-author is kept in `naming/2026-11-20_agentNF163/held/`, and the fix
  (port 0x00DCD160, make 0x00DCF700's copy pick-any) is queued.
- Recheck (`logs/recheck.log`): 10 RELOCATION_MATCH, every test PASS. rebind multi-check 1 (baseline); split gate 0 created;
  includers 87 good, 0 bad. tablecheck: 0x00E01710 is now recorded once, as ??1CDarkwoodTrader (it was a free-function stand-in);
  0x00DAEA70 and 0x00DFC320 shrink to one spelling.
- Link (`logs/link_landing.log`): 3 LNK2005 vs 2 in this landing's HEAD control. The extra one, ??0CDarkwoodTrader, already
  clashes at HEAD between 0x00E01670 and 0x00E01710 (NF163's proof, `logs/link_control_head_darkwoodtrader.log`), so it is queued
  as DEBT, not held.
