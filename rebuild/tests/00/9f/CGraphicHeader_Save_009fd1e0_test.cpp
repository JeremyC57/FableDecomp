#include <cstdio>
#include <cstdlib>
#include <cstring>
static char g_seq[64] = "";
static const void* g_data = 0; static long g_length = 0; static void* g_format = 0; struct SPixelFormat { unsigned char _body[16]; };
struct CDataOutputStream { virtual void v00() { } virtual void v01() { } virtual void v02() { } virtual void v03() { } virtual void Write(const void* data, long length) = 0; void WritePixelFormat(SPixelFormat* f) { std::strcat(g_seq, "P"); g_format = f; } };
struct CStream : CDataOutputStream { virtual void Write(const void* data, long length) { std::strcat(g_seq, "W"); g_data = data; g_length = length; } };
struct CGraphicHeader { unsigned char _pad[0xc]; SPixelFormat m_format; void Save(CDataOutputStream* stream) { stream->Write(this, 0x1c); stream->WritePixelFormat(&m_format); } };

int main() {
    CGraphicHeader h; CStream s; h.Save(&s); if (std::strcmp(g_seq, "WP") || g_data != &h || g_length != 0x1c || g_format != &h.m_format) { std::printf("009fd1e0_TEST FAIL\n"); return 1; }
    std::printf("009fd1e0_TEST PASS\n"); return 0;
}