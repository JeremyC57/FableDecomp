// Grade: DIFFER(796v786) -- first pass: frame slot order (nymph out-slot +0x14, target +0x18, flag word +0x20 in retail), the summon conditional-temporary path (retail tests the flag word after a setcc-free bl join) and the roar/summon branch layout differ; the call sequence and constants are right
// CAIStateGroup_NymphAttack::Update() -- attack the nymph's living target: when the nymph should summon and sees the target, roar, then (target alive and nymph unhurt) summon creatures and, still unhurt, go back out; otherwise mark it ready to summon and, while the target lives within a decent range, shoot at it every interval (up to the def's shot count) or between shots play the hold action, stopping once hit; then clean up and Terminate()
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0091C6A0, length 800 bytes (+ int3 padding), plain `ret` (virtual), `and esp,-8` frame.
// Callees: CVectorMap::LowerBound @ 0x0040F020 (GetInterface 0xF5 = CTCNymph, bit 21 of +0x3c, unchecked), CBaseIntelligentPointer
// GetPItem @ 0x00A01B50 / ctor @ 0x00A01B10 / SetPItem @ 0x00A01B90 / dtor @ 0x00A01C10, CTCNymph::GetShouldSummon @ 0x0080FD30 /
// IsDecentAttackRange @ 0x0080F190 / the target cleanup @ 0x0091C080, IsTargetInLineOfSight @ 0x0091C220, the nymph combat
// actions' ctors @ 0x008CA5A0, 0x008B83E0 (CCreatureAction_SummonCreatures), 0x008CA600 (CCreatureAction_NymphSummonOutOf),
// 0x008CA680, 0x008CA700 / ~CCombatActionBase @ 0x0062E900, AIAction_CombatPerformActionAndWait @ 0x0097DA40,
// IS_THING_ALIVE @ 0x006961F0 (__fastcall, out of line), _ftol @ 0x00BFEA70, CWorld::GetWorldFrame @ 0x0049D870,
// CThing::GetPosition @ 0x004C73D0, Terminate @ 0x008FCF40.
// Data: g_framesPerSecond @ 0x01375550.
// Layout: local models (PBrain +4; brain PCreature +0x18; retail CTCNymph: PDef +0x0c (shot count +0x40, shot interval seconds
// +0x44), ReadyToSummon +0x5a, BeenHit +0x5d, PTarget +0x68; CCombatActionBase 0x114, SummonCreatures 0x118).
// Shape: the summon-creatures action is a conditional temporary (flag word at +0x20, destroyed under `test 1`).
struct CTCBase;
struct C3DVector { float X, Y, Z; };
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
    unsigned char _pad4c[0x91 - 0x4c];
    unsigned char m_flags91;                    // +0x91 bit 0: dead
    const C3DVector& GetPosition() const;       // 0x004C73D0
    template<class T> __forceinline bool GetInterface(T** ppOut, long interfaceType)
    {
        if (m_presentBits[interfaceType >> 5] & (1u << (interfaceType & 31))) {
            long type = interfaceType;
            CInterfaceMap* map = &m_interfaces;
            CInterfaceMap::SEntry* it = map->LowerBound(&type);
            *ppOut = (T*)((it == map->last || interfaceType < it->type) ? map->last : it)->component;
            return true;
        }
        return false;
    }
};
struct CThingCreatureBase : CThing { };
struct CBaseIntelligentPointer {
    virtual void v00();
    CThing* m_pItem;                            // +0x04
    CBaseIntelligentPointer();                  // 0x00A01B10
    ~CBaseIntelligentPointer();                 // 0x00A01C10
    CThing* GetPItem() const;                   // 0x00A01B50
    void SetPItem(const CThing* item);          // 0x00A01B90
};
struct CIntelligentPointerCreature : CBaseIntelligentPointer {     // CIntelligentPointer<CThingCreatureBase>, vtable 0x0123E7D8
    virtual void v00();
    __forceinline CIntelligentPointerCreature(CThing* item) : CBaseIntelligentPointer() { SetPItem(item); }
    CThingCreatureBase* GetPCreature() const { return (CThingCreatureBase*)GetPItem(); }
};
bool __fastcall IS_THING_ALIVE(const CBaseIntelligentPointer& p);   // 0x006961F0
__forceinline bool IsAlive(const CBaseIntelligentPointer& p)
{
    if (p.GetPItem() != 0) {
        if (!(p.GetPItem()->m_flags91 & 1))
            return true;
    }
    return false;
}
struct CNymphDef {
    unsigned char _pad[0x40];
    long ShotCount;                             // +0x40
    float ShotIntervalSeconds;                  // +0x44
};
struct CTCNymph {                               // interface 0xF5
    unsigned char _pad0[0x0c];
    CNymphDef* PDef;                            // +0x0c
    unsigned char _pad10[0x5a - 0x10];
    bool ReadyToSummon;                         // +0x5a
    unsigned char _pad5b[2];
    bool BeenHit;                               // +0x5d
    unsigned char _pad5e[0x68 - 0x5e];
    CBaseIntelligentPointer PTarget;            // +0x68
    bool GetShouldSummon();                     // 0x0080FD30
    bool IsDecentAttackRange(const C3DVector& position);   // 0x0080F190
    void ClearTarget();                         // 0x0091C080
};
struct CCombatActionBase {
    virtual void v00();
    unsigned char _body[0x110];                 // retail sizeof 0x114
    ~CCombatActionBase();                       // 0x0062E900
};
struct CCreatureAction_NymphRoar : CCombatActionBase {
    CCreatureAction_NymphRoar(CThingCreatureBase* owner, CThing* target, bool flag);   // 0x008CA5A0
};
struct CCreatureAction_SummonCreatures : CCombatActionBase {
    bool IsFromInventory;                       // +0x114
    CCreatureAction_SummonCreatures(CThingCreatureBase* owner, CThing* target, bool isFromInventory);   // 0x008B83E0
};
struct CCreatureAction_NymphSummonOutOf : CCombatActionBase {
    CCreatureAction_NymphSummonOutOf(CThingCreatureBase* owner);   // 0x008CA600
};
struct CCreatureAction_NymphShoot : CCombatActionBase {
    CCreatureAction_NymphShoot(CThingCreatureBase* owner, CThing* target, bool flag);   // 0x008CA680
};
struct CCreatureAction_NymphHold : CCombatActionBase {
    CCreatureAction_NymphHold(CThingCreatureBase* owner, CThing* target, bool flag);    // 0x008CA700
};
struct CWorld {
    static long GetWorldFrame();                // 0x0049D870
};
extern long g_framesPerSecond;                  // 0x01375550
struct CAIBrain {
    unsigned char _pad[0x18];
    CThingCreatureBase* PCreature;              // +0x18
};
struct CAIStateGroupBase {
    virtual ~CAIStateGroupBase();
    CAIBrain* PBrain;                           // +0x04
    CThingCreatureBase* GetCreature() const { return PBrain->PCreature; }
    void Terminate();                           // 0x008FCF40
    long AIAction_CombatPerformActionAndWait(CCombatActionBase* action, bool wait);   // 0x0097DA40
};
struct CAIStateGroup_NymphAttack : CAIStateGroupBase {
    bool IsTargetInLineOfSight(CThing* target); // 0x0091C220
    virtual void Update();
};

void CAIStateGroup_NymphAttack::Update()
{
    long shots = 0;
    CTCNymph* nymph;
    GetCreature()->GetInterface(&nymph, 0xf5);
    CIntelligentPointerCreature target(nymph->PTarget.GetPItem());
    if (IsAlive(target)) {
        if (nymph->GetShouldSummon() && IsTargetInLineOfSight(target.GetPItem())) {
            if (AIAction_CombatPerformActionAndWait(&CCreatureAction_NymphRoar(GetCreature(), target.GetPItem(), false), false) == 2) {
                if (IS_THING_ALIVE(target) && !nymph->BeenHit
                        && AIAction_CombatPerformActionAndWait(&CCreatureAction_SummonCreatures(GetCreature(), target.GetPItem(), false), false) == 2
                        && !nymph->BeenHit)
                    AIAction_CombatPerformActionAndWait(&CCreatureAction_NymphSummonOutOf(GetCreature()), false);
            }
        } else {
            long maxShots = nymph->PDef->ShotCount;
            long interval = (long)((float)g_framesPerSecond * nymph->PDef->ShotIntervalSeconds);
            long lastShot = 0;
            nymph->ReadyToSummon = true;
            while (IsAlive(target) && nymph->IsDecentAttackRange(target.GetPItem()->GetPosition())) {
                if (CWorld::GetWorldFrame() >= lastShot + interval && (maxShots <= 0 || ++shots <= maxShots)) {
                    if (!IsTargetInLineOfSight(target.GetPItem()))
                        break;
                    if (AIAction_CombatPerformActionAndWait(&CCreatureAction_NymphShoot(GetCreature(), target.GetPItem(), false), false) != 2)
                        break;
                    lastShot = CWorld::GetWorldFrame();
                } else {
                    shots = 0;
                    if (AIAction_CombatPerformActionAndWait(&CCreatureAction_NymphHold(GetCreature(), target.GetPItem(), false), false) != 2)
                        break;
                }
                if (nymph->BeenHit)
                    break;
            }
        }
        nymph->ClearTarget();
    }
    Terminate();
}
