// CAIStateGroup_SummonerCharge::Update() -- with a live target: mark its stealth as in danger, start the charge-into action, schedule the next charge frame from the def's delay, then idle while the charge action runs until the target is in reach (or the action ends), then stop any running action and play the charge-strike (live target) or charge-out-of action; always terminates
// Leaf: ?Update@CAIStateGroup_SummonerCharge
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0096FF10, 628 bytes, virtual, plain `ret`, esp frame 0x124.
// Callees: CBaseIntelligentPointer::GetPItem @ 0x00A01B50, IS_THING_ALIVE @ 0x004CC340 (__fastcall), CVectorMap::LowerBound
//          @ 0x0040F020 (inlined GetInterface 0x36 = CTCStealth), CTCStealth::SetAsInDanger @ 0x00747B80, CCombatActionBase
//          ctor @ 0x00857F30 / dtor @ 0x0062E900 (CCreatureAction_SummonerChargeInto, vtable 0x012971EC), CThingCreatureBase::
//          SetCurrentAction @ 0x006644F0 / IsPerformingAnyAction @ 0x006622A0 / StopCurrentAction @ 0x00662650,
//          CAIStateGroupBase::GetPWorld @ 0x008FCFC0 / Terminate @ 0x008FCF40 / AIAction_DoNothing @ 0x0097E200 /
//          AIAction_CombatPerformActionAndWait @ 0x0097DA40, CWorld::GetWorldFrame @ 0x0049D870, CCurrentAction::GetActionCode
//          @ 0x00692620, CThing::GetPos @ 0x004C73D0, CCreatureAction_SummonerChargeOutOf ctors @ 0x00970420 /
//          0x00970370, g_FramesPerSecond @ 0x01375550.
// Layout: CAIStateGroupBase PBrain +0x04; CAIStateGroup_SummonerCharge PTarget +0x14, PDef +0x1c, NextFrameToCharge +0x20;
//          CAIBrain PCreature +0x18; CThing PBody +0x60 (Radius +0x34), interface bitset +0x20, map +0x44, flags +0x91;
//          CThingCreatureBase PCurrentAction +0x100 (Finished +0x61); CSummonerDef ChargeReach +0x44, ChargeDelay +0x48.
// Shape: the creature is read DIRECTLY as `PBrain->PCreature` at the loop head and in the reached block (parse-time field
//        chains take distinct scratch registers: PBrain ecx, creature eax, PBody edx), but through the inline GetCreature()
//        accessor at the SetCurrentAction call (the inline-expanded chain leaves eax for the action temp's lea).
class CTCBase;
enum ETCInterfaceType { };                                   // CTCBase interface type (values unused here)
namespace std { template <class T1, class T2> struct pair { T1 first; T2 second; }; }   // STLport 4.6 <utility>
template <class K, class V> class CKeyPairCompareLess;
template <class K, class V, class TCompare> class CVectorMap;
template <> class CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > { public:
    typedef std::pair<ETCInterfaceType, CTCBase*> SEntry;
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const ETCInterfaceType& type);               // 0x0040F020
};
typedef CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > CInterfaceMap;
class C3DVector { public: float x, y, z; };
class C2DVector { public:
    float x, y;
    C2DVector(float ax, float ay) : x(ax), y(ay) {}
    float GetLengthSquared() const { return x * x + y * y; }
};
struct CThingPhysicsBody {
    unsigned char _pad[0x34];
    float Radius;                               // +0x34
};
class CThing { public:
    unsigned char _pad0[0x20];
    unsigned long m_presentBits[9];             // +0x20
    CInterfaceMap m_interfaces;                 // +0x44
    unsigned char _pad4c[0x60 - 0x4c];
    CThingPhysicsBody* PBody;                   // +0x60
    unsigned char _pad64[0x91 - 0x64];
    unsigned char m_flags91;                    // +0x91 bit 0: dead
    const C3DVector& GetPos() const;       // 0x004C73D0
    template<class T> __forceinline bool GetInterfaceX(T** ppOut, long interfaceType)
    {
        if (m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31))) {
            ETCInterfaceType type = (ETCInterfaceType)(interfaceType);
            CInterfaceMap* map = &m_interfaces;
            CInterfaceMap::SEntry* it = map->LowerBound(type);
            *ppOut = (T*)((it == map->last || interfaceType < it->first) ? map->last : it)->second;
            return true;
        }
        return false;
    }
};
class CCreatureActionBase;
struct CCurrentAction {
    unsigned char _pad[0x61];
    unsigned char Finished;                     // +0x61
    long GetActionCode();                       // 0x00692620 (manifest "DrawGetEnvironment" label is propagated)
};
class CThingCreatureBase : public CThing { public:
    unsigned char _pad94[0x100 - 0x94];
    CCurrentAction* PCurrentAction;             // +0x100
    bool SetCurrentAction(const CCreatureActionBase& action);   // 0x006644F0
    bool IsPerformingAnyAction();               // 0x006622A0
    void StopCurrentAction();                   // 0x00662650
};
class CBaseObject;
struct CBaseIntelligentPointer {
    virtual void v00();
    CThing* m_pItem;
    CBaseObject* GetPItem() const;                   // 0x00A01B50
};
struct CIntelligentPointerThing : CBaseIntelligentPointer { };
bool __fastcall IS_THING_ALIVE(const CIntelligentPointerThing& pointer);   // 0x004CC340
inline bool IsTargetAlive(const CIntelligentPointerThing& p)
{
    if (((CThing*)p.GetPItem()) != 0) {
        if (!(((CThing*)p.GetPItem())->m_flags91 & 1))
            return true;
    }
    return false;
}
class CTCStealth { public:                             // interface 0x36
    void SetAsInDanger(CThing* source, long status);   // 0x00747B80
};
class CSoundPair { public:
    long m_first;
    long m_second;
    CSoundPair() : m_first(0), m_second(0) { }
};
class CCreatureActionBase { public:
    virtual void v00();
    unsigned char _body[0xa4];
};
class CCombatActionBase : public CCreatureActionBase { public:
    unsigned char _combat[0x6c];
    CCombatActionBase(CThingCreatureBase* owner, CThing* thing, long actionCode, const CSoundPair& sound);   // 0x00857F30
    virtual ~CCombatActionBase();                       // 0x0062E900
};
struct CCreatureAction_SummonerChargeInto : CCombatActionBase {   // vtable 0x012971EC
    virtual void v00();
    CCreatureAction_SummonerChargeInto(CThingCreatureBase* owner, CThing* thing)
        : CCombatActionBase(owner, thing, 150, CSoundPair()) { }
};
struct CCreatureAction_SummonerChargeOutOf : CCombatActionBase {
    CCreatureAction_SummonerChargeOutOf(CThingCreatureBase* owner, CThing* thing);   // 0x00970420
    CCreatureAction_SummonerChargeOutOf(CThingCreatureBase* owner);   // 0x00970370
};
class CWorld { public:
    static long __fastcall GetFrame();                // 0x0049D870
};
extern long g_FramesPerSecond;                  // 0x01375550
class CSummonerDef { public:
    unsigned char _pad[0x44];
    float ChargeReach;                          // +0x44
    float ChargeDelay;                          // +0x48
};
class CAIBrain { public:
    unsigned char _pad[0x18];
    CThingCreatureBase* PCreature;              // +0x18
};
enum EAIActionReturn { };
class CAIStateGroupBase { public:
    virtual ~CAIStateGroupBase();
    CAIBrain* PBrain;                           // +0x04
    long ScriptAIPriority;                      // +0x08
    void* Name_PStringData;                     // +0x0c
    unsigned char Terminated;                   // +0x10
    unsigned char Priority;
    unsigned char Enabled;
    unsigned char _pad_0x13;
    CWorld* GetPWorld();                        // 0x008FCFC0
    void Terminate();                           protected: // 0x008FCF40
    EAIActionReturn AIAction_DoNothing(); public:                  // 0x0097E200
    long AIAction_CombatPerformActionAndWait(CCreatureActionBase* action, bool wait);   // 0x0097DA40
};
struct CAIStateGroup_SummonerCharge : CAIStateGroupBase {
    CIntelligentPointerThing PTarget;           // +0x14
    const CSummonerDef* PDef;                   // +0x1c CDefPointer<const CSummonerDef>
    long NextFrameToCharge;                     // +0x20
    CThingCreatureBase* GetCreature() const { return PBrain->PCreature; }
    virtual void Update();
};

void CAIStateGroup_SummonerCharge::Update()
{
    if (!IsTargetAlive(PTarget)) {
        Terminate();
        return;
    }
    CTCStealth* stealth;
    if (((CThing*)PTarget.GetPItem())->GetInterfaceX(&stealth, 0x36))
        stealth->SetAsInDanger(GetCreature(), 3);
    CThingCreatureBase* owner = GetCreature();
    if (!GetCreature()->SetCurrentAction(CCreatureAction_SummonerChargeInto(owner, ((CThing*)PTarget.GetPItem())))) {
        Terminate();
        return;
    }
    CWorld* world = GetPWorld();
    long fps = g_FramesPerSecond;
    const CSummonerDef* def = PDef;
    NextFrameToCharge = (long)((float)world->GetFrame() + (float)fps * def->ChargeDelay);
    bool reached = false;
    bool done = false;
    long actionCode = PBrain->PCreature->PCurrentAction->GetActionCode();
    do {
        float reach = PBrain->PCreature->PBody->Radius + PDef->ChargeReach;
        CCurrentAction* action = PBrain->PCreature->PCurrentAction;
        if (IsTargetAlive(PTarget)) {
            if (action == 0 || action->Finished) {
                reached = true;
                done = true;
            } else if (action->GetActionCode() != actionCode) {
                done = true;
            } else {
                CThingCreatureBase* creature = GetCreature();
                CThing* target = ((CThing*)PTarget.GetPItem());
                const C3DVector& from = creature->GetPos();
                const C3DVector& to = target->GetPos();
                float distanceSquared = C2DVector(to.x - from.x, to.y - from.y).GetLengthSquared();
                if (distanceSquared < reach * reach) {
                    reached = true;
                    done = true;
                }
            }
        } else {
            done = true;
        }
        if (AIAction_DoNothing() == 1) {
            Terminate();
            return;
        }
    } while (!done);
    if (reached) {
        if (PBrain->PCreature->IsPerformingAnyAction())
            PBrain->PCreature->StopCurrentAction();
        if (IS_THING_ALIVE(PTarget)) {
            CThingCreatureBase* creature = GetCreature();
            AIAction_CombatPerformActionAndWait(&CCreatureAction_SummonerChargeOutOf(creature, ((CThing*)PTarget.GetPItem())), true);
        } else {
            AIAction_CombatPerformActionAndWait(&CCreatureAction_SummonerChargeOutOf(GetCreature()), true);
        }
    }
    Terminate();
}