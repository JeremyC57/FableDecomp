// CCreatureAction_StealThingFromShop::PerformAction() -- with a live shop item: when the creature's inventory of the
// Grade: DIFFER(719v719) -- CreateThingObject argument order: retail fetches the player creature, the position creature, then the def index (ours: def index first, GetPosition early); SetText: retail loads the font bank before the pushes
// item's category cannot take it, show "You already have this item!" (single-holding items) or the no-room message for
// 3 seconds; otherwise create a copy of the item at the creature, add it to that inventory, raise message 0x1c from the
// thief to the item's shopkeeper (interface 0x33) and make a SIMPLE_NOISE
// Leaf: ?PerformAction@CCreatureAction_StealThingFromShop
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00849700, length 719 bytes, plain `ret` (virtual).
// Callees: CBaseIntelligentPointer::GetPItem 0x00A01B50, CVectorMap::LowerBound 0x0040F020 / const copy 0x004365B0,
//   CTCInventoryItem::GetInventoryType 0x005D9030 / the holding-limit getter 0x005D9050 (descriptive name) /
//   AddToInventory 0x005D8D50, puts 0x00BFEAF2 + abort 0x00BFEAEC (STLport bitset<274>::test range check, "bitset"),
//   CTCInventoryBase::CanAddItem 0x005BF2ED, CCharString 0x0099E4B0 / 0x0099EBF0 / operator=(const char*) 0x0099EFE0 /
//   ~ 0x0099EAE0, CThing::GetMainGameComponent 0x004C7A70 (font bank +0x1614c), CSubtitleRenderer::SetText 0x00492ED1 on
//   the UI singleton 0x013B8790 (+0x18), CThing::GetPosition 0x004C73D0, CreateThingObject 0x00703210 (__fastcall),
//   CCreatureActionBase::GetPWorld 0x00692940 (message manager +0x60), CMessageManager::AddMessage 0x00687540,
//   CThingCreatureBase::MakeNoise 0x00662110.
// Layout: retail CCreatureActionBase is 0xa8 bytes; the shop item pointer is this class's first member (+0xa8).
//   CThing: interface bitset +0x20, map +0x44, def index (word) +0x8c, player +0x90, dead bit +0x91.
// Model: the landed CTCContainer::AddItemToInventory 0x008ED090 (bitset::test lookup), TrollGroundPound 0x008C6B20.
extern "C" int __cdecl puts(const char* text);          // 0x00BFEAF2
extern "C" __declspec(noreturn) void __cdecl abort();   // 0x00BFEAEC
struct C3DVector { float X, Y, Z; };
struct CRightHandedSet;
struct CCharString {
    void* m_pData;
    CCharString();                                      // 0x0099E4B0
    CCharString(const char* text, long length = -1);    // 0x0099EBF0
    ~CCharString();                                     // 0x0099EAE0
    CCharString& operator=(const char* text);           // 0x0099EFE0
};
struct CTCBase;
struct CThing;
struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);                   // 0x0040F020
    const SEntry* LowerBound(const long* type) const;       // 0x004365B0
};
struct CFontBank;
struct CMainGameComponent {
    unsigned char _pad[0x1614c];
    CFontBank*    m_pFontBank;                          // +0x1614c
};
struct CThing {
    unsigned char _pad00[0x20];
    unsigned long UsingComponents[9];                   // +0x20 bitset<274>
    CInterfaceMap Components;                           // +0x44
    unsigned char _pad4c[0x8c - 0x4c];
    unsigned short DefIndex;                            // +0x8c
    unsigned char _pad8e[2];
    char          PlayerNumber;                         // +0x90
    unsigned char m_flags91;                            // +0x91 bit 0: dead
    const C3DVector& GetPosition() const;               // 0x004C73D0
    CMainGameComponent* GetMainGameComponent();         // 0x004C7A70
    void MakeNoise(const CCharString& noise);           // 0x00662110
    bool TestComponent(unsigned long position) const    // STLport bitset<274>::test (exception-free range check)
    {
        if (position >= 274) {
            puts("bitset");
            abort();
        }
        return (UsingComponents[position >> 5] & (1UL << (position & 31))) != 0;
    }
    __forceinline CTCBase* GetTC(long interfaceType)
    {
        long type = interfaceType;
        CInterfaceMap* map = &Components;
        CInterfaceMap::SEntry* it = map->LowerBound(&type);
        if (it == map->last || interfaceType < it->type)
            it = map->last;
        return it->component;
    }
    __forceinline CTCBase* GetTCIfPresent(long interfaceType)
    {
        CTCBase* tc;
        if (TestComponent(interfaceType))
            tc = GetTC(interfaceType);
        return tc;
    }
    template<class T> __forceinline bool GetInterface(T** ppOut, long interfaceType)
    {
        if (UsingComponents[interfaceType >> 5] & (1UL << (interfaceType & 31))) {
            long type = interfaceType;
            CInterfaceMap* map = &Components;
            CInterfaceMap::SEntry* it = map->LowerBound(&type);
            if (it == map->last || interfaceType < it->type)
                it = map->last;
            *ppOut = (T*)it->component;
            return true;
        }
        return false;
    }
    template<class T> __forceinline bool GetInterface(T** ppOut, long interfaceType) const
    {
        if (UsingComponents[interfaceType >> 5] & (1UL << (interfaceType & 31))) {
            long type = interfaceType;
            const CInterfaceMap* map = &Components;
            const CInterfaceMap::SEntry* it = map->LowerBound(&type);
            if (it == map->last || interfaceType < it->type)
                it = map->last;
            *ppOut = (T*)it->component;
            return true;
        }
        return false;
    }
};
CThing* __fastcall CreateThingObject(long defIndex, const C3DVector* position, long player, const CRightHandedSet* set,
                                     unsigned long flags, const CCharString& name);   // 0x00703210
__forceinline CThing* CreateThingObject(const long& defIndex, const C3DVector& position, const CThing* creature, const CCharString& name)
{
    return CreateThingObject(defIndex, &position, creature->PlayerNumber, 0, 0, name);
}
struct CTCInventoryBase {
    bool CanAddItem(long item) const;                   // 0x005BF2ED
};
struct CTCInventoryItem {
    long GetInventoryType();                            // 0x005D9030
    long GetHoldingLimit();                             // 0x005D9050
    bool AddToInventory(CTCInventoryBase* inventory, bool a, bool b, long c, bool d, bool e);   // 0x005D8D50
};
struct CSubtitleRenderer {
    void SetText(CFontBank* font, const CCharString& text, unsigned long colour, float time, bool a, bool b);   // 0x00492ED1
};
struct CGameUI {
    unsigned char     _pad00[0x18];
    CSubtitleRenderer m_subtitles;                      // +0x18
};
extern CGameUI* g_pGameUI;                              // 0x013B8790
struct CMessageManager {
    void* AddMessage(long type, unsigned long delay, const C3DVector& position, CThing* from, CThing* to,
                     float value1, float value2, long data);   // 0x00687540
};
struct CWorld {
    unsigned char    _pad[0x60];
    CMessageManager* MessageManager;                    // +0x60
};
#pragma pack(push, 1)
struct CBaseIntelligentPointer {
    virtual void v00();
    CThing* PItem;                                      // +4
    CThing* GetPItem() const;                           // 0x00A01B50
};
struct CTCShopItem {                                    // interface 0x33
    unsigned char           _pad0[0xd];
    CBaseIntelligentPointer PShopkeeper;                // +0x0d (packed)
};
#pragma pack(pop)
struct CCreatureActionBase {
    virtual void v00();
    unsigned char           _pad04[4];
    CBaseIntelligentPointer PCreature;                  // +0x08
    unsigned char           _pad10[0xa8 - 0x10];
    CThing* GetCreature() const { return PCreature.GetPItem(); }
    CWorld* GetPWorld() const;                          // 0x00692940
};
struct CCreatureAction_StealThingFromShop : CCreatureActionBase {
    CBaseIntelligentPointer PItem;                      // +0xa8
    virtual void PerformAction();
};

void CCreatureAction_StealThingFromShop::PerformAction()
{
    if (PItem.GetPItem() && !(PItem.GetPItem()->m_flags91 & 1)) {
        CTCInventoryItem* inventoryItem;
        static_cast<const CThing*>(PItem.GetPItem())->GetInterface(&inventoryItem, 0x1a);
        CTCInventoryBase* inventory = (CTCInventoryBase*)GetCreature()->GetTCIfPresent(inventoryItem->GetInventoryType());
        if (!inventory->CanAddItem(PItem.GetPItem()->DefIndex)) {
            CCharString message;
            if (inventoryItem->GetHoldingLimit() == 1)
                message = "You already have this item!";
            else
                message = "You don't have room in your inventory for this item!";
            g_pGameUI->m_subtitles.SetText(GetCreature()->GetMainGameComponent()->m_pFontBank, message, 0, 3.0f, false, true);
            return;
        }
        CThing* copy = CreateThingObject(PItem.GetPItem()->DefIndex, GetCreature()->GetPosition(), GetCreature(), "");
        CTCInventoryItem* copyItem;
        copy->GetInterface(&copyItem, 0x1a);
        copyItem->AddToInventory(inventory, false, true, 0, false, false);
        CTCShopItem* shopItem;
        PItem.GetPItem()->GetInterface(&shopItem, 0x33);
        CThing* shopkeeper = shopItem->PShopkeeper.GetPItem();
        CThing* thief = GetCreature();
        GetPWorld()->MessageManager->AddMessage(0x1c, 0x32, thief->GetPosition(), GetCreature(), shopkeeper, 0.0f, 0.0f, 0);
        GetCreature()->MakeNoise("SIMPLE_NOISE");
    }
}
