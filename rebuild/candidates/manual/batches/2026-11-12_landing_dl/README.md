# 2026-11-12 landing dl -- chunk Q21: 1 of 14 parked near misses matched

Source: `pending/2026-11-12_linux_handQ21/`. Landed (RELOCATION_MATCH, test PASS under VC7.1): 0x00B39080
CEngineInternalAttachedPrimitive::GetParentTransformAnimatedMesh.

Headers (9, three-way merged against a8657df96, all unchanged upstream; hdrcheck2 default=same everywhere; every DIFF macro is new to
the landed tree): C3DMeshInfo.h LOADSTATS, CMeshDataBank.h GETANIMATIONMANAGER, C3DMeshStats.h DUMMYOBJECTS,
CEngineInternalAttachedPrimitive.h PARENTTRANSFORM, CEngineInternalPrimitiveOcclusionBase.h RESETOCCLUSIONINFO, CMatrix3x4.h
SETMULTIPLY, card_sorting_types.h CCARD_POSITIONS, fable_ime.h CIME_ISIMEACTIVE, NUISystem_CText.h GETCHARWIDTHW / WINDOW_SLOTS.
Note: the macro name FABLE_RETAIL_CMATRIX3X4_SETMULTIPLY also appears in the never-landed overlay of
`pending/2026-11-08_linux_hand850/` (row 0x00772BB0). If that row is ever ported, use the landed definition.

Checks: bindings_multi_check at baseline; funclet_includes_check 1317 checked, 0 broken. One RELOC_NAME warning: the row's
`??1?$CCountedPointer@$$CBVC3DMeshStats@@@@QAE@XZ` binds 0x004C24C0, landed as `_global::CCountedPointer_Release_004c24c0`, which
is queued for renaming (land1011dl).
Queued (land1011dl): 0x004C24C0 ~CCountedPointer<const C3DMeshStats>; 0x006C1250 is CTCBuyableHouse::FrameUpdate (vtable slot 9);
0x00B4A700 is CEnginePrimitiveRenderer2D::Render; NUISystem_CText.h slots 147-150 default order is wrong (make WINDOW_SLOTS the
default after rechecking includers).
