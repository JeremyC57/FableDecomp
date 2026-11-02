// Behaviour test for 0x007CA630 CTCHeroReceiveItems::OnKill(): every item still alive in Items is killed (immediately), dead
// or empty entries are skipped, then CTCBase::OnKill runs once.
#include <cstdio>
#include <list>
#include <utility>

static int g_baseOnKill = 0;

class CThing {
public:
    CThing() : Dead(0), Kills(0) { }
    unsigned char Dead : 1;
    int Kills;
    bool IsDead() const { return Dead; }
    void Kill(bool immediately) { if (immediately) ++Kills; }
};
class CThingObject : public CThing { };

template <class T> class CIntelligentPointer {
public:
    CIntelligentPointer() : Item(0) { }
    CIntelligentPointer(T* t) : Item(t) { }
    T* Get() const { return Item; }
    T* operator->() const { return Item; }
    T& operator*() const { return *Item; }
private:
    T* Item;
};

template <class T> inline bool IS_THING_ALIVE(const T& thing)
{
    return &*thing && !thing->IsDead();
}

class CTCBase {
public:
    virtual ~CTCBase() { }
    virtual void OnKill() { ++g_baseOnKill; }
};

class CTCHeroReceiveItems : public CTCBase {
public:
    virtual void OnKill();
    std::list<std::pair<CIntelligentPointer<CThingObject>, bool> > Items;
};

void CTCHeroReceiveItems::OnKill()
{
    for (std::list<std::pair<CIntelligentPointer<CThingObject>, bool> >::iterator it = Items.begin(); it != Items.end(); ++it) {
        CThingObject* item = it->first.Get();
        if (IS_THING_ALIVE(item))
            item->Kill(true);
    }
    CTCBase::OnKill();
}

int main()
{
    CThingObject a, b, c;
    c.Dead = 1;
    CTCHeroReceiveItems r;
    r.OnKill();                                               // empty list
    if (g_baseOnKill != 1) { std::printf("007ca630_TEST FAIL\n"); return 1; }
    r.Items.push_back(std::make_pair(CIntelligentPointer<CThingObject>(&a), true));
    r.Items.push_back(std::make_pair(CIntelligentPointer<CThingObject>(), false));
    r.Items.push_back(std::make_pair(CIntelligentPointer<CThingObject>(&c), true));
    r.Items.push_back(std::make_pair(CIntelligentPointer<CThingObject>(&b), false));
    r.OnKill();
    if (a.Kills != 1 || b.Kills != 1 || c.Kills != 0 || g_baseOnKill != 2) { std::printf("007ca630_TEST FAIL\n"); return 1; }
    std::printf("007ca630_TEST PASS\n");
    return 0;
}