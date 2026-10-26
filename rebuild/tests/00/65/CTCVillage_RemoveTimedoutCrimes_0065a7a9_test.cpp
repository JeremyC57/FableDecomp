// Behaviour test for 0065a7a9: a self-contained copy of the candidate's model and body, run over stub callees.
// CTCVillage::RemoveTimedoutCrimes() -- drops every crime handle the hero's deed log no longer knows (the deed timed out); with no crimes left the village is no longer the hero's enemy because of crime
#include <cstdio>
#include <cstring>
#ifndef _MSC_VER
#define __forceinline inline
#define __fastcall
#define __stdcall
#define __cdecl
#define __declspec(x)
#endif
class CTCBase;

struct CBitset {
    bool test(unsigned long position) const;    // 0x00410DE0
};
struct CInterfaceMap {                          // CVectorMap<ETCInterfaceType, CTCBase*> at CThing+0x44: {first, last}
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);       // 0x0040F020
};
class CThing { public:
    unsigned char _pad00[0x20];
    CBitset       m_present;                    // +0x20
    unsigned char _pad24[0x20];
    CInterfaceMap m_interfaces;                 // +0x44
    bool HasTC(long type) const { return m_present.test(type); }
    __forceinline CTCBase* GetTC(long interfaceType)
    {
        long type = interfaceType;
        CInterfaceMap* map = &m_interfaces;
        CInterfaceMap::SEntry* it = map->LowerBound(&type);
        if (it == map->last || interfaceType < it->type)
            it = map->last;
        return it->component;
    }
    template <class T> __forceinline bool GetInterface(T** ppOut, long interfaceType)
    {
        if (HasTC(interfaceType)) {
            *ppOut = (T*)GetTC(interfaceType);
            return true;
        }
        return false;
    }
};

class CPlayer { public:
    CThing* GetHeroThing();                     // 0x00487DC0
};
class CWorld { public:
    CPlayer* GetCurrentPlayer() const;          // 0x0049D9D0
};
class CMainGameComponent { public:
    unsigned char _pad[0x24];
    CWorld*       World;                        // +0x24
};
extern CMainGameComponent* g_pMainGameComponent;   // 0x013B86A0


class COpinionDeed { public:
    long DeedType;                              // +0x00 EOpinionDeedType
};
class CTCHeroOpinionDeedLog { public:
    COpinionDeed* GetDeedFromHandle(long handle);   // 0x006C58F0
};

struct SNode {                                  // STLport _Rb_tree_node<long>
    long   _M_color;
    SNode* _M_parent;
    SNode* _M_left;
    SNode* _M_right;
    long   _M_value_field;                      // +0x10
};
SNode* __cdecl Successor(SNode* node);          // 0x004292C0

struct SCrimeIterator {                         // STLport set<long>::iterator
    SNode* _M_node;
    SCrimeIterator(SNode* node) : _M_node(node) { }
    SCrimeIterator(const SCrimeIterator& other) : _M_node(other._M_node) { }
    long operator*() const { return _M_node->_M_value_field; }
    SCrimeIterator& operator++() { _M_node = Successor(_M_node); return *this; }
    bool operator!=(const SCrimeIterator& other) const { return _M_node != other._M_node; }
};
struct CCrimeSet {                              // retail STLport set<long>, 12 bytes
    SNode*        _M_header;                    // +0x00
    unsigned long _M_node_count;                // +0x04
    unsigned long _M_key_compare;               // +0x08 (empty less<long>, padded)
    SCrimeIterator begin() const { return SCrimeIterator(_M_header->_M_left); }
    SCrimeIterator end() const { return SCrimeIterator(_M_header); }
    unsigned long size() const { return _M_node_count; }
    void erase(SCrimeIterator position);        // 0x00638D70
};

class CTCVillage { public:
    unsigned char _pad_0x00[0x64];
    CCrimeSet     PlayersCrimes;                // +0x64
    unsigned char _pad_0x70[0xc];
    bool          IsEnemyBecauseOfCrime;        // +0x7c
    void RemoveTimedoutCrimes();
};

void CTCVillage::RemoveTimedoutCrimes()
{
    CThing* hero = g_pMainGameComponent->World->GetCurrentPlayer()->GetHeroThing();
    if (hero) {
        CTCHeroOpinionDeedLog* log;
        hero->GetInterface(&log, 0x51);
        CCrimeSet* crimes = &PlayersCrimes;
        SCrimeIterator end = crimes->end();
        SCrimeIterator it = crimes->begin();
        SCrimeIterator next = it;
        while (it != end) {
            ++next;
            if (!log->GetDeedFromHandle(*it))
                crimes->erase(it);
            it = next;
        }
    }
    if (PlayersCrimes.size() == 0)
        IsEnemyBecauseOfCrime = false;
}

// ---- stubs and driver ----
static bool g_hasLog = true;
bool CBitset::test(unsigned long position) const { return position == 0x51 && g_hasLog; }
static CInterfaceMap::SEntry g_entries[1];
CInterfaceMap::SEntry* CInterfaceMap::LowerBound(const long* type)
{ SEntry* it = first; while (it != last && it->type < *type) ++it; return it; }
static CThing g_hero; static CThing* g_heroPtr = &g_hero;
static CPlayer g_player; static CWorld g_world; static CMainGameComponent g_component;
CMainGameComponent* g_pMainGameComponent = &g_component;
CThing* CPlayer::GetHeroThing() { return g_heroPtr; }
CPlayer* CWorld::GetCurrentPlayer() const { return &g_player; }
static COpinionDeed g_deed;
static long g_known[4] = { 1, 0, 1, 0 };     // handles 0 and 2 still have deeds
COpinionDeed* CTCHeroOpinionDeedLog::GetDeedFromHandle(long handle) { return g_known[handle] ? &g_deed : 0; }
SNode* __cdecl Successor(SNode* node) { return node->_M_parent; }   // the test links its nodes through _M_parent
static long g_erased[4]; static int g_erases;
void CCrimeSet::erase(SCrimeIterator position)
{   // unlink the node from the test's _M_parent chain
    if (g_erases < 4) g_erased[g_erases] = position._M_node->_M_value_field; ++g_erases;
    SNode** link = &_M_header->_M_left;
    while (*link != position._M_node) link = &(*link)->_M_parent;
    *link = position._M_node->_M_parent;
    --_M_node_count;
}
static CTCHeroOpinionDeedLog g_log;

int main()
{
    g_component.World = &g_world;
    g_entries[0].type = 0x51; g_entries[0].component = (CTCBase*)&g_log;
    g_hero.m_interfaces.first = g_entries; g_hero.m_interfaces.last = g_entries + 1;
    static SNode header, n[4];
    static CTCVillage village;
    header._M_left = &n[0];
    for (int i = 0; i < 4; ++i) { n[i]._M_value_field = i; n[i]._M_parent = i < 3 ? &n[i + 1] : &header; }
    village.PlayersCrimes._M_header = &header; village.PlayersCrimes._M_node_count = 4;
    village.IsEnemyBecauseOfCrime = true;
    village.RemoveTimedoutCrimes();
    if (g_erases != 2 || g_erased[0] != 1 || g_erased[1] != 3 || village.PlayersCrimes._M_node_count != 2 || !village.IsEnemyBecauseOfCrime
        || header._M_left != &n[0] || n[0]._M_parent != &n[2] || n[2]._M_parent != &header) { std::printf("0065a7a9_TEST FAIL (1)\n"); return 1; }
    g_known[0] = 0; g_known[2] = 0;
    village.RemoveTimedoutCrimes();
    if (g_erases != 4 || village.PlayersCrimes._M_node_count != 0 || village.IsEnemyBecauseOfCrime) { std::printf("0065a7a9_TEST FAIL (2)\n"); return 1; }
    // no hero: only the empty-set check
    g_heroPtr = 0; village.IsEnemyBecauseOfCrime = true;
    village.RemoveTimedoutCrimes();
    if (village.IsEnemyBecauseOfCrime) { std::printf("0065a7a9_TEST FAIL (3)\n"); return 1; }
    std::printf("0065a7a9_TEST PASS\n");
    return 0;
}