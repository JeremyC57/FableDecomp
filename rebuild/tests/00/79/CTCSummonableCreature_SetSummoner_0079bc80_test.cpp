// Behaviour test for 0x0079BC80 CTCSummonableCreature::SetSummoner(CThingCreatureBase*): the summoner pointer is stored through
// a temporary intelligent pointer and the temporary is released afterwards.
#include <cstdio>

class CBaseObject { public: int Id; };
class CThingCreatureBase : public CBaseObject { };

static int g_live = 0;
class CBaseIntelligentPointer {
public:
    CBaseIntelligentPointer() : PItem(0) { ++g_live; }
    virtual ~CBaseIntelligentPointer() { --g_live; }
    CBaseObject* GetPItem() const { return PItem; }
    void SetPItem(CBaseObject* item) { PItem = item; }
private:
    CBaseObject* PItem;
};
template <class T> class CIntelligentPointer : public CBaseIntelligentPointer {
public:
    CIntelligentPointer() { }
    CIntelligentPointer(T* item) { SetPItem((CBaseObject*)item); }
    CIntelligentPointer& operator=(const CIntelligentPointer& other)
    {
        SetPItem(other.CBaseIntelligentPointer::GetPItem());
        return *this;
    }
    T* Get() const { return (T*)CBaseIntelligentPointer::GetPItem(); }
};
class CTCSummonableCreature {
public:
    void SetSummoner(CThingCreatureBase* summoner) { Summoner = summoner; }
    bool HasBeenSummoned;
    CIntelligentPointer<CThingCreatureBase> Summoner;
};

int main()
{
    CThingCreatureBase hero;
    CThingCreatureBase other;
    CTCSummonableCreature creature;
    creature.HasBeenSummoned = true;
    int before = g_live;
    creature.SetSummoner(&hero);
    if (creature.Summoner.Get() != &hero || g_live != before || !creature.HasBeenSummoned) {
        std::printf("0079bc80_TEST FAIL\n");
        return 1;
    }
    creature.SetSummoner(&other);
    creature.SetSummoner(0);
    if (creature.Summoner.Get() != 0 || g_live != before) {
        std::printf("0079bc80_TEST FAIL\n");
        return 1;
    }
    std::printf("0079bc80_TEST PASS\n");
    return 0;
}