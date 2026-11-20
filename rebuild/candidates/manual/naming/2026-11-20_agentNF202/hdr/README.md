# NF202 overlay headers

## engine/CScriptConversationManager.h (copy of rebuild/include/engine/CScriptConversationManager.h; base in ../orig/engine/)
- Adds opt-in `FABLE_RETAIL_CSCRIPTCONVERSATIONMANAGER_ADDLINETOCONVERSATION` (q12195): forward declarations of CThing / CCharString and the
  public declaration `void AddLineToConversation(long, const CThing&, const CThing*, const CCharString&, bool)` (0x006E5950; Ego_r.pdb
  0x0070EE80 + FableWin.pdb 0x02409E00 ?AddLineToConversation@CScriptConversationManager@@QAEXJABVCThing@@PBV2@ABVCCharString@@_N@Z).
  Declaration only, no layout change. Used by rows 0x006E5950 (self), 0x00890750 (caller) and the 0x00890750 test.
- Without the macro the header is token-identical to the repo copy except the pinned assert: pin_asserts.py pinned 1
  (`FABLE_STATIC_ASSERT(sizeof(CScriptConversationManager) == 0x14)` -> `FABLE_STATIC_ASSERT_AT(19, ...)`, its repo line).
- Landed includers (grep rebuild/src/compiled): 00/4a/CWorld_Init_004a6e30.cpp only; it does not define the macro, so nothing changes for it.

## engine/CConsoleCommandBase.h (copy of rebuild/include/engine/CConsoleCommandBase.h; base in ../orig/engine/)
- Adds opt-in FABLE_RETAIL_CCONSOLECOMMANDBASE_GETFORMATTEDNAME (q12225): a partial retail mode (as FABLE_RETAIL_NTHINGMAP_LAYERINFO_ONLY):
  class CConsoleInputBase { virtual ~; protected GetName() (FableWin IBE, inline); CCharString Name +0x04 } and
  class CConsoleCommandBase : CConsoleInputBase { protected: virtual CCharString GetFormattedName() const; ParamTypes[0xc] +0x08 }
  (Ego_r.pdb 0x00B3D6A0 + FableWin.pdb 0x030A9E40 ?GetFormattedName@CConsoleCommandBase@@MBE?AVCCharString@@XZ). It includes fable_string.h
  with FABLE_RETAIL_CCHARSTRING. The flattened default struct sits in the #else branch, unchanged.
- Without the macro the header is token-identical to the repo copy except the 4 asserts pin_asserts.py pinned (FABLE_STATIC_ASSERT ->
  FABLE_STATIC_ASSERT_AT(21..24, ...), their repo lines).
- Landed includers (grep rebuild/src/compiled): NGlobalConsole_Initialise_00419d90.cpp CConsoleCommandBase_scalardeletingdestructor_00414fc0.cpp CConsoleClasslessCommand_CallCommandFunction_00415040.cpp CConsoleCommandBase_CConsoleCommandBase_00414f30.cpp CConsole_Initialise_009ed190.cpp CConsoleCommandBase_GetFormattedName_009ea3a0.cpp  -- none defines the macro, so nothing changes for them.

## engine/NCameraModes_CStringAndCageBase.h (copy of the repo header; base in ../orig/engine/)
- Adds opt-in FABLE_RETAIL_NCAMERAMODES_CSTRINGANDCAGEBASE_GETZCOORD (q12240): private `float GetZCoord() const;` (0x00871D30; Ego_r.pdb
  0x0085F880 + FableWin.pdb 0x029D03C0 ?GetZCoord@CStringAndCageBase@NCameraModes@@ABEMXZ), inserted after UpdateXYVars in the private
  block. Declaration only, no layout change. pin_asserts.py pinned 0 (the header has no asserts).
- Without the macro the header is token-identical to the repo copy. Landed includers: 13 rows (grep rebuild/src/compiled); none defines
  the new macro, so nothing changes for them.

## engine/NCameraModes_CBase.h (copy of the repo header; base in ../orig/engine/)
- Adds opt-in FABLE_RETAIL_NCAMERAMODES_CBASE_GETOBSERVEDTHING (q12240) to the non-CYLINDER branch: a global-scope forward declaration of
  CThingGameObject and, first in CBase's public block, `const CThingGameObject* GetObservedThing() const;` (0x0084F1B0; Ego_r.pdb 0x00846960
  + FableWin.pdb 0x02883590 ?GetObservedThing@CBase@NCameraModes@@QBEPBVCThingGameObject@@XZ; the CYLINDER branch already declares it).
  A non-virtual member: no layout or vtable change. pin_asserts.py pinned 0.
- Without the macro the header is token-identical to the repo copy. Landed includers (directly or through NCameraModes_* headers): 57
  direct; none defines the new macro, so nothing changes for them.
