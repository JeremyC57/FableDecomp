// Behaviour test for 0x0076D1B0 (source body copied from src/0076d1b0.cpp; out-of-line callees stubbed below).
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#ifdef _MSC_VER
inline void* operator new(unsigned int, void* p) { return p; }
#else
#include <new>
#define __fastcall
#define __cdecl
#define __stdcall
#define __fastcall
#define __forceinline inline
#define __declspec(x)
#endif
class CThing;
class CBaseObject;
class CTCBase;

struct CBaseIntelligentPointer {
    virtual void v00();
    CThing* m_pItem;                                // +0x04
    CBaseIntelligentPointer();                      // 0x00A01B10
    ~CBaseIntelligentPointer();                     // 0x00A01C10
    void SetPItem(CBaseObject* item);               // 0x00A01B90
    CThing* GetPItem() const;                       // 0x00A01B50
};
template<class T> class CIntelligentPointer : public CBaseIntelligentPointer { public:   // CIntelligentPointer<const CThing>, vtable 0x01237F1C
    virtual void v00();
    CIntelligentPointer(T* const& item) : CBaseIntelligentPointer() { SetPItem((CBaseObject*)item); }
};

namespace std {
struct input_iterator_tag {
    input_iterator_tag() { }
};
}

struct SThingListNode {
    SThingListNode* _M_next;
    SThingListNode* _M_prev;
};
struct CThingListIterator {
    SThingListNode* _M_node;
    CThingListIterator(SThingListNode* node) : _M_node(node) { }
    CThingListIterator(const CThingListIterator& other) : _M_node(other._M_node) { }
    bool operator==(const CThingListIterator& other) const { return _M_node == other._M_node; }
};

struct SThingListValueNode : SThingListNode {
    CIntelligentPointer<const CThing> _M_data;      // +0x08
};
namespace std {
#pragma auto_inline(off)
// Same-TU stand-in for the STLport COMDAT 0x0076D5D0 (the retail body: it reads the value and never touches the tag),
// so VC7.1 sees that neither reference escapes: the tag shares the `enemy` slot and `enemy` stays in a register.
__declspec(noinline) CThingListIterator __fastcall __find(CThingListIterator first, CThingListIterator last,
                                                          CThing* const& value, const input_iterator_tag&)   // 0x0076D5D0
{
    while (!(first == last) && ((SThingListValueNode*)first._M_node)->_M_data.GetPItem() != value)
        first._M_node = first._M_node->_M_next;
    return first;
}
#pragma auto_inline(on)
inline CThingListIterator find(CThingListIterator first, CThingListIterator last, CThing* const& value)
{
    return __find(first, last, value, input_iterator_tag());
}
}

template<class T> struct CRetailList {              // STLport list: the sentinel node pointer
    SThingListNode* _M_node;
    CThingListIterator begin() const { return CThingListIterator(_M_node->_M_next); }
    CThingListIterator end() const { return CThingListIterator(_M_node); }
    void push_back(const T& value);                 // 0x0065617D
};

struct CInterfaceMap {                              // CVectorMap<ETCInterfaceType, CTCBase*> {first, last}
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);           // 0x0040F020
};

class CTCCreatureOpinionOfHero {                    // interface 0x4F
public:
    void SetHeroOpinionEnemy(bool enemy);           // 0x006815D0
};

class CThing {
public:
    unsigned char _pad00[0x20];
    unsigned long UsingComponents[9];               // +0x20 bitset<274>
    CInterfaceMap m_interfaces;                     // +0x44
    unsigned long HasTC(long type) const { return UsingComponents[type >> 5] & (1UL << (type & 31)); }
    __forceinline bool GetInterface(long type, CTCBase** out)     // the landed 0x007504E0 out-parameter shape
    {
        if (UsingComponents[type >> 5] & (1UL << (type & 31))) {
            long key = type;
            CInterfaceMap* map = &m_interfaces;
            CInterfaceMap::SEntry* it = map->LowerBound(&key);
            if (it == map->last || type < it->type)
                it = map->last;
            *out = it->component;
            return true;
        }
        return false;
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
    void AddEnemyThing(const CThing* enemy);
};

void CTCEnemy::AddEnemyThing(const CThing* enemy)
{
    CThingListIterator end = EnemyThings.end();
    if (std::find(EnemyThings.begin(), end, const_cast<CThing* const&>(enemy)) == end) {
        EnemyThings.push_back(CIntelligentPointer<const CThing>(enemy));
        if (enemy->HasTC(0x51)) {
            CTCBase* opinion;
            if (ParentThing->GetInterface(0x4f, &opinion))
                ((CTCCreatureOpinionOfHero*)opinion)->SetHeroOpinionEnemy(true);
        }
    }
}

// ---- test harness ----
static int g_ctor = 0, g_dtor = 0, g_push = 0, g_opinion = 0, g_opinionValue = -1;
static CTCCreatureOpinionOfHero* g_opinionThis = 0;
void CBaseIntelligentPointer::v00() { }
template <class T> void CIntelligentPointer<T>::v00() { }
CBaseIntelligentPointer::CBaseIntelligentPointer() : m_pItem(0) { ++g_ctor; }
CBaseIntelligentPointer::~CBaseIntelligentPointer() { ++g_dtor; }
void CBaseIntelligentPointer::SetPItem(CBaseObject* item) { m_pItem = (CThing*)item; }
CThing* CBaseIntelligentPointer::GetPItem() const { return m_pItem; }
template <class T> void CRetailList<T>::push_back(const T& value)
{
    ++g_push;
    SThingListValueNode* n = (SThingListValueNode*)malloc(sizeof(SThingListValueNode));
    memcpy((void*)&n->_M_data, (const void*)&value, sizeof(value));
    n->_M_next = _M_node;
    n->_M_prev = _M_node->_M_prev;
    _M_node->_M_prev->_M_next = n;
    _M_node->_M_prev = n;
}
CInterfaceMap::SEntry* CInterfaceMap::LowerBound(const long* type)
{
    SEntry* p = first;
    while (p != last && p->type < *type) ++p;
    return p;
}
void CTCCreatureOpinionOfHero::SetHeroOpinionEnemy(bool enemy) { ++g_opinion; g_opinionValue = enemy ? 1 : 0; g_opinionThis = this; }
void CTCEnemy::v00() { }
static void SetBit(CThing* t, long type) { t->UsingComponents[type >> 5] |= (1UL << (type & 31)); }
int main()
{
    int ok = 1;
    static CThing a, b, c, parent;
    memset(&a, 0, sizeof a); memset(&b, 0, sizeof b); memset(&c, 0, sizeof c); memset(&parent, 0, sizeof parent);
    SetBit(&b, 0x51);                                  // b is the hero (deed log)
    SetBit(&parent, 0x4f);
    static unsigned char opinionStore[4];
    CInterfaceMap::SEntry entries[3];
    entries[0].type = 0x10; entries[0].component = 0;
    entries[1].type = 0x4f; entries[1].component = (CTCBase*)opinionStore;
    entries[2].type = 0x60; entries[2].component = 0;
    parent.m_interfaces.first = entries; parent.m_interfaces.last = entries + 3;
    SThingListNode sentinel; sentinel._M_next = sentinel._M_prev = &sentinel;
    CTCEnemy e; memset((void*)&e, 0, sizeof e);
    e.ParentThing = &parent; e.EnemyThings._M_node = &sentinel;
    e.AddEnemyThing(&a);                               // new, not the hero: pushed, no opinion
    if (g_push != 1 || g_opinion != 0) ok = 0;
    e.AddEnemyThing(&a);                               // already present
    if (g_push != 1 || g_opinion != 0) ok = 0;
    e.AddEnemyThing(&b);                               // the hero: pushed and the opinion set
    if (g_push != 2 || g_opinion != 1 || g_opinionValue != 1 || g_opinionThis != (CTCCreatureOpinionOfHero*)opinionStore) ok = 0;
    SThingListValueNode* n1 = (SThingListValueNode*)sentinel._M_next;
    SThingListValueNode* n2 = (SThingListValueNode*)n1->_M_next;
    if (n1->_M_data.m_pItem != &a || n2->_M_data.m_pItem != &b || n2->_M_next != &sentinel) ok = 0;
    if (g_ctor != 2 || g_dtor != 2) ok = 0;            // one temporary per push
    printf("0076d1b0_TEST %s\n", ok ? "PASS" : "FAIL");
    return ok ? 0 : 1;
}