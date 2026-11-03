# 009da9f0 final handoff

Final status: **VERIFIED_MATCH**. Selected grade: 39. 

ROW.json and root STATUS.tsv are authoritative. Earlier status claims below are historical; they may have been superseded.

## rows/009da9f0/NAMING_EVIDENCE.md

# 009da9f0 original identity and operand proof

Identity: `CRenderManager2D::Render2DDrawList(bool)`, exact Leaf `?Render2DDrawList@CRenderManager2D@@QAEX_N@Z`. Independent original Ego_r and FableWin PDB name sets contain this exact spelling; both full owner TPI records describe the same public nonvirtual member. Native order DrawBufferedPolys009d9c80 -> Render2DDrawList009da9f0 -> AddText009db700 and the full donor control flow establish the retail location. Ego_r original procedure is00b2e390/3246 bytes; FableWin original030e4340/2822 bytes. Donor addresses are never treated as retail addresses.

The raw native3344 bytes, disassembly, full donor2822 bytes/disassembly/resolved calls, complete original PDB locals, and full type records are retained under `evidence/009da9f0_*`. Both original TPI sources independently prove class keys, bases, original access, fields, exact const/reference types and container/template arguments. Final full declarations also appear in `009da9f0_*_final_full_types.json` and `009da9f0_original_interfaces.json`; earlier `types`, `dependencies`, `more`, `utility_types` and `bank_types` cover nested scalar/state/vertex classes. Raw generated bits are in `009da9f0_authored_member_attributes.json`; per-PDB generated display constructor records are separate.

Actual selected object inspection is `history/009da9f0/attempt_37/{symbols.txt,relocations.txt,built.raw.disasm,retail.raw.disasm}`. `RELOCATIONS.json` audits all127 physical operands, including every nonzero global-member addend. No divergent instruction offsets are paired. After masking exactly those four-byte relocation fields, all3344 bytes are equal. `INDIRECT_CALLS.json` covers all7 actual virtual calls: two state Apply(slot0), three SDK DrawPrimitiveUP(slot83), font PrepareToRender(bool)(slot3) and Render(six arguments)(slot5). All57 direct calls have audited original bindings. There are no literal-string operands in this body.

Full native extent is009da9f0..009db6ff inclusive. Final instruction is ret4 at009db6fd; the next real AddText entry begins009db700 without padding. No terminal trap or extent change is involved.

Important identity corrections:

- CTexture pointer constructor is authored and inline: zero pointer/allocation-source then call `InitialiseFromPreallocatedTexture`009fa230. Original FableWin pointer constructor030e5110 is55 bytes; native helper009fa230 is74 bytes. Earlier bindings naming this address as a constructor are rejected. Assignment009fa1c0 returns **const CTexture&** in both original PDBs.
- `GVertexShaderConstantLayoutEmpty` is file-local, not a public fabricated global. Both module S_LDATA32 records independently name its exact type and internal linkage: Ego_r Release/lib_render_manager_2d.obj stream1033 record2e38, FableWin Debug module stream1030 record8bc. C++ `static` emits `_GVertexShaderConstantLayoutEmpty`; native object address013ca8a8.
- All seven used D3D lookup-table names, private ulong array types and complete sizes come from both PDBs. `009da9f0_lookup_table_audit.json` compares every element with the independently named FableWin data array and finds exact equality. These are complete arrays, not scalar stand-ins for selected indexes. LibraryStateBlockFunctions is the real private array of seven CStateBlockFunctionBase pointers.
- Shader singleton is the real private static `CShaderRenderManager` object013bc470. All offset-bearing operands refer to proven original fields: current layout, current layout index, shader-enable bytes, UpdateFlags and WorldMatrixIsIdentity.
- RestoreCaptureBlock has the release no-argument signature in Ego_r; FableWin's same original member takes a debug-only owner void*. Both original TPI records and the scoped destructor identify it; the native call pushes no argument. The native release target00a05840 is not an invented PopBookmark/StateMethods owner.
- STLport erase symbols are not expected in the donor Dinkumware name sets. The original STLport4.6.2 `stl/_vector.h` lines473-488 proves erase(iterator,iterator), pointer iterators and clear forwarding to erase(begin,end). Both original CRenderManager2D TPI records prove the exact class element and std::allocator arguments. Native009e1440 handles32-byte vertices; native009e15e0 handles60-byte CQuickDrawTriInfo records with real string/texture destruction. No invented POD or assignment trait is added.

Full virtual tables are in `009da9f0_retail_vtables.json`, checked from raw retail RTTI type descriptors and contiguous code-pointer entries. CShaderBase retains original GetDebugName atslot1 (native00987cf0), so CountReferences is slot2. CFontBank keeps all six original slots; its complete debug wrapper bodies explicitly push true and null texture before virtual calls. CInitBaseClass and CSystemFont retain Validate slot1. CBankFile and CShaderDataBank retain all20 slots; actual CBankFile ctor009cd480 stores0129b54c, and older correction012b87f4 was rejected. Render core/2D have the single destructor slot. Full DirectX9 SDK119-slot Device interface is the independent SDK identity source for DrawPrimitiveUP.

All in-class helper bodies represent their entire original non-debug behavior. `009da9f0_all_helper_manifest.txt` records full debug extents; original debug assertion/owner tracking absent from the release types is not reconstructed as gameplay behavior. PrepareForRender includes **all flags paths**, even though Main passes0; there is no empty or selectively weakened callee. Original debug PrepareForRender directly accesses private Reserved and ReservedPresets, which supports the required original friendship. Scoped capture likewise directly accesses original private StateManager. The13 setter bodies and private SetRenderState are authored (raw3/1), and only these use the documented Owner expansion gate. Historical exact inline keyword remains UNPROVEN. Ordinary current-header baseline34 fails; selected37 matches.

Every unique actual relocation binding follows; per-site addends and independent proof pointers remain in RELOCATIONS.json. Prior conflicting names are separately retained in NAME_FIXES.json for root reconciliation.

| Retail base | Original symbol |
|---|---|
| 00987fe0 | `?EnableVertexShaders@CShaderRenderManager@@QAEXXZ` |
| 00988020 | `?ApplyVertexShader@CShaderRenderManager@@QAEXABVCVertexShader@@_N@Z` |
| 009880e0 | `?DisableVertexShaders@CShaderRenderManager@@QAEXXZ` |
| 00988110 | `?EnablePixelShaders@CShaderRenderManager@@QAEXXZ` |
| 00988190 | `?DisablePixelShaders@CShaderRenderManager@@QAEXXZ` |
| 00988290 | `?ResetWorldTransform@CShaderRenderManager@@QAEXXZ` |
| 00988a20 | `?UpdatePixelShader@CShaderRenderManager@@AAEXXZ` |
| 00988a50 | `?UpdateCombinedProjectionTransform@CShaderRenderManager@@AAEXXZ` |
| 00989760 | `?UpdateAmbient@CShaderRenderManager@@AAEXXZ` |
| 009897c0 | `?UpdateFogColour@CShaderRenderManager@@AAEXXZ` |
| 00989bf0 | `?UpdatePixelShaderConstants@CShaderRenderManager@@AAEXXZ` |
| 0098a760 | `?UpdateLightGlobals@CShaderRenderManager@@AAEXXZ` |
| 0098b230 | `?RegisterVertexShaderConstantLayout@CShaderRenderManager@@QAEXJPBVCVertexShaderConstantLayout@@@Z` |
| 0098b5e0 | `?SetVertexShaderConstantLayout@CShaderRenderManager@@QAEXJ@Z` |
| 0098b930 | `?SetVSConstants@CShaderRenderManager@@AAEXJPBXK@Z` |
| 009a0cf0 | `?AttachTextureToStage@CRenderManagerCore@@QAEXJPBVCTexture@@@Z` |
| 009e1440 | `?erase@?$vector@VCTVertexRHWColSpecTex1@@V?$allocator@VCTVertexRHWColSpecTex1@@@std@@@std@@QAEPAVCTVertexRHWColSpecTex1@@PAV3@0@Z` |
| 009e15e0 | `?erase@?$vector@VCQuickDrawTriInfo@@V?$allocator@VCQuickDrawTriInfo@@@std@@@std@@QAEPAVCQuickDrawTriInfo@@PAV3@0@Z` |
| 009f9f70 | `?Uninitialise@CTexture@@QAEXXZ` |
| 009fa1c0 | `??4CTexture@@QAEABV0@ABV0@@Z` |
| 009fa230 | `?InitialiseFromPreallocatedTexture@CTexture@@QAE_NPAUIDirect3DTexture9@@@Z` |
| 00a05840 | `?RestoreCaptureBlock@CRenderStateManager@@QAEXXZ` |
| 00a058c0 | `?RealiseRenderState@CRenderStateManager@@QAEXXZ` |
| 00a0aa80 | `?SetAWindow@CRenderManagerCore@@QAEXABVC2DBoxF@@@Z` |
| 01396510 | `?LibraryStateBlockFunctions@CRenderManager2D@@0PAPAVCStateBlockFunctionBase@@A` |
| 01396dc8 | `?D3DTextureUVModeTable@CRenderStateManager@@0PAKA` |
| 01396df8 | `?D3DZBufferFuncModeTable@CRenderStateManager@@0PAKA` |
| 01396e58 | `?D3DTextureFilterModeTable@CRenderStateManager@@0PAKA` |
| 01396e64 | `?D3DTextureMipMapModeTable@CRenderStateManager@@0PAKA` |
| 01396f9c | `?D3DPolyShadeModeTable@CRenderStateManager@@0PAKA` |
| 01396fa4 | `?D3DPolyFillModeTable@CRenderStateManager@@0PAKA` |
| 01396fb0 | `?D3DPolyCullModeTable@CRenderStateManager@@0PAKA` |
| 013bc470 | `?SingletonInstance@CShaderRenderManager@@0V1@A` |
| 013ca8a8 | `_GVertexShaderConstantLayoutEmpty` |
| 013cb508 | `?StateManager@CRenderStateCaptureBlock@@0PAVCRenderStateManager@@A` |

Final37 omits the unused CBankFile::AbortFlag static declaration entirely under the package token restriction; it is not redeclared with altered type. Its exact original qualified type remains in raw PDB evidence. Earlier1-36 header receipts are diagnostic because they retained that disallowed declaration. All35 migration headers, source and fixture now pass the token scan; current-root CInitBaseClass/CSimpleMemoryPool defaults are byte-preserved after removing only the new opt-in additions.

Main39 independently reproduced the full3344 bytes after reviewing and selectively integrating35 headers, verified127 relocation operands with addends,all7 whole lookup tables and all7 indirect calls. Same exact selected Main fixture passed8704 cases under GNU64 and actual VC7.1 /GR-. All bytes equal after applying independently proven original targets; main RESULT.json is accepted.
