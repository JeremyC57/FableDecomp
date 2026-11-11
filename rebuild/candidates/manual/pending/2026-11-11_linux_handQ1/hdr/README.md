# Header overlay of chunk Q1 (grade with FABLE_HDR_OVERLAY=$A/hdr)

New headers (hand-written from the Ego_r.pdb layouts + retail offsets; no generator output exists for them):
- engine/CTCGatherPointBase.h -- CTCGatherPointBase : CTCBase (vtable 0x012421E4, sizeof 0x1C); row 0x004E0ADC.
- engine/CRegionScriptDef.h -- CRegionScriptDef : CParentDefClassBase (sizeof 0x4C); row 0x00F29F60.
- engine/NUISystem_CUILocaleGraphicsDef.h -- NUISystem::CUILocaleGraphicsDef : CParentDefClassBase (sizeof 0x58); row 0x00462FAC.

Changed headers (default path byte-identical; each change behind its own opt-in):
- engine/CPlayerGui.h -- FABLE_RETAIL_CPLAYERGUI_FRIEND_STAT_DRAWERS (needs FABLE_RETAIL_CPLAYERGUI_MEMBERS): friend declarations for
  NPlayerGui::CDrawMoney / CDrawRenown / CDrawMorality, which read the private PMoney / PRenown / PMorality in place. Row 0x00644263.
- engine/CASuspendableProcess.h -- FABLE_RETAIL_CASUSPENDABLEPROCESS_EXCEPTIONEXIT (inside FABLE_RETAIL_SUSPENDABLE_PROCESS): the nested
  empty class CASuspendableProcess::CExceptionExit (RTTI 0x01397754, Ego_r.pdb 1 byte). Row 0x00A446A0.
