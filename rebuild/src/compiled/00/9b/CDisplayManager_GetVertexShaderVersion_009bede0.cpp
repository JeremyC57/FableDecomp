// CDisplayManager::GetVertexShaderVersion() -- the device's vertex shader version as major*1000 + minor*100 (1.1 -> 1100)
// Leaf: ?GetVertexShaderVersion@CDisplayManager@@QBEKXZ
// Retail: Fable.exe (TLC, VC7.1) function @ 0x009BEDE0, 29 bytes (29 + int3 padding). No callees.
// Name: FableWin.pdb / Ego_r.pdb `?GetVertexShaderVersion@CDisplayManager@@QBEKXZ` (unsigned long, const).
// The caller CEngineResourceManager::GetIndexBufferUsage (0x00B3B030, landed) compares it against
// 0x44C (1100 = vs_1_1), and the twin 0x009BEE00 (+8 bytes) is GetPixelShaderVersion, whose landed
// caller CShaderRenderManager::EnablePixelShaders tests >= 1000.
// Layout: the manager holds a D3DCAPS9 at +0x5C (DeviceType +0x5C and AdapterOrdinal +0x60 are the
// CheckDeviceFormat arguments in the landed IsSupportingCompressedTextures 0x009BE800), so
// Caps.VertexShaderVersion (D3DCAPS9 +0xC4) sits at +0x120 and PixelShaderVersion at +0x128.
// Shape: D3DSHADER_VERSION_MAJOR(v) = (v >> 8) & 0xFF folds into `movzx eax, byte [ecx+121h]`, while
// D3DSHADER_VERSION_MINOR(v) = v & 0xFF stays a dword load plus `and ecx,0FFh`; major*10 is the
// `lea eax,[eax+eax*4]` / `lea eax,[ecx+eax*2]` pair and the final *100 an `imul`. No engine header
// exists for CDisplayManager, so the class is a local layout model (D3D caps as plain dwords).

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
    unsigned long GetVertexShaderVersion() const;
private:
    unsigned char _pad00[0x5C];
    SD3DCaps9 Caps;                             // +0x5C
};

unsigned long CDisplayManager::GetVertexShaderVersion() const
{
    return (D3DSHADER_VERSION_MAJOR(Caps.VertexShaderVersion) * 10 + D3DSHADER_VERSION_MINOR(Caps.VertexShaderVersion)) * 100;
}