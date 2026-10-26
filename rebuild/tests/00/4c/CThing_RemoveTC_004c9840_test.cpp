// Behaviour test for 004c9840: body copied from src/004c9840.cpp (CThing::RemoveTC), callees stubbed.
#include <cstdio>
#include <cstring>
static int fails = 0;
#define CHECK(c) do { if (!(c)) { ++fails; printf("FAIL %d: %s\n", __LINE__, #c); } } while (0)
class CThing;
static int g_setPItem = 0;
struct CBaseIntelligentPointer {
    virtual void v00() {}
    CThing* m_pItem;
    CBaseIntelligentPointer() : m_pItem(0) {}
    ~CBaseIntelligentPointer() {}
    CThing* GetPItem() const { return m_pItem; }
    void SetPItem(CThing* item) { ++g_setPItem; m_pItem = item; }
};
struct CIntelligentPointer : CBaseIntelligentPointer {
    virtual void v00() {}
    CIntelligentPointer(CThing* item) { SetPItem(item); }
    CIntelligentPointer(const CIntelligentPointer& other) { SetPItem(other.GetPItem()); }
};
static int g_onRemove = 0; static int g_getType = 0;
class CTCBase { public:
    virtual void v00() {} virtual void v01() {}
    virtual void OnRemove() { ++g_onRemove; CHECK(PendingRemoval); }
    virtual void v03() {} virtual void v04() {} virtual void v05() {} virtual void v06() {} virtual void v07() {} virtual void v08() {}
    virtual void v09() {} virtual void v10() {} virtual void v11() {} virtual void v12() {} virtual void v13() {} virtual void v14() {}
    virtual void v15() {} virtual void v16() {} virtual void v17() {} virtual void v18() {} virtual void v19() {} virtual void v20() {}
    virtual long GetInterfaceType() { ++g_getType; return Type; }
    void* PThing;
    bool  PendingRemoval;
    long  Type;
};
struct STCRemoval {
    CIntelligentPointer PThing;
    CTCBase*            PTC;
    STCRemoval(const CIntelligentPointer& thing, CTCBase* tc) : PThing(thing), PTC(tc) { }
};
struct STCRemovalNode { STCRemovalNode* next; STCRemovalNode* prev; STCRemoval value; };
struct STCRemovalIterator {
    STCRemovalNode* node;
    STCRemovalIterator(STCRemovalNode* n) : node(n) { }
    STCRemovalIterator(const STCRemovalIterator& o) : node(o.node) { }
};
static int g_inserts = 0; static CThing* g_insertThing = 0; static CTCBase* g_insertTC = 0; static STCRemovalNode* g_insertWhere = 0;
struct CTCRemovalList {
    STCRemovalNode* head;
    STCRemovalIterator end() { return STCRemovalIterator(head); }
    struct SInsertResult { STCRemovalNode* node; };
    SInsertResult insert(STCRemovalIterator where, const STCRemoval& value) { ++g_inserts; g_insertWhere = where.node; g_insertThing = value.PThing.GetPItem(); g_insertTC = value.PTC; SInsertResult r = { 0 }; return r; }
    void push_back(const STCRemoval& value) { insert(end(), value); }
};
CTCRemovalList g_TCRemovalList;
static int g_resets = 0; static unsigned int g_resetPos = 0;
struct CTCBitset {
    unsigned long m_words[9];
    CTCBitset& reset(unsigned int pos) { ++g_resets; g_resetPos = pos; m_words[pos >> 5] &= ~(1UL << (pos & 31)); return *this; }
};
struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);
    SEntry* find(const long& type)
    {
        SEntry* it = LowerBound(&type);
        if (it == last || type < it->type)
            it = last;
        return it;
    }
    SEntry* end() const { return last; }
};
CInterfaceMap::SEntry* CInterfaceMap::LowerBound(const long* type)
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
class CThing { public:
    void*         __vftable;
    unsigned char _pad04[0x1c];
    CTCBitset     m_presentBits;
    CInterfaceMap m_interfaces;
    void RemoveTC(long type);
};
void CThing::RemoveTC(long type)
{
    CInterfaceMap::SEntry* it = m_interfaces.find(type);
    if (it == m_interfaces.end())
        return;
    CTCBase* tc = it->component;
    if (tc->PendingRemoval)
        return;
    tc->PendingRemoval = true;
    m_presentBits.reset(tc->GetInterfaceType());
    g_TCRemovalList.push_back(STCRemoval(CIntelligentPointer(this), tc));
    tc->OnRemove();
}
int main()
{
    static CThing thing; static CTCBase tcA, tcB; static unsigned char headStorage[sizeof(STCRemovalNode)]; STCRemovalNode& headNode = *(STCRemovalNode*)headStorage;
    memset(&thing, 0, sizeof(thing)); tcA.PendingRemoval = false; tcA.Type = 5; tcB.PendingRemoval = false; tcB.Type = 40; g_TCRemovalList.head = &headNode;
    CInterfaceMap::SEntry entries[3] = { { 5, &tcA }, { 12, 0 }, { 40, &tcB } };
    thing.m_interfaces.first = entries; thing.m_interfaces.last = entries + 3; thing.m_presentBits.m_words[0] = (1UL << 5) | (1UL << 12); thing.m_presentBits.m_words[1] = 1UL << 8;
    // absent type (between entries, and past the end): nothing happens
    thing.RemoveTC(7); thing.RemoveTC(99); CHECK(g_inserts == 0 && g_resets == 0 && g_onRemove == 0);
    // present: flagged, bit cleared, queued with this thing, notified
    thing.RemoveTC(40);
    CHECK(tcB.PendingRemoval && g_getType == 1 && g_resets == 1 && g_resetPos == 40 && thing.m_presentBits.m_words[1] == 0); CHECK(g_inserts == 1 && g_insertWhere == &headNode && g_insertThing == &thing && g_insertTC == &tcB && g_onRemove == 1);
    // already pending: nothing more
    thing.RemoveTC(40); CHECK(g_inserts == 1 && g_onRemove == 1 && g_resets == 1);
    // the first entry
    thing.RemoveTC(5); CHECK(tcA.PendingRemoval && g_inserts == 2 && g_insertTC == &tcA && g_resetPos == 5 && thing.m_presentBits.m_words[0] == (1UL << 12) && g_onRemove == 2);
    if (fails == 0) printf("004c9840_TEST PASS\n"); else printf("004c9840_TEST FAIL\n");
    return fails;
}