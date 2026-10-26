// CShaderRenderManager::Uninitialise() -- uninitialise every shader resource manager, then forget the Direct3D device
// Leaf: Uninitialise
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0098A7A0, length 79 bytes.
// Callees: 0x00A5D7A0 (the per-resource uninitialise; landed under a propagated
// CMemoryDataOutputStream::Clear label), called on each ResourceManagers[i].Data.
// Identity: the manifest label CPlayerManager::PrepareForLoad is propagated; the callers are the
// CShaderRenderManager destructor 0x0098ABF0 (vtable 0x0129A080) and CDisplayManager's destructor through the
// singleton 0x013BC470, and Ego_r.pdb has ?Uninitialise@CShaderRenderManager@@QAEXXZ.
// Layout (Ego_r.pdb - 0x10 here: PDB containers are 4 bytes wider): D3DDevice +0x1d4, ResourceManagers
// (CArray<CCountedPointer<CShaderResource> >, 8-byte elements) +0x3a8.
// Shape: the PrepareForLoad loop with an 8-byte stride (`sar 3`, `[ecx+edi*8]`), 7-byte `lea esp,[esp]` pad,
// then the device store.
class CShaderResource { public:
    void Uninitialise();                                    // 0x00A5D7A0
};
class CCPPointerInfo;
struct CShaderResourcePointer {                             // CCountedPointer<CShaderResource>, 8 bytes
    CShaderResource* Data;
    CCPPointerInfo*  Info;
    CShaderResource* operator->() const { return Data; }
};

struct CShaderResourceArray {                               // retail CArray<CCountedPointer<CShaderResource> > (12 bytes)
    CShaderResourcePointer* m_pFirst;
    CShaderResourcePointer* m_pLast;
    CShaderResourcePointer* m_pEnd;
    unsigned int size() const { return m_pLast - m_pFirst; }
    CShaderResourcePointer& operator[](unsigned int i) { return m_pFirst[i]; }
};

struct IDirect3DDevice9;

class CShaderRenderManager { public:
    void*                __vftable;                         // +0x000
    unsigned char        _pad004[0x1d0];
    IDirect3DDevice9*    D3DDevice;                         // +0x1d4
    unsigned char        _pad1d8[0x1d0];
    CShaderResourceArray ResourceManagers;                  // +0x3a8
    void Uninitialise();
};

void CShaderRenderManager::Uninitialise()
{
    for (unsigned int i = 0; i < ResourceManagers.size(); ++i)
        ResourceManagers[i]->Uninitialise();
    D3DDevice = 0;
}