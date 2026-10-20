// behaviour test for 007de5d0 CTCWallMount::OnInitialActivate: with a saved UID the trophy pointer is re-linked; a live
// trophy has its physics switched off, its interface-0x82 component cleared and removed, and its targetability cleared
#include <cstdio>
#ifdef _MSC_VER
typedef __int64 int64_t_;
typedef unsigned __int64 uint64_t_;
#define UID_5 0x100000005ui64
#else
typedef long long int64_t_;
typedef unsigned long long uint64_t_;
#define UID_5 0x100000005ULL
#endif
class CThing;
struct CTCBase;
struct CBaseObject;
struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type) { SEntry* it = first; while (it != last && it->type < *type) ++it; return it; }
};
struct CTCPhysicsBase {
    bool active;
    virtual void SetPhysicsActive(bool a) { active = a; }
};
static long g_removed = -1;
class CThing {
public:
    unsigned long   m_presentBits[9];
    CInterfaceMap   m_interfaces;
    CTCPhysicsBase* PhysicsTC;
    unsigned char   m_flags91;
    unsigned long HasTC(long type) const { return m_presentBits[type >> 5] & (1UL << (type & 31)); }
    CTCBase* GetTC(long interfaceType)
    {
        long type = interfaceType;
        CInterfaceMap* map = &m_interfaces;
        CInterfaceMap::SEntry* it = map->LowerBound(&type);
        return (it == map->last || interfaceType < it->type ? map->last : it)->component;
    }
    void RemoveTC(long interfaceType) { g_removed = interfaceType; m_presentBits[interfaceType >> 5] &= ~(1UL << (interfaceType & 31)); }
    CTCPhysicsBase* GetPhysicsTC() const { return PhysicsTC; }
};
static CThing* g_lookup = 0; static int64_t_ g_askedUID = 0;
struct CThingManager { CThing* GetThingWithUID(int64_t_ uid) { g_askedUID = uid; return g_lookup; } };
static CThingManager g_manager;
struct CBaseIntelligentPointer {
    virtual void v00() {}
    CThing* m_pItem;
    CBaseIntelligentPointer() : m_pItem(0) {}
    ~CBaseIntelligentPointer() {}
    CThing* GetPItem() const { return m_pItem; }
    void SetPItem(CThing* item) { m_pItem = item; }
};
template<class T> struct CIntelligentPointer : CBaseIntelligentPointer {
    virtual void v00() {}
    CIntelligentPointer(T* item) { SetPItem(item); }
    CIntelligentPointer& operator=(const CIntelligentPointer& other)
    {
        CThing* item = other.GetPItem();
        SetPItem(item);
        return *this;
    }
};
inline bool IS_THING_ALIVE(const CBaseIntelligentPointer& p)
{
    if (p.GetPItem() != 0) {
        if (!(p.GetPItem()->m_flags91 & 1))
            return true;
    }
    return false;
}
static int g_cleared = 0;
struct CTCComponent82 { void ClearEntries() { ++g_cleared; } };
struct CTCTargeted { bool Targetable; };
struct CTCBaseModel {
    virtual void v00() {}
    CThing* ParentThing;
    CThingManager* GetThingManager() { return &g_manager; }
};
struct CTCWallMount : CTCBaseModel {
    long                        BoughtForAmount;
    CIntelligentPointer<CThing> Trophy;
    uint64_t_                   TrophyID;
    CTCWallMount() : Trophy(0) {}
    virtual void OnInitialActivate();
};
void CTCWallMount::OnInitialActivate()
{
    if (TrophyID == 0)
        return;
    Trophy = CIntelligentPointer<CThing>(GetThingManager()->GetThingWithUID(TrophyID));
    if (IS_THING_ALIVE(Trophy)) {
        if (Trophy.GetPItem()->GetPhysicsTC())
            Trophy.GetPItem()->GetPhysicsTC()->SetPhysicsActive(false);
        CThing* trophy = Trophy.GetPItem();
        if (trophy->HasTC(0x82)) {
            ((CTCComponent82*)trophy->GetTC(0x82))->ClearEntries();
            Trophy.GetPItem()->RemoveTC(0x82);
        }
        CThing* target = Trophy.GetPItem();
        if (target->HasTC(9))
            ((CTCTargeted*)target->GetTC(9))->Targetable = false;
    }
}
int main()
{
    CTCPhysicsBase physics; physics.active = true;
    CTCComponent82 comp; CTCTargeted targeted; targeted.Targetable = true;
    CInterfaceMap::SEntry entries[2] = { { 9, (CTCBase*)&targeted }, { 0x82, (CTCBase*)&comp } };
    CThing trophy; for (int i = 0; i < 9; ++i) trophy.m_presentBits[i] = 0;
    trophy.m_presentBits[0] = 1UL << 9; trophy.m_presentBits[4] = 1UL << 2;
    trophy.m_interfaces.first = entries; trophy.m_interfaces.last = entries + 2; trophy.PhysicsTC = &physics; trophy.m_flags91 = 0;
    CTCWallMount mount; mount.TrophyID = 0;
    mount.OnInitialActivate();
    bool ok = mount.Trophy.GetPItem() == 0 && g_askedUID == 0;
    g_lookup = &trophy; mount.TrophyID = UID_5;
    mount.OnInitialActivate();
    ok = ok && g_askedUID == (int64_t_)UID_5 && mount.Trophy.GetPItem() == &trophy && !physics.active &&
         g_cleared == 1 && g_removed == 0x82 && !targeted.Targetable;
    if (!ok) { std::printf("007de5d0_TEST FAIL\n"); return 1; }
    std::printf("007de5d0_TEST PASS\n");
    return 0;
}
