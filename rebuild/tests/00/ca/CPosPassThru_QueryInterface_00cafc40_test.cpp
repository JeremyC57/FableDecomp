// Behaviour test for CPosPassThru::QueryInterface (0x00CAFC40, DECLARE_IUNKNOWN): the call is forwarded unchanged to the owning (outer) unknown, whose result is returned.
#include <cstdio>
#include <cstring>
#include <cstdlib>
#include <new>
static char g_log[256];
static void Log(char c) { std::size_t n = std::strlen(g_log); g_log[n] = c; g_log[n + 1] = 0; }
struct GuidModel { unsigned long Data1; unsigned short Data2, Data3; unsigned char Data4[8]; };
struct UnknownModel {
    virtual long QueryInterface(const GuidModel& riid, void** ppv) = 0;
    virtual unsigned long AddRef() = 0;
    virtual unsigned long Release() = 0;
};
static const GuidModel* g_riid; static void** g_ppv;
struct OwnerModel : UnknownModel {
    long QueryInterface(const GuidModel& riid, void** ppv) { g_riid = &riid; g_ppv = ppv; *ppv = this; Log('Q'); return 0x1234; }
    unsigned long AddRef() { return 2; }
    unsigned long Release() { return 1; }
};
struct CUnknownModel { UnknownModel* const m_pUnknown; long m_cRef; CUnknownModel(UnknownModel* owner) : m_pUnknown(owner), m_cRef(0) { }
    UnknownModel* GetOwner() const { return m_pUnknown; } };
struct PosPassThruModel : UnknownModel, CUnknownModel {
    PosPassThruModel(UnknownModel* owner) : CUnknownModel(owner) { }
    long QueryInterface(const GuidModel& riid, void** ppv) { return GetOwner()->QueryInterface(riid, ppv); }
    unsigned long AddRef() { return GetOwner()->AddRef(); }
    unsigned long Release() { return GetOwner()->Release(); }
};
int main()
{
    OwnerModel owner;
    PosPassThruModel pass(&owner);
    GuidModel iid = { 0x36B73880, 0xC2C8, 0x11CF, { 0x8B, 0x46, 0x00, 0x80, 0x5F, 0x6C, 0xEF, 0x60 } };
    void* out = 0;
    UnknownModel* asUnknown = &pass;
    long hr = asUnknown->QueryInterface(iid, &out);
    if (hr != 0x1234 || out != &owner || g_riid != &iid || g_ppv != &out || std::strcmp(g_log, "Q") != 0) { std::printf("00cafc40_TEST FAIL\n"); return 1; }
    std::printf("00cafc40_TEST PASS\n");
    return 0;
}