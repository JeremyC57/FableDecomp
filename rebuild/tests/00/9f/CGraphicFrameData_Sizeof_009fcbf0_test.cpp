#include <cstdio>
#include <cstdlib>
#include <cstring>
struct CTexture { long m_0; unsigned long m_header; long Sizeof() { return (m_header & 0xfffffff) + 0xc; } };
static long TextureSizeof(CTexture* texture) { if (texture) return texture->Sizeof(); return 8; }
struct CGraphicFrameData { unsigned char _pad[8]; CTexture* m_pTexture; unsigned char _pad0c[0xe]; unsigned char m_flags; long Sizeof() { long size = 0x13 + 8; if (m_flags & 4) size = TextureSizeof(m_pTexture) + 0x13; return size; } };

int main() {
    CGraphicFrameData d; CTexture t; t.m_0 = 0; t.m_header = 0xf0000005; d.m_pTexture = &t; d.m_flags = 0; if (d.Sizeof() != 0x1b) { std::printf("009fcbf0_TEST FAIL\n"); return 1; } d.m_flags = 4; if (d.Sizeof() != 5 + 0xc + 0x13) { std::printf("009fcbf0_TEST FAIL\n"); return 1; } d.m_pTexture = 0; if (d.Sizeof() != 0x1b) { std::printf("009fcbf0_TEST FAIL\n"); return 1; }
    std::printf("009fcbf0_TEST PASS\n"); return 0;
}