# Landing 2026-11-06l: chunk 811 (1 of 6 rows landed, 5 parked)

One expert agent authored the rows (sharing the lane with the chunk 810, then 812, agents). The six rows were unnamed gap-scan Mains
(6.4-6.8 KB). The agent proved every name from the vtable slot and both PDB name sets (the repo labels were already right). The
landed row is plain C++, uses only repo headers, and grades `genuine`.

| Address | Name | Grade |
|---|---|---|
| 0x00DCA6D0 | NScript::CQ_OpeningGraveyardSecretPassageScript::CSecretPassageGraveyardKeeper::Main | RELOCATION_MATCH PASS (first attempt; the cut-scene block is a nested `{}` so a store follows its dtors) |

- **Headers:** none copied. The landed row needs none of the agent's overlay, so the overlay stays with the parked rows in
  `pending/2026-11-06_linux_hand811/hdr/` (as for chunk 810): CNOVI_Bully (`_BULLY`), CMotherTorture (`_MOTHERTORTURE`) and
  CTOL_AcolyteGood (`_ACOLYTEGOOD`) opt-ins, and a new engine/CQ_TraderConflictEvilScript.h. The agent rechecked the 8 landed
  includers with it: all RELOCATION_MATCH PASS (SIBLINGS.tsv).
- **vland warnings:** RELOC_NAME on 0x00CDBFB0 (the queued map-dtor placeholder).
- **Names.** Labels: `ghidra_out/labels_landing_2026-11-06l.tsv` (the six Mains). 6 queue lines (tag `land0611l`): 0x00DBCD00
  `?GivenTeddy@CNOVI_Bully@...` (catalog name lacks NScript); the cast-map dtor / ctor 0x00CDBFB0 / 0x00CDBF70 (also bound as
  `CCutsceneCastMap` and `AutoTiny...`); 0x01375748 `?GDefaultToUsingPhysicsInCutsceneAnimations@NGlobalConsole@@3_NA` (chunk 809
  found three bound names; this is the agent's PDB-backed pick, to confirm); 0x0099F1F0 `CCharString::Sprintf` (also bound as Format);
  0x0143E8EC `CScriptThing::NullString` (bound under seven names). The two Bully globals 0x013AC85C / 0x013AC860 have descriptive
  names only (no PDB name) and are used only by a parked row: not labelled.
- **Landing:** `vland --land` (`logs/land.log`); labels applied; the manifest sync dry run prints 0; bindings_multi_check prints only
  the known NullCoord case; the row adds no new relocation binding.

## Parked (`pending/2026-11-06_linux_hand811/`)
| Row | Grade | Residue |
|---|---|---|
| 0x00EDD170 NScript::CV_TempleOfLightScript::CTOL_AcolyteGood::Main | DIFFER(6597v6594), 10 attempts | epilogue cross-jumping only (GOTCHAS chunk 801): VC7.1 keeps the shared [~altar, ~seh_me, ret] tail after the first pad, retail after the last |
| 0x00DD6A60 NScript::CQ_PrisonEscapeRescueMotherScript::CMotherTorture::Main | DIFFER(6478v6494), 7 attempts | the CIsThingAlive nested-copy blocker (chunk 810), with ebx / ebp colouring and pop placement at three return pads |
| 0x00DBB310 NScript::CQ_NewOakValeIntroScript::CNOVI_Bully::Main | DIFFER(6527v6639), 7 attempts | the same blocker on the home-loop temporary dtor and loop rotation; two CRGBColour slots; one block |
| 0x00DE4CB0 NScript::CQ_RansomVictimScript::CVictimsFather::Main | DIFFER(6714v6778), 6 attempts | the same blocker on the first hit block's GetHealth temporary; movie / pause frame slots permuted |
| 0x00DF6010 NScript::CQ_TraderConflictEvilScript::Main | DIFFER(6247v6493), 5 attempts | whole-function colouring (retail ebx=this, ebp=0, frame 0x80) and a pads-expand / temporaries-called dtor split |

Three rows stopped below 10 attempts on the documented blocker and one (Evil) because its residue spans the whole function: all
are retry candidates.
