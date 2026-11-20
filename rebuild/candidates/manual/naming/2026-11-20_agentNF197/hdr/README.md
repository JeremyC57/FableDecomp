# NF197 header overlay

## engine/C3DClothPrimitive.h (copy of rebuild/include/engine/C3DClothPrimitive.h at 2026-11-20; base in orig/engine/)
- Adds `FABLE_RETAIL_C3DCLOTHPRIMITIVE_VIRTUAL_DTOR`: `virtual ~C3DClothPrimitive();` in place of the `void* _vftable;` member (same
  layout; the CParticleSimulationRenderData VIRTUAL_DTOR pattern). Used by rows 0x00A93A60 / 0x00A94DF0. The default path (macro
  undefined) is token-identical to the repo header. pin_asserts.py: pinned 0 (the size assert is FABLE_STATIC_ASSERT_AT(20, ...)).
- Landed includers (8, `grep -rl engine/C3DClothPrimitive.h rebuild/src/compiled`) do not define the macro (argued from the text; not regraded with the overlay). Not combinable with FABLE_RETAIL_C3DCLOTHPRIMITIVE_CTOR (it declares the destructor non-virtual): landed 0x00A91640 uses _CTOR, and its HEAD pick-any copy of ?DeleteData@?$CCountedPointer@VC3DClothPrimitive@@@@SIXPAX@Z is 23 bytes (non-virtual dtor + delete) against the owner 0x00A93A60 11 bytes (cmpbody: differs) -- HEAD debt; converging it needs a header decision (a virtual dtor under _CTOR too).
## engine/CFrontendGameComponent.h (copy of rebuild/include/engine/CFrontendGameComponent.h at 2026-11-20; base in orig/engine/)
- Adds `FABLE_RETAIL_CFRONTENDGAMECOMPONENT_GETSAVENAMES`: the declaration `static void __fastcall GetSaveNames(std::vector<CWideString>& names);`
  (0x00495830), placed with the existing GETPROFILENAMES opt-in, before the CLEARSTRING block (still public there). Used by row 0x00495830.
  Default path token-identical. No static asserts in the header (pin_asserts not needed).
- Landed includers (4) do not define the macro: unchanged (argued from the text; not regraded with the overlay).
## engine/CEnvironmentSpellEffectInterpolator.h and engine/NScreenEffect_CScreenEffectPriorityStack.h (copies at 2026-11-20; bases in orig/engine/)
- `FABLE_RETAIL_CENVIRONMENTSPELLEFFECTINTERPOLATOR_RADIALBLURFADEOUT`: declares `void RadialBlurFadeOut(float time, void* handle);` (0x006B82E0)
  just before the `private:` members. `FABLE_RETAIL_CSCREENEFFECTPRIORITYSTACK_DELETERADIALBLURENTRY`: declares
  `void DeleteRadialBlurEntry(void* handle, float fadeTime);` (0x00881820) after the DELETESCREENFILTERENTRY block. Rows 0x006B82E0,
  0x00890150 (and the 0x006B82E0 test). Default paths token-identical; pin_asserts.py pinned 0 in both (the interpolator's assert is
  FABLE_STATIC_ASSERT_AT(60, ...)). Landed includers of the two headers (21) do not define the macros: unchanged (argued from the text; not regraded with the overlay).
