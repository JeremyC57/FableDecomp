#include <cstdio>
// Behaviour test for CTCHeroReceiveItems::GiveItems (0x007CA9E0): same body as src/, with Create, SetInLimbo, the list
// insert, the intelligent-pointer primitives and PrepareToShowItems stubbed and recorded.
class CThing;
class CThingObject;
class CRightHandedSet;
class C3DVector { public: float X, Y, Z; };
class CCharString { public: const char* m_pData; };

class CThing {
public:
    C3DVector pos;
    unsigned char m_flags91;
    bool inLimbo;
    const C3DVector& GetPosition() const { return pos; }
    void SetInLimbo(bool l) { inLimbo = l; }
};
static CThing g_created[4];
static int g_createCount = 0;
static long g_lastPlayer = -1;
static const C3DVector* g_lastPos = 0;
class CThingObject : public CThing {
public:
    static CThingObject* Create(long defIndex, const C3DVector& position, long player,
                                const CRightHandedSet* orientation, unsigned long uid, const CCharString& scriptName)
    {
        g_lastPlayer = player; g_lastPos = &position;
        if (defIndex == 99 || orientation || uid || !scriptName.m_pData) return 0;   // 99: creation fails
        CThing* t = &g_created[g_createCount++];
        t->m_flags91 = (unsigned char)(defIndex == 7 ? 1 : 0);                   // 7: created dead
        return (CThingObject*)t;
    }
};
class CTCBase { public: CThing* ParentThing; };
struct CBaseIntelligentPointer {
    CThing* m_pItem;
    CBaseIntelligentPointer() : m_pItem(0) { }
    CThing* GetPItem() const { return m_pItem; }
    void SetPItem(CThing* item) { m_pItem = item; }
};
template <class T> class CIntelligentPointer : public CBaseIntelligentPointer { public:
    CIntelligentPointer() { }
    CIntelligentPointer(T* item) { SetPItem((CThing*)item); }
    CIntelligentPointer(const CIntelligentPointer& other) { SetPItem(other.GetPItem()); }
    CIntelligentPointer& operator=(const CIntelligentPointer& other)
    {
        CThing* item = other.GetPItem();
        SetPItem(item);
        return *this;
    }
};
template <class T1, class T2> struct pair {
    T1 first;
    T2 second;
    pair(const T1& a, const T2& b) : first(a), second(b) { }
};
typedef pair<CIntelligentPointer<CThingObject>, bool> SItemEntry;
struct SItemNode { SItemNode* _M_next; SItemNode* _M_prev; CThing* item; bool flag; };
struct CItemListIterator {
    SItemNode* _M_node;
    CItemListIterator(SItemNode* node) : _M_node(node) { }
    CItemListIterator(const CItemListIterator& other) : _M_node(other._M_node) { }
};
static SItemNode g_nodes[4];
static int g_nodeCount = 0;
struct CItemList {
    SItemNode* _M_node;
    CItemListIterator end() { return CItemListIterator(_M_node); }
    CItemListIterator insert(CItemListIterator position, const SItemEntry& x)
    {
        SItemNode* n = &g_nodes[g_nodeCount++];
        n->item = x.first.GetPItem(); n->flag = x.second;
        n->_M_next = position._M_node; n->_M_prev = position._M_node->_M_prev;
        position._M_node->_M_prev->_M_next = n; position._M_node->_M_prev = n;
        return CItemListIterator(n);
    }
    void push_back(const SItemEntry& x) { insert(end(), x); }
};
struct SGiveItem { long DefIndex; CCharString ScriptName; };
struct CGiveItemVector {
    SGiveItem* _M_start;
    SGiveItem* _M_finish;
    SGiveItem* _M_end_of_storage;
    unsigned int size() const { return _M_finish - _M_start; }
    const SGiveItem& operator[](unsigned int n) const { return _M_start[n]; }
};
static const C3DVector* g_preparedAt = 0;
class CTCHeroReceiveItems : public CTCBase { public:
    CItemList Items;
    bool SendMessage;
    CIntelligentPointer<CThing> PContainer;
    void PrepareToShowItems(const C3DVector& position) { g_preparedAt = &position; }
    void GiveItems(const CGiveItemVector& items, CThing& container, bool sendMessage);
};

void CTCHeroReceiveItems::GiveItems(const CGiveItemVector& items, CThing& container, bool sendMessage)
{
    for (unsigned int i = 0; i < items.size(); ++i) {
        if (items[i].DefIndex > 0) {
            CThingObject* item = CThingObject::Create(items[i].DefIndex, ParentThing->GetPosition(), 5, 0, 0,
                                                      items[i].ScriptName);
            if (item && !(item->m_flags91 & 1)) {
                item->SetInLimbo(true);
                Items.push_back(SItemEntry(CIntelligentPointer<CThingObject>(item), false));
            }
        }
    }
    SendMessage = sendMessage;
    PContainer = CIntelligentPointer<CThing>(&container);
    PrepareToShowItems(PContainer.GetPItem()->GetPosition());
}

int main()
{
    int fails = 0;
    CThing hero; hero.pos.X = 1.0f;
    CThing chest; chest.pos.X = 2.0f;
    SItemNode sentinel; sentinel._M_next = &sentinel; sentinel._M_prev = &sentinel;
    CTCHeroReceiveItems tc;
    tc.ParentThing = &hero; tc.Items._M_node = &sentinel; tc.SendMessage = false;
    SGiveItem list[5] = { { 3, { "a" } }, { 0, { "b" } }, { 99, { "c" } }, { 7, { "d" } }, { 4, { "e" } } };
    CGiveItemVector v = { list, list + 5, list + 5 };
    tc.GiveItems(v, chest, true);
    if (g_createCount != 3 || g_nodeCount != 2) ++fails;            // def 0 skipped, 99 failed, 7 dead
    if (g_lastPlayer != 5 || g_lastPos != &hero.pos) ++fails;
    if (sentinel._M_next != &g_nodes[0] || g_nodes[0]._M_next != &g_nodes[1] || sentinel._M_prev != &g_nodes[1]) ++fails;
    if (g_nodes[0].item != &g_created[0] || g_nodes[1].item != &g_created[2] || g_nodes[0].flag || g_nodes[1].flag) ++fails;
    if (!g_created[0].inLimbo || !g_created[2].inLimbo || g_created[1].inLimbo) ++fails;
    if (!tc.SendMessage || tc.PContainer.GetPItem() != &chest || g_preparedAt != &chest.pos) ++fails;
    if (fails) { std::printf("007ca9e0_TEST FAIL\n"); return 1; }
    std::printf("007ca9e0_TEST PASS\n");
    return 0;
}