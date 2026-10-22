// Grade: DIFFER(916v970) -- structure approximated: retail passes the look-at intelligent pointer constructed in the argument slot, keeps the defender-zone switch as dec/dec/sub, inlines a nulling release of the attack-style def on both terminate paths and holds creature/target in ebp/edi around GetDefenderZone; ours differs in all four (3 tries)
// CAIStateGroup_MazeTutorialBattle::Update() -- while the target lives: in boss phases 0..2 reset Maze's will-power indicator, give him the tutorial attack style, and either pump combat actions when the target is within combat range and in a defender zone or look at the target; advance the boss phase when ready; outside phases 0..2 do nothing; then answer a knocked-down target with a drain response
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00955C90, length 970 bytes (+ int3 padding), plain `ret`.
#include <math.h>
struct C3DVector {
    float X, Y, Z;
    C3DVector() { }
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) { }
    float GetLength() const { return (float)sqrt(Z * Z + Y * Y + X * X); }
};
inline C3DVector operator-(const C3DVector& a, const C3DVector& b) { return C3DVector(a.X - b.X, a.Y - b.Y, a.Z - b.Z); }
struct CCharString {
    void* PData;
    CCharString(const char* text, long length = -1);  // 0x0099EBF0
    ~CCharString();                              // 0x0099EAE0
};
struct CRefCounted {
    virtual void v00();
    virtual void DeleteThis();
    long RefCount;                              // +0x04
};
struct CAICreatureWillPowerIndicatorDef : CRefCounted { };
struct CAttackStyleDef : CRefCounted { };
template<class T> struct CDefPointer {
    T* P;
    CDefPointer() : P(0) { }
    ~CDefPointer() { if (P) { if (--P->RefCount == 0) P->DeleteThis(); } }
};
template<class T> struct CCountedDefPointer {
    T* P;
    CCountedDefPointer() : P(0) { }
    ~CCountedDefPointer() { Release(); }
    void Release()
    {
        if (P) {
            long count = --P->RefCount;
            if (count == 0)
                P->DeleteThis();
            P = 0;
        }
    }
};
struct CDefClassBase {
    void GetSubDef(CDefPointer<CAICreatureWillPowerIndicatorDef>* out);   // 0x008C3510
};
struct CDefinitionManager {
    void GetDef(const CCharString& name, CCountedDefPointer<CAttackStyleDef>* out);   // 0x00669770
};
struct CTCBase;
struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);       // 0x0040F020
};
struct CCombatTypeDef {
    unsigned char _pad[0x70];
    float Range;                                // +0x70
};
struct CThing {
    unsigned char _pad0[0x20];
    unsigned long m_presentBits[9];             // +0x20
    CInterfaceMap m_interfaces;                 // +0x44
    unsigned char _pad4c[0x6c - 0x4c];
    unsigned char Flags6c;                      // +0x6c
    unsigned char _pad6d[3];
    CDefClassBase* PDef;                        // +0x70
    unsigned char _pad74[0x91 - 0x74];
    unsigned char Flags91;                      // +0x91
    const C3DVector& GetPosition() const;       // 0x004C73D0
    template<class T> __forceinline void GetInterface(T** ppOut, long interfaceType)
    {
        if (m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31))) {
            long type = interfaceType;
            CInterfaceMap* map = &m_interfaces;
            CInterfaceMap::SEntry* it = map->LowerBound(&type);
            *ppOut = (T*)((it == map->last || interfaceType < it->type) ? map->last : it)->component;
        }
    }
};
struct CThingCreatureBase : CThing {
    bool CanBeReadyToAttack();                  // 0x00662410
    CCombatTypeDef* GetCombatTypeDef();         // 0x00666D10
};
bool __fastcall IsKnockedDown(CThing* thing);   // 0x0085F010
#pragma pack(push, 1)
struct CTCBoss {
    long GetBossPhase();                        // 0x007AED10
    void SetNextBossPhase(long phase, bool immediate);   // 0x007AEDA0
    bool IsReadyForPhaseChange();               // 0x007AEE90
};
#pragma pack(pop)
struct CTCAICreatureWillPowerIndicator {
    void Reset(float value);                    // 0x007E0E70
};
struct CTCCreatureCombat {                      // interface 0x30
    void SetAttackStyle(CCountedDefPointer<CAttackStyleDef>& def);   // 0x007355F0
};
struct CCombatManager {
    long GetDefenderZone(CThing* target, CThingCreatureBase* attacker, long flags);   // 0x006E99B0
};
struct CWorld {
    unsigned char _pad0[0x4c];
    CCombatManager* PCombatManager;             // +0x4c
};
struct CBaseIntelligentPointer {
    virtual void v00();
    CThing* m_pItem;
    CThing* GetPItem() const;                   // 0x00A01B50
};
struct CIntelligentPointerConstThing {
    void* vptr;
    const CThing* m_pItem;
    CIntelligentPointerConstThing(const CBaseIntelligentPointer& other);   // 0x007C2930
};
struct CCreatureActionBase {
    virtual void v00();
    unsigned char _body[0x110];
};
struct CCombatAction_GenericResponseToDrain : CCreatureActionBase {
    CCombatAction_GenericResponseToDrain(CThingCreatureBase* creature, CThing* source);   // 0x00906B00
    ~CCombatAction_GenericResponseToDrain();    // 0x0062E900
};
struct CAIBrain {
    unsigned char _pad[0x18];
    CThingCreatureBase* PCreature;              // +0x18
};
inline bool IsAlive(const CBaseIntelligentPointer& p)
{
    if (p.GetPItem() == 0) return false;
    if (p.GetPItem()->Flags91 & 1) return false;
    return true;
}
struct CAIStateGroupBase {
    virtual ~CAIStateGroupBase();
    CAIBrain* PBrain;                           // +0x04
    unsigned char _pad08[0x0c];
    CThingCreatureBase* GetCreature() const { return PBrain->PCreature; }
    CDefinitionManager* GetDefinitionManager(); // 0x008FCFB0
    CWorld* GetPWorld();                        // 0x008FCFC0
    bool IsStopping();                          // 0x008FD010
    void Terminate();                           // 0x008FCF40
    long AIAction_LookAtThing(long repeat, CIntelligentPointerConstThing thing, long flags, float tolerance);   // 0x0097D6F0
    long AIAction_PumpCombatActions();          // 0x0097D980
    long AIAction_DoNothing();                  // 0x0097E200
    long AIAction_PerformActionAndWait(CCreatureActionBase* action, bool wait);   // 0x0097DB70
};
struct CAIStateGroup_MazeTutorialBattle : CAIStateGroupBase {
    CBaseIntelligentPointer PTarget;            // +0x14
    bool IsStillRunnable();                     // 0x009542F0
    void Update();
};

void CAIStateGroup_MazeTutorialBattle::Update()
{
    bool stop = !IsAlive(PTarget) || IsStopping();
    if (stop) {
        Terminate();
        return;
    }
    CThingCreatureBase* creature = GetCreature();
    CTCBoss* boss;
    if (creature->m_presentBits[0xad >> 5] & (1UL << (0xad & 31))) {
        *(long*)&boss = 0xad;
        CInterfaceMap* map = &creature->m_interfaces;
        CInterfaceMap::SEntry* it = map->LowerBound((long*)&boss);
        boss = (CTCBoss*)((it == map->last || 0xad < it->type) ? map->last : it)->component;
    }
    CDefPointer<CAICreatureWillPowerIndicatorDef> willDef;
    creature->PDef->GetSubDef(&willDef);
    CTCCreatureCombat* combat;
    GetCreature()->GetInterface(&combat, 0x30);
    long phase = boss->GetBossPhase();
    if (phase >= 0 && phase <= 2) {
        CTCAICreatureWillPowerIndicator* indicator;
        CThingCreatureBase* me = GetCreature();
        if (me->m_presentBits[0xce >> 5] & (1UL << (0xce & 31))) {
            me->GetInterface(&indicator, 0xce);
            indicator->Reset(10.0f);
        }
        CCountedDefPointer<CAttackStyleDef> style;
        {
            CCharString name("HERO_MAZE_TUTORIAL_ATTACK_STYLE2");
            GetDefinitionManager()->GetDef(name, &style);
        }
        combat->SetAttackStyle(style);
        float distance = (GetCreature()->GetPosition() - PTarget.GetPItem()->GetPosition()).GetLength();
        if (distance < GetCreature()->GetCombatTypeDef()->Range) {
            if (GetCreature()->CanBeReadyToAttack() && (PTarget.GetPItem()->Flags6c & 8)) {
                CThing* target = PTarget.GetPItem();
                CThingCreatureBase* attacker = GetCreature();
                long zone = GetPWorld()->PCombatManager->GetDefenderZone(target, attacker, 0);
                if (zone != 1 && zone != 2 && zone != 4) {
                    AIAction_LookAtThing(1, CIntelligentPointerConstThing(PTarget), 0x21, 1.0f / 12.0f);
                    if (!IsStillRunnable()) {
                        Terminate();
                        style.Release();
                        return;
                    }
                }
                AIAction_PumpCombatActions();
            }
        } else {
            AIAction_LookAtThing(1, CIntelligentPointerConstThing(PTarget), 0x21, 1.0f / 12.0f);
            if (!IsStillRunnable()) {
                Terminate();
                style.Release();
                return;
            }
        }
        if (boss->IsReadyForPhaseChange())
            boss->SetNextBossPhase(boss->GetBossPhase() + 1, false);
    } else {
        AIAction_DoNothing();
    }
    if (IsAlive(PTarget) && !IsStopping() && (PTarget.GetPItem()->Flags6c & 8)) {
        CThing* target = PTarget.GetPItem();
        if (IsKnockedDown(PTarget.GetPItem()))
            AIAction_PerformActionAndWait(&CCombatAction_GenericResponseToDrain(GetCreature(), target), false);
    }
}
