#ifndef _MSC_VER
#define __forceinline inline
#define __fastcall
#define __stdcall
#define __cdecl
#endif
#include <cstdio>
#include <cstring>
class CThing;
class CBaseObject;
class CTCBase;

struct CBaseIntelligentPointer {
    virtual void v00();
    CThing* m_pItem;                                // +0x04
    CBaseIntelligentPointer();                      // 0x00A01B10
    ~CBaseIntelligentPointer();                     // 0x00A01C10
    void SetPItem(CBaseObject* item);               // 0x00A01B90
};
template<class T> struct CIntelligentPointer : CBaseIntelligentPointer {   // CIntelligentPointer<const CThing>, vtable 0x01237F1C
    virtual void v00();
    CIntelligentPointer(T* const& item) : CBaseIntelligentPointer() { SetPItem((CBaseObject*)item); }
};

struct SThingListNode;
struct CThingListIterator {
    SThingListNode* m_pNode;
    CThingListIterator() { }
    CThingListIterator(SThingListNode* node) : m_pNode(node) { }
    CThingListIterator(const CThingListIterator& other) : m_pNode(other.m_pNode) { }
};
struct SThingListNode {
    SThingListNode* _M_next;
    SThingListNode* _M_prev;
};
CThingListIterator __fastcall FindThingInList(CThingListIterator first, CThingListIterator last, CThing* const& value);   // 0x0076D6F0

template<class T> struct CRetailList {              // STLport list: the sentinel node pointer
    SThingListNode* _M_node;
    CThingListIterator begin() const { return CThingListIterator(_M_node->_M_next); }
    CThingListIterator end() const { return CThingListIterator(_M_node); }
    void remove(const T& value);                    // 0x0076D7B0
};

struct CInterfaceMap {                              // CVectorMap<ETCInterfaceType, CTCBase*> {first, last}
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);           // 0x0040F020
};

class CTCCreatureOpinionOfHero {                    // interface 0x4F
public:
    bool IsHeroOpinionEnemy();                      // 0x006827A0
    void SetHeroOpinionEnemy(bool enemy);           // 0x006815D0
};

class CThing {
public:
    unsigned char _pad00[0x20];
    unsigned long UsingComponents[9];               // +0x20 bitset<274>
    CInterfaceMap m_interfaces;                     // +0x44
    bool HasTC(long type) const { return (UsingComponents[type >> 5] >> (type & 31)) & 1; }
    __forceinline CTCBase* GetTC(long type)
    {
        long key = type;
        CInterfaceMap* map = &m_interfaces;
        CInterfaceMap::SEntry* it = map->LowerBound(&key);
        CInterfaceMap::SEntry* end = map->last;
        return (it == end || type < it->type ? end : it)->component;
    }
};

class CTCEnemy {
public:
    virtual void v00();
    CThing* ParentThing;                            // +0x04 (CTCBase)
    unsigned char _pad08[4];
    bool FriendsWithEverythingFlag;                 // +0x0C
    bool EnableFollowersEnemyProxy;                 // +0x0D
    CRetailList< CIntelligentPointer<const CThing> > EnemyThings;   // +0x10
    void RemoveEnemyThing(const CThing* enemy);
};

void CTCEnemy::RemoveEnemyThing(const CThing* enemy)
{
    if (enemy) {
        bool isHero = enemy->HasTC(0x51);
        if (isHero) {
            bool hasOpinion = ParentThing->HasTC(0x4f);
            if (hasOpinion) {
                CTCCreatureOpinionOfHero* opinion = (CTCCreatureOpinionOfHero*)ParentThing->GetTC(0x4f);
                if (opinion->IsHeroOpinionEnemy()) {
                    if (FindThingInList(EnemyThings.begin(), EnemyThings.end(), const_cast<CThing* const&>(enemy)).m_pNode != EnemyThings._M_node)
                        opinion->SetHeroOpinionEnemy(false);
                }
            }
        }
    }
    EnemyThings.remove(CIntelligentPointer<const CThing>(enemy));
}

// ---- test stubs for the retail callees ----
struct SNode : SThingListNode { CIntelligentPointer<const CThing> Value; SNode(const CThing* t) : Value(t) {} };
static bool g_isEnemy; static int g_setCalls; static bool g_setValue; static const CThing* g_removed; static int g_dtors;
CBaseIntelligentPointer::CBaseIntelligentPointer() : m_pItem(0) {}
CBaseIntelligentPointer::~CBaseIntelligentPointer() { ++g_dtors; }
void CBaseIntelligentPointer::SetPItem(CBaseObject* item) { m_pItem = (CThing*)item; }
void CBaseIntelligentPointer::v00() {}
template<class T> void CIntelligentPointer<T>::v00() {}
CThingListIterator __fastcall FindThingInList(CThingListIterator first, CThingListIterator last, CThing* const& value)
{
    for (; first.m_pNode != last.m_pNode; first.m_pNode = first.m_pNode->_M_next)
        if (((SNode*)first.m_pNode)->Value.m_pItem == value) break;
    return first;
}
template<class T> void CRetailList<T>::remove(const T& value) { g_removed = (const CThing*)value.m_pItem; }
CInterfaceMap::SEntry* CInterfaceMap::LowerBound(const long* type)
{ SEntry* it = first; while (it != last && it->type < *type) ++it; return it; }
bool CTCCreatureOpinionOfHero::IsHeroOpinionEnemy() { return g_isEnemy; }
void CTCCreatureOpinionOfHero::SetHeroOpinionEnemy(bool enemy) { ++g_setCalls; g_setValue = enemy; }
void CTCEnemy::v00() {}

int main()
{
    int ok = 1;
    CTCCreatureOpinionOfHero opinion;
    CInterfaceMap::SEntry e; e.type = 0x4f; e.component = (CTCBase*)&opinion;
    CThing owner, hero, other;
    std::memset(&owner, 0, sizeof(owner)); std::memset(&hero, 0, sizeof(hero)); std::memset(&other, 0, sizeof(other));
    owner.m_interfaces.first = &e; owner.m_interfaces.last = &e + 1; owner.UsingComponents[2] = 1u << 15;
    hero.UsingComponents[2] = 1u << 17;
    SThingListNode head; SNode n1(&other), n2(&hero);
    head._M_next = &n1; n1._M_next = &n2; n2._M_next = &head;
    CTCEnemy tc; tc.ParentThing = &owner; tc.EnemyThings._M_node = &head;
    g_isEnemy = true;
    tc.RemoveEnemyThing(&hero);         // hero listed + opinion enemy -> opinion cleared, removed
    if (g_setCalls != 1 || g_setValue != false || g_removed != &hero) ok = 0;
    n2.Value.m_pItem = 0;               // hero no longer in the list -> opinion kept
    tc.RemoveEnemyThing(&hero);
    if (g_setCalls != 1 || g_removed != &hero) ok = 0;
    n2.Value.m_pItem = (CThing*)&hero; g_isEnemy = false;
    tc.RemoveEnemyThing(&hero);         // opinion not enemy
    if (g_setCalls != 1) ok = 0;
    g_isEnemy = true;
    tc.RemoveEnemyThing(&other);        // not the hero
    if (g_setCalls != 1 || g_removed != &other) ok = 0;
    tc.RemoveEnemyThing(0);             // null: only the remove
    if (g_setCalls != 1 || g_removed != 0) ok = 0;
    if (!ok) { std::printf("0076cc00_TEST FAIL\n"); return 1; }
    std::printf("0076cc00_TEST PASS\n");
    return 0;
}
