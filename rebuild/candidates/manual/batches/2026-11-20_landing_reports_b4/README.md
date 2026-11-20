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

## Follow-up (Account A, 2026-11-20; advisor decision, owner delegated): 0x00BB1740 landed with the 0x00BACE10 owner fix
- 0x00BACE10 moved to the nf86 / nf88 pick-any scaffolding: `_CTOR_OUTOFLINE` dropped, the header's inline constructor referenced from an
  anonymous-namespace helper under `#pragma inline_depth(0)` (`// Fidelity: scaffolding, not retail`). Form 1 RELOCATION_MATCH PASS
  (`logs/followup_00bace10_form1.log`); its constructor COMDAT is LINK_ONCE_DISCARD (pick-any).
- B's 0x00BB1740 text and test applied (`winddown_source/src/00bb1740.cpp`); PassPattern `00bb1740_TEST PASS`. Strict recheck of both:
  RELOCATION_MATCH PASS (`logs/followup_recheck.log`).
- Link of 0x00BACE10 + 0x00BB1740 + every landed includer of engine/CEngineSubPrimitiveSprite.h (0x00BADE40) + the owners 0x0099A2F0 /
  0x0099A300 / 0x00BEB550: no LNK2005 (`logs/followup_link.log`); all three constructor copies pick-any. This also cures the pre-existing
  0x00BACE10 / 0x00BADE40 clash recorded above.
- Bindings: 0x00BEB550 lost the FreeMesh stand-in spelling (6 -> 5); nothing grew; bindings_multi_check 1.
