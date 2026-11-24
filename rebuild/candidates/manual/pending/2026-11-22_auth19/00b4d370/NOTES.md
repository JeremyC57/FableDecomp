# 0x00B4D370 NOTES (AUTH19): names and evidence

- **Function:** `?UpdateShadowSpace@CEngineShadowRenderer@@AAEXABVCEngineCamera@@@Z`.
  - Ego_r.pdb 0x009F0370 and FableWin.pdb S_GPROC32 (mod 972, Engine_Shadow_Renderer2.cpp).
  - Retail: 7176 bytes, `ret 4`.
- **Locals: every name from Ego_d.pdb and FableWin.pdb.** The S_BPREL32 lists are identical at the same offsets (`egod_locals.txt`,
  `fw_syms_UpdateShadowSpace.txt`). Scopes come from S_BLOCK32, mapped to FableWin lines by `blk.py`.
  - shadow_light_vector, local_world_to_base_shadow_space, view_mat, view_to_world, sub_divs, point_count.
  - frustrum_points[5], world00/10/01/11, world_space_control_points[26], new_point_id.
  - y, x, weightx0/x1/y0/y1, weight00/10/01/11, total_weight, new_pos.
  - pt0 (twice), pos, shadow_space_control_points[26], shadow_space_corner_control_points[4].
  - new_shadow_space_y / new_shadow_space_x, new_shadow_transform, min_y / max_y, n, corner_point.
  - transform_setup_for_perspective_shadows, perspective_interp_range_start / _end, perspective_interp.
  - frustrum_forground_width, frustrum_background_width, max_frustrum_background_width, perspective_shift, max_projected_range_x,
    scale, projected_corner_point, clipped_projected_y, clipped_uniform_y, frustrum_ratio, frustrum_reduction_factor,
    perspective_transform.
  - end_frustrum_width, diag, diag_to_center_ratio, sub_center_distance, reduced_frust_width, new_shadow_distance.
  - new_shadow_minx/maxx/miny/maxy, rotated_point.
  - The PDB spellings are kept as they are ("frustrum", "forground").
  - No descriptive names.
- **Helpers:**
  - `GFLimitUpper<float,float>` / `GFLimitLower<float,float>`: FableWin S_GPROC32, type `void (float&, const float&)`. FableWin call
    sites are 14/17-byte two-register calls (lines 479/480/535/630..633).
  - `std::swap<float>`: FableWin S_GPROC32 (line 493, 17 B).
  - `GFLimit<float,int,int>`: FableWin TU COMDAT (line 508, 44 B).
  - `C2DVector::operator=(const C3DVector&)`: FableWin S_GPROC32 `C2DVector::operator=` (lines 565/566, 34 B = direct call). Enabled with
    the existing opt-in `FABLE_RETAIL_C2DVECTOR_ASSIGN3D`.
- **Callees (out of line):**
  - C3DVector::AccurateNormalise 0x00A14440.
  - C2DVector::AccurateNormalise 0x00A14510.
  - CalcLocalWorldToBaseShadowSpace 0x00B4B8B0.
  - BuildDerivedShadowMatriciesFromWorldToShadowMatrix 0x00B4BA60.
  - CalcClipVolume 0x00B4BCC0.
  - Global NEngineInternal::GPLightingManager 0x01436E9C (as in the acctC draft).
- **Asserts:** two dead-arm `FABLE_RETAIL_ASSERT`s (FableWin lines 636/637, 225 B each; 6 unwind funclets; frameproc `has inline asm |
  has eh`). The conditions (`maxx > minx`, `maxy > miny`) are inferred from the following division; the exact text is not in the PDB.
- **Uncertain text, no PDB local involved:**
  - lines 487/488 (`min_y = 0; max_y = 0`; the dead store is byte-neutral, measured p4).
  - line 590 (diag).
  - line 614 (the ShadowFadeStart clamp, kept as the draft's one-line if/else chain).
