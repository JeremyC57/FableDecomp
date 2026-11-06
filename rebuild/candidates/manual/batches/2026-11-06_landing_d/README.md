# Landing 2026-11-06d: chunk 803 (4 of 6 rows landed, 2 parked)

One expert agent authored the rows (sharing the lane with the chunk 802, then 804, agents). The six rows came from the gap scan with
no name. The coordinator placed them by their landed neighbours and single .rdata reference. The agent proved every name from the
vtable slot-1 address plus the Ego_r.pdb / FableWin.pdb symbol. All four landed rows are plain C++ and grade `genuine`.

| Address | Name | Grade |
|---|---|---|
| 0x00F1B9C0 | NScript::CQ_ArenaScript::CArenaCellExitGuard::Main | RELOCATION_MATCH PASS (vtable 0x012F9D2C) |
| 0x00E158F0 | NScript::CQ_WhiteBalverineKnotholeGladeScript::CWB_WhiteBalverine::Main | RELOCATION_MATCH PASS (vtable 0x012E0A50) |
| 0x00DB6C60 | NScript::CQ_NewOakValeIntroScript::CNOVI_BarrelThug::Main | RELOCATION_MATCH PASS (vtable 0x012D9234) |
| 0x00DB09E0 | NScript::CQ_NewOakValeIntroScript::CNOVI_AffairMan::Main | RELOCATION_MATCH PASS (vtable 0x012D89EC) |

- **Headers** (`hdr_README.md`):
  - entity opt-ins in CQ_NewOakValeIntroScript.h (AffairMan, BarrelThug), CQ_WhiteBalverineKnotholeGladeScript.h and
    CV_SwordInTheStoneScript.h (CSwordCrowd; `WatchForDeathDuringSwordPull(CScriptThing)` in the opt-in branch only);
  - new engine/CQ_PrisonWardenGameScript.h;
  - fable_string.h gains FABLE_RETAIL_CCHARSTRING_SPRINTF (`CCharString::Sprintf` 0x0099F1F0).
  - hdrcheck2 (`logs/hdrcheck2.log`) reads `default=same` for every header and no existing macro reads DIFF. The two landed
    includers of the changed headers (0x00ED5830, 0x00E13F10) still grade RELOCATION_MATCH
    (`pending/2026-11-06_linux_hand803/SIBLINGS.tsv`).
- **vland warnings:** 1 RELOC_ALIAS, `_rand` binds the MSVCR71 import thunk 0x00BFEB16 (expected).
- **Names.**
  - Labels: `ghidra_out/labels_landing_2026-11-06d.tsv` (the six Mains).
  - 2 queue lines (tag `land0611d`):
    - AddBadDeed 0x00DAEA70: the landed 0x00DBCD00 binds a `long`, un-namespaced stand-in; the PDB spelling takes the EBadDeeds enum.
    - WatchForDeathDuringSwordPull 0x00ED7320: takes a CScriptThing, but the header's default branch still declares `()`.
- **Landing:** `vland --land` (`logs/land.log`); labels applied; the manifest sync dry run prints 0; bindings_multi_check prints only
  the known NullCoord case.

## Parked (`pending/2026-11-06_linux_hand803/`)
| Row | Grade | Residue |
|---|---|---|
| 0x00ED5F50 NScript::CV_SwordInTheStoneScript::CSwordCrowd::Main | DIFFER(4971v5024), 10 attempts | frame packing of the temporaries; retail expands ~CScriptThing at one exit only, which neither the implicit-copy nor a row-local dtor spelling reproduces |
| 0x00DDF090 NScript::CQ_PrisonWardenGameScript::CPrisonWarden::Main | DIFFER(4455v4599), 10 attempts | VC7.1's inline budget for the first two cast-map operator= stores and one seh_me dtor, plus frame layout |
