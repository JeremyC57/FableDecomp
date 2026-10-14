#include <cstdio>
#include <cstdlib>
#include <cstring>
static int g_base = 0;
struct CResource { virtual void v00() { } unsigned char _body[0x1c - 4]; CResource() { ++g_base; } };
struct STextureDims_00a69980 { long m_0, m_4, m_8, m_c, m_10, m_14, m_18, m_1c, m_20, m_24; STextureDims_00a69980() : m_0(0), m_4(0), m_8(0), m_c(0), m_10(0), m_14(0), m_18(0), m_1c(0), m_20(0), m_24(0) { } };
struct STextureFlags_00a69980 { bool m_a, m_b, m_c; STextureFlags_00a69980() : m_a(false), m_b(false), m_c(false) { } };
struct CManagedTextureData : CResource { short m_width; short m_height; STextureDims_00a69980 m_dims; STextureFlags_00a69980 m_flags; virtual void v00() { } CManagedTextureData() : CResource(), m_width(0), m_height(0) { } };
int main() {
    CManagedTextureData* d = new CManagedTextureData; if (g_base != 1 || d->m_width || d->m_height || d->m_dims.m_0 || d->m_dims.m_24 || d->m_flags.m_a || d->m_flags.m_c) { std::printf("00a69980_TEST FAIL\n"); return 1; } delete d;
    std::printf("00a69980_TEST PASS\n"); return 0;
}
