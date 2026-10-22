#include <cstdio>
// Behaviour test for 00ca53f0: the pin's IUnknown::AddRef delegates to the owner's AddRef (DECLARE_IUNKNOWN).
struct IUnknownLike { virtual unsigned long AddRef() = 0; };
struct Owner : IUnknownLike { unsigned long refs; unsigned long AddRef() { return ++refs; } };
struct Pin { IUnknownLike* m_pUnknown; IUnknownLike* GetOwner() const { return m_pUnknown; }
    unsigned long AddRef() { return GetOwner()->AddRef(); } };
int main()
{
    Owner o; o.refs = 1;
    Pin p; p.m_pUnknown = &o;
    int ok = (p.AddRef() == 2 && p.AddRef() == 3 && o.refs == 3);
    if (!ok) { std::printf("00ca53f0_TEST FAIL\n"); return 1; }
    std::printf("00ca53f0_TEST PASS\n");
    return 0;
}