// CAIStateGroup_MazeDefensiveMeleeCombat::Update() -- Maze's defensive duel loop: enflame and quit once his will power is active; inside melee range defend (face the enemy unless he is in a defender zone) and pump combat actions; inside the def's retreat range teleport-move to the retreat marker and look at the enemy; inside the throw range heal when hurt and the heal delay elapsed, else throw a shot and follow up with a blast; beyond it blast; terminate on every exit
// Grade: DIFFER(1635v1647) -- PDef load order before the def zero store, creature register for the 0xce bitset test, inline lower_bound register roles (retail count edi / first ecx), 2D distance pop order, interface 0xad/0x31 block order, def release through a register without the null test
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00955620, length 1647 bytes (+ int3 padding).
// Callees: CDefClassBase::GetSubDef @ 0x008C3510, CBaseIntelligentPointer::GetPItem @ 0x00A01B50 / ctor @ 0x00A01B10 /
//          SetPItem @ 0x00A01B90 / dtor @ 0x00A01C10, IsStopping @ 0x008FD010, CThing::GetPosition @ 0x004C73D0,
//          CThingCreatureBase::GetCombatTypeDef @ 0x00666D10 / IsRegisteredCombatant @ 0x00662410 / IsUsingShield @ 0x006621A0 /
//          GetHealthFraction @ 0x006A4E40, CCombatManager::GetDefenderZone @ 0x006E99B0, AIAction_LookAtThing @ 0x0097D6F0,
//          AIAction_PumpCombatActions @ 0x0097D980, CVectorMap::LowerBound @ 0x0040F020 (interfaces 0xad, 0x31; the 0xce
//          lookup is the inline lower_bound), CTCCreatureModeManager::HasMode @ 0x0073A8A0 / AddMode @ 0x0073AE10,
//          AIAction_MoveToPosProximity @ 0x0097E890, CBrainOutput copy ctor @ 0x006D6110, CBrainOutput::SetAsLookAtThing
//          @ 0x0093E760, CAIBrain::SetBrainOutput @ 0x0088B8F0, operator delete @ 0x00BFE9BC, GetPWorld @ 0x008FCFC0,
//          CWorld::GetWorldFrame @ 0x0049D870, CCombatAction_WillHealInto @ 0x00956870, CCombatAction_WillEnflameInto
//          @ 0x00956760, CCreatureAction_ThrowShotObject @ 0x009566C0, CCombatActionBase @ 0x00857F30 (vtable 0x012632B4) /
//          ~CCombatActionBase @ 0x0062E900, AIAction_PerformActionAndWait @ 0x0097DB70, Terminate @ 0x008FCF40.
// Globals: frames per second @ 0x01375550, the throw range @ 0x012944C0, 1.0f @ 0x0122DED8.
// Layout: engine/CAIStateGroup_MazeDefensiveMeleeCombat.h (LastHealFrame +0x20); PTarget is the +0x14 intelligent pointer
//          of the landed MazeRangedCombat model.
#include "engine/CAIStateGroup_MazeDefensiveMeleeCombat.h"

struct CTCBase;
struct CThing;
extern "C" double __cdecl sqrt(double);
#pragma intrinsic(sqrt)
struct C3DVector {
    float X, Y, Z;
};
struct C2DVector {
    float X, Y;
    C2DVector(float x, float y) : X(x), Y(y) { }
    float LengthSquared() const { return X * X + Y * Y; }
    float Length() const { return (float)sqrt(LengthSquared()); }
};
struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);       // 0x0040F020
};
template <class T> struct CDefPointer;
struct CAICreatureWillPowerIndicatorDef;
struct CDefClassBase {
    bool GetSubDef(CDefPointer<CAICreatureWillPowerIndicatorDef>* out);   // 0x008C3510
};
struct CThing {
    unsigned char _pad0[0x20];
    unsigned long m_presentBits[9];             // +0x20 std::bitset of interface types
    CInterfaceMap m_interfaces;                 // +0x44
    unsigned char _pad4c[0x70 - 0x4c - 4];
    unsigned long m_flags6c;                    // +0x6c bit 3: can be fought (descriptive)
    CDefClassBase* PDef;                        // +0x70
    unsigned char _pad74[0x91 - 0x74];
    unsigned char m_flags91;                    // +0x91 bit 0: dead
    const C3DVector& GetPosition();             // 0x004C73D0
    template<class D> __forceinline bool GetSubDef(D* def) { return PDef->GetSubDef(def); }
    bool TestTC(long interfaceType) const
    {
        return ((m_presentBits[interfaceType >> 5] >> (interfaceType & 31)) & 1) != 0;
    }
    template<class T> __forceinline bool GetInterfaceT(T** ppOut, long interfaceType)
    {
        bool has = TestTC(interfaceType);
        if (has) {
            long type = interfaceType;
            CInterfaceMap* map = &m_interfaces;
            CInterfaceMap::SEntry* it = map->LowerBound(&type);
            if (it == map->last || interfaceType < it->type)
                it = map->last;
            *ppOut = (T*)it->component;
        }
        return has;
    }
    // the fully inlined lower_bound form (no LowerBound call)
    template<class T> __forceinline bool GetInterfaceInline(T** ppOut, long interfaceType)
    {
        bool has = TestTC(interfaceType);
        if (has) {
            long count = m_interfaces.last - m_interfaces.first;
            CInterfaceMap::SEntry* first = m_interfaces.first;
            while (count > 0) {
                long half = count / 2;
                if (first[half].type < interfaceType) {
                    first += half + 1;
                    count -= half + 1;
                } else
                    count = half;
            }
            *ppOut = (T*)((first == m_interfaces.last || interfaceType < first->type) ? m_interfaces.last : first)->component;
        }
        return has;
    }
};
struct CCombatTypeDef {
    unsigned char _pad[0x70];
    float MeleeRange;                           // +0x70 (descriptive)
};
struct CThingCreatureBase : CThing {
    CCombatTypeDef* GetCombatTypeDef();         // 0x00666D10
    bool IsRegisteredCombatant();               // 0x00662410
    bool IsUsingShield();                       // 0x006621A0 (descriptive)
    float GetHealthFraction();                  // 0x006A4E40 (descriptive)
};
struct CTCWillPower {                           // interface 0xce
    unsigned char _pad[0x0c];
    bool Active;                                // +0x0c (descriptive)
};
struct CTCMazeMarker {                          // interface 0xad (the retreat marker position is unaligned at +0x22)
    unsigned char _pad[0x22];
    unsigned char RetreatPos[12];               // +0x22 C3DVector
};
struct CTCCreatureModeManager {                 // interface 0x31
    bool HasMode(long mode);                    // 0x0073A8A0
    void AddMode(unsigned long mode);           // 0x0073AE10
};
struct CAICreatureWillPowerIndicatorDef {       // the maze def (descriptive member names)
    virtual void v00();
    virtual void Destroy();                     // slot 1
    long m_refCount;                            // +0x04
    unsigned char _pad08[0x3c - 0x08];
    long EnflameParam;                          // +0x3c
    unsigned char _pad40[0x44 - 0x40];
    float RetreatRange;                         // +0x44
    unsigned char _pad48[0x54 - 0x48];
    float HealDelay;                            // +0x54 seconds
    long HealParam;                             // +0x58
};
template <class T> struct CDefPointer {
    T* Object;
    CDefPointer() : Object(0) { }
    ~CDefPointer() { if (Object) { if (--Object->m_refCount == 0) Object->Destroy(); } }
    T* operator->() const { __assume(Object != 0); return Object; }
};
void* FindSubDef_CAICreatureWillPowerIndicatorDef(const CDefClassBase* def);
// Same-TU stand-in for 0x008C3510 (out of line in retail; see the Shape line).
#pragma auto_inline(off)
__declspec(noinline) bool CDefClassBase::GetSubDef(CDefPointer<CAICreatureWillPowerIndicatorDef>* out)
{
    void* found = FindSubDef_CAICreatureWillPowerIndicatorDef(this);
    if (!found)
        return false;
    out->Object = (CAICreatureWillPowerIndicatorDef*)found;
    return true;
}
#pragma auto_inline(on)
struct CBaseIntelligentPointer {
    virtual void v00();
    CThing* m_pItem;                            // +0x04
    CBaseIntelligentPointer();                  // 0x00A01B10
    ~CBaseIntelligentPointer();                 // 0x00A01C10
    CThing* GetPItem() const;                   // 0x00A01B50
    void SetPItem(const CThing* item);          // 0x00A01B90
};
struct CIntelligentPointerConstThing : CBaseIntelligentPointer {   // CIntelligentPointer<const CThing>, vtable 0x01237F1C
    virtual void v00();
    CIntelligentPointerConstThing(const CThing* item) { SetPItem(item); }
};
inline bool IS_THING_ALIVE(const CBaseIntelligentPointer& p)
{
    if (p.GetPItem() != 0) {
        if (!(p.GetPItem()->m_flags91 & 1))
            return true;
    }
    return false;
}
struct CCreatureActionBase;
struct SCountBlock {
    long m_references;
    void (__fastcall* m_pfnDestroy)(CCreatureActionBase*);
    CCreatureActionBase* m_pObject;
};
struct CCountedPointer_CCreatureActionBase {
    CCreatureActionBase* m_pObject;
    SCountBlock* m_pCount;
    __forceinline ~CCountedPointer_CCreatureActionBase()
    {
        if (m_pCount) {
            --m_pCount->m_references;
            if (m_pCount->m_references == 0) {
                m_pCount->m_pfnDestroy(m_pCount->m_pObject);
                delete m_pCount;
            }
        }
        m_pObject = 0;
        m_pCount = 0;
    }
};
struct CIntelligentPointerThing : CBaseIntelligentPointer {
    virtual void v00();
};
struct CBrainOutput {                           // 0x48 bytes
    unsigned long Types;                        // +0x00
    long LookFlags;                             // +0x04
    unsigned char NavigationInput[0x18];        // +0x08
    CCountedPointer_CCreatureActionBase PAction;   // +0x20
    C3DVector MoveToPos;                        // +0x28
    C3DVector LookAtPos;                        // +0x34
    CIntelligentPointerThing LookAtThing;       // +0x40
    CBrainOutput(const CBrainOutput& other);    // 0x006D6110
    void SetAsLookAtThing(const CThing* thing, long lookFlags);   // 0x0093E760
};
struct CSoundPair {
    long m_first;
    long m_second;
    CSoundPair() : m_first(0), m_second(0) { }
};
struct CCreatureActionBase {
    virtual void v00();
};
struct CCombatActionBase : CCreatureActionBase {   // retail sizeof 0x114
    unsigned char _body[0x114 - 4];
    CCombatActionBase(CThingCreatureBase* owner, CThing* target, long actionCode, const CSoundPair& sound);   // 0x00857F30
    ~CCombatActionBase();                       // 0x0062E900
};
struct CCombatAction_MazeBlast : CCombatActionBase {   // vtable 0x012632B4
    virtual void v00();
    CCombatAction_MazeBlast(CThingCreatureBase* owner, CThing* target) : CCombatActionBase(owner, target, 0x32, CSoundPair()) { }
};
struct CCombatAction_WillHealInto : CCombatActionBase {
    unsigned char _heal[0x0c];
    CCombatAction_WillHealInto(CThingCreatureBase* owner, long param);   // 0x00956870
};
struct CCombatAction_WillEnflameInto : CCombatActionBase {
    unsigned char _enflame[0x0c];
    CCombatAction_WillEnflameInto(CThingCreatureBase* owner, long param, float time);   // 0x00956760
};
struct CCreatureAction_ThrowShotObject : CCombatActionBase {
    CCreatureAction_ThrowShotObject(CThingCreatureBase* owner, CThing* target);   // 0x009566C0
};
struct CCombatManager {
    long GetDefenderZone(CThing* target, CThingCreatureBase* defender, long flags);   // 0x006E99B0
};
struct CWorld {
    unsigned char _pad0[0x4c];
    CCombatManager* CombatManager;              // +0x4c
    static long GetWorldFrame();                // 0x0049D870
};
extern long g_GameFramesPerSecond;              // 0x01375550
extern float g_MazeThrowRange;                  // 0x012944C0
struct CAIBrain {
    unsigned char _pad[0x18];
    CThingCreatureBase* PCreature;              // +0x18
    unsigned char _pad1c[0x44 - 0x1c];
    CBrainOutput Output;                        // +0x44
    void SetBrainOutput(const CBrainOutput& output);  // 0x0088B8F0
};

struct CAIStateGroupMazeDefensiveMeleeCombatMethods : CAIStateGroup_MazeDefensiveMeleeCombat {
    CAIBrain* Brain() const { return *(CAIBrain* const*)_pad_0x04; }
    CThingCreatureBase* GetCreature() const { return Brain()->PCreature; }
    CBaseIntelligentPointer& Target() { return *(CBaseIntelligentPointer*)_pad_0x14; }
    CWorld* GetPWorld();                        // 0x008FCFC0
    bool IsStopping();                          // 0x008FD010
    void Terminate();                           // 0x008FCF40
    long AIAction_PerformActionAndWait(CCreatureActionBase* action, bool wait);   // 0x0097DB70
    long AIAction_LookAtThing(long repeat, CIntelligentPointerConstThing thing, long flags, float turnSpeed);   // 0x0097D6F0
    long AIAction_PumpCombatActions();          // 0x0097D980
    long AIAction_MoveToPosProximity(long repeat, C3DVector pos, float radius, bool* reached, long movementType,
                                     bool a, bool b, bool c, CThing* leader, bool d, float e);   // 0x0097E890
    void Update();
};

void CAIStateGroupMazeDefensiveMeleeCombatMethods::Update()
{
    CDefPointer<CAICreatureWillPowerIndicatorDef> def;
    GetCreature()->GetSubDef(&def);
    for (;;) {
        if (!IS_THING_ALIVE(Target()) || IsStopping()) {
            Terminate();
            return;
        }
        CThingCreatureBase* creature = GetCreature();
        CThing* target = Target().GetPItem();
        const C3DVector& myPos = creature->GetPosition();
        const C3DVector& targetPos = target->GetPosition();
        float distance = C2DVector(targetPos.X - myPos.X, targetPos.Y - myPos.Y).Length();
        CTCWillPower* willPower;
        if (GetCreature()->GetInterfaceInline(&willPower, 0xce) && willPower->Active) {
            AIAction_PerformActionAndWait(&CCombatAction_WillEnflameInto(GetCreature(), def->EnflameParam, 5.0f), false);
            Terminate();
            return;
        }
        if (distance < GetCreature()->GetCombatTypeDef()->MeleeRange) {
            if (!GetCreature()->IsRegisteredCombatant())
                continue;
            if (!(Target().GetPItem()->m_flags6c & 8))
                continue;
            CThing* enemy = Target().GetPItem();
            CThingCreatureBase* me = GetCreature();
            long zone = GetPWorld()->CombatManager->GetDefenderZone(enemy, me, 0);
            if (zone != 1 && zone != 2 && zone != 4) {
                AIAction_LookAtThing(1, CIntelligentPointerConstThing(Target().GetPItem()), 0x21, 1.0f / 12.0f);
                if (!IS_THING_ALIVE(Target()) || IsStopping()) {
                    Terminate();
                    return;
                }
            }
            AIAction_PumpCombatActions();
        } else if (distance < def->RetreatRange) {
            CThingCreatureBase* me = GetCreature();
            CTCMazeMarker* marker;
            me->GetInterfaceT(&marker, 0xad);
            CTCCreatureModeManager* modes;
            me->GetInterfaceT(&modes, 0x31);
            if (me->IsUsingShield() && !modes->HasMode(0x19))
                modes->AddMode(0x19);
            bool reached;
            AIAction_MoveToPosProximity(2, *(C3DVector*)marker->RetreatPos, 1.0f, &reached, 5, false, false, true, 0, true, 0.0f);
            if (!GetCreature()->IsUsingShield())
                continue;
            if (!IS_THING_ALIVE(Target()) || IsStopping())
                continue;
            CBrainOutput output(Brain()->Output);
            output.SetAsLookAtThing(Target().GetPItem(), 0xa7);
            Brain()->SetBrainOutput(output);
        } else if (distance < g_MazeThrowRange) {
            long fps = g_GameFramesPerSecond;
            float sinceHeal = (float)(GetPWorld()->GetWorldFrame() - LastHealFrame) / fps;
            if (GetCreature()->GetHealthFraction() < 1.0f && sinceHeal > def->HealDelay) {
                AIAction_PerformActionAndWait(&CCombatAction_WillHealInto(GetCreature(), def->HealParam), false);
                LastHealFrame = GetPWorld()->GetWorldFrame();
                if (!IS_THING_ALIVE(Target()) || IsStopping()) {
                    Terminate();
                    return;
                }
            } else {
                AIAction_PerformActionAndWait(&CCreatureAction_ThrowShotObject(GetCreature(), Target().GetPItem()), false);
                if (!IS_THING_ALIVE(Target()) || IsStopping()) {
                    Terminate();
                    return;
                }
                if (!(Target().GetPItem()->m_flags6c & 8))
                    continue;
                AIAction_PerformActionAndWait(&CCombatAction_MazeBlast(GetCreature(), Target().GetPItem()), false);
            }
        } else {
            if (!(Target().GetPItem()->m_flags6c & 8))
                continue;
            AIAction_PerformActionAndWait(&CCombatAction_MazeBlast(GetCreature(), Target().GetPItem()), false);
        }
    }
}
