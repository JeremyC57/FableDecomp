# Relocation audit

The exact COFF leaf `?IsInsideFustrum@CEnginePrimitiveManagerStaticMeshesBase@@AAE_NPBVC3DPrimitive2@@ABVCEngineInternalPrimitiveStaticMesh@@@Z` is 429 bytes. The single absolute relocation at leaf offset +0x79 is the camera pointer loaded at 0x00B9D7A7. Retail 0x01436EA0 resolves in the bindings table to the emitted symbol `?GPCamera@NEngineInternal@@3PAVCEngineCamera@@A`.
