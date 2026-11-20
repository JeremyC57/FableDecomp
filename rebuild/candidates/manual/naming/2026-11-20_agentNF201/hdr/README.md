# NF201 header overlay (2026-11-20)

Base copies in ../orig/ are the repo working-tree headers at the start of this slice (byte-equal at copy time). Every addition is an
add-only `#ifdef` opt-in with a member-specific macro; with the macro undefined each header is token-identical to orig/ (the opt-in
blocks are the only diff). pin_asserts.py was run on each header after every edit: it pinned 0 lines (the only assert,
CEnvironmentSpellEffectInterpolator.h `FABLE_STATIC_ASSERT_AT(60, ...)`, already carries its line id).
Other agents' overlays checked (re-listed at hand-back): NF199 hdr/ holds only README.md; NF200 hdr/engine has CCreatureAction_FinishFireballSpell.h, CTCFireballSpell.h, NGraphicAppearance_CTCGraphicAppearanceReplaceableMesh.h; NF202 hdr/engine has CConsoleCommandBase.h, CScriptConversationManager.h, NCameraModes_CBase.h, NCameraModes_CStringAndCageBase.h. None overlays a header in this hand-back.

## engine/CEnvironmentSpellEffectInterpolator.h
- `FABLE_RETAIL_CENVIRONMENTSPELLEFFECTINTERPOLATOR_RADIALBLURFADETO_VECTOR`: declaration of the C3DVector overload
  `RadialBlurFadeTo(float, NScreenEffect::ScreenEffectPriority, const C3DVector&, float, const C3DVector&, float, float, float)` = retail
  0x006B8550 (Ego_r.pdb 0x006F44E0 + FableWin.pdb 0x0246CD10). Used by row 0x0079C750.
- Landed includers (14, `grep -rlE engine/CEnvironmentSpellEffectInterpolator.h rebuild/src/compiled`): none defines the macro, so the
  opt-in changes none of them.

## engine/CEnvironmentSpellEffectInterface.h
- `FABLE_RETAIL_CENVIRONMENTSPELLEFFECTINTERFACE_SETRADIALBLURCENTERWORLDPOS`: declaration of
  `SetRadialBlurCenterWorldPos(const C3DVector&, bool)` = retail 0x00885110 (Ego_r.pdb 0x008C4B60 + FableWin.pdb 0x02A56CC0). Used by row
  0x0079C750.
- Landed includers (2): none defines the macro; unchanged.

## engine/CIVCountedPointer.h
- `FABLE_RETAIL_CIVCOUNTEDPOINTER_COUNTED_COPY` (only when FABLE_RETAIL_CIVCOUNTEDPOINTER is not defined): the counted copy
  constructor alone (the same text as the FABLE_RETAIL_CIVCOUNTEDPOINTER block's), without that block's Release / Reset bodies. Row
  0x007464A0 needs the copy expanded (0x007464D4..0x007464E3) with the default Release (zero store after the null test's join,
  0x0074653B); FABLE_RETAIL_CIVCOUNTEDPOINTER gave DIFFER(177v177) on exactly that store.
- Direct landed includers: 33 (`grep -rlE engine/CIVCountedPointer.h rebuild/src/compiled`; many more reach it through other
  headers); 0 define the macro, so the opt-in changes none of them. pin_asserts: the header has no assert (pinned 0).

## engine/CEngineScreenEffectColourFilter.h
- `FABLE_RETAIL_CENGINESCREENEFFECTCOLOURFILTER_RENDER` (inside the MEMBERS class, public): declaration of the override
  `virtual void Render(const CEngineComponentRenderParams&)` = retail 0x00B5AF60 (Ego_r.pdb 0x00A007F0 + FableWin.pdb 0x02D26590); it
  overrides CEngineComponent::Render, so no slot is added. Used by row 0x00B5AF60.
- pin_asserts.py pinned 20 lines: the header's `FABLE_STATIC_ASSERT(...)` lines (unpinned at HEAD) were rewritten to
  `FABLE_STATIC_ASSERT_AT(<HEAD line>, ...)` so the typedef names do not move with the 6 inserted lines.
- Landed includers: 7; none defines the macro. With the macro undefined the header differs from orig/ only by the opt-in block and
  the pinned assert spelling (same typedef names as HEAD).
