MUTANTS = {
 "no_copy": ("{ M4 m; m = self; AlignedSetMultiplyTransposed(self, matrix, m); }", "{ AlignedSetMultiplyTransposed(self, matrix, self); }"),
 "operands_swapped": ("AlignedSetMultiplyTransposed(self, matrix, m); }", "AlignedSetMultiplyTransposed(self, m, matrix); }"),
 "x87_lhs_index": ("lhs.d[c1][3] * rhs.d[3][c2];", "lhs.d[c1][2] * rhs.d[3][c2];"),
 "no_counter": ("            counter += no_bones;\n", ""),
 "loop_bound": ("for (unsigned long c_bone = 0; c_bone < no_bones; ++c_bone)", "for (unsigned long c_bone = 1; c_bone < no_bones; ++c_bone)"),
 "no_cache": ("if (render_info->transforms == 0) {", "if (true) {"),
 "blend_flag": ("render_info->transforms, no_bones, true);", "render_info->transforms, no_bones, false);"),
}
