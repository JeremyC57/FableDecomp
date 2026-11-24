# 0x00402510 GFMain -- matched; open doubts for the coordinator
- The overlay adds 6 NEW headers taken from hand851 (CSystemManagerInit, CDisplayManagerInit, CInputManagerInit, CSystemRegistry,
  CBankFileManager, NUISystem_CIME). CSystemManagerInit.h models the native sound-initialisation block at +0x38 as `unsigned long
  Body_38[8]` ("absent from all three donor versions"), and the row writes it through reinterpret_cast<float*> (0x40 = 32.0f, 5.0f, 1.0f,
  0x100, 0x10000): bytes match, but the members are unnamed. Ego_r has StreamingFontBankHandle +48 .. DisplayManagerInit +60 only.
- vland warns RELOC_NAME on four manifest LABELS that are wrong (NAME_FIXES.tsv); data-global spelling conflicts with older landed
  rows (g_b* placeholders) are listed in NAME_FIXES.tsv too: vland does not check data globals.
- Payload flags /Gr /GS (Codex pass-12 flags; the TU of GFMain is fastcall by default and /GS for the security cookie at 0x0139C8A8).
