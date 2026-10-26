#include <cstdio>
static int g_destroyed = 0;
class CThingAICreature { public:
    long Property;
    virtual long GetAIProperty() const { return Property; }   // slot 48 in retail
    virtual ~CThingAICreature() {}
};
struct CBaseIntelligentPointer {
    void* VTable;
    void* PItem;
    CBaseIntelligentPointer(void* item) : VTable(0), PItem(item) {}
    CBaseIntelligentPointer(const CBaseIntelligentPointer& other) : VTable(other.VTable), PItem(other.PItem) {}
    ~CBaseIntelligentPointer() { ++g_destroyed; }
    void* GetPItem() const { return PItem; }
};
template <class T> class CIntelligentPointer : public CBaseIntelligentPointer { public:
    CIntelligentPointer(T* item) : CBaseIntelligentPointer(item) {}
    T* GetPItem() const { return (T*)CBaseIntelligentPointer::GetPItem(); }
};
struct HasGuardProperty {
    bool operator()(CIntelligentPointer<CThingAICreature> creature) const;
};
bool HasGuardProperty::operator()(CIntelligentPointer<CThingAICreature> creature) const
{
    return creature.GetPItem()->GetAIProperty() == 3;
}
int main()
{
    CThingAICreature guard, other;
    guard.Property = 3; other.Property = 2;
    CIntelligentPointer<CThingAICreature> pg(&guard), po(&other);
    HasGuardProperty f;
    int bad = 0;
    if (!f(pg)) bad = 1;
    if (f(po)) bad = 1;
    if (g_destroyed != 2) bad = 1;
    if (bad) { std::printf("0065f152_TEST FAIL\n"); return 1; }
    std::printf("0065f152_TEST PASS\n");
    return 0;
}