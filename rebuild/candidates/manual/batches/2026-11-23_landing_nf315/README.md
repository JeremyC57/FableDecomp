# Landing nf315 (Account A, 2026-11-23): NF313's in-place leftovers + uncovered T1 entries (20 rows, 8 table lines)

Hand-back: `pending/2026-11-23_nf315_handback/` (`held_t3q/` keeps the rows T3_PORT_QUEUE names; `unreached.txt`); helpers under
`naming_batch/agent_tools/nf315/` (offl.py v3 scans every T3_PORT_QUEUE column). 32 entries: 1 done, 8 done-partial, 10 skipped-port
(0x00738F90 reverted: its `const T**` respell changes the `.drectve` /alternatename, NF303 Q2), 13 skipped-held; no header, test, flag or
catalog change.

- The 18 landing-ready rows: B1 0x00919A70 `CMessageEventFilter_IsOriginatedByThing(const CThing&)` (keeps its existing cast, now
  dereferenced); B2 0x008C9F40 / 0x008CA4A0 `GetHeightZBelowPos(const C3DVector&) const`; B3 0x006D56E0 protected `RemoveEmoteIcon(CThing&)`;
  B5 0x0091E050 / 0x009621F0 the PDB IAE `AIAction_CombatPerformActionAndWait(const CCombatActionBase&, bool)` (owner still a stand-in,
  NF302 Q2); B6 0x00B2C460 `AddResource(CResource&)`; G the 7 free `long` GiveTutorialForCategory binders (0x005BE028, 0x005EE418,
  0x006D77C0, 0x0072E940, 0x0077D4B0, 0x00784600, 0x007E00D0) onto the PDB enum through `FABLE_RETAIL_TUTORIAL_CATEGORIES_PASS26` +
  `engine/ETutorialCategory.h` (NF300 Q1); Q the 0x00843F50 ctor self onto its PDB form (full Leaf) with 0x007F03D0 and 0x007F01F0; W2
  0x006EBEE0 `CreatureUnsheathWeapon(CThingCreatureBase&, EUnsheatheWeaponType)` on the shared `engine/EUnsheatheWeaponType.h`.
- Plus two held units, landed under the NF316 Q3 ruling (a T3_PORT_QUEUE column-4 mention is not a reservation): B1+ 0x00934920 with the
  `??0CMessageEventFilter_IsOriginatedByThing@@QAE@PAVCThing@@@Z` delete, Q+ 0x007EF600 with the ActivateQuest reference / reference delete.
  Still held (named in a unit's column 3): B5+ 0x009624D0, and the W owner unit 0x006EBDD0 + 0x0092B3E0 + 0x006ED510.
- Proof: HEAD and new RELOCATION_MATCH; objsec ALL_EQ under one rename (`.drectve` included); COMDAT sets SAME; links lose nothing HEAD
  resolved. Staleness: all 24 orig/ copies (18 + 6 held) equal HEAD at landing (after nf314).
- Table: 7 deletes and 1 rewitness: SetPItem `PAVCThing` from the stale 0x0089E540 onto 0x007FDEB0 (of 76 sources declaring it, 3 make real
  retail calls and all 3 objects bind it; 0x007FDEB0 confirmed at landing). The rewitness came as `old -> new` and apply_table.py's new guard
  took the new side (`logs/table.tsv`).
- Rulings: Q0 above. Q1 keeping an existing cast, now dereferenced, adds no cast debt: lands. Q2 replacing made-up local enumerators with
  the shared enum's PDB names is NF300 Q1. Q3 the owners' standalone tests stay as they are (NF284 D2). Q4 the 0x007008C0 ClearOpinionSource
  owner unit stays held until 0x007846B0 leaves the T3 queue and NF314 has landed (it has).
- Queued: 10 T3 ports; NAME_FIXES_QUEUE: owners 0x0097DA40, 0x009FC380.

## Recheck
`logs/recheck.log` (landed tree, 20 rows + witness 0x007FDEB0): WINS 21, 0 DIFFER, 0 RELOC_CONFLICT.
