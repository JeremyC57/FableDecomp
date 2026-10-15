// CMediaGraphOwner_00ca5170::DetachTarget() -- ask the graph object at +0xD8 (virtual slot 35) to detach; on success release the interface at +0x9C (tail call), on failure return its error code
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00CA5170, length 31 bytes (+1 int3 padding).
// Retail bytes:  push esi ; mov esi,ecx ; mov ecx,[esi+0D8h] ; mov eax,[ecx] ; call [eax+8Ch] ; test eax,eax ; jl F ;
//                mov ecx,esi ; pop esi ; jmp ReleaseInterface ; F: pop esi ; ret
// Manifest label "CRenderTarget::DetachTarget" is propagated (0x00CAxxxx media module, beside "COM_GetOutputPinName");
// the class is named by address. The member at +0xD8 is a C++ object (`this` in ecx, so `__thiscall` virtual slot 35,
// +0x8C) returning an HRESULT; `jl` tests it signed. Callee 0x00CA8B50 (manifest "CSurface::GetRefCount", rejected):
// releases the COM interface at +0x9C (slot 6 then IUnknown::Release, clears it) and returns an HRESULT -- reached by
// tail jump with `this` restored to ecx.
struct CMediaGraph_00ca5170 {
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04();
    virtual void v05(); virtual void v06(); virtual void v07(); virtual void v08(); virtual void v09();
    virtual void v10(); virtual void v11(); virtual void v12(); virtual void v13(); virtual void v14();
    virtual void v15(); virtual void v16(); virtual void v17(); virtual void v18(); virtual void v19();
    virtual void v20(); virtual void v21(); virtual void v22(); virtual void v23(); virtual void v24();
    virtual void v25(); virtual void v26(); virtual void v27(); virtual void v28(); virtual void v29();
    virtual void v30(); virtual void v31(); virtual void v32(); virtual void v33(); virtual void v34();
    virtual long Detach();                         // slot 35 (+0x8C)
};

struct CMediaGraphOwner_00ca5170 {
    unsigned char         _pad[0xd8];
    CMediaGraph_00ca5170* m_pGraph;                // +0xD8
    long ReleaseInterface();                       // 0x00CA8B50
    long DetachTarget();
};

long CMediaGraphOwner_00ca5170::DetachTarget()
{
    long result = m_pGraph->Detach();
    if (result < 0)
        return result;
    return ReleaseInterface();
}
