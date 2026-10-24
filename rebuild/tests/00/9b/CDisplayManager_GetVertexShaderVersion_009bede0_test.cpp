#include <cstdio>
#include <cstring>

#define D3DSHADER_VERSION_MAJOR(_Version) (((_Version) >> 8) & 0xFF)
#define D3DSHADER_VERSION_MINOR(_Version) (((_Version) >> 0) & 0xFF)

struct SD3DCaps9 {
    unsigned int DeviceType;
    unsigned int AdapterOrdinal;
    unsigned char _pad08[0xC4 - 0x08];
    unsigned int VertexShaderVersion;
    unsigned int MaxVertexShaderConst;
    unsigned int PixelShaderVersion;
};

class CDisplayManager {
public:
    unsigned int GetVertexShaderVersion() const
    {
        return (D3DSHADER_VERSION_MAJOR(Caps.VertexShaderVersion) * 10 + D3DSHADER_VERSION_MINOR(Caps.VertexShaderVersion)) * 100;
    }
    unsigned char _pad00[0x5C];
    SD3DCaps9 Caps;
};

int main()
{
    CDisplayManager m;
    std::memset(&m, 0, sizeof(m));
    m.Caps.VertexShaderVersion = 0xFFFE0101u;                     // D3DVS_VERSION(1,1) style token
    if (m.GetVertexShaderVersion() != 1100) { std::printf("009bede0_TEST FAIL\n"); return 1; }
    m.Caps.VertexShaderVersion = 0xFFFF0300u;                     // 3.0
    if (m.GetVertexShaderVersion() != 3000) { std::printf("009bede0_TEST FAIL\n"); return 1; }
    m.Caps.VertexShaderVersion = 0xFFFF0104u;                     // 1.4
    if (m.GetVertexShaderVersion() != 1400) { std::printf("009bede0_TEST FAIL\n"); return 1; }
    m.Caps.VertexShaderVersion = 0;
    if (m.GetVertexShaderVersion() != 0) { std::printf("009bede0_TEST FAIL\n"); return 1; }
    std::printf("009bede0_TEST PASS\n");
    return 0;
}