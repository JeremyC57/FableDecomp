# C3DBoundingBox::InitialiseFromOBB

The VC7.1 object exports `?InitialiseFromOBB@C3DBoundingBox@@QAEXABV1@ABVCMatrix3x4@@@Z`. The source-order record in Ego_d identifies `C3DBoundingBox::InitialiseFromOBB`. The submitted extent is 2735 bytes.

- egor donor procedure at 00bcfda0
- fablewin donor procedure at 03313830

- call at 0x00B9DBBF in landed 0x00B9DB80 CEngineInternalPrimitiveStaticMesh::GetBoundingBoxWorldSpace
- call at 0x00B9E4CF in landed 0x00B9E490 CEngineInternalPrimitive2DStaticMesh::GetBoundingBoxWorldSpace

Portable SSE intrinsics and the scalar four-diagonal path produce different operand scheduling and instruction selection starting in the SIMD arm.

The actual two branches are compared to an independent enumeration of all eight corners, including in-place output, 48 configurations, degenerate extents, rotation, shear, scaling and translation.
