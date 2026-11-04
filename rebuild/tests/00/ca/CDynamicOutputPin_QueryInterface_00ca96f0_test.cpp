// Behaviour test for CDynamicOutputPin::QueryInterface 0x00CA96F0 (DECLARE_IUNKNOWN): the pin's IUnknown::QueryInterface is delegated
// to the owning outer unknown with the same riid / ppv, and its HRESULT is returned.
#include <cstdio>
struct GUID { unsigned long a; unsigned short b, c; unsigned char d[8]; };
struct IUnknown { virtual long QueryInterface(const GUID& riid, void** ppv) = 0; virtual unsigned long AddRef() = 0; virtual unsigned long Release() = 0; };
struct COuter : IUnknown { const GUID* Seen; void** SeenPpv; COuter() : Seen(0), SeenPpv(0) {}
    long QueryInterface(const GUID& riid, void** ppv) { Seen = &riid; SeenPpv = ppv; *ppv = this; return 0x1234; }
    unsigned long AddRef() { return 2; } unsigned long Release() { return 1; } };
struct CDynamicOutputPin : IUnknown { IUnknown* m_pUnknown; CDynamicOutputPin(IUnknown* o) : m_pUnknown(o) {}
    IUnknown* GetOwner() const { return m_pUnknown; }
    long QueryInterface(const GUID& riid, void** ppv) { return GetOwner()->QueryInterface(riid, ppv); }
    unsigned long AddRef() { return GetOwner()->AddRef(); } unsigned long Release() { return GetOwner()->Release(); } };
int main()
{
    COuter outer; CDynamicOutputPin pin(&outer); IUnknown& u = pin; GUID g = { 1, 2, 3, { 0 } }; void* out = 0;
    long hr = u.QueryInterface(g, &out);
    if (hr != 0x1234 || outer.Seen != &g || outer.SeenPpv != &out || out != (IUnknown*)&outer) { std::printf("00ca96f0_TEST FAIL\n"); return 1; }
    std::printf("00ca96f0_TEST PASS\n");
    return 0;
}