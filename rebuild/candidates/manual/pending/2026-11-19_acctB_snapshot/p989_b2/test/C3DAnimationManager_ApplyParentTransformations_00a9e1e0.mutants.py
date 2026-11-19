MUTANTS = {
 "normal_operands_swapped": ("dest[c_bone].AlignedSetMultiplyTransposed(dest[parent_index], local_matrix, sse);", "dest[c_bone].AlignedSetMultiplyTransposed(local_matrix, dest[parent_index], sse);"),
 "transposed_operands_swapped": ("dest[c_bone].AlignedSetMultiplyTransposed(local_matrix, dest[parent_index], sse);", "dest[c_bone].AlignedSetMultiplyTransposed(dest[parent_index], local_matrix, sse);"),
 "root_test_le": ("            if (parent_index < 0)\n                src[c_bone].ToMatrix(", "            if (parent_index <= 0)\n                src[c_bone].ToMatrix("),
 "normal_uses_transposed_kf": ("                src[c_bone].ToMatrix(local_matrix, false);", "                src[c_bone].ToMatrixTransposed(local_matrix, false);"),
 "x87_rhs_index": ("a(c1, 3) * b(3, c2);", "a(c1, 3) * b(2, c2);"),
 "loop_bound": ("for (unsigned long c_bone = 0; c_bone < no_bones; ++c_bone) {\n            long parent_index = mesh.GetBone(c_bone)->GetParent();\n            if (parent_index < 0)\n                src[c_bone].ToMatrixTransposed", "for (unsigned long c_bone = 1; c_bone < no_bones; ++c_bone) {\n            long parent_index = mesh.GetBone(c_bone)->GetParent();\n            if (parent_index < 0)\n                src[c_bone].ToMatrixTransposed"),
}
