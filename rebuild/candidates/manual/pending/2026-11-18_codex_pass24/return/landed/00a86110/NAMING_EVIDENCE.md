# Identity and extent

Ego_r and FableWin identify the protected static method with the complete signature in the source. Ego_d places it in lib_physics_mesh_segment.obj. Its native caller at 0x00A868F9 passes the hidden plane result, vertex array and three remaining arguments consistently with that signature. The checked continuous extent is 1,967 bytes.

Ego_d records SDividingPlaneNormalTable as an anonymous-namespace C3DVector array. FableWin records its named dynamic initializer, which Ego_d places in the same translation unit. The DIR32 operand targets 0x01398658, holding exactly the three authored axis vectors in Z, X, Y order. The anonymous-namespace hash varies with the file path. The remaining operands reference independently checked single-precision constants.

The indices, face coordinates, extrema and final scalar and vector names come from the scoped Ego_d record. The optimized inner scan uses a moving pointer in place of the debug build’s integer c2 and reference loop_pos. The retail centroid loop weights the second vertex once and the third twice. Tests check this independently for all axes, with 1 through 9 triangles and repeated, signed and equal coordinates.
