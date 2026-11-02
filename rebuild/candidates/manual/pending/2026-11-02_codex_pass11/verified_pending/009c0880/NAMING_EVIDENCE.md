# 009c0880 naming and layout evidence

The manifest constructor label is correct but its old pointer parameter binding is wrong. Original Ego_r PUBLIC is `??0CDisplayManager@@QAE@AAVCSystemManager@@@Z` (donor00b25330), confirmed by FableWin donor0301a7a0 and original TPI classV CDisplayManager: public thiscall constructor taking CSystemManager&. Native009a67ef calls this row while constructing CSystemManager; +4 stores that argument as the original SystemManager reference. The exact Leaf is emitted once in objdump -t. The full1475-byte span ends at009c0e43, after ret4 and before alignment padding.

## Names and native witnesses

| Native | Exact declaration | Independent witness |
|---|---|---|
|0099a2f0|protected CBaseClassNonCopyable::CBaseClassNonCopyable()|Original TPI/PUBLIC and first base construction on the same receiver|
|0129b3f8|CDisplayManager vftable|Original PUBLIC; native locator01355a00 points to type descriptor01396218 `.?AVCDisplayManager@@`; slot0 is009c30b0|
|0122f84c|CSurface vftable|Original CSurface TPI/PUBLIC; four embedded16-byte surfaces receive this pointer|
|0099aed0|public CWideString::CWideString()|Original TPI/PUBLIC and ShaderResourcePath member offset|
|00a61880|public CBusyResourceManager::CBusyResourceManager()|Original TPI/PUBLIC and the native constructor allocating its two list sentinels and setting PendingFrame=1/FinishedFrame=0|
|009e3830|public CPixelFormat::Initialise(_D3DFORMAT)|Original Ego_r PUBLIC donor00b356e0 and FableWin donor0308ca90; native maps the D3D format into FormatIndex. It is not the CPixelFormat constructor, whose debug donor is01e474c0|
|009c2e30|protected std::vector<CPixelFormat>::_M_insert_overflow|Real STLport4.6 declaration plus five native arguments: position, const element reference, const empty tag reference, count1, at_end=true. Original TPI BackBufferFormats fixes element classV CPixelFormat|
|009c2ca0|protected std::vector<ESurfaceMultisampleType>::_M_insert_overflow|Same original STLport declaration and native argument shape; original TPI MultisampleTypes fixes enum ESurfaceMultisampleType|
|00bfea0e|CRT malloc|Native import thunk and real STLport allocator implementation|

The six format values25,31,21,23,24,22 are original _D3DFORMAT enumeration members A1R5G5B5,A2B10G10R10,A8R8G8B8,R5G6B5,X1R5G5B5,X8R8G8B8. Sixteen multisample entries are ESurfaceMultisampleType values0,2 through16, verified against the original TPI enumeration and native immediates. PDB_NAMING_PROOF.json retains decoded original fields, enum/type declarations and PUBLIC records. SDK structures retain the Microsoft DirectX9 names and layout, cross-checked with the original PDB member types.

## Shared headers

The class models use genuine engine headers. Existing CSurface, CTexture and CPixelFormat defaults remain unchanged under opt-in branches. CArray selects an independent FABLE_RETAIL_CARRAY_DISPLAY branch while preserving signature, geometry and default branches. New CDisplayManager, CBusyResourceManager, C2DExtentsI, C2DBoxI/F headers contain original member names/access/types; unused untouched fields have genuine types instead of artificial byte pads. CPresentationIntervalDesc is a nested classV, and CCreateDeviceParameters is a nested structU. Original spelling FreeQueoryObjects/PendingQueories and VirtualCoordsResolutionIndependant is retained.

The original donor CDisplayManager is0x2ec; retail is0x2cc. The difference is the proven STLport4-byte lists and12-byte vectors replacing donor12-byte lists and16-byte vectors. Native offsets include DeviceResetCallbacks264, LastDeviceCreationParameters268, VsyncCallbackTimer294, BackBufferFormats298, MultisampleTypes2a4, BusyResourceManager2b0, Gamma2c4 and screenshot flags2c8/2c9. Original CPixelFormat has protected FormatIndex at0; CSurface has private PD3DSurface4,AllocationSource8,PAllocatedMemoryc; CTexture has private PD3DTexture0 plus ByteLength28-bit and AllocationSource4-bit fields at4. Untouched CSurface allocated-memory pointers, caps, dimensions and other constructor-uninitialised fields are deliberately retained.

STLport4.6 source supplies list sentinel allocation, vector push_back, constructors, allocator, and insertion/copy/fill context. The final empty tag is exactly `struct __false_type {};`, with no invented constructor. push_back uses ordinary in-class inline. The real insertion body is visible and retained out of line using the established noinline container method convention documented in GOTCHAS. No stronger inline annotation is used by the selected constructor/header path.

## Binding collision requiring correction

The stale input binds `??_7CDisplayManager@@6B@` to012b8df8 from old row00c57310. That local model is contradicted by the native RTTI above: the actual CDisplayManager table is0129b3f8. The old012b8df8 has no valid MSVC locator at[-4]; the old model also labels its imported teardown function ShutdownDisplay, but the actual IAT01440648 resolves to ole32!CoUninitialize. This audit does not invent a replacement name for the unassigned old row. Preserve the correct CDisplayManager symbol at0129b3f8 and separately correct the old landed row. NAME_FIXES.tsv and BINDING_REVIEW.json record the collision. All other selected symbols have no conflicting address in the input bindings.

This row was independently audited at every one of34 relocatable operands:32 relative calls and2 absolute vtable references,9 distinct symbols. Matching is relocation-masked, and all relocation targets were checked against the exact declared symbols rather than accepted from the mask alone.
