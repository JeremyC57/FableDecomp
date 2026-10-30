// CAIStateGroup_SeekPerceivedThing::Update() -- alert nearby perceivers of the perceived thing, play ALARM facing it, then run to its position
// Grade: DIFFER(499v498) nd=112 (was 503v498 nd=113) -- lever: named first/last list iterators before the for_each call (size within 1). Tried: x/y float locals for the box (170). Residue: this in esi vs retail edi with push edi at the top, frame 0xe0 v 0xdc, the list sentinel reload across GetAllTCsInRoughArea (GOTCHAS 474 lane-gap family). Next: the for_each hidden-return slot (STL for_each returns the functor by value).
// Leaf: Update
// Retail: Fable.exe (TLC, VC7.1) @ 0x0091B570; best of the probes graded under VC7.1 RTM (probe/0091b570c.cpp).
struct CTCBase;
struct C3DVector { float x, y, z; };
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
    const C3DVector& GetPosition() const;       // 0x004C73D0
    template<class T> __forceinline bool GetInterface(T** ppOut, long interfaceType)
    {
        bool has = (unsigned char)(m_presentBits[interfaceType >> 5] >> (interfaceType & 31)) & 1;
        if (has) {
            long type = interfaceType;
            CInterfaceMap* map = &m_interfaces;
            CInterfaceMap::SEntry* it = map->LowerBound(&type);
            *ppOut = (T*)((it == map->last || interfaceType < it->type) ? map->last : it)->component;
            return true;
        }
        return false;
    }
};
struct CCharString {
    void* m_pData;
    CCharString(const char* text, long length = -1);    // 0x0099EBF0
    ~CCharString();                                 // 0x0099EAE0
};
struct CSoundPair {
    long m_first;
    long m_second;
    CSoundPair() : m_first(0), m_second(0) { }
};
struct CAnimationEntry;
struct CThingCreatureBase;
struct CThingCreature : CThing {
    CAnimationEntry* GetAnimFromName(const CCharString& name, long index);   // 0x00662A00
};
struct CTCPerceiveThing {                       // interface 0x34
    unsigned char _pad[0x18];
    float Alertness;                            // +0x18
    bool HasPerceivedThingToAlert();            // 0x00744430
    CThing* GetPerceivedThing();                // 0x007446A0
    void SetAsIndirectlyAwareOfPerceivedThing();// 0x007449E0
};
extern "C" void* malloc(unsigned int size);     // 0x00BFEA0E
struct CListNode { CListNode* next; CListNode* prev; CTCPerceiveThing* data; };
struct CListIter {
    CListNode* m_node;
    CListIter(CListNode* node) : m_node(node) { }
    CListIter(const CListIter& other) : m_node(other.m_node) { }
};
struct CPerceiveList {
    CListNode* m_node;
    CPerceiveList() { CListNode* node = (CListNode*)malloc(0xc); node->next = node; node->prev = node; m_node = node; }
    ~CPerceiveList();                           // 0x0091B910
    CListIter begin() { return CListIter(m_node->next); }
    CListIter end() { return CListIter(m_node); }
};
struct CMemFun {
    void (CTCPerceiveThing::*m_pm)();
    CMemFun(void (CTCPerceiveThing::*pm)()) : m_pm(pm) { }
};
struct CForEachResult {
    void* m_fn;
    CForEachResult* ForEach(CListIter first, CListIter last, CMemFun fn);   // 0x0091B890
};
struct CRoughBox {
    long x0, y0, x1, y1;
    CRoughBox(long a, long b, long c, long d) : x0(a), y0(b), x1(c), y1(d) { }
};
struct CThingFilter_IsPerceivingThing { CThing* m_thing; CThingFilter_IsPerceivingThing(CThing* t) : m_thing(t) { } };
struct CThingFilter_IsNot { CThing* m_thing; CThingFilter_IsNot(CThing* t) : m_thing(t) { } };
template<class A, class B> struct CFilter_Bind2 {
    const A* m_a;
    const B* m_b;
    CFilter_Bind2(const A& a, const B& b) : m_a(&a), m_b(&b) { }
};
typedef CFilter_Bind2<CThingFilter_IsPerceivingThing, CThingFilter_IsNot> CPerceiveFilter;
struct CThingSearchTools {
    void GetAllTCsInRoughArea(const CRoughBox* box, const CPerceiveFilter* filter, CPerceiveList* out);  // 0x0091BA40
};
struct CWorldMapHolder { CThingSearchTools* GetSearchTools(); };   // 0x0051E490
struct CCreatureActionBase {
    virtual void v00();
    unsigned char _body[0xa4];
    ~CCreatureActionBase();                     // 0x00693EF0
};
struct CCreatureAction_PlayAnimation : CCreatureActionBase {
    unsigned char _anim[0x0c];
    CCreatureAction_PlayAnimation(CThingCreatureBase* creature, CAnimationEntry* anim, const CSoundPair& sound,
        bool a, bool b, bool c, long d, long e);    // 0x00842570
};
struct CBaseIntelligentPointer {
    virtual void v00();
    CThing* PItem;
    CBaseIntelligentPointer();                  // 0x00A01B10
    void SetPItem(CThing* item);                // 0x00A01B90
};
struct CIntelligentPointerConstThing : CBaseIntelligentPointer {   // vtable 0x01237F1C
    virtual void v00();
    CIntelligentPointerConstThing(CThing* item) : CBaseIntelligentPointer() { SetPItem(item); }
};
struct CAIBrain {
    unsigned char _pad[0x18];
    CThingCreature* PCreature;                  // +0x18
};
struct CAIStateGroupBase {
    virtual ~CAIStateGroupBase();
    CAIBrain* PBrain;                           // +0x04
    CWorldMapHolder* GetWorldMapHolder();       // 0x008FCF60
    long AIAction_PerformActionTargetFaceAndWait(CCreatureActionBase* action, CIntelligentPointerConstThing target, bool b);   // 0x0097DCB0
};
struct CAIStateGroup_SeekPerceivedThing : CAIStateGroupBase {
    void RunToPerceivedThingPosition();         // 0x0091B3A0
    void Update();
};

void CAIStateGroup_SeekPerceivedThing::Update()
{
    CTCPerceiveThing* perception;
    if (!PBrain->PCreature->GetInterface(&perception, 0x34))
        return;
    if (perception->Alertness == 0.0f)
        return;
    if (perception->HasPerceivedThingToAlert()) {
        CPerceiveList list;
        const C3DVector& pos = PBrain->PCreature->GetPosition();
        CRoughBox box((long)(pos.x - 8.0f), (long)(pos.y - 8.0f), (long)(pos.x + 8.0f), (long)(pos.y + 8.0f));
        CPerceiveFilter filter(CThingFilter_IsPerceivingThing(perception->GetPerceivedThing()), CThingFilter_IsNot(PBrain->PCreature));
        GetWorldMapHolder()->GetSearchTools()->GetAllTCsInRoughArea(&box, &filter, &list);
        CListIter first = list.begin();
        CListIter last = list.end();
        CForEachResult result;
        result.ForEach(first, last, CMemFun(&CTCPerceiveThing::SetAsIndirectlyAwareOfPerceivedThing));
    }
    CAnimationEntry* anim;
    {
        CCharString name("ALARM");
        anim = PBrain->PCreature->GetAnimFromName(name, 0);
    }
    CCreatureAction_PlayAnimation action((CThingCreatureBase*)PBrain->PCreature, anim, CSoundPair(), false, true, false, 0, 0);
    AIAction_PerformActionTargetFaceAndWait(&action, CIntelligentPointerConstThing(perception->GetPerceivedThing()), false);
    RunToPerceivedThingPosition();
}
