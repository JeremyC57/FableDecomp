// behaviour test for 0071b380 CTCThingOwner::OnLevelUnload: every owned object that is alive and level-persistent is
// disowned (its owned-entity component's owner cleared), in reverse order; the local list frees every node
#include <cstdio>
#include <cstdlib>
#include <new>
class CThing;
struct CTCBase;
static int g_mallocs = 0, g_frees = 0;
static void* tmalloc(unsigned int n) { ++g_mallocs; return std::malloc(n); }
static void tfree(void* p) { ++g_frees; std::free(p); }
#define malloc tmalloc
#define free tfree
struct CInterfaceEntry { long type; CTCBase* component; };
static int g_order[8]; static int g_disowned = 0;
struct CTCOwnedEntity {
    int id; CThing* owner;
    void SetPOwner(CThing* o) { owner = o; g_order[g_disowned++] = id; }
};
class CThing {
public:
    unsigned long    m_presentBits[9];
    CInterfaceEntry* m_interfacesFirst;
    CInterfaceEntry* m_interfacesLast;
    unsigned char    m_flags91;
    bool             m_persistent;
    bool IsDead() const { return (m_flags91 & 1) != 0; }
    bool IsLevelPersistent() { return m_persistent; }
    template<class T> void GetInterface(T** ppOut, long type)
    {
        if (m_presentBits[type >> 5] & (1UL << (type & 31))) {
            CInterfaceEntry* first = m_interfacesFirst;
            CInterfaceEntry* last = m_interfacesLast;
            long count = last - first;
            while (count > 0) {
                long half = count / 2;
                if (first[half].type < type) {
                    first = first + half + 1;
                    count -= half + 1;
                } else
                    count = half;
            }
            *ppOut = (T*)((first == last || type < first->type) ? last : first)->component;
        }
    }
};
struct CBaseIntelligentPointer {
    void* _vfptr;
    CThing* m_pItem;
    CThing* GetPItem() const { return m_pItem; }
};
struct SOwnedListNode { SOwnedListNode* next; SOwnedListNode* prev; CBaseIntelligentPointer value; };
struct SThingListNode { SThingListNode* next; SThingListNode* prev; CThing* value; };
struct CThingPtrList {
    SThingListNode* head;
    CThingPtrList()
    {
        head = (SThingListNode*)malloc(sizeof(SThingListNode));
        head->next = head;
        head->prev = head;
    }
    ~CThingPtrList()
    {
        SThingListNode* node = head->next;
        while (node != head) {
            SThingListNode* dead = node;
            node = node->next;
            free(dead);
        }
        head->next = head;
        head->prev = head;
        free(head);
    }
    bool empty() const { return head->next == head; }
    CThing*& back() { return head->prev->value; }
    void push_back(CThing* const& value)
    {
        SThingListNode* node = (SThingListNode*)malloc(sizeof(SThingListNode));
        new (&node->value) CThing*(value);
        SThingListNode* prev = head->prev;
        node->next = head;
        node->prev = prev;
        prev->next = node;
        head->prev = node;
    }
    void pop_back()
    {
        SThingListNode* node = head->prev;
        SThingListNode* next = node->next;
        SThingListNode* prev = node->prev;
        prev->next = next;
        next->prev = prev;
        free(node);
    }
};
struct CTCThingOwner {
    virtual void v00() {}
    SOwnedListNode* ObjectList;
    virtual void OnLevelUnload();
};
void CTCThingOwner::OnLevelUnload()
{
    CThingPtrList things;
    for (SOwnedListNode* node = ObjectList->next; node != ObjectList; node = node->next) {
        CBaseIntelligentPointer& item = node->value;
        if (item.GetPItem() && !item.GetPItem()->IsDead() && item.GetPItem()->IsLevelPersistent())
            things.push_back(item.GetPItem());
    }
    CTCOwnedEntity* owned;
    while (!things.empty()) {
        things.back()->GetInterface(&owned, 0x33);
        owned->SetPOwner(0);
        things.pop_back();
    }
}
int main()
{
    CTCOwnedEntity ent[4]; CThing things[4]; CInterfaceEntry entries[4][3];
    for (int i = 0; i < 4; ++i) {
        ent[i].id = i; ent[i].owner = &things[0];
        for (int w = 0; w < 9; ++w) things[i].m_presentBits[w] = 0;
        things[i].m_presentBits[1] = 1UL << 19;   // interface 0x33
        entries[i][0].type = 0x10; entries[i][0].component = 0;
        entries[i][1].type = 0x33; entries[i][1].component = (CTCBase*)&ent[i];
        entries[i][2].type = 0x40; entries[i][2].component = 0;
        things[i].m_interfacesFirst = entries[i]; things[i].m_interfacesLast = entries[i] + 3;
        things[i].m_flags91 = 0; things[i].m_persistent = true;
    }
    things[1].m_flags91 = 1;            // dead
    things[2].m_persistent = false;     // not persistent
    SOwnedListNode head, nodes[5];
    SOwnedListNode* prev = &head;
    for (int i = 0; i < 5; ++i) { nodes[i].value.m_pItem = i < 4 ? &things[i] : 0; prev->next = &nodes[i]; nodes[i].prev = prev; prev = &nodes[i]; }
    prev->next = &head; head.prev = prev;
    CTCThingOwner owner; owner.ObjectList = &head;
    owner.OnLevelUnload();
    bool ok = g_disowned == 2 && g_order[0] == 3 && g_order[1] == 0 && ent[0].owner == 0 && ent[3].owner == 0 &&
              ent[1].owner == &things[0] && ent[2].owner == &things[0] && g_mallocs == 3 && g_frees == 3;
    if (!ok) { std::printf("0071b380_TEST FAIL\n"); return 1; }
    std::printf("0071b380_TEST PASS\n");
    return 0;
}