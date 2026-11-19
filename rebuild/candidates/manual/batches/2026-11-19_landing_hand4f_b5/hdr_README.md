# hand4f header overlay
b1-b4 rows use existing repo opt-ins only; the b5 row 0x006CA8C0 needs the two new opt-ins below.
- engine/CEngineTextureMorph.h  FABLE_RETAIL_CENGINETEXTUREMORPH_CTORS (hand4f Account B, row 0x006CA8C0): CEngineTextureMorph's user constructors from
  Ego_d.pdb field list 0xB9D4: (long original_texture, long morph_id) with init-list body (FableWin.pdb ??0CEngineTextureMorph@@QAE@JJ@Z 0x01E5F690,
  tc_appearance_base.obj #4; retail inlines it) and the default constructor (FableWin.pdb ??0CEngineTextureMorph@@QAE@XZ 0x02D87870, engine_surface_composition_manager.obj #2, 44 bytes /Od;
  body not readable here, so declaration only; not byte-required for 0x006CA8C0, measured). The init list (not body assignment) is byte-required.
- engine/CEngineTextureMorph.h  FABLE_RETAIL_CENGINETEXTUREMORPHSET_ADDENGINEMORPH (nested in FABLE_RETAIL_CENGINETEXTUREMORPHSET, row 0x006CA8C0):
  protected `CEngineTextureMorph* AddEngineMorph(long original_texture);` (Ego_r / FableWin ?AddEngineMorph@CEngineTextureMorphSet@@IAEPAVCEngineTextureMorph@@J@Z).
  Both additions move the two FABLE_STATIC_ASSERT lines: pinned with pin_asserts.py (FABLE_STATIC_ASSERT_AT(17, ...), (30, ...)); default path unchanged.
