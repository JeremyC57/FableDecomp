// Behaviour test for CBaseAllocator::QueryInterface (0x00CAA470): the interface's QueryInterface delegates to the owner's IUnknown with the same arguments.
#include <stdio.h>
struct GUIDish { int v; };
struct IUnknownT {
    virtual long QueryInterface(const GUIDish& riid, void** ppv) = 0;
    virtual ~IUnknownT() { }
};
struct Owner : IUnknownT {
    const GUIDish* seenIid; void** seenOut; int calls;
    Owner() : seenIid(0), seenOut(0), calls(0) { }
    virtual long QueryInterface(const GUIDish& riid, void** ppv) { seenIid = &riid; seenOut = ppv; ++calls; *ppv = this; return 0x1234; }
};
struct CUnknownT {
    IUnknownT* m_pUnknown;
    IUnknownT* GetOwner() const { return m_pUnknown; }
    virtual ~CUnknownT() { }
};
struct IMemAllocatorCallbackTemp : IUnknownT { };
struct CBaseAllocator : CUnknownT, IMemAllocatorCallbackTemp {
    virtual long QueryInterface(const GUIDish& riid, void** ppv);
};
long CBaseAllocator::QueryInterface(const GUIDish& riid, void** ppv)
{
    return GetOwner()->QueryInterface(riid, ppv);
}
int main(void)
{
    Owner owner; CBaseAllocator object; object.m_pUnknown = &owner;
    GUIDish iid; iid.v = 7; void* out = 0;
    IMemAllocatorCallbackTemp* iface = &object;
    long hr = iface->QueryInterface(iid, &out);
    if (hr != 0x1234 || owner.calls != 1 || owner.seenIid != &iid || owner.seenOut != &out || out != &owner) { printf("00caa470_TEST FAIL\n"); return 1; }
    printf("00caa470_TEST PASS\n");
    return 0;
}