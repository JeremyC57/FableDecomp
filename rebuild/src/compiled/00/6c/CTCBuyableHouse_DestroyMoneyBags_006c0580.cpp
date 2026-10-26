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
// Shape (LowerBound): the CVectorMap<ETCInterfaceType, CTCBase*>::LowerBound instantiation is defined in this TU (noinline),
//   as retail compiled it in the caller's TU; with its body visible VC7.1 packs the lookup keys and temporaries as retail.
class CTCBase;
class CBaseObject;
class CCharStringData { public:
    char* m_pData;                                      // +0x00
    long  m_length;                                     // +0x04
};
class CCharString { public:
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
class CThing;
template <class T> class CIntelligentPointer : public CBaseIntelligentPointer { public:
    T* Get() const { return (T*)GetPItem(); }
};
struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);               // 0x0040F020
};
#pragma auto_inline(off)
// Same-TU body of the CVectorMap<ETCInterfaceType, CTCBase*>::LowerBound instantiation (a template member, compiled in the
// caller's TU): binary search for the first entry whose type is not below the key read through the pointer.
__declspec(noinline) CInterfaceMap::SEntry* CInterfaceMap::LowerBound(const long* type)
{
    SEntry* it = first;
    long count = last - first;
    while (count > 0) {
        long half = count / 2;
        if (it[half].type < *type) {
            it += half + 1;
            count -= half + 1;
        } else
            count = half;
    }
    return it;
}
#pragma auto_inline(on)

struct SThingNode {
    SThingNode*                 next;
    SThingNode*                 prev;
    CIntelligentPointer<CThing> thing;                  // +0x08
};
struct CTCThingList_0x21 {
    unsigned char _pad_0x00[0xc];
    SThingNode*   m_head;                               // +0x0c
};
class CThing { public:
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
class CTCGold { public:
    long GetGold();                                     // 0x007CBDC0
};
class CTCHeroStats { public:
    void AddMoney(long amount, bool b);                 // 0x0057B327
};
class CPlayer { public:
    CThing* GetHeroThing();                             // 0x00487DC0
};
class CPlayerManager { public:
public:
    CPlayer& GetMainPlayer();   // 0x00449970
};
inline bool IsThingAlive(const CIntelligentPointer<CThing>& p)
{
    return p.Get() && !(p.Get()->m_stateFlags & 1);
}
class CTCBase
{
public:
    virtual void v00();
protected:
    CPlayerManager& GetPlayerManager();   // 0x00686D40
};

class CTCBuyableHouse : public CTCBase {
public:
    CThing*       ParentThing;                          // +0x04
    void DestroyMoneyBags(bool giveToHero);
};

void CTCBuyableHouse::DestroyMoneyBags(bool giveToHero)
{
    CTCThingList_0x21* list;
    if (ParentThing->GetInterface(&list, 0x21)) {
        for (SThingNode* node = list->m_head->next; node != list->m_head; node = node->next) {
            CTCGold* gold;
            if (IsThingAlive(node->thing) && node->thing.Get()->GetInterface(&gold, 0xcc)) {
                CThing* hero = GetPlayerManager().GetMainPlayer().GetHeroThing();
                if (hero && !(hero->m_stateFlags & 1) && giveToHero && hero->HasInterface(4))
                    hero->GetTC<CTCHeroStats>(4)->AddMoney(gold->GetGold(), true);
                node->thing.Get()->Kill(true);
            }
        }
    }
}