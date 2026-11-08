# Landing 2026-11-08n: CWorldMap::GetMapNavigationAreaInit (chunk 864, after two coordinator passes)

0x0050A650: RELOCATION_MATCH + PASS (2,233 bytes). Read-only vland with a trimmed overlay of ten headers, then `--land`.

- **The lever:** with the callees' bodies visible in the TU, the three result vectors are no longer address-exposed. c2 then splits each
  `_M_start` web as retail does. A declared-only callee keeps one web from the ctor store to the dtor reload, and the reload coalesces
  with the loop iterator.
- **Held once, then fixed:** the first hand-back used stand-in bodies that did not do what retail does. They were rewritten from retail's
  algorithm, translated from the landed 0x005185F0 / 0x00518900: the cell walk over the rough area, the CTCMapwho search-ID stamp,
  GetTC<T> or the driver-type test, the filter, push_back. The caller's bytes are identical either way. A second pass replaced a
  `friend` with the PDB accessors `CThing::HasTCDriver` / `GetTCDriver` and gave `IsDriverType` its PDB parameter type (`long`).
  The template instances this TU emits are 748 / 748 / 778 bytes against retail's 597 / 597 / 683. They are not graded rows.
- **Headers landed** (additive opt-ins; hdrcheck2: default=same for all; no landed source defines any new macro):
  - CThingSearchTools.h: TCSINROUGHAREA / TCSINROUGHAREA_BODY; the ClipAndConvertWorldMapAreaToLocalArea guard also accepts the body opt-in.
  - CThing.h: TCDRIVER (HasTCDriver, GetTCDriver).
  - CTCDriverBase.h: ISDRIVERTYPE (slot 28).
  - CTCDRegionExit.h and the new CTCDNavigationSeed.h: STATGETDRIVERTYPE (25 / 21).
  - C2DBoxF COPY, C2DCoordI TOFLOAT, CNavigationPosition CTORS, CThingAICreature / CThingVillage STATGETTYPE.
- **Names:** `ghidra_out/labels_landing_2026-11-08n.tsv`. 0x00518340 is the CTCCreatureGenerator instance: it was mislabelled as a
  CTCPerceiveThing instance, and the evidence is this row's relocation. Queue `land0811m`: the landed 0x005185F0 / 0x00518900 still
  carry the local-model name `CThingSearchTools_Calls`, so re-author them on the shared template. vland's three RELOC_NAME warnings
  on the GetAllLeafThings instances are only a `>>` / `> >` spelling difference.
