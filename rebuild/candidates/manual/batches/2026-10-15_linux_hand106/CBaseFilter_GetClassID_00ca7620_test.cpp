#include <cstdio>
#include <cstring>
struct GUID { unsigned long Data1; unsigned short Data2, Data3; unsigned char Data4[8]; };
struct CBaseFilter {
    GUID m_clsid;
    long GetClassID(GUID* pClsID) { if (pClsID == 0) return (long)0x80004003L; *pClsID = m_clsid; return 0; }
};
int main()
{
    CBaseFilter f; f.m_clsid.Data1 = 0x12345678; f.m_clsid.Data2 = 1; f.m_clsid.Data3 = 2; std::memcpy(f.m_clsid.Data4, "abcdefgh", 8);
    GUID g; std::memset(&g, 0, sizeof g);
    bool ok = f.GetClassID(0) == (long)0x80004003L && f.GetClassID(&g) == 0 && !std::memcmp(&g, &f.m_clsid, sizeof g);
    if (!ok) { std::printf("00ca7620_TEST FAIL\n"); return 1; }
    std::printf("00ca7620_TEST PASS\n");
    return 0;
}
