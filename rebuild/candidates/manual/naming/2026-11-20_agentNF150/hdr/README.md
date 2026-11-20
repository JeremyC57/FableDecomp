# NF150 header overlay

## engine/CEnginePrimitiveRenderer.h
- Base: the repo working tree copy (orig/engine/CEnginePrimitiveRenderer.h, byte-equal to rebuild/include at hand-back).
- Added (add-only, declaration-only `#ifdef` blocks, identical text in both class bodies: the FABLE_RETAIL_CENGINEPRIMITIVERENDERER
  retail branch and the generated default branch, each right after the existing ADDSTATICMESH opt-in):
  - FABLE_RETAIL_CENGINEPRIMITIVERENDERER_ADDPRIMITIVE: AddPrimitive(const CEnginePrimitive&) 0x00B32E90
  - FABLE_RETAIL_CENGINEPRIMITIVERENDERER_SETPERSISTENTPRIMITIVE: SetPersistentPrimitive(CEnginePrimitiveHandle&, const CEnginePrimitive&, unsigned long, bool) 0x00B324A0
  - FABLE_RETAIL_CENGINEPRIMITIVERENDERER_SETPRIMITIVEANIMATION: SetPrimitiveAnimation 0x00B31E90
  - FABLE_RETAIL_CENGINEPRIMITIVERENDERER_SETPRIMITIVEPOSITION: SetPrimitivePosition 0x00B31EC0
  - FABLE_RETAIL_CENGINEPRIMITIVERENDERER_ADDCHILDPRIMITIVE: AddChildPrimitive 0x00B31EF0
  - FABLE_RETAIL_CENGINEPRIMITIVERENDERER_SETCHILDPRIMITIVE: both SetChildPrimitive overloads 0x00B31F20 (5 args) / 0x00B31F70 (4 args)
  - FABLE_RETAIL_CENGINEPRIMITIVERENDERER_GETCHILDPRIMITIVEHANDLE: GetChildPrimitiveHandle 0x00B31F50
  - FABLE_RETAIL_CENGINEPRIMITIVERENDERER_CLEARCHILDPRIMITIVE: ClearChildPrimitive 0x00B31FA0
- Parameter types are elaborated (`class CEnginePrimitiveHandle&` ...), as the existing ADDSTATICMESH opt-in does: the header includes no
  new header (the users model CEnginePrimitiveHandle locally). Spellings: Ego_r.pdb + FableWin.pdb name sets (identical in both).
- Default path token-identical: removing the two inserted blocks gives the repo header byte for byte (checked with a script).
- pin_asserts.py: `engine/CEnginePrimitiveRenderer.h pinned 0` (every assert already FABLE_STATIC_ASSERT_AT; none moved name).
- Header users: 68 landed rows include engine/CEnginePrimitiveRenderer.h (`grep -rlE 'engine/CEnginePrimitiveRenderer\.h'
  rebuild/src/compiled`); none defines any of the eight new macros (grep of rebuild/: no hit outside this overlay), so the opt-ins change
  none of them. Sibling overlays at hand-back: NF148 hdr/engine/CCombatActionBase.h, CCreatureAction_ThrowShotObject.h; NF149
  hdr/engine/CVertexBufferWin32.h: neither overlays this header. orig/ re-compared byte-equal to rebuild/include at hand-back.
- Users of the opt-ins: rows/00b32e90 (ADDPRIMITIVE + SETPERSISTENTPRIMITIVE), 00b31e90, 00b31ec0, 00b31ef0, 00b31f20, 00b31f50,
  00b31f70, 00b31fa0.

## Not overlaid
- engine/CRenderManagerCore.h, engine/CNavigatorManager.h, engine/CNavigatorAStar.h, engine/CRenderStateManager.h: the rows use existing
  opt-ins only.
