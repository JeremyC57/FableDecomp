#include <cstdio>
// CDynamicOutputPin::AddRef (DECLARE_IUNKNOWN): delegate to the owner unknown's AddRef.
struct Unk { int refs; virtual unsigned AddRef() { return ++refs; } };
struct Pin {
    Unk* m_pUnknown;
    Unk* GetOwner() const { return m_pUnknown; }
    unsigned AddRef() { return GetOwner()->AddRef(); }
};
int main()
{
    Unk outer; outer.refs = 3; Pin p; p.m_pUnknown = &outer;
    if (p.AddRef() != 4 || outer.refs != 4 || p.AddRef() != 5) { std::printf("00ca9700_TEST FAIL\n"); return 1; }
    std::printf("00ca9700_TEST PASS\n");
    return 0;
}