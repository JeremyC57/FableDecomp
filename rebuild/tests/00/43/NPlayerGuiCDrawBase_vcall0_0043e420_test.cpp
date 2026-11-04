// Behaviour test for the __fastcall vcall{0} thunk 0x0043E420: calling through a pointer to the slot-0 virtual
// (&CDrawBase::Clear, as CPlayerGui::ClearGui's for_each(mem_fun(...)) does) must dispatch to each object's own override.
#include <cstdio>
namespace NPlayerGui {
struct CDrawBase { int cleared; CDrawBase() : cleared(0) {} virtual void Clear() { cleared = 1; } virtual void PrepareForLoad() {} virtual ~CDrawBase() {} };
struct CDrawNPCName : CDrawBase { void Clear() { cleared = 2; } };
}
int main()
{
    using namespace NPlayerGui;
    CDrawBase base; CDrawNPCName name;
    CDrawBase* draws[2] = { &base, &name };
    void (CDrawBase::*clear)() = &CDrawBase::Clear;
    for (int i = 0; i < 2; ++i) (draws[i]->*clear)();
    if (base.cleared != 1 || name.cleared != 2) { std::printf("0043e420_TEST FAIL\n"); return 1; }
    std::printf("0043e420_TEST PASS\n");
    return 0;
}