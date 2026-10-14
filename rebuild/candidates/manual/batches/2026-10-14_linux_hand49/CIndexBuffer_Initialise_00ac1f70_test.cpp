#include <cstdio>
#include <cstdlib>
#include <cstring>
struct IDirect3DIndexBuffer { int x; }; static IDirect3DIndexBuffer g_buffer; static unsigned long g_args[4]; static void* g_shared = (void*)1;
struct IDirect3DDevice { virtual long CreateIndexBuffer(unsigned long length, unsigned long usage, long format, long pool, IDirect3DIndexBuffer** ppBuffer, void* sharedHandle) { g_args[0] = length; g_args[1] = usage; g_args[2] = format; g_args[3] = pool; *ppBuffer = &g_buffer; g_shared = sharedHandle; return 0; } };
static IDirect3DDevice g_device; struct SRenderDevice { unsigned char _pad[0x58]; IDirect3DDevice* m_pDevice; }; struct SDeviceRecord { unsigned char _pad[0x60]; SRenderDevice* m_pRender; };
static SRenderDevice g_render; static SDeviceRecord g_record; static SDeviceRecord* GetDeviceRecord() { g_render.m_pDevice = &g_device; g_record.m_pRender = &g_render; return &g_record; }
struct CIndexBuffer { virtual void v00() { } IDirect3DIndexBuffer* m_pBuffer; unsigned long m_length; long m_count; void Initialise(unsigned long length, unsigned long usage, long format, long pool, long count) { GetDeviceRecord()->m_pRender->m_pDevice->CreateIndexBuffer(length, usage, format, pool, &m_pBuffer, 0); m_length = length; m_count = count; } };

int main() {
    CIndexBuffer b; b.m_pBuffer = 0; b.Initialise(100, 8, 101, 1, 50); if (b.m_pBuffer != &g_buffer || b.m_length != 100 || b.m_count != 50 || g_args[0] != 100 || g_args[1] != 8 || g_args[2] != 101 || g_args[3] != 1 || g_shared != 0) { std::printf("00ac1f70_TEST FAIL\n"); return 1; }
    std::printf("00ac1f70_TEST PASS\n"); return 0;
}
