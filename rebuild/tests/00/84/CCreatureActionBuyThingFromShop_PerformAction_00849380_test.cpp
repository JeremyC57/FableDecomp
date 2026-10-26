// Behaviour test for 0x00849380 CCreatureAction_BuyThingFromShop::PerformAction: runs the reconstructed body over stub things,
// components and managers; checks the dead/missing target and missing hero-stats exits, the missing-inventory exit, the two
// refusal subtitles ("already have" when the item's max count is 1, "no room" otherwise; font bank, colour 0, 3 seconds), and
// the purchase: the price paid, the copy created at the buyer for its player with an empty name, added to the inventory at
// the price, the shopkeeper's sold-item event (buyer, item, false) only when the keeper has an event component, and the
// message 0x13 (1000 frames, buyer position, buyer).
#include <cstdio>
#include <cstdlib>
#include <cstring>

#ifndef _MSC_VER
#define __forceinline inline
#define __fastcall
#endif

class CTCBase { public: };
class C3DVector { public: float X, Y, Z; };

static int g_liveStrings = 0;
class CCharString { public:
    char m_text[96];
    CCharString() { m_text[0] = 0; ++g_liveStrings; }
    CCharString(const char* text, long = -1) { std::strcpy(m_text, text); ++g_liveStrings; }
    ~CCharString() { --g_liveStrings; }
    CCharString& operator=(const char* text) { std::strcpy(m_text, text); return *this; }
};

struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry  entries[8];
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type)
    {
        SEntry* it = first;
        int len = int(last - first);
        while (len > 0) {
            int half = len / 2;
            SEntry* middle = it + half;
            if (middle->type < *type) {
                it = middle + 1;
                len = len - half - 1;
            } else
                len = half;
        }
        return it;
    }
    const SEntry* LowerBound(const long* type) const
    {
        const SEntry* it = first;
        int len = int(last - first);
        while (len > 0) {
            int half = len / 2;
            const SEntry* middle = it + half;
            if (middle->type < *type) {
                it = middle + 1;
                len = len - half - 1;
            } else
                len = half;
        }
        return it;
    }
};

class CFontBank { public: int id; };
class CMainGameComponent { public: CFontBank* FontBank; };
static CMainGameComponent g_component;

class CThing { public:
    unsigned long  m_presentBits[9];
    CInterfaceMap  m_interfaces;
    unsigned short DefIndex;
    signed char    PlayerNumber;
    unsigned char  m_flags91;
    C3DVector      m_position;
    CThing() : DefIndex(0), PlayerNumber(0), m_flags91(0)
    {
        for (int i = 0; i < 9; ++i)
            m_presentBits[i] = 0;
        m_interfaces.first = m_interfaces.last = m_interfaces.entries;
        m_position.X = m_position.Y = m_position.Z = 0.0f;
    }
    void AddInterface(long type, CTCBase* component)   // test helper: keep the map sorted by type
    {
        CInterfaceMap::SEntry* it = m_interfaces.last;
        while (it != m_interfaces.first && (it - 1)->type > type) {
            *it = *(it - 1);
            --it;
        }
        it->type = type;
        it->component = component;
        ++m_interfaces.last;
        m_presentBits[type >> 5] |= 1UL << (type & 31);
    }
    long GetPlayer() const { return PlayerNumber; }
    const C3DVector& GetPosition() const { return m_position; }
    CMainGameComponent* GetMainGameComponent() { return &g_component; }
    bool TestComponent(unsigned long position) const
    {
        if (position >= 274) {
            std::puts("bitset");
            std::abort();
        }
        return (m_presentBits[position >> 5] & (1UL << (position & 31))) != 0;
    }
    template<class T> __forceinline bool GetInterface(T** ppOut, long interfaceType)
    {
        if (m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31))) {
            long type = interfaceType;
            CInterfaceMap* map = &m_interfaces;
            CInterfaceMap::SEntry* it = map->LowerBound(&type);
            *ppOut = (T*)((it == map->last || interfaceType < it->type) ? map->last : it)->component;
            return true;
        }
        return false;
    }
    template<class T> __forceinline bool GetInterface(T** ppOut, long interfaceType) const
    {
        if (m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31))) {
            long type = interfaceType;
            const CInterfaceMap* map = &m_interfaces;
            const CInterfaceMap::SEntry* it = map->LowerBound(&type);
            *ppOut = (T*)((it == map->last || interfaceType < it->type) ? map->last : it)->component;
            return true;
        }
        return false;
    }
    template<class T> __forceinline bool GetCheckedInterface(T** ppOut, long interfaceType)
    {
        if (TestComponent(interfaceType)) {
            long type = interfaceType;
            CInterfaceMap* map = &m_interfaces;
            CInterfaceMap::SEntry* it = map->LowerBound(&type);
            *ppOut = (T*)((it == map->last || interfaceType < it->type) ? map->last : it)->component;
            return true;
        }
        return false;
    }
};

class CRightHandedSet;
static int       g_createCalls = 0;
static long      g_createDef = -1;
static C3DVector g_createPos;
static long      g_createPlayer = -99;
static const void* g_createOrientation = (const void*)1;
static unsigned long g_createFlags = 99;
static char      g_createName[96];
static CThing*   g_createResult = 0;
CThing* __fastcall CreateThingObject(long defIndex, const C3DVector* position, long player, const CRightHandedSet* orientation,
                                     unsigned long flags, const CCharString& name)
{
    ++g_createCalls; g_createDef = defIndex; g_createPos = *position; g_createPlayer = player;
    g_createOrientation = orientation; g_createFlags = flags; std::strcpy(g_createName, name.m_text);
    return g_createResult;
}
__forceinline CThing* CreateThingAt(const CThing* item, const CThing* at, const CThing* owner, const CCharString& name)
{
    long defIndex = item->DefIndex;
    return CreateThingObject(defIndex, &at->GetPosition(), owner->GetPlayer(), 0, 0, name);
}

struct CBaseIntelligentPointer {
    CThing* m_pItem;
    CBaseIntelligentPointer() : m_pItem(0) { }
    CThing* GetPItem() const { return m_pItem; }
    operator CThing*() const { return GetPItem(); }
};
inline bool IS_THING_ALIVE(const CBaseIntelligentPointer& p)
{
    if (p.GetPItem() != 0) {
        if (!(p.GetPItem()->m_flags91 & 1))
            return true;
    }
    return false;
}

static int        g_setTextCalls = 0;
static CFontBank* g_textFont = 0;
static char       g_text[96];
static unsigned long g_textColour = 99;
static float      g_textTime = 0.0f;
static bool       g_textA = true, g_textB = false;
class CSubtitleRenderer { public:
    void SetText(CFontBank* font, const CCharString& text, unsigned long colour, float time, bool a, bool b)
    {
        ++g_setTextCalls; g_textFont = font; std::strcpy(g_text, text.m_text); g_textColour = colour; g_textTime = time;
        g_textA = a; g_textB = b;
    }
};
class CPlayerGui { public: CSubtitleRenderer SubtitleRenderer; };
static CPlayerGui  g_gui;
static CPlayerGui* g_pPlayerGui = &g_gui;

class CTCHeroStats : public CTCBase { public:
    long Money;
    int  m_calls;
    bool m_show;
    void AddMoney(long amount, bool showMessage) { Money += amount; ++m_calls; m_show = showMessage; }
};
struct CTCInventoryBase : CTCBase {
    bool m_canAdd;
    long m_askedDef;
    bool CanAddItem(long defIndex) const { const_cast<CTCInventoryBase*>(this)->m_askedDef = defIndex; return m_canAdd; }
};
static CTCInventoryBase* g_addInventory = 0;
static long g_addPrice = 0;
static bool g_addFlags[4];
static int  g_addCalls = 0;
class CTCInventoryItem : public CTCBase { public:
    long InventoryType;
    long MaxNumberItems;
    long GetInventoryType() const { return InventoryType; }
    long GetMaxNumberItems() const { return MaxNumberItems; }
    bool AddToInventory(CTCInventoryBase* inventory, bool a, bool b, long price, bool c, bool d)
    {
        ++g_addCalls; g_addInventory = inventory; g_addPrice = price;
        g_addFlags[0] = a; g_addFlags[1] = b; g_addFlags[2] = c; g_addFlags[3] = d;
        return true;
    }
};
class CTCShop { public:
    CBaseIntelligentPointer PShopKeeper;
    CThing* GetShopKeeper() const { return PShopKeeper.GetPItem(); }
};
class CTCStockItem : public CTCBase { public:
    long     Price;
    CTCShop* m_shop;
    CTCShop* GetShopTC() const { return m_shop; }
};
struct CEventBase { virtual ~CEventBase() { } };
namespace NEntityEvents {
struct CEventSoldItem : CEventBase {
    CThing* m_giver;
    CThing* m_item;
    bool    m_ownsItem;
    CEventSoldItem(CThing* giver, CThing* item, bool ownsItem) : m_giver(giver), m_item(item), m_ownsItem(ownsItem) { }
};
}
class CTCEntityEvents : public CTCBase { public:
    CEventBase* m_last;
    int         m_count;
    void AddEvent(CEventBase* event) { delete m_last; m_last = event; ++m_count; }
};
static int       g_messageCalls = 0;
static long      g_messageType = 0;
static unsigned long g_messageDelay = 0;
static C3DVector g_messagePos;
static CThing*   g_messageFrom = 0;
static CThing*   g_messageTo = (CThing*)1;
static float     g_messageV1 = 1.0f, g_messageV2 = 1.0f;
static long      g_messageData = 1;
class CMessageEventManager { public:
    void* AddMessage(long type, unsigned long delay, const C3DVector& position, CThing* from, CThing* to,
                     float value1, float value2, long data)
    {
        ++g_messageCalls; g_messageType = type; g_messageDelay = delay; g_messagePos = position; g_messageFrom = from;
        g_messageTo = to; g_messageV1 = value1; g_messageV2 = value2; g_messageData = data;
        return 0;
    }
};
class CWorld { public: CMessageEventManager* MessageEventManager; };
static CMessageEventManager g_manager;
static CWorld g_world;

class CCreatureActionBase { public:
    CBaseIntelligentPointer PCreature;
    virtual ~CCreatureActionBase() { }
    virtual void PerformAction() { }
    CWorld* GetWorld() { return &g_world; }
};
struct CCreatureAction_BuyThingFromShop : CCreatureActionBase {
    CBaseIntelligentPointer PTarget;
    virtual void PerformAction();
};

void CCreatureAction_BuyThingFromShop::PerformAction()
{
    if (IS_THING_ALIVE(PTarget)) {
        CSubtitleRenderer* subtitles = &g_pPlayerGui->SubtitleRenderer;
        CTCStockItem* stockItem;
        static_cast<const CThing*>(PTarget.GetPItem())->GetInterface(&stockItem, 0x54);
        CTCHeroStats* stats;
        if (PCreature.GetPItem()->GetInterface(&stats, 4)) {
            CTCInventoryItem* inventoryItem;
            static_cast<const CThing*>(PTarget.GetPItem())->GetInterface(&inventoryItem, 0x1a);
            CThing* creature = PCreature.GetPItem();
            CTCInventoryBase* inventory;
            if (creature->GetCheckedInterface(&inventory, inventoryItem->GetInventoryType())) {
                if (!inventory->CanAddItem(PTarget.GetPItem()->DefIndex)) {
                    CCharString message;
                    if (inventoryItem->GetMaxNumberItems() == 1)
                        message = "You already have this item!";
                    else
                        message = "You don't have room in your inventory for this item!";
                    CMainGameComponent* component = PCreature.GetPItem()->GetMainGameComponent();
                    subtitles->SetText(component->FontBank, message, 0, 3.0f, false, true);
                    return;
                }
                stats->AddMoney(-stockItem->Price, true);
                CThing* item = CreateThingAt(PTarget.GetPItem(), PCreature.GetPItem(), PCreature.GetPItem(), "");
                CTCInventoryItem* newItem;
                item->GetInterface(&newItem, 0x1a);
                newItem->AddToInventory(inventory, false, true, stockItem->Price, false, false);
                CThing* shopKeeper = stockItem->GetShopTC()->GetShopKeeper();
                CTCEntityEvents* events;
                if (shopKeeper && shopKeeper->GetInterface(&events, 0x42))
                    events->AddEvent(new NEntityEvents::CEventSoldItem(PCreature, PTarget.GetPItem(), false));
                CThing* buyer = PCreature.GetPItem();
                CMessageEventManager* manager = GetWorld()->MessageEventManager;
                manager->AddMessage(0x13, 1000, buyer->GetPosition(), PCreature.GetPItem(), 0, 0.0f, 0.0f, 0);
            }
        }
    }
}

int main()
{
    bool ok = true;
    CFontBank font; font.id = 7;
    g_component.FontBank = &font;
    g_world.MessageEventManager = &g_manager;

    // the buyer: hero stats (4) and a weapons inventory (type 0x21)
    CTCHeroStats stats; stats.Money = 500; stats.m_calls = 0; stats.m_show = false;
    CTCInventoryBase inventory; inventory.m_canAdd = true; inventory.m_askedDef = -1;
    CThing buyer;
    buyer.PlayerNumber = -2;
    buyer.m_position.X = 1.0f; buyer.m_position.Y = 2.0f; buyer.m_position.Z = 3.0f;
    buyer.AddInterface(0x21, &inventory);
    buyer.AddInterface(4, &stats);

    // the shop and its keeper
    CTCEntityEvents keeperEvents; keeperEvents.m_last = 0; keeperEvents.m_count = 0;
    CThing keeper; keeper.AddInterface(0x42, &keeperEvents);
    CTCShop shop; shop.PShopKeeper.m_pItem = &keeper;

    // the stock item on display
    CTCStockItem stock; stock.Price = 120; stock.m_shop = &shop;
    CTCInventoryItem displayItem; displayItem.InventoryType = 0x21; displayItem.MaxNumberItems = 1;
    CThing target;
    target.DefIndex = 0x1234;
    target.AddInterface(0x54, &stock);
    target.AddInterface(0x1a, &displayItem);

    // the created copy
    CTCInventoryItem newItem; newItem.InventoryType = 0x21; newItem.MaxNumberItems = 1;
    CThing created; created.AddInterface(0x1a, &newItem);
    g_createResult = &created;

    CCreatureAction_BuyThingFromShop action;
    action.PCreature.m_pItem = &buyer;
    action.PTarget.m_pItem = &target;

    // purchase
    action.PerformAction();
    ok = ok && stats.Money == 380 && stats.m_calls == 1 && stats.m_show && inventory.m_askedDef == 0x1234;
    ok = ok && g_createCalls == 1 && g_createDef == 0x1234 && g_createPlayer == -2 && g_createOrientation == 0;
    ok = ok && g_createFlags == 0 && std::strcmp(g_createName, "") == 0;
    ok = ok && g_createPos.X == 1.0f && g_createPos.Y == 2.0f && g_createPos.Z == 3.0f;
    ok = ok && g_addCalls == 1 && g_addInventory == &inventory && g_addPrice == 120;
    ok = ok && !g_addFlags[0] && g_addFlags[1] && !g_addFlags[2] && !g_addFlags[3];
    ok = ok && keeperEvents.m_count == 1;
    NEntityEvents::CEventSoldItem* sold = (NEntityEvents::CEventSoldItem*)keeperEvents.m_last;
    ok = ok && sold && sold->m_giver == &buyer && sold->m_item == &target && !sold->m_ownsItem;
    ok = ok && g_messageCalls == 1 && g_messageType == 0x13 && g_messageDelay == 1000 && g_messageFrom == &buyer;
    ok = ok && g_messageTo == 0 && g_messageV1 == 0.0f && g_messageV2 == 0.0f && g_messageData == 0;
    ok = ok && g_messagePos.X == 1.0f && g_messagePos.Y == 2.0f && g_messagePos.Z == 3.0f;
    ok = ok && g_setTextCalls == 0 && g_liveStrings == 0;

    // a keeper without an event component, or no keeper: no event, the rest unchanged
    CThing plainKeeper;
    shop.PShopKeeper.m_pItem = &plainKeeper;
    action.PerformAction();
    ok = ok && keeperEvents.m_count == 1 && stats.Money == 260 && g_createCalls == 2 && g_messageCalls == 2;
    shop.PShopKeeper.m_pItem = 0;
    action.PerformAction();
    ok = ok && keeperEvents.m_count == 1 && stats.Money == 140 && g_addCalls == 3 && g_messageCalls == 3;
    shop.PShopKeeper.m_pItem = &keeper;

    // refused, unique item: "already have" subtitle, nothing bought
    inventory.m_canAdd = false;
    action.PerformAction();
    ok = ok && g_setTextCalls == 1 && std::strcmp(g_text, "You already have this item!") == 0 && g_textFont == &font;
    ok = ok && g_textColour == 0 && g_textTime == 3.0f && !g_textA && g_textB && g_liveStrings == 0;
    ok = ok && stats.Money == 140 && g_createCalls == 3 && g_messageCalls == 3;

    // refused, stackable item: "no room"
    displayItem.MaxNumberItems = 5;
    action.PerformAction();
    ok = ok && g_setTextCalls == 2 && std::strcmp(g_text, "You don't have room in your inventory for this item!") == 0;

    // no inventory of the item's type: nothing at all
    displayItem.InventoryType = 0x22;
    inventory.m_canAdd = true;
    action.PerformAction();
    ok = ok && g_setTextCalls == 2 && g_createCalls == 3 && stats.m_calls == 3;
    displayItem.InventoryType = 0x21;

    // a buyer without hero stats: nothing
    CThing poorBuyer; poorBuyer.AddInterface(0x21, &inventory);
    action.PCreature.m_pItem = &poorBuyer;
    action.PerformAction();
    ok = ok && g_createCalls == 3 && g_setTextCalls == 2 && stats.m_calls == 3;
    action.PCreature.m_pItem = &buyer;

    // dead or missing target: nothing
    target.m_flags91 = 1;
    action.PerformAction();
    action.PTarget.m_pItem = 0;
    action.PerformAction();
    ok = ok && g_createCalls == 3 && g_setTextCalls == 2 && stats.m_calls == 3 && g_messageCalls == 3;

    delete keeperEvents.m_last;
    if (!ok) { std::printf("00849380_TEST FAIL\n"); return 1; }
    std::printf("00849380_TEST PASS\n");
    return 0;
}