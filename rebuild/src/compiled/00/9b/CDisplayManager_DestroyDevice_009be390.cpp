// CDisplayManager::DestroyDevice() -- release the back-buffer and depth-buffer surfaces, then the Direct3D device
// Retail: Fable.exe (TLC, VC7.1) function @ 0x009BE390, length 34 bytes.
// Callees: CSurface::Uninitialise() @ 0x009F2E20 on BackBufferSurface (+0x2c) and DepthBufferSurface (+0x3c), IDirect3DDevice9
// vslot 2 Release (COM `__stdcall`, `push eax ; call [ecx+8]`) on D3DDevice (+0x58) when present.
// PDB CDisplayManager: BackBufferSurface +44, DepthBufferSurface +60 (16-byte CSurface), D3DDevice +88.
struct IDirect3DDevice9 {
    virtual long __stdcall QueryInterface(const void* iid, void** object);
    virtual unsigned long __stdcall AddRef();
    virtual unsigned long __stdcall Release();
};

class CSurface { public:
    unsigned char _body[0x10];         // vptr-less 16-byte surface record
    void Uninitialise();               // 0x009F2E20
};

class CDisplayManager { public:
    unsigned char     _pad[0x2c];      // +0x00 vptr, SystemManager, PRenderManager, render-target surfaces
    CSurface          BackBufferSurface;       // +0x2c
    CSurface          DepthBufferSurface;      // +0x3c
    unsigned char     _pad2[0xc];              // +0x4c BackBufferCopyTexture, D3D
    IDirect3DDevice9* D3DDevice;               // +0x58
    void DestroyDevice();
};

void CDisplayManager::DestroyDevice()
{
    BackBufferSurface.Uninitialise();
    DepthBufferSurface.Uninitialise();
    if (D3DDevice)
        D3DDevice->Release();
}