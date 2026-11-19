MUTANTS = {
 "pmt_operands_swapped": ("AlignedSetMultiplyTransposed(matrix, m); }", "AlignedSetMultiplyTransposed(m, matrix); }"),
 "pmt_no_copy": ("{ ALIGN16 CMatrix4x4 m; m = *this; AlignedSetMultiplyTransposed(matrix, m); }", "{ AlignedSetMultiplyTransposed(matrix, *this); }"),
 "x87_lhs_index": ("a.Data[c1][3] * b.Data[3][c2];", "a.Data[c1][2] * b.Data[3][c2];"),
 "copy_fail_ignored": ("        if (!sim.CopyFrom(InitialSimulation))\n            return false;\n", "        sim.CopyFrom(InitialSimulation);\n"),
 "bone_loop_bound": ("for (unsigned long c_bone = 0; c_bone < no_bones; ++c_bone)", "for (unsigned long c_bone = 1; c_bone < no_bones; ++c_bone)"),
 "slots_swapped": ("ApplyConstraints(sim, context.GetMaximumIterations(), context.GetMaximumDistanceError());", "ApplyConstraints(sim, (long)context.GetMaximumDistanceError(), (float)context.GetMaximumIterations());"),
 "last_before_constraints": ("        ApplyConstraints(sim, context.GetMaximumIterations(), context.GetMaximumDistanceError());\n        memcpy(sim.LastPositions, sim.Positions, sim.Size * sizeof(C4DVector));", "        memcpy(sim.LastPositions, sim.Positions, sim.Size * sizeof(C4DVector));\n        ApplyConstraints(sim, context.GetMaximumIterations(), context.GetMaximumDistanceError());"),
}
