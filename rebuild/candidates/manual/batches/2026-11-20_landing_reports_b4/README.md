# 2026-11-20 landing reports_b4 (Account B's re-port, landed by A after B's wind-down): 0x00BACE10 onto CEngineSubPrimitiveSprite.h

Source: `pending/2026-11-20_winddown/reports_b4/` (B's README kept as `B_README.md`). Both bases equal HEAD (checked by reversing B's
diffs).

## Landed: 0x00BACE10 CEngineSubPrimitiveSprite::CEngineSubPrimitiveSprite
The local CBaseClassNonCopyable / CEngineInternalPrimitive* / CEngineSubPrimitive* models become the shared header with
FABLE_RETAIL_PASS22_PRIMITIVE_IMPLICIT_DESTRUCTORS and B's new add-only opt-in FABLE_RETAIL_CENGINESUBPRIMITIVESPRITE_CTOR_OUTOFLINE (the
constructor as a declaration; FableWin.pdb ??0CEngineSubPrimitiveSprite@@QAE@PAVCEngineInternalPrimitiveSprite@@@Z). RELOCATION_MATCH
PASS as before; defined-symbol set unchanged; no binding changed. The header's includer 0x00BADE40 regrades RELOCATION_MATCH PASS.

## Held: 0x00BB1740 (the `??_G` rewrite)
B's scaffolding emits the vtable and `??_G` through the header's inline constructor, so the object also emits that constructor pick-any,
which 0x00BACE10 defines strongly as its leaf: a new LNK2005. Left at HEAD (stand-in leaf, PassPattern VDD_OK); queued with the fix:
put 0x00BACE10 on the nf86 / nf88 pick-any scaffolding, which also cures the pre-existing clash below, then land B's 0x00BB1740 text.

## Checks
- Context link (0x00BACE10, 0x00BADE40, the constructor's base owner 0x00B98A50): one LNK2005, 0x00BACE10's strong constructor against
  0x00BADE40's header copy, reproduced with HEAD's 0x00BACE10 object (`logs/link_control_head.log`): pre-existing, queued.
- Table unchanged; gate 0; bindings_multi_check 1.
