#include <cstdio>
// Behaviour of CUnknown::CUnknown (0x00CACB80): the live-object count goes up by one, the owner is the outer
// IUnknown when one is given and the object's own non-delegating interface otherwise, the count starts at 0.
static long g_objects = 0;
static long TestInterlockedIncrement(long volatile* addend) { return ++*addend; }
struct GUID { unsigned long d[4]; };
struct IUnknown {
    virtual long QueryInterface(const GUID&, void**) = 0;
    virtual unsigned long AddRef() = 0;
    virtual unsigned long Release() = 0;
};
struct INonDelegatingUnknown {
    virtual long NonDelegatingQueryInterface(const GUID&, void**) = 0;
    virtual unsigned long NonDelegatingAddRef() = 0;
    virtual unsigned long NonDelegatingRelease() = 0;
};
class CBaseObject {
public:
    CBaseObject(const char*) { TestInterlockedIncrement(&g_objects); }
};
class CUnknown : public INonDelegatingUnknown, public CBaseObject {
public:
    IUnknown* const m_pUnknown;
    volatile long m_cRef;
    CUnknown(const char* pName, IUnknown* pUnk)
        : CBaseObject(pName),
          m_pUnknown(pUnk != 0 ? pUnk : reinterpret_cast<IUnknown*>(static_cast<INonDelegatingUnknown*>(this))),
          m_cRef(0)
    {
    }
    long NonDelegatingQueryInterface(const GUID&, void**) { return 0; }
    unsigned long NonDelegatingAddRef() { return 0; }
    unsigned long NonDelegatingRelease() { return 0; }
};
struct COuter : IUnknown {
    long QueryInterface(const GUID&, void**) { return 0; }
    unsigned long AddRef() { return 1; }
    unsigned long Release() { return 0; }
};
int main()
{
    COuter outer;
    CUnknown inner("inner", &outer);
    CUnknown alone("alone", 0);
    if (g_objects != 2 || inner.m_pUnknown != &outer || inner.m_cRef != 0 || alone.m_cRef != 0 ||
        (void*)alone.m_pUnknown != (void*)static_cast<INonDelegatingUnknown*>(&alone)) { std::printf("00cacb80_TEST FAIL\n"); return 1; }
    std::printf("00cacb80_TEST PASS\n");
    return 0;
}