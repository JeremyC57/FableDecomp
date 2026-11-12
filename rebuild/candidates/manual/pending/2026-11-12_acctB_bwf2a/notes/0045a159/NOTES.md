## 0x0045A159 CInventoryDef::operator= -- RELOCATION_MATCH (grade 1 of 2; strict regrade RELOCATION_MATCH; vt PASS)
- What made it match: the scout's facts as given -- explicit user operator= (base op= 0x00431ED0, then the 391 Ego_r members in
  declaration order, members_body.txt pasted as-is), `#pragma optimize("s", on)`, no flags. The ClothingDoll/DollCentreOffset
  scheduling swap came out of the compiler by itself.
- Headers: overlay hdr/engine/CInventoryDef.h opt-in FABLE_RETAIL_CINVENTORYDEF_FULL (remaining members +0xB4..+0xA30, empty
  4-byte `enum EInventoryIdentifier {}` -- PDB enum name, enumerators not recovered -- and the operator= declaration); row defines
  FABLE_RETAIL_DEF_CLASS_BASE, FABLE_RETAIL_CDEFSTRINGTABLE, FABLE_RETAIL_CCHARSTRING, FABLE_RETAIL_C2DVECTOR,
  FABLE_RETAIL_STLP_VECTOR_ASSIGN.
- Name fix: manifest label CParentDefClassBase::operator= for 0x0045A159 is propagated/wrong; real = ??4CInventoryDef@@QAEAAV0@ABV0@@Z
  (only caller 0x0045A154 = ?Copy@CInventoryDef@@ jmp; Ego_r.pdb names the symbol). Landed Copy row 0x0045A154 forwards to
  `ForwardTarget_0045a159` -- respell to CInventoryDef::operator=.
