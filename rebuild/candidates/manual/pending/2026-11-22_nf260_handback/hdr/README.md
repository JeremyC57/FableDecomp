# NF260 header overlay

## engine/CEngineLandscapeRenderer.h (base: orig/engine/CEngineLandscapeRenderer.h = the repo copy of 2026-11-22, byte-equal at start)
- New add-only opt-in `FABLE_RETAIL_CENGINELANDSCAPERENDERER_GPLANDSCAPERENDERER`: one block after `#endif // FABLE_RETAIL_CENGINELANDSCAPERENDERER`
  (outside both layout branches), before the include guard's `#endif`. It declares only
  `namespace NEngineInternal { extern CEngineLandscapeRenderer* GPLandscapeRenderer; }` (0x01436EA8,
  `?GPLandscapeRenderer@NEngineInternal@@3PAVCEngineLandscapeRenderer@@A`: Ego_r.pdb S_GDATA32 3:0007be04 / FableWin.pdb 4:0008808c, both at
  GPResourceManager + 0x10 after GPLightingManager / GPCamera / GPEngine, which the table binds at 0x01436E98 / 9C / A0 / A4).
- No existing line is edited: the diff against orig/ is 8 added lines at the end. Default path (macro undefined) is token-identical.
- pin_asserts.py: 0 pinned (the block sits after every FABLE_STATIC_ASSERT_AT).
- equiv_check (work/equiv.log / equiv.tsv; a hard-link copy of rebuild/include with this header v the repo rebuild/include): 1,462 rows
  checked (every landed row whose source or test names CEngineLandscapeRenderer.h or a header that does, plus every row that includes
  `../src/compiled/`), 0 differ.
- Landed includers: 78 sources include the header directly; none defines the new macro (grep of rebuild/src, rebuild/include,
  rebuild/tests: 0 hits), so none changes. No running agent's overlay has this header (agentNF46/hdr is a landed 2026-11 overlay).
- Users in this hand-back: the 42 header-route rows of units 1 and 2 (STATUS.tsv notes "header route"); each defines the macro before its
  first #include and includes the header itself (NF214 lesson: an overlaid header must be included by the row).
