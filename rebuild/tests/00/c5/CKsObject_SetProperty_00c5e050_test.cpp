// Behaviour test for CKsObject::SetProperty @ 0x00C5E050 (DeviceIoControl replaced by a recording fake)
#include <cstdio>
#include <cstring>
typedef unsigned int DWORD;
struct GUID { unsigned int Data1; unsigned short Data2, Data3; unsigned char Data4[8]; };
struct KSPROPERTY { GUID Set; unsigned int Id; unsigned int Flags; };
static KSPROPERTY g_seen; static DWORD g_code, g_inSize, g_outSize; static void* g_device; static void* g_out; static int g_result;
static int FakeDeviceIoControl(void* device, DWORD code, void* in, DWORD inSize, void* out, DWORD outSize, DWORD* bytes, void* overlapped)
{
    g_device = device; g_code = code; std::memcpy(&g_seen, in, sizeof(g_seen)); g_inSize = inSize; g_out = out; g_outSize = outSize;
    if (*bytes != 0 || overlapped != 0) return 0;
    *bytes = 4;
    return g_result;
}
struct CKsObject {
    void* m_vtableOrOwner; void* m_handle;
    bool SetProperty(const GUID& propertySet, unsigned int id, void* value, unsigned int size)
    {
        DWORD bytesReturned = 0;
        KSPROPERTY property;
        property.Set = propertySet; property.Id = id; property.Flags = 2;
        return FakeDeviceIoControl(m_handle, 0x002F0003, &property, sizeof(property), value, size, &bytesReturned, 0) != 0;
    }
};
int main()
{
    CKsObject o; int handle = 0; o.m_handle = &handle;
    GUID g = { 0x12345678, 1, 2, { 1, 2, 3, 4, 5, 6, 7, 8 } };
    int value = 2;
    g_result = 7;
    bool ok = o.SetProperty(g, 9, &value, 4);
    if (!ok || g_device != &handle || g_code != 0x2F0003 || g_inSize != 24 || g_out != &value || g_outSize != 4 ||
        g_seen.Set.Data1 != 0x12345678 || g_seen.Set.Data4[7] != 8 || g_seen.Id != 9 || g_seen.Flags != 2) {
        std::printf("00c5e050_TEST FAIL\n"); return 1;
    }
    g_result = 0;
    if (o.SetProperty(g, 9, &value, 4)) { std::printf("00c5e050_TEST FAIL\n"); return 1; }
    std::printf("00c5e050_TEST PASS\n");
    return 0;
}