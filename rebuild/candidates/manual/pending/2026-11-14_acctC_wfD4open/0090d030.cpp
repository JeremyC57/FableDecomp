// CAIStateGroup_MinionGossip::Update() -- pair up with a gossip partner (answer a nearby gossip request or post one and wait), look at it, greet, walk up, interact, then take turns listening and talking until a 1-in-5 roll ends it
// Grade: DIFFER(3243v3269) -- frame layout (0x5D0 v 0x580: CThingFilter_IsNavLineOfSightToThing is 8 bytes in Ego_r, see v7; retail places LOS 0x58 / Bind4 0x60 / range 0x70 / gossipee 0x98 / actions from 0xC8), the second GetPOriginator<CThingAICreature> expansion (retail inlines two and calls 0x0090DF30 for the third; ours inlines one), xor/test vs not/test in the inline type test, the HasTC byte retail stores at [esp+17h]
// Next idea: the shared engine/CIntelligentPointer.h + engine/CThing.h (FABLE_RETAIL_CTHING_GETTC_EXPANDED) models instead of the local ones, which may change VC7.1's inline budget so the second GetPOriginator expands; then the 8-byte LOS filter with retail's local order
// Leaf: ?Update@CAIStateGroup_MinionGossip@@MAEXXZ
#include "engine/CMainGameComponent.h"

class C3DVector { public:
    float X, Y, Z;
    C3DVector() { }
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) { }
    C3DVector operator-(const C3DVector& o) const { return C3DVector(X - o.X, Y - o.Y, Z - o.Z); }
    float GetLength() const;
};
extern "C" double __cdecl sqrt(double);
#pragma intrinsic(sqrt)
inline float C3DVector::GetLength() const { return (float)sqrt(X * X + Y * Y + Z * Z); }
class CCharString;
class CAnimFilterBase;

class CTCBase;
enum ETCInterfaceType { };
#include "engine/CVectorMap.h"
typedef CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > CInterfaceMap;

class CBaseObject;
struct CBaseIntelligentPointer {
    virtual void v00();
    CBaseObject* PData;                         // +0x04
    CBaseIntelligentPointer();                  // 0x00A01B10
    virtual ~CBaseIntelligentPointer();         // 0x00A01C10
    CBaseObject* GetPItem() const;              // 0x00A01B50
    void SetPItem(CBaseObject* item);           // 0x00A01B90
};
template <class T> class CIntelligentPointer : public CBaseIntelligentPointer { public:
    virtual void v00();
    __forceinline CIntelligentPointer(T* item) { SetPItem((CBaseObject*)item); }
    CIntelligentPointer(const CIntelligentPointer& other);    // 0x0050F410 (CThingAICreature)
    __forceinline T* GetPItem() const { return (T*)CBaseIntelligentPointer::GetPItem(); }
    __forceinline CIntelligentPointer& operator=(const CIntelligentPointer& other) { SetPItem((CBaseObject*)other.GetPItem()); return *this; }
};

class CThing { public:
    unsigned char _pad0[0x20];
    unsigned long PresentBits[9];               // +0x20 std::bitset of interface types
    CInterfaceMap Components;                   // +0x44
    unsigned char _pad54[0x6c - 0x54];
    unsigned long ThingTypeFlags;               // +0x6c
    unsigned char _pad70[0x91 - 0x70];
    unsigned char Flags91;                      // +0x91 bit 0: dead
    const C3DVector& GetPos() const;            // 0x004C73D0
    __forceinline bool HasTC(long id) const { return ((PresentBits[id >> 5] >> (id & 31)) & 1) != 0; }
    template <class T> __forceinline bool GetTC(long id, T** out)
    {
        bool present = HasTC(id);
        if (present) {
            ETCInterfaceType type = (ETCInterfaceType)id;
            CInterfaceMap* map = &Components;
            CInterfaceMap::value_type* it = map->LowerBound(type);
            *out = (T*)((it == map->end() || id < it->first) ? map->end() : it)->second;
        }
        return present;
    }
    unsigned long GetTypeFlags() const { return ThingTypeFlags; }
    bool IsDead() const { return (Flags91 & 1) != 0; }
};
class CThingPhysical : public CThing { public:
    const C3DVector& PeekPos() const;           // 0x006A4D00
};
class CThingCreatureBase : public CThingPhysical { public:
    long GetMovementMode();                     // 0x00662000
    long GetSex();                              // 0x00661FF0
    bool HasAnim(const CCharString& name, const CAnimFilterBase& filter) const;   // 0x00662C70
};
class CThingAICreature : public CThingCreatureBase { public:
    static unsigned long __fastcall StatGetTypeFlags() { return 0x1f; }
    unsigned char _pad[0x168 - sizeof(CThingCreatureBase)];
    CIntelligentPointer<CThing> PHome;          // +0x168
};
template <class T> __forceinline bool IsThingOfType(const CThing* thing)
{
    unsigned long mask = T::StatGetTypeFlags();
    return (thing->ThingTypeFlags & mask) == mask;
}

class CCharString { public:
    void* PStringData;
    CCharString(const char* text, long length = -1);   // 0x0099EBF0
    ~CCharString();                                    // 0x0099EAE0
};
class CAnimFilterBase { public:
    virtual ~CAnimFilterBase() { }              // vtable 0x0122FFCC
};
class CAnimFilterAnyGroup : public CAnimFilterBase { public:   // vtable 0x0127261C
    virtual ~CAnimFilterAnyGroup() { }
};

class CTCGossip { public:                       // interface 0xC1
    unsigned char _pad00[0x0c];
    CIntelligentPointer<CThingAICreature> Gossipee;   // +0x0C
    bool MyTurnToSpeak;                         // +0x14
};
class CTCCreatureModeManager { public:          // interface 0x31
    bool AddMode(unsigned long mode);           // 0x0073AE10
    void RemoveMode(unsigned long mode);        // 0x0073ABA0
};

class CMessageEvent { public:
    unsigned char _pad00[0x48];
    CIntelligentPointer<CThing> POriginator;    // +0x48
    template <class T> T* GetPOriginator() const
    {
        if (!POriginator.GetPItem())
            return 0;
        if ((static_cast<CThing*>(POriginator.GetPItem())->GetTypeFlags() ^ T::StatGetTypeFlags()) & T::StatGetTypeFlags())
            return 0;
        return (T*)POriginator.GetPItem();
    }
};
template <class T> __forceinline T* GetTypedOriginator(const CMessageEvent* message)
{
    T* result = 0;
    if (message->POriginator.GetPItem() && IsThingOfType<T>(message->POriginator.GetPItem()))
        result = (T*)message->POriginator.GetPItem();
    return result;
}

class CThingFilter_IsNavLineOfSightToThing { public:
    unsigned char _pad[0x40];
    CThingFilter_IsNavLineOfSightToThing(const CThing* thing);   // 0x006A8710
};
class CThingFilter_IsValidGossipee { public:
    CThingAICreature* Thing;
    void* PFaction;
    void* PTCVillage;
    CThingFilter_IsValidGossipee(CThingAICreature* thing);       // 0x0090C870
};
class CThingFilter_In2DRange { public:
    C3DVector Pos;
    float Range;
    CThingFilter_In2DRange(const C3DVector& pos, float range) : Pos(pos), Range(range) { }
};
class CMessageEventFilter_IsOfType { public:
    long Type;
    CMessageEventFilter_IsOfType(long type) : Type(type) { }
};
template <class F> class CMessageEventFilter_ThingSearchFilter { public:
    const F* PFilter;
    CMessageEventFilter_ThingSearchFilter(const F& filter) : PFilter(&filter) { }
};
template <class F1, class F2, class F3, class F4> class CFilter_Bind4 { public:
    const F1* PF1; const F2* PF2; const F3* PF3; const F4* PF4;
    CFilter_Bind4(const F1& a, const F2& b, const F3& c, const F4& d) : PF1(&a), PF2(&b), PF3(&c), PF4(&d) { }
};
typedef CFilter_Bind4<CMessageEventFilter_IsOfType, CMessageEventFilter_ThingSearchFilter<CThingFilter_In2DRange>,
    CMessageEventFilter_ThingSearchFilter<CThingFilter_IsValidGossipee>,
    CMessageEventFilter_ThingSearchFilter<CThingFilter_IsNavLineOfSightToThing> > CGossipFilter;

class CMessageEventManager { public:
    template <class Filter> const CMessageEvent* FindMostRecentMessage(const Filter& filter) const;
    const CMessageEvent* AddMessage(long type, unsigned long lifespan, const C3DVector& pos, CThing* originator, CThing* target,
                                    float sightRadius, float hearingRadius, long extraData);   // 0x00687540
    void RemoveMessage(const CMessageEvent* message);   // 0x00493B5B
};

class CWorld { public:
    unsigned char _pad[0x60];
    CMessageEventManager* PMessageEventManager;   // +0x60
    CMessageEventManager& GetMessageEventManager() { return *PMessageEventManager; }
    static long __fastcall GetFrame();          // 0x0049D870
};

class CSoundPair { public:
    long FirstSound, LastSound;
    CSoundPair(long first, long last) : FirstSound(first), LastSound(last) { }
};
class CCreatureActionBase { public:
    virtual ~CCreatureActionBase();             // 0x00693EF0
    unsigned char _pad04[0xb0];
};
class CCreatureAction_PlayAnimation : public CCreatureActionBase { public:
    CCreatureAction_PlayAnimation(CThingCreatureBase* creature, const CCharString& animation, const CSoundPair& sound,
                                  bool a, bool b, bool c, long d, long e);   // 0x008424F0
};

enum EAIActionReturn { };
enum EAIActionRepeat { };
enum EMovementType { };
unsigned long __fastcall GFROR13(unsigned long value);   // 0x00497890

class CAIBrain { public:
    unsigned char _pad[0x18];
    CThingAICreature* PCreature;                // +0x18
};
class CAIStateGroupBase { public:
    virtual ~CAIStateGroupBase();
    CAIBrain* PBrain;                           // +0x04
    long ScriptAIPriority;                      // +0x08
    void* Name;                                 // +0x0c
    unsigned char Terminated, Priority, Enabled, _pad13;
    CThingAICreature* GetCreature() const { return PBrain->PCreature; }
    CWorld& GetWorld() const;                   // 0x008FCFC0
    unsigned long* GetPWorldSeed();             // 0x008FCFD0
    __forceinline unsigned long Random(unsigned long range)
    {
        unsigned long* seed = GetPWorldSeed();
        unsigned long next = *seed * 0x24A1U + 0x24DFU;
        *seed = next;
        unsigned long rotated = GFROR13(next);
        *seed = rotated;
        return rotated % range;
    }
    void Terminate();                           // 0x008FCF40
    EAIActionReturn AIAction_DoNothing();       // 0x0097E200
    EAIActionReturn AIAction_LookAtThing(EAIActionRepeat repeat, CIntelligentPointer<const CThing> thing, long frames, float speed);   // 0x0097D6F0
    EAIActionReturn AIAction_PerformActionAndWait(const CCreatureActionBase& action, bool wait);   // 0x0097DB70
    EAIActionReturn AIAction_MoveToPosProximity(EAIActionRepeat repeat, C3DVector pos, float proximity, bool& arrived,
                                                EMovementType movementType, bool a, bool b, bool c, const CThing* avoid, bool d,
                                                float e);   // 0x0097E890
    EAIActionReturn AIAction_InteractWith(EAIActionRepeat repeat, CIntelligentPointer<CThingAICreature> creature, CCharString interaction,
                                          int duration, bool initiator);   // 0x00981410
};
class CAIStateGroup_MinionGossip : public CAIStateGroupBase { public:
    long FrameOfLastGossip;                     // +0x14
    bool Critical;                              // +0x18
    bool NowRelyingOnGossipee;                  // +0x19
    CTCGossip* PTCGossip;                       // +0x1C
    CTCGossip* PTCGossipOther;                  // +0x20
protected:
    virtual void Update();
};

static const char* s_GreetingsNear[2];          // 0x0138C3C4
static const char* s_GreetingsFar[4];           // 0x0138C3CC
static const char* s_TalkAnimsRare[1];          // 0x0138C3DC
static const char* s_TalkAnims[10];             // 0x0138C3E0
static const char* s_Interactions[9];           // 0x0138C408

void CAIStateGroup_MinionGossip::Update()
{
    Critical = false;
    NowRelyingOnGossipee = false;
    GetCreature()->GetTC(0xC1, &PTCGossip);

    CThingFilter_IsNavLineOfSightToThing lineOfSight(GetCreature());
    CMessageEventFilter_ThingSearchFilter<CThingFilter_IsNavLineOfSightToThing> lineOfSightFilter(lineOfSight);
    CMessageEventFilter_ThingSearchFilter<CThingFilter_IsValidGossipee> gossipeeFilter((CThingFilter_IsValidGossipee(GetCreature())));
    CThingFilter_In2DRange range(GetCreature()->GetPos(), 16.0f);
    CMessageEventFilter_ThingSearchFilter<CThingFilter_In2DRange> rangeFilter(range);
    const CMessageEvent* message = GetWorld().GetMessageEventManager().FindMostRecentMessage(
        CGossipFilter(CMessageEventFilter_IsOfType(0x2C), rangeFilter, gossipeeFilter, lineOfSightFilter));

    bool initiator;
    if (message) {
        {
            CThingAICreature* originator = message->GetPOriginator<CThingAICreature>();
            if (!originator || originator->IsDead())
                goto terminate;
        }
        if (!message->GetPOriginator<CThingAICreature>()->HasTC(0xC1))
            goto terminate;
        PTCGossip->Gossipee = CIntelligentPointer<CThingAICreature>(message->GetPOriginator<CThingAICreature>());
        PTCGossip->Gossipee.GetPItem()->GetTC(0xC1, &PTCGossipOther);
        GetWorld().GetMessageEventManager().RemoveMessage(message);
        PTCGossip->MyTurnToSpeak = true;
        PTCGossipOther->Gossipee = CIntelligentPointer<CThingAICreature>(GetCreature());
        PTCGossipOther->MyTurnToSpeak = false;
        initiator = false;
    } else {
        GetWorld().GetMessageEventManager().AddMessage(0x2C, CMainGameComponent::GetConstantFPS() * 4, GetCreature()->PeekPos(),
                                                       GetCreature(), 0, 16.0f, 16.0f, 0);
        long timeout = CWorld::GetFrame() + CMainGameComponent::GetConstantFPS() * 4;
        while (!PTCGossip->Gossipee.GetPItem() || PTCGossip->Gossipee.GetPItem()->IsDead()) {
            if (AIAction_DoNothing() == 1)
                goto terminate;
            if (CWorld::GetFrame() > timeout) {
                Terminate();
                return;
            }
        }
        GetCreature()->GetTC(0xC1, &PTCGossip);
        PTCGossip->Gossipee.GetPItem()->GetTC(0xC1, &PTCGossipOther);
        initiator = true;
    }
    NowRelyingOnGossipee = true;

    if (initiator || Random(100) < 20) {
        if (AIAction_LookAtThing((EAIActionRepeat)1, CIntelligentPointer<const CThing>(PTCGossip->Gossipee.GetPItem()), 0x11, 1.0f / 360.0f) != 2)
            goto terminate;
        const char* greeting;
        if ((PTCGossip->Gossipee.GetPItem()->GetPos() - GetCreature()->GetPos()).GetLength() < 5.0f)
            greeting = s_GreetingsNear[Random(2)];
        else
            greeting = s_GreetingsFar[Random(4)];
        if (AIAction_PerformActionAndWait(CCreatureAction_PlayAnimation(GetCreature(), greeting, CSoundPair(0, 0), false, true,
                                                                         false, 0, 0), false) != 2)
            goto terminate;
    }

    {
        long mySex = GetCreature()->GetMovementMode() == 1 ? 0 : GetCreature()->GetSex();
        long otherSex = PTCGossip->Gossipee.GetPItem()->GetMovementMode() == 1 ? 0 : PTCGossip->Gossipee.GetPItem()->GetSex();
        const char* interaction = s_Interactions[mySex * 3 + otherSex];
        if (mySex != 0 && otherSex != 0 && Random(7) == 0)
            interaction = "VILLAGER_INTERACTION_EXCHANGE_MONEY";
        if (GetCreature()->PHome.GetPItem() == PTCGossip->Gossipee.GetPItem()->PHome.GetPItem()) {
            long roll = Random(2);
            if (roll == 0)
                interaction = 0;
        }
        bool arrived;
        while (true) {
            long result = AIAction_MoveToPosProximity((EAIActionRepeat)2, PTCGossip->Gossipee.GetPItem()->GetPos(), 1.02f, arrived, (EMovementType)3, true, false,
                                                      true, 0, true, 0.0f);
            if (result == 4)
                continue;
            if (result != 2)
                goto terminate;
            break;
        }
        if (interaction) {
            Critical = true;
            AIAction_InteractWith((EAIActionRepeat)1, PTCGossip->Gossipee, interaction, 30, initiator);
            Critical = false;
        }
        CTCCreatureModeManager* modeManager;
        GetCreature()->GetTC(0x31, &modeManager);
        do {
            if (mySex != 1) {
                if (AIAction_LookAtThing((EAIActionRepeat)1, CIntelligentPointer<const CThing>(PTCGossip->Gossipee.GetPItem()), 0x11, 1.0f / 360.0f) == 1)
                    goto terminate;
                if (AIAction_PerformActionAndWait(CCreatureAction_PlayAnimation(GetCreature(), "CONV_LISTENING_INTO", CSoundPair(0, 0),
                                                                                 false, true, false, 0, 0), false) != 2)
                    goto terminate;
                while (!PTCGossip->MyTurnToSpeak) {
                    if (AIAction_PerformActionAndWait(CCreatureAction_PlayAnimation(GetCreature(), "CONV_LISTENING_LOOP",
                                                                                     CSoundPair(0, 0), false, true, false, 0, 0), false) != 2)
                        break;
                }
                if (AIAction_PerformActionAndWait(CCreatureAction_PlayAnimation(GetCreature(), "CONV_LISTENING_OUTOF", CSoundPair(0, 0),
                                                                                 false, true, false, 0, 0), false) != 2)
                    goto terminate;
            } else {
                while (!PTCGossip->MyTurnToSpeak) {
                    if (AIAction_LookAtThing((EAIActionRepeat)1, CIntelligentPointer<const CThing>(PTCGossip->Gossipee.GetPItem()), 0x11, 1.0f / 360.0f) == 1)
                        goto terminate;
                }
            }
            if (otherSex == 0 && mySex != 0) {
                if (AIAction_PerformActionAndWait(CCreatureAction_PlayAnimation(GetCreature(), "ST_CROUCH_TALK_TO_CHILD_INTO",
                                                                                 CSoundPair(0, 0), false, true, false, 0, 0), false) != 2)
                    goto terminate;
                long count = Random(4) + 3;
                for (long i = 0; i < count; i++) {
                    if (AIAction_PerformActionAndWait(CCreatureAction_PlayAnimation(GetCreature(), "ST_CROUCH_TALK_TO_CHILD_LOOP",
                                                                                     CSoundPair(0, 0), false, true, false, 0, 0), false) != 2)
                        break;
                }
                PTCGossip->MyTurnToSpeak = false;
                PTCGossipOther->MyTurnToSpeak = true;
                if (AIAction_PerformActionAndWait(CCreatureAction_PlayAnimation(GetCreature(), "ST_CROUCH_TALK_TO_CHILD_OUTOF",
                                                                                 CSoundPair(0, 0), false, true, false, 0, 0), false) != 2)
                    goto terminate;
            } else {
                modeManager->AddMode(0x1B);
                long count = Random(4) + 3;
                for (long i = 0; i < count; i++) {
                    const char* anim;
                    if (Random(11) == 0) {
                        do
                            anim = s_TalkAnims[Random(10)];
                        while (anim == 0);
                    } else if (Random(2) == 0)
                        anim = s_TalkAnimsRare[Random(1)];
                    else
                        anim = "ST_IDLE";
                    if (GetCreature()->HasAnim(anim, CAnimFilterAnyGroup())) {
                        if (AIAction_PerformActionAndWait(CCreatureAction_PlayAnimation(GetCreature(), anim, CSoundPair(0, 0), false,
                                                                                         true, false, 0, 0), false) != 2)
                            break;
                    } else if (AIAction_DoNothing() != 2)
                        break;
                }
                PTCGossip->MyTurnToSpeak = false;
                PTCGossipOther->MyTurnToSpeak = true;
            }
            modeManager->RemoveMode(0x1B);
        } while (Random(100) >= 20);
    }
terminate:
    Terminate();
}
