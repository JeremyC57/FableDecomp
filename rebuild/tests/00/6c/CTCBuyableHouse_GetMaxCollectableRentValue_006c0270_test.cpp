// Behaviour test for 006c0270 CTCBuyableHouse::GetMaxCollectableRentValue: (long)(trophies * 0.1 + Rent[level]) * MaxRentBags, with the def reference count balanced.
#include <cstdio>

static int g_destroyed = 0;

class CBuyableHouseDef
{ public:
    long  m_refCount;
    long* Rent_First;
    long  MaxRentBags;
    virtual void Destroy() { ++g_destroyed; }
    virtual ~CBuyableHouseDef() {}
    void AddRef() { ++m_refCount; }
    void Release() { if (--m_refCount == 0) Destroy(); }
};

struct CDefRef
{
    CBuyableHouseDef* m_p;
    CDefRef(CBuyableHouseDef* p) : m_p(p) { if (m_p) m_p->AddRef(); }
    ~CDefRef() { m_p->Release(); }
    CBuyableHouseDef* operator->() const { return m_p; }
};

struct CDefPointer
{
    CBuyableHouseDef* Object;
    CDefRef operator->() const { return CDefRef(Object); }
};

class CTCBuyableHouse
{ public:
    CDefPointer PDef;
    long        CurrentDressLevel;
    long        m_trophies;

    long GetValueOfTrophies() const { return m_trophies; }
    long GetMaxCollectableRentValue() const
    {
        const long* rents = PDef.Object->Rent_First;
        long rent = (long)(GetValueOfTrophies() * 0.1f + rents[CurrentDressLevel]);
        return PDef->MaxRentBags * rent;
    }
};

int main()
{
    long rents[2] = { 40, 90 };
    CBuyableHouseDef def; def.m_refCount = 1; def.Rent_First = rents; def.MaxRentBags = 3;
    CTCBuyableHouse house; house.PDef.Object = &def; house.CurrentDressLevel = 1; house.m_trophies = 105;
    long v = house.GetMaxCollectableRentValue();     // (long)(10.5 + 90) = 100, * 3
    if (v != 300 || def.m_refCount != 1 || g_destroyed != 0) { std::printf("006c0270_TEST FAIL\n"); return 1; }
    house.CurrentDressLevel = 0; house.m_trophies = 0;
    if (house.GetMaxCollectableRentValue() != 120) { std::printf("006c0270_TEST FAIL\n"); return 1; }
    std::printf("006c0270_TEST PASS\n");
    return 0;
}