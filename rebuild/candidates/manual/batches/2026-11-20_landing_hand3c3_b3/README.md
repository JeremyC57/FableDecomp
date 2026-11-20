# Landing hand3c3 b3: 0x00B075D0 NParticleEngine::CPSCSingleSprite::AddToBoundingSphere (Account B, 2026-11-20)

RELOCATION_MATCH, test PASS, strict vland `--land`. Audited by the rolling auditor ($S/stage/audit_roll; report kept in Account B's scratch).

- **Source:** written from Ego_d (psc_single_sprite.obj, debug length 1109). Locals and scopes match the text, and the 47 line records map 1:1.
  No fitted line.
- **Audit changes:**
  - The author's `__declspec(noinline)` same-TU ScaleParticleDimensions copy is gone. It is now a shared-header inline under a nested
    opt-in, and its COMDAT equals retail 0x00AF8F00.
  - A redundant extern was dropped.
- **Mutants:** 72 killed, 3 equivalent.
- **Headers** (hdrcheck2 default=same; the only DIFFs are the new opt-ins, which no landed row defines; ppcmp 7,791/7,791 includers raw-identical):
  - new: engine/CPSCSingleSprite.h, engine/CEnginePrimitiveSpriteTrail.h;
  - add-only opt-ins: C3DVector.h (TOC3DVECTOR), CPSCBase.h (GETUPDATEPARAM_NESTED), CParticleSystem.h.
- **Fragile:** the match depends on the count of locals and temporaries in every expanded helper (the c2 symbol-number order; see GOTCHAS).
  Recheck this row after any change to fable_containers.h (list), CMatrix3x4.h, CParticleSystem.h, CPSCBase.h, CVectorMap.h, CArray.h,
  CGraphicInfo.h, GFFloatToLongFloor.h or GFGetInterpolatedValue.h.
- **Newly flagged rows** (owner decision 2026-11-20: land now, re-port later): the two new headers newly flag 29 landed rows with local
  copies (20 CPSCSingleSprite, 9 CEnginePrimitiveSpriteTrail; `newly_flagged_rows.tsv`). The bytes are unchanged. These rows join B's
  re-port backlog.
- **NAME_FIXES.tsv** is for Account A.
