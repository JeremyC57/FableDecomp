// Behaviour test for 0x008116d0 CEventShownItem::~CEventShownItem
#include <cstdio>
static int g_shownEnded = 0, g_pointerDtors = 0;
struct CTCShowableItem { void OnShowEnded() { ++g_shownEnded; } };
class CThing { public:
    unsigned int m_presentBits[9]; bool m_dead; CTCShowableItem* m_pShowable;
    bool GetInterface(CTCShowableItem** out, long type)
    {
        if (m_presentBits[type >> 5] & (1U << (type & 31))) { *out = m_pShowable; return true; }
        return false;
    }
};
struct CBaseIntelligentPointer {
    CThing* m_pItem;
    CThing* GetPItem() const { return m_pItem; }
    ~CBaseIntelligentPointer() { ++g_pointerDtors; }
};
inline bool IS_THING_ALIVE(const CBaseIntelligentPointer& p) { if (p.GetPItem() != 0) { if (!p.GetPItem()->m_dead) return true; } return false; }
struct CEventShownItem {
    CBaseIntelligentPointer ThingShowingItem, ItemShown;
    ~CEventShownItem()
    {
        if (IS_THING_ALIVE(ItemShown)) {
            CTCShowableItem* item;
            if (ItemShown.GetPItem()->GetInterface(&item, 0x1a)) item->OnShowEnded();
        }
    }
};
int main()
{
    CTCShowableItem tc; CThing thing; for (int i = 0; i < 9; ++i) thing.m_presentBits[i] = 0;
    thing.m_dead = false; thing.m_pShowable = &tc; thing.m_presentBits[0] = 1U << 26;
    { CEventShownItem e; e.ThingShowingItem.m_pItem = 0; e.ItemShown.m_pItem = &thing; }
    bool ok = g_shownEnded == 1 && g_pointerDtors == 2;
    thing.m_dead = true; { CEventShownItem e; e.ThingShowingItem.m_pItem = 0; e.ItemShown.m_pItem = &thing; }
    thing.m_dead = false; thing.m_presentBits[0] = 0; { CEventShownItem e; e.ThingShowingItem.m_pItem = 0; e.ItemShown.m_pItem = &thing; }
    { CEventShownItem e; e.ThingShowingItem.m_pItem = 0; e.ItemShown.m_pItem = 0; }
    ok = ok && g_shownEnded == 1 && g_pointerDtors == 8;
    if (!ok) { std::printf("008116d0_TEST FAIL\n"); return 1; }
    std::printf("008116d0_TEST PASS\n");
    return 0;
}