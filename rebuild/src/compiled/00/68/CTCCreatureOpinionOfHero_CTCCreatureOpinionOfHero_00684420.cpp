// CTCCreatureOpinionOfHero::CTCCreatureOpinionOfHero(CThing* parent) -- opinion-of-hero thing component ctor: default counters, all-on attitude/reaction/deed masks, an empty opinion set, deed lists and the threat pointer, then the masks and personality applied from the owner's COpinionOfHeroDef and every deed's last-applied frame set to -1
// Leaf: ??0CTCCreatureOpinionOfHero
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00684420, length 496 bytes (body 0x1ed + int3 padding), `ret 4`.
// Label correction: the catalog says CEngineSubPrimitive2DClockSprite::ctor, but the vtable stored, 0x0125BC3C, is
//   CTCCreatureOpinionOfHero (the ??_G at 0x006866D0 / dtor 0x006866F0 store the same vtable; labels_chunk190_195 corrections).
// Callees: CTCBase::CTCBase(CThing*) 0x00686800, malloc 0x00BFEA0E (STLport node/array allocator), CBaseIntelligentPointer
//   ctor 0x00A01B10 and SetPItem 0x00A01B90 (CIntelligentPointer<CThing> vtable 0x01238C6C), CDefClassBase::GetSubDef
//   <COpinionOfHeroDef> 0x00685C00 on the owner's def (+0x70), SetPersonalityOverride(long, bool) 0x00681E60,
//   SetOpinionReactionMask(long) 0x00681CD0, SetOpinionDeedMask(long) 0x006821E0.
// Layout: engine/CTCCreatureOpinionOfHero.h (PDB, 0x65c) does not fit past +0x24: retail STLport vectors are 12 bytes and
//   lists 4, so ReactionMask +0x30, DeedMask +0x3c, RelativeOpinion +0x48, PendingDeeds +0x54, LastFrameAppliedDeedEffect
//   +0x58, KnownDeeds[92] +0x1c8, TransientOffsets +0x338, ForcedAttitude +0x33c, the four flags +0x340, the threat
//   pointer +0x344 and FrameLastUpdatedUnknownThreat +0x34c (the landed SetOpinionReactionMask 0x00681E30 agrees).
// Shape: the masks are byte vectors of 18/79/92 entries filled with 1 (memset), the opinion set five zero floats; the
//   92 list heads are built by an inline counted loop; the def fields are read through the member pointer each call.
#include <string.h>
class CThing;
extern "C" void* __cdecl malloc(unsigned int size);   // 0x00BFEA0E

class CTCBase { public:
    virtual void v00();
    CThing*       ParentThing;                      // +0x04
    bool          ToBeRemoved;                      // +0x08
    bool          IsInGlobalUpdateTCs;              // +0x09
    CTCBase(CThing* parent);                        // 0x00686800
};

template <class T> struct CRetailList {             // STLport list: one sentinel-node pointer
    struct node { node* next; node* prev; T value; };
    node* head;
    __forceinline CRetailList() : head(0)
    {
        node* sentinel = (node*)malloc(sizeof(node));
        sentinel->next = sentinel;
        sentinel->prev = sentinel;
        head = sentinel;
    }
};

// STLport uninitialized_fill_n over a byte/float range: memset, then the end pointer from the argument
template <class T> __forceinline T* UninitializedFillZeroOr(T* first, unsigned int count, int byteValue)
{
    memset(first, byteValue, count * sizeof(T));
    return first + count;
}

template <class T> struct SAllocProxy {           // STLport _STLP_alloc_proxy<T*, T, allocator<T> >
    T* _M_data;
    SAllocProxy(T* data) : _M_data(data) {}
    T* allocate(unsigned int count) { return (T*)malloc(count * sizeof(T)); }
};

struct CRetailMaskVector {                          // the retail mask vectors: one byte per entry
    unsigned char* _M_start;
    unsigned char* _M_finish;
    SAllocProxy<unsigned char> _M_end_of_storage;
    __forceinline CRetailMaskVector(unsigned int count, bool value) : _M_start(0), _M_finish(0), _M_end_of_storage(0)
    {
        _M_start = _M_end_of_storage.allocate(count);
        _M_end_of_storage._M_data = _M_start + count;
        _M_finish = UninitializedFillZeroOr(_M_start, count, value);
    }
};

class COpinionSet { public:                                // five opinion values (vector<float>)
    float* _M_start;
    float* _M_finish;
    SAllocProxy<float> _M_end_of_storage;
    __forceinline COpinionSet() : _M_start(0), _M_finish(0), _M_end_of_storage(0)
    {
        _M_start = _M_end_of_storage.allocate(5);
        _M_end_of_storage._M_data = _M_start + 5;
        _M_finish = UninitializedFillZeroOr(_M_start, 5, 0);
    }
};

class COpinionTransientOffset { public: unsigned char _body[0x18]; };

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
template <class T> class CIntelligentPointer : public CBaseIntelligentPointer {   // CIntelligentPointer<CThing>, vtable 0x01238C6C
public:
    CIntelligentPointer(T* item) : CBaseIntelligentPointer() { SetPItem(item); }

    T* GetPItem() const { return (T*)CBaseIntelligentPointer::GetPItem(); }
    void SetPItem(T* item) { CBaseIntelligentPointer::SetPItem((CBaseObject*)item); }
};

class COpinionOfHeroDef { public:
    unsigned char _pad[0x30];
    long          PersonalityIndex;                 // +0x30
    long          ReactionMaskIndex;                // +0x34
    long          DeedMaskIndex;                    // +0x38
};
template <class T> class CDefPointer { public:
    T* Object;
    CDefPointer() : Object(0) {}
    T* operator->() const { return Object; }
};
class COpinionPersonalityDef;

class CDefClassBase { public:
    bool GetSubDef(CDefPointer<COpinionOfHeroDef>& out);   // 0x00685C00
};
struct CThingDefView {
    unsigned char  _pad[0x70];
    CDefClassBase* PDef;                            // +0x70
};

class CTCCreatureOpinionOfHero : public CTCBase { public:         // vtable 0x0125BC3C
    long                                 LastOpinionReactionFrame;      // +0x0c
    float                                NumberOfTimesHit;              // +0x10
    float                                ToleranceToBeingHitOverride;   // +0x14
    long                                 FrameToDecayNumberOfTimesHit;  // +0x18
    CDefPointer<COpinionOfHeroDef>       PDef;                          // +0x1c
    CDefPointer<COpinionPersonalityDef>  PPersonalityDef;               // +0x20
    CRetailMaskVector                    AttitudeMask;                  // +0x24
    CRetailMaskVector                    ReactionMask;                  // +0x30
    CRetailMaskVector                    DeedMask;                      // +0x3c
    COpinionSet                          RelativeOpinion;               // +0x48
    CRetailList<long>                    PendingDeeds;                  // +0x54
    long                                 LastFrameAppliedDeedEffect[92];   // +0x58
    CRetailList<long>                    KnownDeeds[92];                // +0x1c8
    CRetailList<COpinionTransientOffset> TransientOffsets;              // +0x338
    long                                 ForcedAttitude;                // +0x33c
    bool                                 InteractedFlag;                // +0x340
    bool                                 GreetedFlag;                   // +0x341
    bool                                 HeroOpinionEnemy;              // +0x342
    bool                                 InFindAGuardAndYellForHelp;    // +0x343
    CIntelligentPointer<CThing>             PUnknownThreatOpinionSource;   // +0x344
    long                                 FrameLastUpdatedUnknownThreat; // +0x34c

    virtual void v00();
    CTCCreatureOpinionOfHero(CThing* parent);
    void SetPersonalityOverride(long index, bool apply);   // 0x00681E60
    void SetOpinionReactionMask(long index);                // 0x00681CD0
    void SetOpinionDeedMask(long index);                    // 0x006821E0
};

CTCCreatureOpinionOfHero::CTCCreatureOpinionOfHero(CThing* parent)
    : CTCBase(parent)
    , LastOpinionReactionFrame(0)
    , NumberOfTimesHit(0.0f)
    , ToleranceToBeingHitOverride(-1.0f)
    , FrameToDecayNumberOfTimesHit(0x7fffffff)
    , AttitudeMask(18, true)
    , ReactionMask(79, true)
    , DeedMask(92, true)
    , ForcedAttitude(18)
    , InteractedFlag(false)
    , GreetedFlag(false)
    , HeroOpinionEnemy(false)
    , InFindAGuardAndYellForHelp(false)
    , PUnknownThreatOpinionSource(0)
    , FrameLastUpdatedUnknownThreat(0)
{
    ((CThingDefView*)ParentThing)->PDef->GetSubDef(PDef);
    SetPersonalityOverride(PDef->PersonalityIndex, true);
    SetOpinionReactionMask(PDef->ReactionMaskIndex);
    SetOpinionDeedMask(PDef->DeedMaskIndex);
    for (long i = 0; i < 92; ++i) LastFrameAppliedDeedEffect[i] = -1;
}