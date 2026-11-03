#ifndef FABLE_ENGINE_CPIXELSHADER_H
#define FABLE_ENGINE_CPIXELSHADER_H
// Hand-written (chunk 723): CPixelShader : CShaderBase (RTTI), vtable 0x0129A054: slot 0 ??_G 0x0098C3D0, slot 1 GetDebugName
// 0x00987CF0, slot 2 CountReferences 0x00987EE0. 8 bytes: vptr + the shared CPixelShaderData* Data (+0x04). The destructor drops
// one reference on Data and deletes it (virtual, flag 1) at zero, in the landed ??_G 0x0098C3D0 shape: null test on a local copy,
// `dec [Data+8]`; expanded inline by every owner (0x00B36EC0, 0x00B67920, 0x00B71880). CPixelShaderData: Ego_r.pdb, 0x14 bytes.
// Functions: Ego_r.pdb / FableWin.pdb publics.
#include "rebuild_abi.h"
#include "engine/CShaderBase.h"
struct IDirect3DPixelShader9;
class CPixelShader;
class CPixelShaderData {
    friend class CPixelShader;
    virtual ~CPixelShaderData();                                         // slot 0 (deleting dtor called with 1); ??1 EAE (private)
public:
    IDirect3DPixelShader9* PixelShader;                                  // +0x04
    unsigned long          ReferenceCount;                               // +0x08
    unsigned long          ConstCount;                                   // +0x0C
    long*                  ConstIndices;                                 // +0x10
private:
    void AddReference() { ++ReferenceCount; }                            // ?AddReference@CPixelShaderData@@AAEXXZ
    unsigned long CountReferences() const { return ReferenceCount; }     // ?CountReferences@CPixelShaderData@@ABEKXZ
    void Release() { --ReferenceCount; if (CountReferences() == 0) delete this; }   // ?Release@CPixelShaderData@@AAEXXZ
};
FABLE_STATIC_ASSERT(sizeof(CPixelShaderData) == 0x14);
class CPixelShader : public CShaderBase {
public:
    CPixelShader();                                                      // 0x00A5EC00 ??0CPixelShader@@QAE@XZ
    // operator= (??4CPixelShader@@QAEXABV0@@Z, returns void; out of line at 0x00987E50): release and null Data through a local
    // copy, then share the other's data.
    void operator=(const CPixelShader& other)
    {
        CPixelShaderData* data = Data;
        if (data) {
            if (--data->ReferenceCount == 0)
                delete data;
            Data = 0;
        }
        Data = other.Data;
        Data->AddReference();
    }
    virtual ~CPixelShader()                                              // slot 0  ??_G 0x0098C3D0; out of line 0x00987E20
    {
        if (Data)
            Data->Release();
    }
    virtual CCharString GetDebugName() const;                            // slot 1  ?GetDebugName@CPixelShader@@UBE?AVCCharString@@XZ
    IDirect3DPixelShader9* GetD3DPixelShader() const;                    // ?GetD3DPixelShader@CPixelShader@@QBEPAUIDirect3DPixelShader9@@XZ
    bool IsInitialised() const;                                          // ?IsInitialised@CPixelShader@@QBE_NXZ
    void Uninitialise();                                                 // ?Uninitialise@CPixelShader@@QAEXXZ
private:
    virtual unsigned long CountReferences();                             // slot 2  0x00987EE0 ?CountReferences@CPixelShader@@EAEKXZ
public:
    CPixelShaderData* Data;                                              // +0x04
};
FABLE_STATIC_ASSERT(sizeof(CPixelShader) == 0x8);

#endif // FABLE_ENGINE_CPIXELSHADER_H
