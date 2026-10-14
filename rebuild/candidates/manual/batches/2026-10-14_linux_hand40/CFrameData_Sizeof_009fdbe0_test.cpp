#include <cstdio>
#include <cstdlib>
#include <cstring>
struct CGraphicFrameData_009fdbe0 { long Sizeof() { return 100; } };
struct CManagedTexture_009fdbe0 { long m_0; void* m_pData; bool HasData() const { return m_pData != 0; } long Sizeof() { return 7; } };
struct CFrameData { unsigned char _pad[0x1c]; CGraphicFrameData_009fdbe0* m_pGraphic; unsigned char _pad20[8]; CManagedTexture_009fdbe0 m_texture;
    long Sizeof() { long size = (m_pGraphic ? m_pGraphic->Sizeof() + 4 : 4) + 0x44; if (m_texture.HasData()) size += m_texture.Sizeof(); return size; } };
int main() {
    CFrameData f; std::memset(&f, 0, sizeof f); if (f.Sizeof() != 0x48) { std::printf("009fdbe0_TEST FAIL\n"); return 1; } CGraphicFrameData_009fdbe0 g; f.m_pGraphic = &g; if (f.Sizeof() != 0x48 + 100) { std::printf("009fdbe0_TEST FAIL\n"); return 1; }
    int d; f.m_texture.m_pData = &d; if (f.Sizeof() != 0x48 + 100 + 7) { std::printf("009fdbe0_TEST FAIL\n"); return 1; }
    std::printf("009fdbe0_TEST PASS\n"); return 0;
}
