#include <cstdio>
#include <cstring>
// CBaseInputPin::NonDelegatingQueryInterface: IID_IMemInputPin -> GetInterface(IMemInputPin sub-object), else CBasePin's.
struct GUID { unsigned d1; unsigned short d2, d3; unsigned char d4[8]; };
static bool eq(const GUID& a, const GUID& b) { return !std::memcmp(&a, &b, sizeof(GUID)); }
static const GUID IID_IMemInputPin = { 0x56A8689D, 0x0AD4, 0x11CE, { 0xB0, 0x3A, 0x00, 0x20, 0xAF, 0x0B, 0xA7, 0x70 } };
static const GUID IID_IPin = { 0x56A86891, 0x0AD4, 0x11CE, { 0xB0, 0x3A, 0x00, 0x20, 0xAF, 0x0B, 0xA7, 0x70 } };
static int GetInterface(void* p, void** ppv) { *ppv = p; return 0; }
struct MemIn { int x; };
struct BasePin { int y; int BaseNDQI(const GUID& r, void** ppv) { if (eq(r, IID_IPin)) return GetInterface(this, ppv); *ppv = 0; return (int)0x80004002; } };
struct InPin : BasePin, MemIn {
    int NDQI(const GUID& r, void** ppv) { if (eq(r, IID_IMemInputPin)) return GetInterface((MemIn*)this, ppv); else return BasePin::BaseNDQI(r, ppv); }
};
int main()
{
    InPin p; void* v = 0; GUID other = IID_IPin; other.d1 = 1;
    if (p.NDQI(IID_IMemInputPin, &v) != 0 || v != (void*)(MemIn*)&p) { std::printf("00ca89a0_TEST FAIL\n"); return 1; }
    if (p.NDQI(IID_IPin, &v) != 0 || v != (void*)(BasePin*)&p) { std::printf("00ca89a0_TEST FAIL\n"); return 1; }
    if (p.NDQI(other, &v) != (int)0x80004002 || v != 0) { std::printf("00ca89a0_TEST FAIL\n"); return 1; }
    std::printf("00ca89a0_TEST PASS\n");
    return 0;
}