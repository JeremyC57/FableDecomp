// Behaviour test for 0064b5c1: body copied from src/0064b5c1.cpp (NPlayerGui::CDrawQuickAccessIcons::GetActivationTimeForButton), the callees stubbed.
#include <cstdio>
#include <cstring>
static int fails = 0;
#define CHECK(c) do { if (!(c)) { ++fails; printf("FAIL %d: %s\n", __LINE__, #c); } } while (0)
class CTCBase;
struct CPresenceBits {
    unsigned long w[9];
    bool test(long bit) { return (w[bit >> 5] & (1UL << (bit & 31))) != 0; }
};
struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type) const { SEntry* it = first; while (it != last && it->type < *type) ++it; return it; }
};
class CThing { public:
    unsigned char _pad0[0x20];
    CPresenceBits m_present;
    CInterfaceMap m_interfaces;
    bool alive;
    bool HasInterface(long type) { return m_present.test(type); }
    template <class T> __forceinline T* GetInterfaceUnchecked(long type)
    {
        long key = type;
        CInterfaceMap* map = &m_interfaces;
        CInterfaceMap::SEntry* it = map->LowerBound(&key);
        return (T*)((it == map->last || type < it->type) ? map->last : it)->component;
    }
};
template <class T> bool __fastcall IS_THING_ALIVE(const T& thing) { return thing != 0 && thing->alive; }
class CThingCreatureBase : public CThing { public: bool IsChild() { return false; } };
class CPlayer { public: CThingCreatureBase* creature; CThingCreatureBase* GetControlledCreature() { return creature; } };
class CPlayerManager { public: CPlayer player; CPlayer* GetMainPlayer() { return &player; } };
struct CRefCounted {
    long m_count;
    void DecRef() { --m_count; }
    long GetRefCount() const { return m_count; }
};
static int g_destroyCalls = 0;
struct CDefVirtual {
    virtual void v00() {}
    virtual void Destroy() { ++g_destroyCalls; }
};
class CInventoryItemDef : public CDefVirtual, public CRefCounted { public:
    unsigned char _def[0x40];
    float ActivationTime;
    void ReleaseTemporary()
    {
        DecRef();
        if (GetRefCount() == 0)
            Destroy();
    }
};
struct CDefPointer {
    CInventoryItemDef* m_p;
    CDefPointer() : m_p(0) {}
    CDefPointer(const CDefPointer& o) : m_p(o.m_p) { if (m_p) ++m_p->m_count; }   // test only: keeps the count balanced whether or not the by-value return is elided
    ~CDefPointer()
    {
        if (m_p)
            m_p->ReleaseTemporary();
    }
    CInventoryItemDef* operator->() const { return m_p; }
};
static CInventoryItemDef* g_defToGive = 0; static long g_slotSeen = -99; static int g_defCalls = 0;
class CTCInventory { public:
    CDefPointer GetQuickAccessItemDef(long slot) { ++g_defCalls; g_slotSeen = slot; CDefPointer p; p.m_p = g_defToGive; if (p.m_p) ++p.m_p->m_count; return p; }
};
__forceinline CTCInventory* GetInventory(CThing* thing)
{
    CTCInventory* inventory = 0;
    if (thing->HasInterface(0x11))
        inventory = thing->GetInterfaceUnchecked<CTCInventory>(0x11);
    return inventory;
}
namespace NPlayerGui {
class CDrawQuickAccessIcons { public:
    unsigned char _pad[8];
    CPlayerManager* PlayerManager;
    float GetActivationTimeForButton(long button);
};
float CDrawQuickAccessIcons::GetActivationTimeForButton(long button)
{
    CThingCreatureBase* hero = PlayerManager->GetMainPlayer()->GetControlledCreature();
    if (IS_THING_ALIVE<CThing*>(hero)) {
        if (button == -2)
            return 2.0f;
        CDefPointer def = GetInventory(hero)->GetQuickAccessItemDef(button);
        if (def.m_p) {
            float time = def->ActivationTime;
            return time;
        }
    }
    return 0.0f;
}
}
int main()
{
    CThingCreatureBase hero; memset(&hero, 0, sizeof(hero)); CTCInventory inv;
    CInterfaceMap::SEntry entries[2] = { { 0x10, (CTCBase*)0x99 }, { 0x11, (CTCBase*)&inv } };
    hero.m_interfaces.first = entries; hero.m_interfaces.last = entries + 2; hero.m_present.w[0x11 >> 5] = 1UL << (0x11 & 31);
    CPlayerManager pm; pm.player.creature = &hero;
    NPlayerGui::CDrawQuickAccessIcons d; d.PlayerManager = &pm;
    // 1. hero dead / absent: 0, no lookup
    hero.alive = false; CHECK(d.GetActivationTimeForButton(1) == 0.0f); CHECK(g_defCalls == 0);
    pm.player.creature = 0; CHECK(d.GetActivationTimeForButton(1) == 0.0f); CHECK(g_defCalls == 0);
    pm.player.creature = &hero; hero.alive = true;
    // 2. the special button: 2.0 without a lookup
    CHECK(d.GetActivationTimeForButton(-2) == 2.0f); CHECK(g_defCalls == 0);
    // 3. no item in the slot: 0
    g_defToGive = 0; CHECK(d.GetActivationTimeForButton(3) == 0.0f); CHECK(g_defCalls == 1); CHECK(g_slotSeen == 3);
    // 4. an item: its activation time; the temporary def pointer is released (ref count back, destroyed at zero)
    CInventoryItemDef def; def.m_count = 0; def.ActivationTime = 1.25f; g_defToGive = &def;
    CHECK(d.GetActivationTimeForButton(0) == 1.25f); CHECK(g_slotSeen == 0); CHECK(def.m_count == 0); CHECK(g_destroyCalls == 1);
    def.m_count = 1; CHECK(d.GetActivationTimeForButton(2) == 1.25f); CHECK(def.m_count == 1); CHECK(g_destroyCalls == 1);
    if (fails == 0) printf("0064b5c1_TEST PASS\n"); else printf("0064b5c1_TEST FAIL\n");
    return fails;
}