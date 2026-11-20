# Landing hand3c3 b9: 0x0041AFA0 NUISystem::CSprite::Draw (Account B, 2026-11-20)

RELOCATION_MATCH, test PASS (links the row object), strict vland `--land`. Audited by the rolling auditor ($S/stage/audit_roll).

- **Source:** on engine/NUISystem_CSprite.h. The row had been parked as a "lane gap"; the missing piece was the inline GetFrame chain of the
  landed SetGraphicIndex 0x0041B550.
- **Audit fixes:**
  - `if (!PTexture.IsNull())` became `if (PTexture.Get())`. Ego_d's line 156 is 25 bytes; IsNull is 28 at /Od. The same retail bytes either way.
  - The new texture/box constructor body uses Ego_d's parameter names.
- **TU contents:** the row object's two constructor COMDATs equal retail 0x0041BF60 (167/167) and 0x0041BEB0 (171/171).
- **Labels:** `// Inferred:` on the existing FABLE_RETAIL_CENGINEPRIMITIVEHANDLE_FORCEINLINE_CLEAR (1404v1455 without it). One landed row
  already uses that opt-in.
- **Owner review item:** forced-inline opt-ins on helpers that are not asm. b9 depends on one that already exists. hand3c3 b12, still pending,
  would add another.
- **Headers** (add-only):
  - C2DBoxF.h +17 (FABLE_RETAIL_C2DBOXF_MOVE);
  - CEnginePrimitive2DSprite.h +26 (TEXTURE_BOX_CTOR, plus an `#ifndef FABLE_RETAIL_CENGINEPRIMITIVE2DSPRITE_LAYER_ENUM_FROM_CMANAGER` guard,
    because NUISystem_CManager.h defines the same EEngine2DLayer enum);
  - hdrcheck2 default=same; 1,390 includers preprocess raw-identical; no new header file.
- **Mutants:** 211 killed. The 3 survivors are in the texture constructor and cannot be observed from this row; the constructor is pinned by
  its COMDAT matching retail.
- **Expected warnings:** RELOC_CONFLICT + RELOC_NAME on 0x004194B5, the folded jmp thunk the CCountedPointer<CGraphicDataBank> destructor binds
  to. The landed sibling 0x0041B550 makes the same call.
