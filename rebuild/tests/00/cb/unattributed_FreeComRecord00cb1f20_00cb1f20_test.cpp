#include <cstdio>
#include <cstdlib>
#include <cstring>
static int g_released = 0; static void* g_freed[2]; static int g_nFreed = 0;
static void CoTaskMemFree(void* p) { if (g_nFreed < 2) g_freed[g_nFreed] = p; ++g_nFreed; }
struct IUnknownLike { virtual long QueryInterface(const void*, void**) { return 0; } virtual unsigned long AddRef() { return 1; } virtual unsigned long Release() { ++g_released; return 0; } };
struct SComRecord { IUnknownLike* m_pInterface; unsigned long m_payloadSize; void* m_pPayload; };
static void FreeComRecord(SComRecord* record)
{
    if (record != 0) {
        if (record->m_payloadSize != 0) { CoTaskMemFree(record->m_pPayload); record->m_payloadSize = 0; record->m_pPayload = 0; }
        if (record->m_pInterface != 0) { record->m_pInterface->Release(); record->m_pInterface = 0; }
        CoTaskMemFree(record);
    }
}
static bool Check()
{
    FreeComRecord(0); if (g_nFreed != 0) return false;
    IUnknownLike iface; char payload[4]; SComRecord r; r.m_pInterface = &iface; r.m_payloadSize = 4; r.m_pPayload = payload;
    FreeComRecord(&r);
    return g_nFreed == 2 && g_freed[0] == payload && g_freed[1] == &r && g_released == 1 && r.m_pInterface == 0 && r.m_payloadSize == 0 && r.m_pPayload == 0;
}
int main()
{
    if (!Check()) { std::printf("00cb1f20_TEST FAIL\n"); return 1; }
    std::printf("00cb1f20_TEST PASS\n");
    return 0;
}