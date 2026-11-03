# 0098acf0 final handoff

Final status: **VERIFIED_MATCH**. Selected grade: 3. 

ROW.json and root STATUS.tsv are authoritative. Earlier status claims below are historical; they may have been superseded.

## rows/0098acf0/NAMING_EVIDENCE.md

# 0098acf0 — CShaderRenderManager::Initialise

Verified exact Leaf: `?Initialise@CShaderRenderManager@@QAEXPAVCRenderManager@@PAUIDirect3DDevice9@@@Z`.

## Identity and extent

Both original PDB name sets identify the public nonvirtual thiscall method with void return, class CRenderManager pointer and struct IDirect3DDevice9 pointer. Ego_r donor00ae72f0 and FableWin0302fe70 agree. Ego_r TPI0x40d2f supplies the complete original member types/access. Native caller009c102e passes CDisplayManager's renderer(+8) and device(+58), with manager013bc470 in ecx; the native constructor0098ab00 stores owner vtable0129a080 and constructs the nested layout at+18. This independently distinguishes the owner from a guessed manifest label.

The assigned extent is exact: 0098acf0..0098b225, 1333 bytes. A ret8 at0098b222 is followed by alignment at0098b225. The final object has exactly one matching Leaf and 1333 bytes. All40 relocated operands were decoded at identical native/object offsets and individually verified; all remaining bytes are identical.

## Headers and members

Full new CShaderRenderManager header retains every original private member in order (donor1040/native1000 bytes). Native member accesses in this row and the original constructor fix native offsets; original PDB TPI proves names/types, including the original spelling CurrentVertexShaderVertexDeclairation. LightPool, CurrentPixelShader and BoneMatrices retain their complete opaque subobject storage and type comments; they are untouched here. The seven alignment bytes after WorldMatrixIsIdentity are native and donor-observed; no packing directive was introduced.

The opt-in CVertexShaderConstantLayout branch restores the original CBaseClass base and private data; default generated branch is unchanged. Original donor336/native332 bytes differ only by the trailing CArray representation. No fake class substitutes for the real owner.

CMatrix4x4 (TPI0xa231) has the original public E11..E44 and overlapping Data[4][4], size64. Native identity stores confirm these offsets. C4DVector (TPI0xc3e2) has original X/Y/Z/W and four-float constructor, size16. CRGBFloatColour (TPI0x2cd2) has R/G/B/A and original four-float Initialise. Unused default constructors are declarations only: no unsupported default behavior is claimed.

CIndexedArray<CCharString> (TPI0x40eb7) has private CArray<CCharString> Array followed by map<CCharString,long> IndexMap; native0098db20 independently shows the12-byte vector and map beginning+12. The correct public method is push_back, not Add/Insert. CShaderResource (TPI0xa158) derives from CBaseClassNonCopyable with full original data; private Initialise stores the manager at+14 in native00a5d4d0. The friend declaration permits this observed private caller access; it is inferred, not claimed as a recovered friend record.

Original parameter spellings render_manager/d3ddevice appear in both PDB procedure-local records. c_res/c_tt are recovered from FableWin (not claimed present in Ego_r).

## Relocations

`RELOCATIONS.json` enumerates all40 operand sites and exact strings. CCharString const-char*/long constructor and destructor are original PDB symbols bound to0099ebf0/0099eae0. CShaderResource::Initialise is private AAE at00a5d4d0, not old public QAE. CIndexedArray<CCharString>::push_back is at0098db20.

The three STLport callees use the original container template arguments from the PDB members plus real STLport4.6 method names/types, corroborated by native element stride and method behavior: vector<const CVertexShaderConstantLayout*>::resize0098d870; vector<C4DVector>::erase0098cf70; vector<C4DVector>::_M_fill_insert0098d360. No Anniversary Dinkumware method body is substituted. The exact declared template symbols occur in the compiler's COFF symbol table; source-level STLport specializations keep the two observed out-of-line methods out of line.

Seven addresses had stale spellings in supplied bindings. These are recorded in NAME_FIXES.json and merged into root NAME_FIXES.tsv, including incorrect fake vector classes and incorrect CCharString spellings. No online or existing binding file was changed. No correct emitted symbol was found bound to another address.

## Attempts, validation and limitations

First model already produced1333 bytes; only the identity matrices' store order differed. Separate ascending field assignments in the original SetIdentity method, diagonal ones first, produced exact parity (grade2). Final main confirmation grade3 is independent. No extra attempt minimum applies to an early exact match.

Standalone C++03 fixture contains the selected body and matrix helper, with dependency doubles.336 scenarios pass GNU64 and actual VC7.1 /GR-: existing layouts, resources including live growth during initialization, existing constant-name entries, all resize branches, exact literal order, identity matrices/colors, preserved inactive fields and balanced string lifetimes. Test doubles do not substitute for byte or symbol validation; those are separately checked on the production TU.

Full evidence: evidence/shader_init_types.json, shader_support_types.json, shader_resource_types.json, function_locals_CShaderRenderManager_Initialise.json; original sources in input/names and the native independently cut disassembly. All historical sources and header snapshots remain immutable.
