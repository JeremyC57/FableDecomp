// Behaviour test for 0x00981ea0: the six-argument speech action forwards every argument (pointers copied by value,
// the copies released after the call) and appends `true`.
#include <cstdio>
class CBaseObject { public: int refs; };
struct CBaseIntelligentPointer {
    CBaseObject* m_pItem;
    CBaseIntelligentPointer() : m_pItem(0) {}
    ~CBaseIntelligentPointer() { SetPItem(0); }
    CBaseObject* GetPItem() const { return m_pItem; }
    void SetPItem(CBaseObject* item) { if (m_pItem) m_pItem->refs--; m_pItem = item; if (item) item->refs++; }
};
struct CIP : CBaseIntelligentPointer {
    CIP() {}
    CIP(const CIP& other) : CBaseIntelligentPointer() { SetPItem(other.GetPItem()); }
};
struct CGroup {
    long seen[7]; CBaseObject* who[2]; int refsDuring;
    long Target(long repeat, CIP speaker, CIP target, unsigned long speech, bool interrupt, long priority, bool wait)
    {
        seen[0] = repeat; seen[1] = (long)speech; seen[2] = interrupt; seen[3] = priority; seen[4] = wait;
        who[0] = speaker.GetPItem(); who[1] = target.GetPItem(); refsDuring = who[0]->refs;
        return 9;
    }
    long Speak(long repeat, CIP speaker, CIP target, unsigned long speech, bool interrupt, long priority)
    {
        return Target(repeat, speaker, target, speech, interrupt, priority, true);
    }
};
int main()
{
    CBaseObject a = { 0 }, b = { 0 };
    CIP pa; pa.SetPItem(&a); CIP pb; pb.SetPItem(&b);
    CGroup g;
    long r = g.Speak(2, pa, pb, 0x1234, false, 5);
    if (r != 9 || g.seen[0] != 2 || g.seen[1] != 0x1234 || g.seen[2] != 0 || g.seen[3] != 5 || g.seen[4] != 1
        || g.who[0] != &a || g.who[1] != &b || g.refsDuring < 2 || a.refs != 1 || b.refs != 1) {
        std::printf("00981ea0_TEST FAIL\n"); return 1;
    }
    std::printf("00981ea0_TEST PASS\n");
    return 0;
}