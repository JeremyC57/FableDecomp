// CShaderRenderManager::SetVSMatrixConstantTransposed(unsigned long startRegister, const CMatrix* matrix, unsigned long rows) -- transpose the 3x4 matrix (four rows of three: C3DMatrix layout, 12 dwords) into a frame block and upload its first `rows` float4 rows as vertex-shader constants
// Leaf: SetVSMatrixConstantTransposed
// Retail: Fable.exe (TLC, VC7.1) function @ 0x009887B0, length 144 (127 + int3 padding) bytes.
// Callees: IDirect3DDevice9::SetVertexShaderConstantF (COM slot 94, +0x178, `__stdcall` with the device pushed).
// Shape: the transposed copy is an element-by-element store into a 12-dword local array (one scratch
// register), then `if (rows)` around the device call. The device pointer sits at +0x1d4 (PDB D3DDevice
// +0x1e4; retail's 12-byte CArrays put it 0x10 lower). Floats are copied as dwords (unsigned int).
struct CMatrix {
    unsigned int m[12];
};

struct IDirect3DDevice9 {
    virtual void __stdcall v00();
    virtual void __stdcall v01();
    virtual void __stdcall v02();
    virtual void __stdcall v03();
    virtual void __stdcall v04();
    virtual void __stdcall v05();
    virtual void __stdcall v06();
    virtual void __stdcall v07();
    virtual void __stdcall v08();
    virtual void __stdcall v09();
    virtual void __stdcall v10();
    virtual void __stdcall v11();
    virtual void __stdcall v12();
    virtual void __stdcall v13();
    virtual void __stdcall v14();
    virtual void __stdcall v15();
    virtual void __stdcall v16();
    virtual void __stdcall v17();
    virtual void __stdcall v18();
    virtual void __stdcall v19();
    virtual void __stdcall v20();
    virtual void __stdcall v21();
    virtual void __stdcall v22();
    virtual void __stdcall v23();
    virtual void __stdcall v24();
    virtual void __stdcall v25();
    virtual void __stdcall v26();
    virtual void __stdcall v27();
    virtual void __stdcall v28();
    virtual void __stdcall v29();
    virtual void __stdcall v30();
    virtual void __stdcall v31();
    virtual void __stdcall v32();
    virtual void __stdcall v33();
    virtual void __stdcall v34();
    virtual void __stdcall v35();
    virtual void __stdcall v36();
    virtual void __stdcall v37();
    virtual void __stdcall v38();
    virtual void __stdcall v39();
    virtual void __stdcall v40();
    virtual void __stdcall v41();
    virtual void __stdcall v42();
    virtual void __stdcall v43();
    virtual void __stdcall v44();
    virtual void __stdcall v45();
    virtual void __stdcall v46();
    virtual void __stdcall v47();
    virtual void __stdcall v48();
    virtual void __stdcall v49();
    virtual void __stdcall v50();
    virtual void __stdcall v51();
    virtual void __stdcall v52();
    virtual void __stdcall v53();
    virtual void __stdcall v54();
    virtual void __stdcall v55();
    virtual void __stdcall v56();
    virtual void __stdcall v57();
    virtual void __stdcall v58();
    virtual void __stdcall v59();
    virtual void __stdcall v60();
    virtual void __stdcall v61();
    virtual void __stdcall v62();
    virtual void __stdcall v63();
    virtual void __stdcall v64();
    virtual void __stdcall v65();
    virtual void __stdcall v66();
    virtual void __stdcall v67();
    virtual void __stdcall v68();
    virtual void __stdcall v69();
    virtual void __stdcall v70();
    virtual void __stdcall v71();
    virtual void __stdcall v72();
    virtual void __stdcall v73();
    virtual void __stdcall v74();
    virtual void __stdcall v75();
    virtual void __stdcall v76();
    virtual void __stdcall v77();
    virtual void __stdcall v78();
    virtual void __stdcall v79();
    virtual void __stdcall v80();
    virtual void __stdcall v81();
    virtual void __stdcall v82();
    virtual void __stdcall v83();
    virtual void __stdcall v84();
    virtual void __stdcall v85();
    virtual void __stdcall v86();
    virtual void __stdcall v87();
    virtual void __stdcall v88();
    virtual void __stdcall v89();
    virtual void __stdcall v90();
    virtual void __stdcall v91();
    virtual void __stdcall v92();
    virtual void __stdcall v93();
    virtual long __stdcall SetVertexShaderConstantF(unsigned int startRegister, const float* data, unsigned int vector4fCount);   // slot 94 (+0x178)
};

struct CShaderRenderManager {
    unsigned char     _pad000[0x1d4];
    IDirect3DDevice9* D3DDevice;                 // +0x1d4
    void SetVSMatrixConstantTransposed(unsigned long startRegister, const CMatrix* matrix, unsigned long rows);
};

void CShaderRenderManager::SetVSMatrixConstantTransposed(unsigned long startRegister, const CMatrix* matrix, unsigned long rows)
{
    unsigned int transposed[12];
    transposed[0] = matrix->m[0];
    transposed[1] = matrix->m[3];
    transposed[2] = matrix->m[6];
    transposed[3] = matrix->m[9];
    transposed[4] = matrix->m[1];
    transposed[5] = matrix->m[4];
    transposed[6] = matrix->m[7];
    transposed[7] = matrix->m[10];
    transposed[8] = matrix->m[2];
    transposed[9] = matrix->m[5];
    transposed[10] = matrix->m[8];
    transposed[11] = matrix->m[11];
    if (rows)
        D3DDevice->SetVertexShaderConstantF(startRegister, (const float*)transposed, rows);
}
