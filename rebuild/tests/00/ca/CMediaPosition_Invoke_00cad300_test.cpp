#include <cstdio>
#include <cstring>
// CMediaPosition::Invoke: DISP_E_UNKNOWNINTERFACE unless riid is IID_NULL; GetTypeInfo(0, lcid, &pti) failure is returned;
// else pti->Invoke(this, ...) and pti->Release().
struct GUID { unsigned d1; unsigned short d2, d3; unsigned char d4[8]; };
static const GUID IID_NULL = { 0, 0, 0, { 0, 0, 0, 0, 0, 0, 0, 0 } };
struct TypeInfo { int refs; void* inst; int memid; virtual int Invoke(void* p, int id) { inst = p; memid = id; return 11; } void Release() { --refs; } };
struct MediaPosition {
    TypeInfo* ti; int fail; unsigned lastLcid;
    int GetTypeInfo(unsigned it, unsigned lcid, TypeInfo** pp) { if (fail) return (int)0x80004005; lastLcid = lcid; ++ti->refs; *pp = ti; return 0; }
    int Invoke(int id, const GUID& riid, unsigned lcid)
    {
        if (std::memcmp(&IID_NULL, &riid, sizeof(GUID)) != 0) return (int)0x80020001;
        TypeInfo* pti;
        int hr = GetTypeInfo(0, lcid, &pti);
        if (hr < 0) return hr;
        hr = pti->Invoke(this, id);
        pti->Release();
        return hr;
    }
};
int main()
{
    TypeInfo t; t.refs = 1; t.inst = 0; t.memid = 0; MediaPosition m; m.ti = &t; m.fail = 0; m.lastLcid = 0;
    GUID g = IID_NULL; g.d1 = 1;
    if (m.Invoke(3, g, 1) != (int)0x80020001 || t.inst != 0) { std::printf("00cad300_TEST FAIL\n"); return 1; }
    if (m.Invoke(3, IID_NULL, 0x409) != 11 || t.inst != &m || t.memid != 3 || t.refs != 1 || m.lastLcid != 0x409) { std::printf("00cad300_TEST FAIL\n"); return 1; }
    m.fail = 1; if (m.Invoke(4, IID_NULL, 0) != (int)0x80004005 || t.memid != 3) { std::printf("00cad300_TEST FAIL\n"); return 1; }
    std::printf("00cad300_TEST PASS\n");
    return 0;
}