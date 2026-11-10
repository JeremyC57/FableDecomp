// Behaviour test for NHeroStatModifierEvent::CBase::~CBase (0x0083AD90): the non-virtual destructor of the abstract event
// base only resets the vtable pointer; a derived event dispatches ModifyHeroStats through it and is destroyed normally.
#include <cstdio>
class CTCHeroStats { public: int changes; };
namespace NHeroStatModifierEvent {
class CBase {
public:
    ~CBase() { }
    virtual void ModifyHeroStats(CTCHeroStats& stats) const = 0;
};
class CKilledThing : public CBase {
public:
    virtual void ModifyHeroStats(CTCHeroStats& stats) const { stats.changes++; }
};
}
int main()
{
    CTCHeroStats stats = { 0 };
    {
        NHeroStatModifierEvent::CKilledThing e;
        const NHeroStatModifierEvent::CBase& base = e;
        base.ModifyHeroStats(stats);
    }
    bool ok = stats.changes == 1 && sizeof(NHeroStatModifierEvent::CBase) == sizeof(void*);
    if (!ok) { std::printf("0083ad90_TEST FAIL\n"); return 1; }
    std::printf("0083ad90_TEST PASS\n");
    return 0;
}