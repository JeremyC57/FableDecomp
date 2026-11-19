MUTANTS = {
 "no_copy": ("    ALIGN16 CMatrix4x4 m;\n    m = *this;\n    AlignedSetMultiplyTransposed(matrix, m);", "    AlignedSetMultiplyTransposed(matrix, *this);"),
 "operands_swapped": ("    AlignedSetMultiplyTransposed(matrix, m);", "    AlignedSetMultiplyTransposed(m, matrix);"),
 "plain_multiply": ("    AlignedSetMultiplyTransposed(matrix, m);", "    AlignedSetMultiply(matrix, m);"),
}
