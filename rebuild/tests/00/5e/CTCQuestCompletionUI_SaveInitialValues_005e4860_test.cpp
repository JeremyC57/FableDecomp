// Behaviour test for 0x005E4860: CTCQuestCompletionUI::SaveInitialValues(const CCharString& questName) -- when the named quest's active card has a def index, remember the hero's renown, gold and experience at the quest start under that index
// A copy of the source model and logic (compiled separately from src/005e4860.cpp) with test stand-ins for the callees.
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <new>
#if !defined(_MSC_VER)
#define __fastcall
#define __forceinline inline
#define __declspec(x)
#define __cdecl
#define __fastcall
#define __int64 long long
#endif
class CCharString;
class CTCBase;

class CInterfaceMap {
public:
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);              // 0x0040F020
};

class CThing {
public:
    unsigned char  _pad00[0x20];
    unsigned long  m_presentBits[9];                   // +0x20
    CInterfaceMap  m_interfaces;                       // +0x44
    unsigned char  _pad4c[0x8c - 0x4c];
    unsigned short DefGlobalIndex;                     // +0x8c
    template <class T> __forceinline bool GetInterface(T** ppOut, long interfaceType)
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
    __forceinline bool HasTC(long interfaceType) const
    {
        if (m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31)))
            return true;
        return false;
    }
    template <class T> __forceinline T* GetTC(long interfaceType)
    {
        long type = interfaceType;
        CInterfaceMap* map = &m_interfaces;
        CInterfaceMap::SEntry* it = map->LowerBound(&type);
        return (T*)((it == map->last || interfaceType < it->type) ? map->last : it)->component;
    }
};

#pragma auto_inline(off)
inline __declspec(noinline) CInterfaceMap::SEntry* CInterfaceMap::LowerBound(const long* type)
{
    SEntry* lo = first;
    int n = last - first;
    while (n > 0) {
        int half = n >> 1;
        if (lo[half].type < *type) { lo += half + 1; n -= half + 1; }
        else n = half;
    }
    return lo;
}
#pragma auto_inline(on)

class CQuestManager {
public:
    CThing* GetActiveQuestCardFromScriptName(const CCharString& name);   // 0x004B0C80
};
extern CQuestManager* g_pQuestManager;                 // 0x013B89FC

class CTCBaseRoot {
public:
    void*   __vftable;                                 // +0x00
    CThing* Thing;                                     // +0x04
};
class CTCQuestCard : public CTCBaseRoot { };           // interface 0x6C
class CTCHeroStats : public CTCBaseRoot {              // interface 0x04
public:
    unsigned char _pad08[0x48 - 0x08];
    unsigned long Gold;                                // +0x48
    unsigned char _pad4c[0x7c - 0x4c];
    unsigned long Renown;                              // +0x7c
};
class CTCHeroExperience : public CTCBaseRoot {         // interface 0x68
public:
    unsigned char _pad08[0x10 - 0x08];
    unsigned long Experience;                          // +0x10
};

namespace std {
template <class _Tp> struct less { };
template <class _T1, class _T2> struct pair { _T1 first; _T2 second; };
template <class _Tp> class allocator { };
template <class _Key, class _Tp, class _Compare = less<_Key>, class _Alloc = allocator<pair<const _Key, _Tp> > >
class map {
public:
    void*        _M_header;
    unsigned int _M_node_count;
    _Compare     _M_key_compare;
    _Tp& operator[](const _Key& __k);                  // 0x005E9760 for map<long, unsigned long>
};
}

class CTCQuestCompletionUI : public CTCBaseRoot {
public:
    unsigned char _pad08[0x84 - 0x08];
    std::map<long, unsigned long> StartQuestExperience;   // +0x84
    std::map<long, unsigned long> StartQuestGold;         // +0x90
    std::map<long, unsigned long> StartQuestRenown;       // +0x9c
    void SaveInitialValues(const CCharString& questName);
};

void CTCQuestCompletionUI::SaveInitialValues(const CCharString& questName)
{
    CThing* cardThing = g_pQuestManager->GetActiveQuestCardFromScriptName(questName);
    CTCQuestCard* card;
    if (cardThing && cardThing->GetInterface(&card, 0x6c) && card) {
        unsigned short questIndex = card->Thing->DefGlobalIndex;
        if (questIndex) {
            CTCHeroStats* stats;
            if (Thing->GetInterface(&stats, 4)) {
                unsigned long renown = stats->Renown;
                StartQuestRenown[questIndex] = renown;
            }
            if (Thing->GetInterface(&stats, 4)) {
                long key = card->Thing->DefGlobalIndex;
                unsigned long gold = stats->Gold;
                StartQuestGold[key] = gold;
            }
            CTCHeroExperience* exp;
            if (Thing->GetInterface(&exp, 0x68)) {
                long key = card->Thing->DefGlobalIndex;
                unsigned long experience = exp->Experience;
                StartQuestExperience[key] = experience;
            }
        }
    }
}

// ---- test stand-ins ----
class CCharString { public: const char* Text; };
static CThing* g_cardForQuest = 0;
CThing* CQuestManager::GetActiveQuestCardFromScriptName(const CCharString& name)
{
    return std::strcmp(name.Text, "Q_ORCHARD") == 0 ? g_cardForQuest : 0;
}
static CQuestManager g_questManager;
CQuestManager* g_pQuestManager = &g_questManager;

typedef std::pair<long, unsigned long> MapEntry;
namespace std {
template <class _Key, class _Tp, class _Compare, class _Alloc>
_Tp& map<_Key, _Tp, _Compare, _Alloc>::operator[](const _Key& __k)
{                                                         // test stand-in: _M_header points at a flat entry table
    MapEntry* table = (MapEntry*)_M_header;
    for (unsigned int i = 0; i < _M_node_count; ++i)
        if (table[i].first == __k) return table[i].second;
    table[_M_node_count].first = __k; table[_M_node_count].second = 0;
    return table[_M_node_count++].second;
}
}

static int Fail() { std::printf("005e4860_TEST FAIL\n"); return 1; }
static void SetBit(CThing& t, long type) { t.m_presentBits[type >> 5] |= 1UL << (type & 31); }
static unsigned long Lookup(std::map<long, unsigned long>& m, long key, bool& found)
{
    MapEntry* table = (MapEntry*)m._M_header;
    for (unsigned int i = 0; i < m._M_node_count; ++i)
        if (table[i].first == key) { found = true; return table[i].second; }
    found = false;
    return 0;
}
static MapEntry g_tables[3][8];
static void ResetMaps(CTCQuestCompletionUI& ui)
{
    ui.StartQuestExperience._M_header = g_tables[0]; ui.StartQuestExperience._M_node_count = 0;
    ui.StartQuestGold._M_header = g_tables[1]; ui.StartQuestGold._M_node_count = 0;
    ui.StartQuestRenown._M_header = g_tables[2]; ui.StartQuestRenown._M_node_count = 0;
}

int main()
{
    static CThing hero, cardThing;
    std::memset(&hero, 0, sizeof hero); std::memset(&cardThing, 0, sizeof cardThing);
    CTCHeroStats stats; stats.Thing = &hero; stats.Gold = 300; stats.Renown = 500;
    CTCHeroExperience exp; exp.Thing = &hero; exp.Experience = 1234;
    CTCQuestCard card; card.Thing = &cardThing;
    CInterfaceMap::SEntry heroEntries[2] = { { 4, (CTCBase*)&stats }, { 0x68, (CTCBase*)&exp } };
    hero.m_interfaces.first = heroEntries; hero.m_interfaces.last = heroEntries + 2;
    SetBit(hero, 4); SetBit(hero, 0x68);
    CInterfaceMap::SEntry cardEntries[3] = { { 1, 0 }, { 0x6c, (CTCBase*)&card }, { 0x70, 0 } };
    cardThing.m_interfaces.first = cardEntries; cardThing.m_interfaces.last = cardEntries + 3;
    SetBit(cardThing, 0x6c);
    cardThing.DefGlobalIndex = 12;
    g_cardForQuest = &cardThing;

    CTCQuestCompletionUI ui; ui.Thing = &hero; ResetMaps(ui);
    CCharString quest; quest.Text = "Q_ORCHARD";
    CCharString other; other.Text = "Q_OTHER";
    bool found = false;
    ui.SaveInitialValues(quest);
    if (Lookup(ui.StartQuestRenown, 12, found) != 500 || !found) return Fail();
    if (Lookup(ui.StartQuestGold, 12, found) != 300 || !found) return Fail();
    if (Lookup(ui.StartQuestExperience, 12, found) != 1234 || !found) return Fail();

    ResetMaps(ui);
    ui.SaveInitialValues(other);                            // no active card for the quest
    if (ui.StartQuestRenown._M_node_count || ui.StartQuestGold._M_node_count || ui.StartQuestExperience._M_node_count) return Fail();

    cardThing.DefGlobalIndex = 0;                           // a card without a def index stores nothing
    ui.SaveInitialValues(quest);
    if (ui.StartQuestRenown._M_node_count || ui.StartQuestExperience._M_node_count) return Fail();

    cardThing.DefGlobalIndex = 40;                          // hero without the experience component
    hero.m_presentBits[0x68 >> 5] = 0;
    stats.Gold = 77; stats.Renown = 88;
    ui.SaveInitialValues(quest);
    if (Lookup(ui.StartQuestRenown, 40, found) != 88 || Lookup(ui.StartQuestGold, 40, found) != 77) return Fail();
    if (ui.StartQuestExperience._M_node_count != 0) return Fail();

    cardThing.m_presentBits[0x6c >> 5] = 0;                 // the card thing lost its quest-card component
    ResetMaps(ui);
    ui.SaveInitialValues(quest);
    if (ui.StartQuestRenown._M_node_count || ui.StartQuestGold._M_node_count) return Fail();
    std::printf("005e4860_TEST PASS\n");
    return 0;
}