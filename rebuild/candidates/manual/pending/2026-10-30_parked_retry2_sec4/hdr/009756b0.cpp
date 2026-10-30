// CAIStateGroup_JackOfBladesStage3::Update() -- the final Jack fight: kill the sword emitter, scream, wait out the will-power rise time, shrink back to 1.2 scale, fall underground into boss phase 5, stumble at random until the phase change is ready, then phase 6: teleport to the arena centre, rise and wait; terminate on every exit
// Grade: DIFFER(1270v1276) nd=214 (unchanged) -- header move: the local counted-pointer model moved onto engine/CCountedPointer.h as CCountedPointer<NCreatureMode::CModeJackOfBladesScream> (Ego_r.pdb member type). Tried: no def alias (226). Residue: register roles (retail creature ebp, def ebx), loop-1 rotation jmp, frame 8 larger. Next: frame packing of the action temporaries.
// Retail: Fable.exe (TLC, VC7.1) function @ 0x009756B0, length 1276 bytes (+ int3 padding).
// Callees: CVectorMap::LowerBound @ 0x0040F020 (inlined GetInterface 0xad = CTCBoss), CDefClassBase::GetSubDef
//          <CAICreatureWillPowerIndicatorDef> @ 0x008C33E0, CBaseIntelligentPointer::GetPItem @ 0x00A01B50, CThing::Kill
//          @ 0x004C9B80, the physics body's slots 31/38/66 (+0x7c/+0x98/+0x108), CCreatureAction_JackScream @ 0x00977100,
//          CCreatureAction_JackFall @ 0x00977320, CCreatureAction_JackDeathInto @ 0x009771F0, CCreatureAction_PlayCombatAnimation
//          @ 0x008B7FF0, ~CCombatActionBase @ 0x0062E900, AIAction_PerformActionAndWait @ 0x0097DB70, AIAction_PerformAction
//          @ 0x0097D8B0, AIAction_DoNothing @ 0x0097E200, GetPWorld @ 0x008FCFC0, CWorld::GetWorldFrame @ 0x0049D870,
//          CWorld::GetPRandomSeed @ 0x0049D810, GFROR13 @ 0x00497890, IsStopping @ 0x008FD010,
//          CAIStateGroup_JackOfBladesBase::IsStillRunnable @ 0x00971B00, CTCBoss::SetNextBossPhase @ 0x007AEDA0 /
//          IsReadyForPhaseChange @ 0x007AEE90, CTCGraphicAppearance::SetScale @ 0x004BFA50, CCharString @ 0x0099EBF0 /
//          0x0099EAE0, operator delete @ 0x00BFE9BC, Terminate @ 0x008FCF40.
// Globals: frames per second @ 0x01375550, 1.2f @ 0x0125A690.
// Layout: engine/CAIStateGroup_JackOfBladesBase.h (PSwordEmitter +0x14, PArenaCentre +0x1c, PTargetThing +0x24).
#include "engine/CCountedPointer.h"
#include "engine/CAIStateGroup_JackOfBladesBase.h"

struct CTCBase;
struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);       // 0x0040F020
};
struct C3DVector { float X, Y, Z; };
struct CPhysicsBody {                           // the thing's +0x60 object
#define VF(n) virtual void v##n();
#define VF10(p) VF(p##0) VF(p##1) VF(p##2) VF(p##3) VF(p##4) VF(p##5) VF(p##6) VF(p##7) VF(p##8) VF(p##9)
    VF10(0) VF10(1) VF10(2)
    VF(30)
    virtual void SetPosition(const C3DVector& position);   // slot 31 (+0x7c)
    VF(32) VF(33) VF(34) VF(35) VF(36) VF(37)
    virtual void SetPhysicsEnabled(bool enabled);         // slot 38 (+0x98)
    VF(39) VF10(4) VF10(5)
    VF(60) VF(61) VF(62) VF(63) VF(64) VF(65)
    virtual void SetCollisionEnabled(bool enabled);       // slot 66 (+0x108)
#undef VF10
#undef VF
};
struct CTCGraphicAppearance {
    unsigned char _pad[0x34];
    float Scale;                                // +0x34
    void SetScale(float scale);                 // 0x004BFA50
};
struct CAICreatureWillPowerIndicatorDef {
    virtual void v00();
    virtual void Destroy();                     // slot 1
    long m_refCount;                            // +0x04
    unsigned char _pad08[0x48 - 0x08];
    float RiseTime;                             // +0x48 seconds (descriptive)
    unsigned char _pad4c[0x5c - 0x4c];
    float ShrinkTime;                           // +0x5c seconds (descriptive)
    float RecoverTime;                          // +0x60 seconds (descriptive)
};
struct CDefClassBase {
    bool GetSubDef(CAICreatureWillPowerIndicatorDef** out);   // 0x008C33E0
};
struct CThing {
    unsigned char _pad0[0x20];
    unsigned long m_presentBits[9];             // +0x20 std::bitset of interface types
    CInterfaceMap m_interfaces;                 // +0x44
    unsigned char _pad4c[0x60 - 0x4c];
    CPhysicsBody* PhysicsBody;                  // +0x60
    CTCGraphicAppearance* Appearance;           // +0x64
    unsigned char _pad68[0x70 - 0x68];
    CDefClassBase* PDef;                        // +0x70
    unsigned char _pad74[0x91 - 0x74];
    unsigned char m_flags91;                    // +0x91 bit 0: dead
    const C3DVector& GetPosition();             // 0x004C73D0
    void Kill(bool immediately);                // 0x004C9B80
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
};
struct CThingCreatureBase : CThing { };
struct CTCBoss {                                // interface 0xad
    void SetNextBossPhase(long phase, bool immediate);   // 0x007AEDA0
    bool IsReadyForPhaseChange();               // 0x007AEE90
};
struct CBaseIntelligentPointer {
    virtual void v00();
    CThing* m_pItem;                            // +0x04
    CThing* GetPItem() const;                   // 0x00A01B50
};
inline bool IS_THING_ALIVE(const CBaseIntelligentPointer& p)
{
    if (p.GetPItem() != 0) {
        if (!(p.GetPItem()->m_flags91 & 1))
            return true;
    }
    return false;
}
struct CCharString {
    void* m_pData;
    CCharString(const char* text, long length = -1);   // 0x0099EBF0
    ~CCharString();                             // 0x0099EAE0
};
struct CCountBlock {
    long Count;
    void (__fastcall* Destroy)(void* object);
    void* Object;
};
struct CCreatureActionBase {
    virtual void v00();
};
struct CCombatActionBase : CCreatureActionBase {   // retail sizeof 0x114
    unsigned char _body[0x114 - 4];
    ~CCombatActionBase();                       // 0x0062E900
};
namespace NCreatureMode { class CModeJackOfBladesScream { public: virtual ~CModeJackOfBladesScream(); }; }
struct CCreatureAction_JackScream : CCombatActionBase {
    CCountedPointer<NCreatureMode::CModeJackOfBladesScream> PModeCountedPointer;   // +0x114 (Ego_r.pdb)
    CCreatureAction_JackScream(CThingCreatureBase* owner);   // 0x00977100
};
struct CCreatureAction_JackFall : CCombatActionBase {
    CCreatureAction_JackFall(CThingCreatureBase* owner);     // 0x00977320
};
struct CCreatureAction_JackDeathInto : CCombatActionBase {
    CCreatureAction_JackDeathInto(CThingCreatureBase* owner);   // 0x009771F0
};
struct CCreatureAction_PlayCombatAnimation : CCombatActionBase {
    long m_loop;
    CCreatureAction_PlayCombatAnimation(CThingCreatureBase* creature, const CCharString& animation, bool loop);   // 0x008B7FF0
};
struct CWorld {
    static long GetWorldFrame();                // 0x0049D870
    unsigned long* GetPRandomSeed();            // 0x0049D810
};
unsigned long __fastcall GFROR13(unsigned long value);   // 0x00497890
extern long g_GameFramesPerSecond;              // 0x01375550
struct CAIBrain {
    unsigned char _pad[0x18];
    CThingCreatureBase* PCreature;              // +0x18
};

struct CAIStateGroupJackOfBladesStage3Methods : CAIStateGroup_JackOfBladesBase {
    CAIBrain* Brain() const { return *(CAIBrain* const*)_pad_0x04; }
    CThingCreatureBase* GetCreature() const { return Brain()->PCreature; }
    CBaseIntelligentPointer& SwordEmitter() { return *(CBaseIntelligentPointer*)PSwordEmitter; }
    CBaseIntelligentPointer& ArenaCentre() { return *(CBaseIntelligentPointer*)PArenaCentre; }
    CBaseIntelligentPointer& TargetThing() { return *(CBaseIntelligentPointer*)PTargetThing; }
    CWorld* GetPWorld();                        // 0x008FCFC0
    bool IsStopping();                          // 0x008FD010
    void Terminate();                           // 0x008FCF40
    long AIAction_PerformActionAndWait(CCreatureActionBase* action, bool wait);   // 0x0097DB70
    long AIAction_PerformAction(CCreatureActionBase* action);   // 0x0097D8B0
    long AIAction_DoNothing();                  // 0x0097E200
    bool IsStillRunnable();                     // 0x00971B00
    void Update();
};

void CAIStateGroupJackOfBladesStage3Methods::Update()
{
    CThingCreatureBase* creature = GetCreature();
    CTCBoss* boss;
    creature->GetInterface(&boss, 0xad);
    CAICreatureWillPowerIndicatorDef* subDef = 0;
    creature->PDef->GetSubDef(&subDef);
    if (IS_THING_ALIVE(SwordEmitter()))
        SwordEmitter().GetPItem()->Kill(true);
    GetCreature()->PhysicsBody->SetCollisionEnabled(false);
    AIAction_PerformActionAndWait(&CCreatureAction_JackScream(GetCreature()), false);

    long start = GetPWorld()->GetWorldFrame();
    CAICreatureWillPowerIndicatorDef* def = subDef;
    do {
        if (!IS_THING_ALIVE(TargetThing()) || IsStopping() || !IS_THING_ALIVE(ArenaCentre())) {
            Terminate();
            if (def) {
                if (--def->m_refCount == 0)
                    def->Destroy();
            }
            return;
        }
        AIAction_DoNothing();
    } while ((float)(GetPWorld()->GetWorldFrame() - start) < def->RiseTime * g_GameFramesPerSecond);

    float scale = GetCreature()->Appearance->Scale;
    float step = (1.2f - scale) / (g_GameFramesPerSecond * def->ShrinkTime);
    while (scale > 1.2f && IS_THING_ALIVE(TargetThing()) && !IsStopping() && IS_THING_ALIVE(ArenaCentre())) {
        scale = step + GetCreature()->Appearance->Scale;
        GetCreature()->Appearance->SetScale(scale);
        AIAction_DoNothing();
    }

    if (IsStillRunnable()) {
        GetCreature()->Appearance->SetScale(1.2f);
        GetCreature()->PhysicsBody->SetPhysicsEnabled(true);
        AIAction_PerformActionAndWait(&CCreatureAction_JackFall(GetCreature()), false);
        boss->SetNextBossPhase(5, false);
        if (IsStillRunnable()) {
            long last = GetPWorld()->GetWorldFrame();
            while (IsStillRunnable() && !boss->IsReadyForPhaseChange()) {
                AIAction_DoNothing();
                if ((GetPWorld()->GetWorldFrame() - last) / g_GameFramesPerSecond > 3) {
                    unsigned long* seed = GetPWorld()->GetPRandomSeed();
                    *seed = *seed * 0x24A1 + 0x24DF;
                    *seed = GFROR13(*seed);
                    if (*seed % 100 < 33) {
                        CCharString animation("STUMBLE");
                        AIAction_PerformAction(&CCreatureAction_PlayCombatAnimation(GetCreature(), animation, true));
                        last = GetPWorld()->GetWorldFrame();
                    }
                }
            }
            if (IsStillRunnable() && boss->IsReadyForPhaseChange()) {
                boss->SetNextBossPhase(6, false);
                if (IsStillRunnable()) {
                    GetCreature()->PhysicsBody->SetCollisionEnabled(false);
                    GetCreature()->PhysicsBody->SetPosition(ArenaCentre().GetPItem()->GetPosition());
                    AIAction_PerformActionAndWait(&CCreatureAction_JackDeathInto(GetCreature()), false);
                    if (IsStillRunnable()) {
                        long riseStart = GetPWorld()->GetWorldFrame();
                        while (IsStillRunnable()) {
                            AIAction_DoNothing();
                            long elapsed = GetPWorld()->GetWorldFrame() - riseStart;
                            if (!((float)elapsed < g_GameFramesPerSecond * def->RecoverTime))
                                break;
                        }
                    }
                }
            }
        }
    }
    Terminate();
    if (--def->m_refCount == 0)
        def->Destroy();
}
