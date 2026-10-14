#include <cstdio>
#include <cstdlib>
#include <cstring>
static char g_seq[64] = "";
struct SIndexBufferDesc { long Format; long Type; unsigned long Usage; long Pool; unsigned long Size; }; static unsigned long g_descSize = 0, g_lock[3]; static int g_dataWord; static void* g_data = &g_dataWord;
struct IDirect3DIndexBuffer { virtual long Lock(unsigned long offset, unsigned long size, void** ppData, unsigned long flags) = 0; virtual long GetDesc(SIndexBufferDesc* desc) = 0; };
struct CBuffer : IDirect3DIndexBuffer { virtual long Lock(unsigned long offset, unsigned long size, void** ppData, unsigned long flags) { std::strcat(g_seq, "L"); g_lock[0] = offset; g_lock[1] = size; g_lock[2] = flags; *ppData = g_data; return 0; } virtual long GetDesc(SIndexBufferDesc* desc) { std::strcat(g_seq, "D"); desc->Size = g_descSize; return 0; } };
static IDirect3DIndexBuffer* g_locked = 0; static float g_weight = 0; struct CRenderStatistics { void NotifyLock(IDirect3DIndexBuffer* buffer, float weight) { std::strcat(g_seq, "N"); g_locked = buffer; g_weight = weight; } };
struct SRenderDevice { unsigned char _pad[8]; CRenderStatistics* m_pStatistics; }; struct SDeviceRecord { unsigned char _pad[0x60]; SRenderDevice* m_pRender; }; static CRenderStatistics g_stats; static SRenderDevice g_render; static SDeviceRecord g_record;
static SDeviceRecord* GetDeviceRecord() { g_render.m_pStatistics = &g_stats; g_record.m_pRender = &g_render; return &g_record; }
struct CIndexBuffer { virtual void v00() { } IDirect3DIndexBuffer* m_pBuffer;
    void* Lock(unsigned long flags, unsigned long offset, unsigned long size) { if (size == 0) { SIndexBufferDesc desc; m_pBuffer->GetDesc(&desc); size = desc.Size; } SDeviceRecord* record = GetDeviceRecord(); record->m_pRender->m_pStatistics->NotifyLock(m_pBuffer, 1.0f); void* data; m_pBuffer->Lock(offset, size, &data, flags); return data; } };

int main() {
    CIndexBuffer b; CBuffer buf; b.m_pBuffer = &buf; g_descSize = 640; if (b.Lock(4, 8, 0) != g_data || std::strcmp(g_seq, "DNL") || g_lock[0] != 8 || g_lock[1] != 640 || g_lock[2] != 4 || g_locked != &buf || g_weight != 1.0f) { std::printf("00ac1fb0_TEST FAIL\n"); return 1; }
    g_seq[0] = 0; if (b.Lock(1, 2, 3) != g_data || std::strcmp(g_seq, "NL") || g_lock[0] != 2 || g_lock[1] != 3 || g_lock[2] != 1) { std::printf("00ac1fb0_TEST FAIL\n"); return 1; }
    std::printf("00ac1fb0_TEST PASS\n"); return 0;
}
