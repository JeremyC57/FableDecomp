# 00989cb0 final handoff

Final status: **VERIFIED_MATCH**. Selected grade: 34. 

ROW.json and root STATUS.tsv are authoritative. Earlier status claims below are historical; they may have been superseded.

## rows/00989cb0/NAMING_EVIDENCE.md

# 00989cb0 — CShaderRenderManager::UpdateObjectSpaceLighting

Current status: VERIFIED_MATCH. Selected32, independent main regrade34, all2188 bytes and11 relocation operands checked;6912 dual-compiler behavior scenarios pass. This supersedes the initial parked review.

Original private void thiscall Leaf: `?UpdateObjectSpaceLighting@CShaderRenderManager@@AAEXK@Z`, present in both PDBs (Ego_r00ae6500/FableWin030372c0), TPI owner0x40d2f. Native PrepareForRender call009e13a6 passes its flags and same owner receiver, after checking native UpdateFlags+3d8. This supplies independent caller/type proof; the stale public QAE spelling is returned in NAME_FIXES.

## Extent and table

Assigned2188 bytes are one complete function: code00989cb0..0098a52c followed by its own16-byte switch table0098a52c..0098a53c and four alignment bytes. The ret4 is0098a529. This is not a bad span: all four table entries point into this function, selected by its own native switch at00989def. Compiler labels are local, not invented external bindings. Original CShaderGenericLight::EParameterType enumerators0..3 identify colour, direction, position and attenuation cases. RELOCATIONS.json records every native/object case label separately; the final matching object has the same table and case offsets, independently checked by main.

## Exact original data, helpers and naming

Complete original manager header from0098acf0 is reused. Lighting opt-in restores the genuine nested class CLightInfo (V key, size56, Light+0, IsEnabled+52, UpdateFlag+53), proven by TPI0x40d33 and native0x38 stride and flag displacements. CShaderGenericLight TPI0x40b52 supplies its complete52-byte layout, two original parameters and type enums, attenuation fields and Size+48. CShaderConstantRange TPI0x40b7d has actual CBaseClass base, public Offset+4 and Count+8, size12. The original layout's private LightArray, LightAttenuations and CameraPos opaque storage becomes these exact types only in the lighting opt-in. Friend access is an inference from the private original member records and this native owner access, not a claimed recovered friend record. Existing defaults are untouched.

CMatrix4x4::SetInverseOfUniformScaleRotationTranslation is the original public float-returning const-reference method at00a5d000. Both PDBs give matrix/mult locals; native211-byte body computes reciprocal squared scale, transposed inverse and translation. Its actual body was tested for same-TU context in probe7 and rejected for the target because it rotated the initial saved receiver into edx. It is not a separate scope completion and is absent from the selected TU.

NVectorMath::GFNormalise3DVectors at00a5c360 is exactly `?GFNormalise3DVectors@NVectorMath@@YAXPBVC4DVector@@KPAV2@KK@Z`, proved by both PDB names and native five-argument normalization implementation. No vendor or assembly implementation is authored. The native call takes the destination vector as both source and output, zero strides and count1.

The genuine private SetVSConstants signature is long register, const void* data, unsigned long count, corroborated by both PDBs and native0098b930. Expanded calls reach the original Direct3D9 SetVertexShaderConstantF slot94 (+178), HRESULT return and stdcall with UINT arguments. Direct3D9Device.h transcribes the entire119-slot SDK interface; all method names were checked against original Ego_r IDirect3DDevice9 TPI. The SDK/type evidence and native call displacement are independent. No fake numbered virtual methods exist.

C4DVector::Initialise and operator* and both CMatrix4x4 transform overloads use original signatures and real math bodies. FableWin procedure records recover the three-argument transform's src/dest/w and copied C4DVector vect, plus operator* argument sc. The selected helper preserves that copy, required for source/destination aliasing. Both debug transform overloads have out-of-line PDB entries; neither appears as an unaligned transform in Ego_r release. The selected three-argument helper uses strong inline because the observed native sites are expanded and plain inline outlined them. This is documented evidence and a probe preference, not proof of the original keyword. No forwarding overload is authored: the unused two-argument method is declaration-only.

Main locals flags, update_begin/update_end/update_lights, unit, scale, inverse_world, c_light, light_info, dest_light/dest_pos, src_light/no_params/c_param/src_param/dest_param, att0/att1/att2/attbase, attenuation_count/reg_count/dest_reg/c_att/localised_attenuation and object_space_pos come from original procedure-local records. Native loads, stores and loop bounds independently corroborate their roles. The original localised_attenuation is four C4DVectors, despite the compiler emitting an unrolled path for larger loop counts; no invented larger array is substituted.

## Initial attempts and validation (historical; superseded below)

19 actual grade receipts are retained, grouped conservatively into13 source families. Repair1, invalid alias diagnostic18 and confirmation19 do not count. Probe3 wording falsely claimed no debug out-of-line copy; PROBES corrects it explicitly. Probe18 is rejected because a reference in place of the copied transform vector is unsafe when input/output alias. Selected17 restored and final19 independently confirmed2176v2188, correct0xe8 frame, most control/register structure but wrong aggregate/spill slots and remaining x87/index ordering. Named attenuation sequencing removed the earlier extra temporary and reproduced the scalar attenuation block.

GNU64 C++03 and actual VC7.1 /GR- pass6912 scenarios. The fixture contains the selected row and transform body, models external dependencies, and independently predicts maximal dirty upload runs and math results for0..8 lights,0/1/2 parameter slots, all four parameter kinds, half/unit/double scales, quarter-turn transforms, translations, camera and flags. All values are exactly representable. External normalization and device operations are dependency doubles; tests validate this row's behavior, not an unclaimed reimplementation of those callees. Eleven relocations, four SDK virtual sites, exact zero/one literals and four switch targets are reviewed. PARKED, not a match.

Evidence: shader_init_types.json, shader_light_types.json, shader_support_types.json, shader_d3d9_types.json, shader_d3d9_sdk_slots.json, function_locals_CShaderRenderManager_UpdateObjectSpaceLighting.json, function_locals_CMatrix4x4_TransformTransposedAssumeW.json, function_locals_CMatrix4x4_SetInverseOfUniformScaleRotationTranslation.json and function_locals_C4DVector_operator*.json under evidence/. Source/header snapshots preserve every experiment.


## Expert recovery and main acceptance

Attempts20–32 recovered the original separate temporary camera vector with the default-constructed object_space_pos destination, then restored the original unparenthesized Y arithmetic. The final indexed-operand mismatch was fixed by the two original statements `UpdateFlags |= 0x200; UpdateFlags &= ~0x100;`. The camera constructor/three-argument transform call shape is confirmed in the original donor, not an invented math rewrite. Selected32 matches2188/2188, confirmed33 and independently34. The final tests paste the selected row and alias-safe helper body; all6912 cases pass GNU C++03 and actual VC7.1 /GR-. Main checked all11 relocated operands,4 case targets and4 SDK slot94 calls, and patched the object operands to prove full retail-byte equality.

Ordinary-inline comparisons35–39 retained out-of-line calls and do not match. The newer Owner decisions in corrections/docs/EXTRA_RULES.md prefer ordinary inline *when the bytes hold*, requiring documentation of the form needed. Main applied that conditional header-helper rule to the genuine original CMatrix arithmetic helper; CMatrix is an interpretation of the category, not an enumerated example. The original keyword is NOT proven. The helper body, exact types and native expansion are proven. This decision and the independent expert concurrence are in evidence/00989cb0_INLINE_RULE_REVIEW.json. The earlier annotation hold is superseded.
