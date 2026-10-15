// CThing::AddToRenderList(long context, long a, long b, long c) -- (propagated label) when the object's virtual availability test (slot 4) passes, forward three arguments plus its handle (+0x50) to the global service @ 0x0143BB70; else return -9
// Retail: Fable.exe (TLC, VC7.1) @ 0x00C597E0, 55 bytes. Callees: virtual slot 4 (+0x10) on `this`, the service
// method @ 0x00C51F00 (manifest "Model_WritePropertyIfValid", __fastcall on the object @ 0x0143BB70).
// Identity: the catalog's "CThing::AddToRenderList" is propagated; the body sits in the 0x00C5xxxx no-RTTI module
// and is referenced from five vtables (0x012B5C4C, 0x012B5C9C, 0x012B5D0C, 0x012B5DAC, 0x012B8B1C), so it is
// a shared COM-style wrapper method; modelled locally.
// Shape: `ret 10h` = four stack arguments, the first unused; the forwarded call takes (a, b, c, handle), pushed
// right to left with the member loaded first. -9 (0xFFFFFFF7) is the failure code.
struct CService {
    long Forward(long a, long b, long c, long handle);   // 0x00C51F00
};

extern CService* g_pService;                         // 0x0143BB70

struct CThing {
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03();
    virtual bool IsValid();                          // slot 4 (+0x10)
    unsigned char _pad04[0x50 - 0x04];
    long          m_handle;                          // +0x50
    long AddToRenderList(long context, long a, long b, long c);
};

long CThing::AddToRenderList(long context, long a, long b, long c)
{
    if (IsValid())
        return g_pService->Forward(a, b, c, m_handle);
    return -9;
}