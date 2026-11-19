# Landing hand3c b15 (2026-11-19): CTCFlammable::FrameUpdate (0x00780D80), CPlayerGui::UpdateMoney (0x00439BB0)

Both rows pass strict vland --land with RELOCATION_MATCH + `<addr>_TEST PASS`.

**0x00780D80 CTCFlammable::FrameUpdate** (/d2inlT59 with an `// Inline-budget:` line; FableWin 0x01F51180 shape)
- New headers: engine/NFire_CFirePoint.h and engine/CFlammableDef.h.
- New opt-in FABLE_RETAIL_CTCFLAMMABLE_FRAMEUPDATE in CTCFlammable.h: typed PDef, members +0x1C..+0x31, IsOnFire inline, and the ExtinguishFire 0x00780C80 declaration.
- vland RELOC_NAME warnings:
  - 0x00415DD0 is the CDefString -> CCharString conversion, under the same name.
  - 0x0129BA3C is emitted as the FableWin name DELTA_NOTIONAL_ZERO, labelled g_NormaliseEpsilon (`NAME_FIXES.tsv`; already in the naming queue).

**0x00439BB0 CPlayerGui::UpdateMoney** (no flags; FableWin 0x01A814F0 shape with Ego_d local names)
- The parked local-model draft was rewritten onto shared headers.
- The uninitialised branch is three stores through PMoney in member order.
- `// Retail-asm:` line for the GFFloatToLongNear fistp at 0x00439C3D.
- Declaration-only opt-ins:
  - CPlayerGui.h FABLE_RETAIL_CPLAYERGUI_UPDATEMONEY;
  - NPlayerGui_CStatBar.h FABLE_RETAIL_NPLAYERGUI_CSTATBAR_CHANGETEXTS.

**Headers:** pure additions, and pin_asserts found no moved assert.

**Header-user recheck** (168 rows): 167 good. The other is 0x007E2A60, which is known and on hold (ROADMAP (c)).
