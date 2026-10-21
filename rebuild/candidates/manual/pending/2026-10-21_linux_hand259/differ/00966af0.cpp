// CAIStateGroup_TavernServer::GetBar() -- collect the pub's live tavern bars into a list and pick one at random as PBar
// Leaf: GetBar
// Grade: DIFFER(479v502) -- rough (one grade): retail keeps the local list sentinel in esi through the inlined push_back loop and spills this to the frame; inlines the list destructor on the first empty-list return only and calls 0x00792240 elsewhere; uses the out-of-line folded IP ctor 0x0065E133 / operator= 0x007B82C0 for the pick; ours reloads the sentinel and keeps this in ebx
// Retail: Fable.exe (TLC, VC7.1) @ 0x00966AF0; best of the probes graded under VC7.1 RTM (probe/00966af0.cpp).
struct CTCBase;
struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);       // 0x0040F020
};
struct CThing {
    unsigned char _pad0[0x20];
    unsigned long m_presentBits[9];             // +0x20
    CInterfaceMap m_interfaces;                 // +0x44
    unsigned char _pad1[0x91 - 0x4c];
    unsigned char m_flags91;                    // +0x91 bit 0: dead
    template<class T> __forceinline bool GetInterface(T** ppOut, long interfaceType)
    {
        if (m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31))) {
            long type = interfaceType;
            CInterfaceMap* map = &m_interfaces;
            CInterfaceMap::SEntry* it = map->LowerBound(&type);
            if (it == map->last || interfaceType < it->type)
                it = map->last;
            *ppOut = (T*)it->component;
            return true;
        }
        return false;
    }
    bool HasTavernBar() const { return (m_presentBits[0xb9 >> 5] & (1UL << (0xb9 & 31))) != 0; }
};
struct CBaseIntelligentPointer {
    virtual void v00();
    CThing* m_pItem;
    CBaseIntelligentPointer();                  // 0x00A01B10
    CThing* GetPItem() const;                   // 0x00A01B50
    void SetPItem(CThing* item);                // 0x00A01B90
    ~CBaseIntelligentPointer();                 // 0x00A01C10
};
struct CIntelligentPointerObject : CBaseIntelligentPointer {    // CIntelligentPointer<CThingObject>, vtable 0x0125A858
    virtual void v00();
    CIntelligentPointerObject(CThing* item) : CBaseIntelligentPointer() { SetPItem(item); }
    CIntelligentPointerObject& operator=(const CIntelligentPointerObject& other) { SetPItem(other.GetPItem()); return *this; }
};
struct CIntelligentPointerThing : CBaseIntelligentPointer {     // folded out-of-line copy, same vtable
    virtual void v00();
    CIntelligentPointerThing(CThing* item);     // 0x0065E133
};
struct CIntelligentPointerBar : CIntelligentPointerObject {
    using CIntelligentPointerObject::operator=;
    CIntelligentPointerBar& operator=(const CIntelligentPointerThing& other);   // 0x007B82C0 (folded)
};
__forceinline bool IS_THING_ALIVE(const CBaseIntelligentPointer& p)
{
    if (p.GetPItem() != 0) {
        if (!(p.GetPItem()->m_flags91 & 1))
            return true;
    }
    return false;
}
extern "C" void* malloc(unsigned int size);     // 0x00BFEA0E
extern "C" void free(void* p);                  // 0x00BFEA14
struct CThingNode { CThingNode* next; CThingNode* prev; CThing* data; };
inline void* operator new(unsigned int, void* where) { return where; }
struct CThingList {
    CThingNode* m_node;
    CThingList() { CThingNode* node = (CThingNode*)malloc(0xc); node->next = node; node->prev = node; m_node = node; }
    ~CThingList();                              // 0x00792240
    bool empty() const { return m_node->next == m_node; }
    void push_back(CThing* const& value)
    {
        CThingNode* node = (CThingNode*)malloc(0xc);
        new (&node->data) CThing*(value);
        CThingNode* prev = m_node->prev;
        node->next = m_node;
        node->prev = prev;
        prev->next = node;
        m_node->prev = node;
    }
};
struct CPointerNode { CPointerNode* next; CPointerNode* prev; CBaseIntelligentPointer data; };
struct CTCBuilding {                            // interface 0x21
    unsigned char _pad[0xc];
    CPointerNode* Things;                       // +0x0c list<CIntelligentPointer<CThing> > sentinel
};
unsigned long __fastcall GFROR13(unsigned long value);  // 0x00497890
inline unsigned long WorldRandomRange(unsigned long& seed, unsigned long range)
{
    seed = seed * 0x24A1 + 0x24DF;
    seed = GFROR13(seed);
    if (range == 0)
        return 0;
    return seed % range;
}
struct CAIStateGroupBase {
    virtual ~CAIStateGroupBase();
    void* PBrain;                               // +0x04
    long ScriptAIPriority;                      // +0x08
    void* Name_PStringData;                     // +0x0c
    unsigned char Terminated;                   // +0x10
    unsigned char Priority;
    unsigned char Enabled;
    unsigned char _pad_0x13;
    unsigned long* GetPWorldSeed();             // 0x008FCFD0
};
struct CAIStateGroup_TavernServer : CAIStateGroupBase {
    CBaseIntelligentPointer PJug;               // +0x14
    CIntelligentPointerBar PBar;                // +0x1c
    CBaseIntelligentPointer PTable;             // +0x24
    CBaseIntelligentPointer PTankard;           // +0x2c
    CBaseIntelligentPointer PPub;               // +0x34
    bool GetBar();
};

bool CAIStateGroup_TavernServer::GetBar()
{
    CTCBuilding* building;
    PPub.GetPItem()->GetInterface(&building, 0x21);
    CThingList bars;
    for (CPointerNode* it = building->Things->next; it != building->Things; it = it->next) {
        if (IS_THING_ALIVE(it->data) && IS_THING_ALIVE(it->data) && it->data.GetPItem()->HasTavernBar())
            bars.push_back(it->data.GetPItem());
    }
    if (bars.empty())
        return false;
    PBar = CIntelligentPointerObject(0);
    long count = 0;
    for (CThingNode* n = bars.m_node->next; n != bars.m_node; n = n->next)
        ++count;
    if (count == 0)
        return false;
    unsigned long index = WorldRandomRange(*GetPWorldSeed(), count);
    for (CThingNode* n = bars.m_node->next; n != bars.m_node; n = n->next, --index) {
        if (index == 0) {
            PBar = CIntelligentPointerThing(n->data);
            break;
        }
    }
    return true;
}
