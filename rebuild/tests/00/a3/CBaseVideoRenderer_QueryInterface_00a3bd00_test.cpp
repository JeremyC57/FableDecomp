// Test for CBaseVideoRenderer::QueryInterface (0x00A3BD00): DECLARE_IUNKNOWN forwards to the owner's QueryInterface with the
// caller's riid / ppv untouched and returns the owner's result.
#include <cstdio>
struct GUIDT { unsigned int Data1; unsigned short Data2, Data3; unsigned char Data4[8]; };
struct IUnknownT {
    virtual long QueryInterface(const GUIDT& riid, void** ppv) = 0;
    virtual unsigned int AddRef() = 0;
    virtual unsigned int Release() = 0;
    virtual ~IUnknownT() {}
};
struct Outer : IUnknownT {
    const GUIDT* lastRiid; void** lastPpv; int calls;
    Outer() : lastRiid(0), lastPpv(0), calls(0) {}
    long QueryInterface(const GUIDT& riid, void** ppv) { lastRiid = &riid; lastPpv = ppv; ++calls; *ppv = this; return riid.Data1 == 7 ? 0 : (long)0x80004002L; }
    unsigned int AddRef() { return 2; }
    unsigned int Release() { return 1; }
};
class CUnknownModel {
    IUnknownT* m_pUnk;
public:
    CUnknownModel(IUnknownT* pUnk) : m_pUnk(pUnk) {}
    IUnknownT* GetOwner() const { return m_pUnk; }
};
class CBaseVideoRendererModel : public CUnknownModel {
public:
    CBaseVideoRendererModel(IUnknownT* pUnk) : CUnknownModel(pUnk) {}
    long QueryInterface(const GUIDT& riid, void** ppv) { return GetOwner()->QueryInterface(riid, ppv); }
};
int main()
{
    Outer outer;
    CBaseVideoRendererModel r(&outer);
    GUIDT yes = { 7, 0, 0, { 0 } }, no = { 9, 0, 0, { 0 } };
    void* out = 0;
    long hr = r.QueryInterface(yes, &out);
    if (hr != 0 || out != (void*)&outer || outer.lastRiid != &yes || outer.lastPpv != &out || outer.calls != 1) { std::printf("00a3bd00_TEST FAIL\n"); return 1; }
    hr = r.QueryInterface(no, &out);
    if (hr != (long)0x80004002L || outer.lastRiid != &no || outer.calls != 2) { std::printf("00a3bd00_TEST FAIL\n"); return 1; }
    std::printf("00a3bd00_TEST PASS\n");
    return 0;
}