// Behaviour test for CDynamicOutputPin::Release 0x00CA9710: the IPin Release delegates to the owner's Release (GetOwner()->Release())
#include <cstdio>
struct IUnknownT {
    virtual long QueryInterface() = 0;
    virtual unsigned long AddRef() = 0;
    virtual unsigned long Release() = 0;
};
struct Owner : IUnknownT {
    unsigned long refs;
    Owner() : refs(3) {}
    long QueryInterface() { return 0; }
    unsigned long AddRef() { return ++refs; }
    unsigned long Release() { return --refs; }
};
class CUnknownT {
    IUnknownT* const m_pUnknown;
public:
    CUnknownT(IUnknownT* pUnk) : m_pUnknown(pUnk) {}
    IUnknownT* GetOwner() const { return m_pUnknown; }
};
class CDynamicOutputPinT : public CUnknownT {
public:
    CDynamicOutputPinT(IUnknownT* pUnk) : CUnknownT(pUnk) {}
    unsigned long Release() { return GetOwner()->Release(); }
};
int main()
{
    Owner owner;
    CDynamicOutputPinT pin(&owner);
    if (pin.Release() != 2 || owner.refs != 2) { std::printf("00ca9710_TEST FAIL\n"); return 1; }
    if (pin.Release() != 1 || pin.Release() != 0) { std::printf("00ca9710_TEST FAIL\n"); return 1; }
    std::printf("00ca9710_TEST PASS\n");
    return 0;
}