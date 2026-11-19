# Landing hand4e b4 (2026-11-19): CReplaceSerialise<..>::ConvertPreviousUnknownVersionUp (0x005148D0), CTCHeroExperience::GetTrainableStatPossibleLevelUpAmount (0x006D6FA0)

**Result:** strict vland --land gives RELOCATION_MATCH / MATCH + PASS; no flags. hand4e closed all 10 of its rows.

**0x005148D0** is CReplaceSerialise<COldFileFormatMapHeader2,COldFileFormatMapHeader1>::ConvertPreviousUnknownVersionUp (Ego_r + FableWin).
- It uses the template body of the landed twin 0x00826C90.
- New opt-in FABLE_RETAIL_CFILEFORMAT_OLDMAPHEADER12, added inside the class in engine/CFileFormatBrush.h. Every existing assert was pinned
  `FABLE_STATIC_ASSERT_AT(<old line>, ...)`; checked at landing: 29 of 29 assert names kept, plus two new ones inside the opt-in.
- NAME_FIXES gives the PDB spellings for callees 0x00512C70 / 0x0050D1F0 (the vland RELOC_NAME warnings).

**0x006D6FA0** adds new opt-in FABLE_RETAIL_CTCHEROEXPERIENCE_GETSTATEXPERIENCE_BODY. The body also MATCHes retail's own copy at 0x006D6510.

**Header-user recheck:** 99 / 99 good, run together with hand3c b20.
