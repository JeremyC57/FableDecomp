#include <stdio.h>
class CTCHeroStats;
static int g_calls; static CTCHeroStats* g_stats; static const void* g_ev;
namespace NHeroStatModifierEvent {
class CBase { public: virtual void ApplyTo(CTCHeroStats* stats) const = 0; };
class CKnockedThingUnconscious : public CBase {
public: virtual void ApplyTo(CTCHeroStats* stats) const { ++g_calls; g_stats = stats; g_ev = this; }
};
}
class CTCHeroStats {
public:
    int x;
    void InformOfEvent(const NHeroStatModifierEvent::CBase& event);
};

void CTCHeroStats::InformOfEvent(const NHeroStatModifierEvent::CBase& event)
{
    event.ApplyTo(this);
}

int main()
{
    CTCHeroStats s; NHeroStatModifierEvent::CKnockedThingUnconscious e;
    s.InformOfEvent(e);
    bool ok = g_calls == 1 && g_stats == &s && g_ev == &e;
    printf(ok ? "0057a56c_TEST PASS\n" : "0057a56c_TEST FAIL\n");
    return ok ? 0 : 1;
}