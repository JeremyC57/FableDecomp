// Grade: DIFFER(329v329) -- frame only: retail shares one key slot (L0) for the 0x21, 0xCC and 4 lookups with `this` homed at L1 and the list at L2 (frame 0Ch); ours gives the in-loop keys a second slot (frame 10h, every frame offset +4)
// CTCBuyableHouse::DestroyMoneyBags(bool giveToHero) -- kill every live money bag (interface 0xCC) in the house, first crediting its gold to the live hero's stats when asked
// Leaf: DestroyMoneyBags
// Retail 0x006C0580, 336 bytes, `ret 4`.
// Callees: CVectorMap::LowerBound 0x0040F020 (interfaces 0x21 thing list, 0xCC gold, 4 hero stats), CBaseIntelligentPointer::
// GetPItem 0x00A01B50, the TC's player-manager accessor 0x00686D40, CPlayerManager::GetMainPlayer 0x00449970,
// CPlayer::GetHeroThing 0x00487DC0, CTCGold::GetGold 0x007CBDC0, CTCHeroStats::AddMoney(long, bool) 0x0057B327,
// CThing::Kill(bool) 0x004C9B80.
// Layout: CTCBase ParentThing +4; interface-0x21 component list header +0x0C (landed CountMoneyBags 0x006C04D0 model);
// CThing bitset +0x20, map +0x44, dead flag +0x91.
// Shape: the CountMoneyBags walk (three GetPItem calls for the alive and interface tests) with HasTC + GetTC lookups;
// `this` is homed in the frame across the loop; AddMoney's `true` is pushed before GetGold runs.
struct CTCBase;
struct CBaseObject;
struct CCharStringData {
    char* m_pData;                                      // +0x00
    long  m_length;                                     // +0x04
};
struct CCharString {
    CCharStringData* PStringData;
    CCharString(const char* text, long length = -1);   // 0x0099EBF0
    ~CCharString();                                     // 0x0099EAE0
    const char* GetPString() const { return PStringData == 0 ? "" : PStringData->m_pData; }   // "" = 0x0129AAF4
    long Length() const { return PStringData == 0 ? 0 : PStringData->m_length; }
    char operator[](long i) const { return GetPString()[i]; }
    CCharString GetSubString(unsigned long start, unsigned long end) const;   // 0x0099EC70
};
struct CBaseIntelligentPointer {
    virtual void v00();
    CBaseObject* m_pItem;                               // +0x04
    CBaseObject* GetPItem() const;                      // 0x00A01B50
};
struct CThing;
template <class T> struct CIntelligentPointer : CBaseIntelligentPointer {
    T* Get() const { return (T*)GetPItem(); }
};
struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);               // 0x0040F020
};
struct SThingNode {
    SThingNode*                 next;
    SThingNode*                 prev;
    CIntelligentPointer<CThing> thing;                  // +0x08
};
struct CTCThingList_0x21 {
    unsigned char _pad_0x00[0xc];
    SThingNode*   m_head;                               // +0x0c
};
struct CThing {
    unsigned char _pad0[0x20];
    unsigned long m_presentBits[9];                     // +0x20
    CInterfaceMap m_interfaces;                         // +0x44
    unsigned char _pad_0x4c[0x78 - 0x4c];
    CCharString   ScriptName;                           // +0x78
    unsigned char _pad_0x7c[0x91 - 0x7c];
    unsigned char m_stateFlags;                         // +0x91 (bit 0 = dead)
    void Kill(bool immediately);                        // 0x004C9B80
    unsigned long HasInterface(long type) const { return m_presentBits[type >> 5] & (1UL << (type & 31)); }
    template <class T> __forceinline T* GetTC(long type)
    {
        long key = type;
        CInterfaceMap* map = &m_interfaces;
        CInterfaceMap::SEntry* it = map->LowerBound(&key);
        return (T*)((it == map->last || type < it->type) ? map->last : it)->component;
    }
    template <class T> bool GetInterface(T** ppOut, long interfaceType)
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
};
struct CTCGold {
    long GetGold();                                     // 0x007CBDC0
};
struct CTCHeroStats {
    void AddMoney(long amount, bool b);                 // 0x0057B327
};
struct CPlayer {
    CThing* GetHeroThing();                             // 0x00487DC0
};
struct CPlayerManager {
    CPlayer* GetMainPlayer();                           // 0x00449970
};
inline bool IsThingAlive(const CIntelligentPointer<CThing>& p)
{
    return p.Get() && !(p.Get()->m_stateFlags & 1);
}
class CTCBuyableHouse {
public:
    virtual void v00();
    CThing*       ParentThing;                          // +0x04
    CPlayerManager* GetPlayerManager();                 // 0x00686D40
    void DestroyMoneyBags(bool giveToHero);
};

void CTCBuyableHouse::DestroyMoneyBags(bool giveToHero)
{
    CTCThingList_0x21* list;
    if (ParentThing->GetInterface(&list, 0x21)) {
        for (SThingNode* node = list->m_head->next; node != list->m_head; node = node->next) {
            CTCGold* gold;
            if (IsThingAlive(node->thing) && node->thing.Get()->GetInterface(&gold, 0xcc)) {
                CThing* hero = GetPlayerManager()->GetMainPlayer()->GetHeroThing();
                if (hero && !(hero->m_stateFlags & 1) && giveToHero && hero->HasInterface(4))
                    hero->GetTC<CTCHeroStats>(4)->AddMoney(gold->GetGold(), true);
                node->thing.Get()->Kill(true);
            }
        }
    }
}
