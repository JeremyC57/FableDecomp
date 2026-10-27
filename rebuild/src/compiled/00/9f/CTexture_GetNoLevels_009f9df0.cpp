// CTexture::GetNoLevels() const -- returns the Direct3D texture's mip level count
// Retail: Fable.exe 0x009F9DF0, 9 bytes: `mov eax,[ecx] ; mov ecx,[eax] ; push eax ; call [ecx+34h] ; ret`.
// Identity: catalog name; Ego_r.pdb ?GetNoLevels@CTexture@@QBEJXZ. PD3DTexture (+0x00, engine/CTexture.h) is an
// IDirect3DTexture9; vtable slot 13 (+0x34) is IDirect3DBaseTexture9::GetLevelCount (IUnknown 0-2,
// IDirect3DResource9 3-10, SetLOD 11, GetLOD 12), a __stdcall COM call.
// Leaf: ?GetNoLevels@CTexture@@QBEJXZ
struct IDirect3DTexture9 {
    virtual long __stdcall QueryInterface(const void* riid, void** ppv) = 0;
    virtual unsigned long __stdcall AddRef() = 0;
    virtual unsigned long __stdcall Release() = 0;
    virtual long __stdcall GetDevice(void** ppDevice) = 0;
    virtual long __stdcall SetPrivateData(const void* refguid, const void* pData, unsigned long size, unsigned long flags) = 0;
    virtual long __stdcall GetPrivateData(const void* refguid, void* pData, unsigned long* pSize) = 0;
    virtual long __stdcall FreePrivateData(const void* refguid) = 0;
    virtual unsigned long __stdcall SetPriority(unsigned long priority) = 0;
    virtual unsigned long __stdcall GetPriority() = 0;
    virtual void __stdcall PreLoad() = 0;
    virtual int __stdcall GetType() = 0;
    virtual unsigned long __stdcall SetLOD(unsigned long lod) = 0;
    virtual unsigned long __stdcall GetLOD() = 0;
    virtual unsigned long __stdcall GetLevelCount() = 0;
};

#define CTexture DonorCTexture
#include "engine/CTexture.h"
#undef CTexture

class CTexture : public DonorCTexture {
public:
    long GetNoLevels() const;
};

long CTexture::GetNoLevels() const
{
    return PD3DTexture->GetLevelCount();
}