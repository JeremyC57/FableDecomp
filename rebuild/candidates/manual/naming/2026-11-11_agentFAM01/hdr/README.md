# agentFAM01 header overlay (full copies of the CURRENT repo headers; `#ifdef` additions only, default path byte-identical)

Grade with `FABLE_HDR_OVERLAY=$S/w22/agentFAM01/hdr`.

- `engine/CCombatActionBase.h` -- new opt-in `FABLE_RETAIL_CCOMBATACTIONBASE_STRIKEEVENT` in the default (`#else`) class branch:
  `public: class CStrikeEvent;` (the PDB nested class, already declared the same way in the other two branches). Declaration only,
  no layout change. Used by row 0x0062D9A0 ~CCountedPointer<CCombatActionBase::CStrikeEvent>.
- `engine/NPlayerGui_CDrawQuestInfo.h` -- new opt-in `FABLE_RETAIL_CDRAWQUESTINFO_ELEMENTS`: the nested element classes
  CQuestInfoElementTimer / CQuestInfoElementCounterList / CQuestInfoElementTick (FableWin.pdb CCountedPointer instances of all three;
  members from Ego_r.pdb; single inheritance from CQuestInfoElement at +0, as the retail converting ctors show). Used by rows
  0x0043E1C0, 0x00654B0A..0x00654C8F (with the existing FABLE_RETAIL_CDRAWQUESTINFO_COUNTER for CQuestInfoElementCounter).

No other header changed. engine/CCountedPointer.h is used as-is (opt-in FABLE_RETAIL_CCOUNTEDPOINTER_RELEASEDATA).
