// CTCThingOwner::OnLevelUnload() -- collects the owned objects that are alive and level-persistent into a local list, then disowns each one (its owned-entity component's owner is cleared) from the back of the list
// Leaf: ?OnLevelUnload@CTCThingOwner
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0071B380, 309 bytes (plain `ret`); CTCThingOwner vtable 0x012412A4 slot 15
// (the manifest says "vfunc_15"; Ego_r.pdb names CTCThingOwner::OnLevelUnload, the only remaining virtual of the class
// with this shape: OnKill is slot 14 0x0071B200, OnSetInLimbo 0x0071B2D0).
// Callees: malloc 0x00BFEA0E (list head and nodes); CBaseIntelligentPointer::GetPItem() 0x00A01B50 (x4);
//          CThing::IsLevelPersistent() 0x004C82A0; CTCOwnedEntity::SetPOwner(CThing*) 0x007E8590; free 0x00BFEA14.
// Layout (retail, as the landed ClearDeadEntries 0x0071B240): ObjectList +0x0c (one-pointer sentinel list, nodes {next,
//          prev, CIntelligentPointer<CThing> at +8}); CThing bitset +0x20 (interface 0x33 = CTCOwnedEntity: bit 19 of
//          the word +0x24), interface map +0x44, dead bit +0x91.
// Shape: the local list is the same sentinel list over CThing* (malloc'd head, self-linked; push_back constructs the
//        value with the placement null test and links before the head; pop_back unlinks the last node; the dtor frees
//        every node, re-links the head and frees it); the component lookup is the inline STLport/Dinkumware lower_bound
//        (`count / 2` halving) with the out-parameter end/hit choice, the pointer declared before the loop (retail
//        keeps the uninitialised frame word when the bit is clear).
class CThing;
struct CTCBase;
struct CBaseObject;
extern "C" void* __cdecl malloc(unsigned int size);   // 0x00BFEA0E
extern "C" void __cdecl free(void* block);            // 0x00BFEA14
inline void* operator new(unsigned int, void* where) { return where; }

struct CInterfaceEntry { long type; CTCBase* component; };
class CThing {
public:
    unsigned char    _pad00[0x20];
    unsigned long    m_presentBits[9];          // +0x20 interface-present bitset
    CInterfaceEntry* m_interfacesFirst;         // +0x44
    CInterfaceEntry* m_interfacesLast;          // +0x48
    unsigned char    _pad4c[0x91 - 0x4c];
    unsigned char    m_flags91;                 // +0x91 bit 0: dead
    bool IsDead() const { return (m_flags91 & 1) != 0; }
    bool IsLevelPersistent();                   // 0x004C82A0
    template<class T> __forceinline void GetInterface(T** ppOut, long type)
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
    CBaseObject* m_pItem;
    CThing* GetPItem() const;                   // 0x00A01B50
};
struct SOwnedListNode {                         // CTCThingOwner::ObjectList node
    SOwnedListNode*         next;               // +0x00
    SOwnedListNode*         prev;               // +0x04
    CBaseIntelligentPointer value;              // +0x08
};

struct SThingListNode {                         // local list<CThing*> node
    SThingListNode* next;
    SThingListNode* prev;
    CThing*         value;
};
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

struct CTCOwnedEntity {
    void SetPOwner(CThing* owner);              // 0x007E8590
};

struct CTCThingOwner {
    virtual void v00();
    unsigned char   _pad_0x04[0x8];
    SOwnedListNode* ObjectList;                 // +0x0c (sentinel)
    bool            CallOnDie;                  // +0x10
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
