// CTCCreatureOpinionOfHero::~CTCCreatureOpinionOfHero() -- destructor: the unknown-threat intelligent pointer, the transient offsets, the 92 known-deed lists, the pending deeds, the relative-opinion and three mask vectors, the two def pointers, then CTCBase
// Leaf: ??1CTCCreatureOpinionOfHero
// Retail: Fable.exe (TLC, VC7.1) @ 0x006866F0, 222 bytes; called only from the deleting destructor 0x006866D0 (vtable
//   0x0125BC3C slot 0 = CTCCreatureOpinionOfHero, rtti-vtables.tsv).
// Label correction: the catalog's "CEngineInternalPrimitiveMeshBase::~CEngineInternalPrimitiveMeshBase" is propagated; the
//   vtable that reaches this body is CTCCreatureOpinionOfHero's.
// Callees: ~CBaseIntelligentPointer 0x00A01C10 (PUnknownThreatOpinionSource), the out-of-line list dtors 0x00450DAB
//   (TransientOffsets) and 0x0042AB73 (PendingDeeds; both size-optimised copies whose manifest label "OnReadFinished" is
//   propagated), free 0x00BFEA14, the def pointees' slot 1 (+4, destroy), ~CTCBase 0x00686830 (tail jump).
// Layout (retail; Ego_r.pdb / engine/CTCCreatureOpinionOfHero.h agree up to +0x24, then the retail containers are 12-byte
//   vectors and 4-byte lists): PDef +0x1c, PPersonalityDef +0x20, AttitudeMask +0x24, ReactionMask +0x30, DeedMask +0x3c,
//   RelativeOpinion (COpinionSet, one POD vector) +0x48, PendingDeeds +0x54, LastFrameAppliedDeedEffect long[92] +0x58,
//   KnownDeeds list<long>[92] +0x1c8, TransientOffsets +0x338, ForcedAttitude +0x33c, four flags +0x340,
//   PUnknownThreatOpinionSource +0x344, FrameLastUpdatedUnknownThreat +0x34c (0x350 bytes).
// Shape: the class is `__declspec(novtable)` here (no vtable store); members die in reverse order; the KnownDeeds array is
//   the inlined last-to-first counted loop over inline STLport list dtors (`clear()` walking `tmp = cur; cur = cur->next;
//   free(tmp)` with the head re-read, then the sentinel reset and freed); the POD vectors free their block; the def pointers
//   release through a local copy (`dec [ecx+4] ; jne ; call [vt+4]`) and clear the member.
extern "C" void __cdecl free(void* block);             // 0x00BFEA14

struct SLongListNode {
    SLongListNode* _M_next;
    SLongListNode* _M_prev;
    long           _M_data;
};
struct CLongList {                                     // STLport list<long> (retail 4 bytes: the sentinel node)
    SLongListNode* _M_node;
    void clear()
    {
        SLongListNode* cur = _M_node->_M_next;
        while (cur != _M_node) {
            SLongListNode* tmp = cur;
            cur = cur->_M_next;
            free(tmp);
        }
        _M_node->_M_next = _M_node;
        _M_node->_M_prev = _M_node;
    }
    ~CLongList()
    {
        clear();
        if (_M_node)
            free(_M_node);
    }
};
struct CPendingDeedList {                              // list<long> (out-of-line copy)
    void* _M_node;
    ~CPendingDeedList();                               // 0x0042AB73
};
struct CTransientOffsetList {                          // list<COpinionTransientOffset>
    void* _M_node;
    ~CTransientOffsetList();                           // 0x00450DAB
};

template <class T> struct SPodVector {                 // STLport vector of a POD element (12 bytes)
    T* _M_start;
    T* _M_finish;
    T* _M_end_of_storage;
    ~SPodVector()
    {
        if (_M_start)
            free(_M_start);
    }
};

class CDefPointeeBase { public:
    virtual void v00();
    virtual void Destroy();                            // slot 1 (+0x04)
    long RefCount;                                     // +0x04
};
struct CDefPointer {
    CDefPointeeBase* Object;
    ~CDefPointer()
    {
        CDefPointeeBase* object = Object;
        if (object) {
            if (--object->RefCount == 0)
                object->Destroy();
            Object = 0;
        }
    }
};

class CBaseObject;
class CBaseObjectPointer;
class CBaseIntelligentPointer {                       // vtable 0x0129C95C (Ego_r: vptr + PData, 8 bytes)
public:
    CBaseIntelligentPointer();                           // 0x00A01B10
    virtual ~CBaseIntelligentPointer();                  // 0x00A01C10
    CBaseObject* GetPItem() const;                       // 0x00A01B50
    void SetPItem(CBaseObject* item);                    // 0x00A01B90
private:
    CBaseObjectPointer* PData;                           // +0x04
};

class COpinionSet { public:                                   // retail 12 bytes: one POD vector
    SPodVector<float> Opinions;
};

class CTCBase { public:
    virtual ~CTCBase();                                // 0x00686830
    unsigned char _body[8];
};

struct __declspec(novtable) CTCCreatureOpinionOfHero : CTCBase {   // vtable 0x0125BC3C
    long                    LastOpinionReactionFrame;  // +0x0c
    float                   NumberOfTimesHit;          // +0x10
    float                   ToleranceToBeingHitOverride;   // +0x14
    long                    FrameToDecayNumberOfTimesHit;  // +0x18
    CDefPointer             PDef;                      // +0x1c
    CDefPointer             PPersonalityDef;           // +0x20
    SPodVector<unsigned long> AttitudeMask;            // +0x24 (vector<bool> words)
    SPodVector<unsigned long> ReactionMask;            // +0x30
    SPodVector<unsigned long> DeedMask;                // +0x3c
    COpinionSet             RelativeOpinion;           // +0x48
    CPendingDeedList        PendingDeeds;              // +0x54
    long                    LastFrameAppliedDeedEffect[92];   // +0x58
    CLongList               KnownDeeds[92];            // +0x1c8
    CTransientOffsetList    TransientOffsets;          // +0x338
    long                    ForcedAttitude;            // +0x33c
    bool                    InteractedFlag;            // +0x340
    bool                    GreetedFlag;               // +0x341
    bool                    HeroOpinionEnemy;          // +0x342
    bool                    InFindAGuardAndYellForHelp;   // +0x343
    CBaseIntelligentPointer PUnknownThreatOpinionSource;  // +0x344
    long                    FrameLastUpdatedUnknownThreat;   // +0x34c
    ~CTCCreatureOpinionOfHero();
};

CTCCreatureOpinionOfHero::~CTCCreatureOpinionOfHero()
{
}