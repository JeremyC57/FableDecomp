// CDisplayManager::GetPixelShaderVersion() -- the device's pixel shader version as major*1000 + minor*100 (ps_1_4 -> 1400)
// Leaf: ?GetPixelShaderVersion@CDisplayManager@@QBEKXZ
// Retail: Fable.exe (TLC, VC7.1) function @ 0x009BEE00, 29 bytes. No callees.
// Name: FableWin.pdb / Ego_r.pdb `?GetPixelShaderVersion@CDisplayManager@@QBEKXZ` (unsigned long, const);
// the landed caller CShaderRenderManager::EnablePixelShaders (0x00988110) enables pixel shaders when it
// is >= 1000 (ps_1_0). Twin of GetVertexShaderVersion 0x009BEDE0, eight bytes further into the caps.
// Layout: the manager holds a D3DCAPS9 at +0x5C (DeviceType +0x5C / AdapterOrdinal +0x60 feed
// CheckDeviceFormat in the landed IsSupportingCompressedTextures 0x009BE800), so
// Caps.PixelShaderVersion (D3DCAPS9 +0xCC) sits at +0x128.
// Shape: D3DSHADER_VERSION_MAJOR folds into a `movzx` of byte +0x129, D3DSHADER_VERSION_MINOR stays a
// dword load plus `and 0FFh`; major*10 + minor is two `lea`s, then `imul eax,eax,64h`. No engine
// header exists for CDisplayManager, so the class is a local layout model.

#define D3DSHADER_VERSION_MAJOR(_Version) (((_Version) >> 8) & 0xFF)
#define D3DSHADER_VERSION_MINOR(_Version) (((_Version) >> 0) & 0xFF)

struct SD3DCaps9 {                              // D3DCAPS9 (only the members this file reads)
    unsigned long DeviceType;                   // +0x00
    unsigned long AdapterOrdinal;               // +0x04
    unsigned char _pad08[0xC4 - 0x08];
    unsigned long VertexShaderVersion;          // +0xC4
    unsigned long MaxVertexShaderConst;         // +0xC8
    unsigned long PixelShaderVersion;           // +0xCC
};

class CDisplayManager {
public:
    unsigned long GetPixelShaderVersion() const;
private:
    unsigned char _pad00[0x5C];
    SD3DCaps9 Caps;                             // +0x5C
};

unsigned long CDisplayManager::GetPixelShaderVersion() const
{
    return (D3DSHADER_VERSION_MAJOR(Caps.PixelShaderVersion) * 10 + D3DSHADER_VERSION_MINOR(Caps.PixelShaderVersion)) * 100;
}