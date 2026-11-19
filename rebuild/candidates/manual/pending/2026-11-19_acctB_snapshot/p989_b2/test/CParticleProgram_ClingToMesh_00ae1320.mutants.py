MUTANTS = {
 "pmt_operands_swapped": ("AlignedSetMultiplyTransposed(matrix, m); }", "AlignedSetMultiplyTransposed(m, matrix); }"),
 "pmt_no_copy": ("{ ALIGN16 CMatrix4x4 m; m = *this; AlignedSetMultiplyTransposed(matrix, m); }", "{ AlignedSetMultiplyTransposed(matrix, *this); }"),
 "x87_rhs_index": ("a.Data[c1][3] * b.Data[3][c2];", "a.Data[c1][3] * b.Data[2][c2];"),
 "bone_loop_bound": ("for (unsigned long c_bone = 0; c_bone < no_bones; ++c_bone)\n            bone_transforms", "for (unsigned long c_bone = 1; c_bone < no_bones; ++c_bone)\n            bone_transforms"),
 "calc_not_world": ("bone_transforms, no_bones, true);", "bone_transforms, no_bones, false);"),
 "skin_from_sim": ("TransformVertices(mesh_vertices, InitialSimulation.Positions,", "TransformVertices(mesh_vertices, sim.Positions,"),
 "blend_swapped": ("GFInterpolateVectors(sim.Positions, sizeof(C4DVector), mesh_vertices, sizeof(C4DVector), sim.Positions,", "GFInterpolateVectors(sim.Positions, sizeof(C4DVector), sim.Positions, sizeof(C4DVector), mesh_vertices,"),
}
