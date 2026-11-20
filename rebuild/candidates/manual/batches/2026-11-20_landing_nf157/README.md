# Landing nf157 (Account A, 2026-11-20): NCreatureMode / NCameraModes names (27 rows)

Agent NF157 (round 10) handed back 21 entries (15 done, 4 done-already, 1 blocked, 1 deferred); its files are in
`naming/2026-11-20_agentNF157/`.

- 27 rows: 12 self renames (RENAMES section 1) and their callers onto both-PDB spellings. 0x00740450, 0x00742880, 0x008C0D10 and
  0x008C2D60 move the Jack of Blades scream mode into NCreatureMode together, which closes the nf147 class-move PRIORITY lines; split
  gate 0. 0x0084E110 is ported whole and its own name ends an existing split. 0x0069C3D0 now includes the class header, so its
  DeleteData copy calls the virtual destructor (it was instantiated on a forward-declared class).
- Header: engine/NCameraModes_CBase.h, one declaration opt-in; `orig` equals HEAD.
- Advisor decisions (owner delegated): 0x00872490, 0x00878DB0 and 0x008FAA80 reach CThing::GetPos through a cast of the
  forward-declared CThingGameObject result, marked `// Fidelity: cast, not retail` (port queued). The header-less local models of
  NCreatureMode::CModeDragonHover / CModeJackOfBladesScream (no shared header exists) are accepted.
- Recheck (`logs/recheck.log`): 1 MATCH + 26 RELOCATION_MATCH, every test PASS. rebind multi-check 1 (baseline); split gate 0 created;
  includers 169 good, 0 bad; tablecheck: 0x005ECF50, 0x0084DC10, 0x0084F1B0 shrink.
- Link (`logs/link_landing.log`): 2 LNK2005. CAnimFilterOfGroup(const CCharString&) is at HEAD too (`logs/link_control_head.log`).
  CCharString::GetCRC is new for these rows (0x008AC3F0 now uses FABLE_RETAIL_CCHARSTRING_GETCRC_INLINE) but the symbol already clashes
  at HEAD: 0x00847DF0 emits the same COMDAT against owner 0x00404310 (`logs/link_control_head_getcrc.log`). Queued as DEBT.
- Queued: the three cast rows' port, 0x008E3860's ??_G name with the dragon family (q11874), and a sweep for DeleteData instantiated
  on forward-declared classes.
