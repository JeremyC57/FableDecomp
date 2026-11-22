# Landing nf224 (Account A, 2026-11-22): 0x00415B80 CGraphicDataBankInit::CGraphicDataBankInit (1 row)

Agent NF224. Hand-back: `pending/2026-11-22_nf224_handback/`. Entry q12561 (briefed as a catalog edit; the row was already landed under a
stand-in name, so NF224 ported it).
- 0x00415B80 lands MATCH (48 B), with its test rewritten on the shared header (the old one was a host model of the class).
  - The leaf moves from the made-up `??0SNavigationLayerDefaults_00415b80@@QAE@XZ` (no PDB has it) to `??0CGraphicDataBankInit@@QAE@XZ`
    (Ego_r, FableWin, Ego_d).
  - The row uses engine/CGraphicDataBankInit.h on its default path. An anonymous-namespace placement-new scaffold makes the TU emit the
    header's inline ctor, as landed 0x00BB1740 / 0x00485420 / 0x00BACE10 do.
  - All 4 callers already bind the new spelling: 2 spellings -> 1, no respell.
- Files, build Module and catalog name move to `CGraphicDataBankInit_CGraphicDataBankInit_00415b80*`. Labels (`logs/LABELS.tsv`):
  labels_manifest_sync_2026-11-03.tsv line 222, rebuild/manifest/functions.tsv line 455 and ghidra_out/engine_api.tsv line 390 are
  respelled. manifest-gaps.tsv line 129 is a derived dump, left for its regeneration.
- Recheck (`logs/recheck.log`): the row and its 4 callers, 1 MATCH + 4 RELOCATION_MATCH, all PASS, no stand-in warning left.
  - Link (NF224): 6 objects, 0 duplicates.
  - Pick-any: placement new equals 0x00405C10; the existing ctor copies in 0x00B2CB40 / 0x009F83D0 equal 0x00415B80.
- Not landed (queued, tag land1122nf224):
  - 0x00494840 / 0x0054ED90: every existing decl-only opt-in DIFFERs. Size-body opt-ins are authorised for the next agent.
  - 0x0042DB20: the 3-row inline-Clear unit may go ahead only if a PDB shows Clear inlined.
  - The GPUniqueInstance spelling.
