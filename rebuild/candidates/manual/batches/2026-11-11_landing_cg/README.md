# 2026-11-11 landing cg -- chunk H40j (long-rows lane, Opus 5.5): 0x00E40E80 NScript::CV_BordelloScript::CMagicman::Main

Source: `pending/2026-11-11_linux_handH40j/`. vland --land: 1 win (RELOCATION_MATCH, 15,104 bytes) and the test passes.
- The row grades against the repo headers with no overlay.
- Shared headers replace the row-local enums: engine/ETextGroupSelectionMethod.h, engine/EScriptEntityMoveType.h and engine/fable_script_text.h.
- vland warned RELOC_NAME at 0x0099E4C0: the catalog label is spelled `CCharString::operator_char_const*` for ??BCCharString@@QBEPBDXZ. It is the same symbol.
