MUTANTS = {
 "operands_swapped": ("transform.AlignedSetMultiplyTransposed(WorldMatrix, TextureTransforms[c_transform]);", "transform.AlignedSetMultiplyTransposed(TextureTransforms[c_transform], WorldMatrix);"),
 "no_row_clamp": ("unsigned long rows = range.Count < 4 ? range.Count : 4;", "unsigned long rows = range.Count < 4 ? range.Count : (range.Count > 4 ? 3 : 4);"),
 "clamp_off_by_one": ("range.Count < 4 ? range.Count : 4", "range.Count < 3 ? range.Count : 3"),
 "no_skip_empty": ("if (range.Count) {", "if (1) {"),
 "loop_bound": ("c_transform < (unsigned long)TextureTransformCount", "c_transform <= (unsigned long)TextureTransformCount && c_transform < 4"),
 "flag_set_missing": ("        UpdateFlags |= 0x1000;\n        UpdateFlags &= ~0x800;\n", "        UpdateFlags &= ~0x800;\n"),
 "flag_clear_missing": ("        UpdateFlags |= 0x1000;\n        UpdateFlags &= ~0x800;\n", "        UpdateFlags |= 0x1000;\n"),
 "x87_rhs_index": ("a(c1, 3) * r(3, c2)", "a(c1, 3) * r(2, c2)"),
 "offset_wrong": ("SetVSConstants(range.Offset, &transform, rows);", "SetVSConstants(range.Offset + 1, &transform, rows);"),
}
