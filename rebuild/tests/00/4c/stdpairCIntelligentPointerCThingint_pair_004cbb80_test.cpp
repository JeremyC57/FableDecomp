// Behaviour test for 0x004CBB80 pair<CIntelligentPointer<CThing>, int>(a, b): the first member is a fresh intelligent
// pointer set to a's thing through SetPItem (so the thing gains a watcher) and the second is b.
#include <cstdio>

class CThing { public: int watchers; };
struct CBaseIP {
    CThing* item;
    CBaseIP() : item(0) {}
    CThing* GetPItem() const { return item; }
    void SetPItem(CThing* t) { if (item) --item->watchers; item = t; if (t) ++t->watchers; }
};
struct CIP : CBaseIP { CIP() {} CIP(const CIP& o) : CBaseIP() { SetPItem(o.GetPItem()); } };
struct Pair { CIP first; int second; Pair(const CIP& a, const int& b) : first(a), second(b) {} };

int main()
{
    CThing t = { 0 };
    CIP a; a.SetPItem(&t);
    Pair p(a, 42);
    if (p.first.GetPItem() != &t || p.second != 42 || t.watchers != 2) { std::printf("004cbb80_TEST FAIL\n"); return 1; }
    CIP none;
    Pair q(none, -1);
    if (q.first.GetPItem() != 0 || q.second != -1 || t.watchers != 2) { std::printf("004cbb80_TEST FAIL\n"); return 1; }
    std::printf("004cbb80_TEST PASS\n");
    return 0;
}