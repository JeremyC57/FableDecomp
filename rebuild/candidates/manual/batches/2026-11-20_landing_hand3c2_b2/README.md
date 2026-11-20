# Landing hand3c2 b2: 0x0064DD9A NPlayerGui::CDrawMiniMap::UpdateRegionExitNames (Account B, 2026-11-20)

RELOCATION_MATCH, test PASS, strict vland `--land`. Audited in batches/2026-11-19_landing_hand3c2_b1_b7/audit_REPORT.md (section b2: FAIL as
delivered, fix built). The audit fix makes the row call 0x007791F0 as `CTCDRegionExit::GetRegionName()`; Account A respelled that row in
batches/2026-11-19_landing_b791_getregionname.
- **Headers:** additive opt-ins, rebased onto today's files.
  - The audit's MINIMAPLABEL opt-in no longer declares GetRegionName, because A now declares it in the default path.
  - New opt-ins: CTCDREGIONEXIT_MINIMAPLABEL (IsActive, GetDisplayName), NPLAYERGUI_CDRAWMINIMAP_REGIONEXITS (RegionExits typed by its
    PDB type), CTCACTIONUSESCRIPTEDHOOK_STATCLASSTYPENAME, CTHINGFILTER_HASTCOFLEAFTYPE_BODY, and the CRegion / CThing opt-ins
    (hdr_diffs/).
  - hdrcheck2: default=same; only the new macros differ.
- **Mutants:** 37/37 (audit plus author).
- **Same-name opt-in:** the queued re-port set w26/reports/leaftype (0x00518DC0) adds the same macro,
  FABLE_RETAIL_CTCACTIONUSESCRIPTEDHOOK_STATCLASSTYPENAME, so that set gets rebased onto this landing.
- **For Account A:** NAME_FIXES.tsv, and the three RELOC_NAME stand-in labels (0x00615520 / 0x00615540 `_List_base<const CThing*>`
  ctor / dtor, 0x00617D30 PeekTCDriver<const CTCDRegionExit>), already queued as q11589+.
