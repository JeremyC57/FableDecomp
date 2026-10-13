// CRenderManagerCore::SetVertexFormat(unsigned long)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A0AA30, oracle length 23 bytes.
// Grade: candidate (authored from the retail bytes; byte parity NOT yet proven -- run
//        tools/decomp_pipeline/verify_and_land.py on this batch's payload to grade it).
//
// Retail bytes:  mov edx,[esp+4] ; mov eax,[ecx+3CF0h] ; mov ecx,[eax] ; push edx ; push eax ; call [ecx+164h] ; ret 4
// engine/CRenderManagerCore.h (PDB): +0x3CF0 IDirect3DDevice9* PD3DDevice. COM vtable slot 89
// (+0x164) of IDirect3DDevice9 is SetFVF(DWORD); the interface is __stdcall so `this` is pushed.
// d3d9.h is not on the verifier include path, so the interface shape is declared locally.
#include "engine/CRenderManagerCore.h"

struct IDirect3DDevice9 {
    virtual void __stdcall v00(); virtual void __stdcall v04(); virtual void __stdcall v08(); virtual void __stdcall v0c();
    virtual void __stdcall v10(); virtual void __stdcall v14(); virtual void __stdcall v18(); virtual void __stdcall v1c();
    virtual void __stdcall v20(); virtual void __stdcall v24(); virtual void __stdcall v28(); virtual void __stdcall v2c();
    virtual void __stdcall v30(); virtual void __stdcall v34(); virtual void __stdcall v38(); virtual void __stdcall v3c();
    virtual void __stdcall v40(); virtual void __stdcall v44(); virtual void __stdcall v48(); virtual void __stdcall v4c();
    virtual void __stdcall v50(); virtual void __stdcall v54(); virtual void __stdcall v58(); virtual void __stdcall v5c();
    virtual void __stdcall v60(); virtual void __stdcall v64(); virtual void __stdcall v68(); virtual void __stdcall v6c();
    virtual void __stdcall v70(); virtual void __stdcall v74(); virtual void __stdcall v78(); virtual void __stdcall v7c();
    virtual void __stdcall v80(); virtual void __stdcall v84(); virtual void __stdcall v88(); virtual void __stdcall v8c();
    virtual void __stdcall v90(); virtual void __stdcall v94(); virtual void __stdcall v98(); virtual void __stdcall v9c();
    virtual void __stdcall va0(); virtual void __stdcall va4(); virtual void __stdcall va8(); virtual void __stdcall vac();
    virtual void __stdcall vb0(); virtual void __stdcall vb4(); virtual void __stdcall vb8(); virtual void __stdcall vbc();
    virtual void __stdcall vc0(); virtual void __stdcall vc4(); virtual void __stdcall vc8(); virtual void __stdcall vcc();
    virtual void __stdcall vd0(); virtual void __stdcall vd4(); virtual void __stdcall vd8(); virtual void __stdcall vdc();
    virtual void __stdcall ve0(); virtual void __stdcall ve4(); virtual void __stdcall ve8(); virtual void __stdcall vec();
    virtual void __stdcall vf0(); virtual void __stdcall vf4(); virtual void __stdcall vf8(); virtual void __stdcall vfc();
    virtual void __stdcall v100(); virtual void __stdcall v104(); virtual void __stdcall v108(); virtual void __stdcall v10c();
    virtual void __stdcall v110(); virtual void __stdcall v114(); virtual void __stdcall v118(); virtual void __stdcall v11c();
    virtual void __stdcall v120(); virtual void __stdcall v124(); virtual void __stdcall v128(); virtual void __stdcall v12c();
    virtual void __stdcall v130(); virtual void __stdcall v134(); virtual void __stdcall v138(); virtual void __stdcall v13c();
    virtual void __stdcall v140(); virtual void __stdcall v144(); virtual void __stdcall v148(); virtual void __stdcall v14c();
    virtual void __stdcall v150(); virtual void __stdcall v154(); virtual void __stdcall v158(); virtual void __stdcall v15c();
    virtual void __stdcall v160();
    virtual long __stdcall SetFVF(unsigned long fvf);   // slot 89 (+0x164)
};

struct CRenderManagerCore_Methods : CRenderManagerCore {
    void SetVertexFormat(unsigned long fvf);
};

void CRenderManagerCore_Methods::SetVertexFormat(unsigned long fvf)
{
    this->PD3DDevice->SetFVF(fvf);
}
