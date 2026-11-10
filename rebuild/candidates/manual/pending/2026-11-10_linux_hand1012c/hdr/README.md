# Header overlay, chunk 1012c

Every grade and probe of this chunk ran with `FABLE_HDR_OVERLAY=$A/hdr` exported (g.sh, gall.sh and gr.sh export it).
Every change sits behind its own new `FABLE_RETAIL_*` opt-in; with the opt-in off each header is byte-identical to rebuild/include.

- `engine/CTCPerceiveThing.h` -- new opt-in `FABLE_RETAIL_CTCPERCEIVETHING_SENSES` (inside the existing `..._AWARENESS` layout).
  Replaces the two inline AWARENESS helpers with the PDB member set the rows call:
  `StatGetInterfaceType()` (TCI_PERCEIVE_THING), `bool IsAwareOfThing() const` (0x00643B3B, out of line, PDB `QBE_NXZ`),
  inline `IsThingLineOfSight / IsThingExtendedLineOfSight / IsThingSmelt / IsThingHeard` (bytes +0x2d / +0x2e / +0x31 / +0x32),
  and the out-of-line float getters `GetSoundRadius` 0x00744440 (+0x3c), `GetSmellRadius` 0x00744450 (+0x48), `GetSightRadius`
  0x007448F0 (+0x40 x visibility multiplier), `GetExtendedSightRadius` 0x00744950 (+0x44), `GetFOV` 0x007445D0 (PDef+0x28 / 360).
  Names: Ego_r.pdb and FableWin.pdb (`?Get...@CTCPerceiveThing@@QBEMXZ`). Used by differ/0089ce70.cpp and differ/0089c910.cpp.
- `engine/CGameScriptInterface.h` -- new opt-in `FABLE_RETAIL_CGAMESCRIPTINTERFACE_SENSED` declares the private
  `bool CanThingBe_Sensed_ByOtherThing(const CScriptThing&, const CScriptThing&, long) const` (0x0089C910,
  PDB `?CanThingBe_Sensed_ByOtherThing@CGameScriptInterface@@ABE_NABVCScriptThing@@0J@Z`). Used by differ/0089c910.cpp.
- `engine/CTCFollowed.h` -- NEW header. Under `FABLE_RETAIL_CTCFOLLOWED`: `class CTCFollowed : public CTCBase` with
  `StatGetInterfaceType()` (TCI_FOLLOWED 0xBC) and `void GetFollowers(std::vector<CThing*>&)` 0x006D5850 (PDB
  `?GetFollowers@CTCFollowed@@QAEXAAV?$vector@PAVCThing@@...@Z`); without the opt-in it is only a forward declaration.
  Used by differ/008a7a70.cpp. The landing needs it promoted to rebuild/include/engine/CTCFollowed.h.
- `engine/CGameEventDataPackage.h` -- carried over from chunk 1012b (`FABLE_RETAIL_CGAMEEVENTDATAPACKAGE_TYPEDREADERS`, six inline
  typed readers `GetULONG/GetUBYTE/GetFloat/GetSLONG/GetUHUGE/GetUWORD`, each `T value; GetData(sizeof(value), &value); return value;`).
  Only the 1012b draft of 0x0062BD80 uses it; nothing in this chunk changed it.
