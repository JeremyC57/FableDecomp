// CAIStateGroup_MeleeCombatNearby::MoveToEnemyCreatureCombatPosition() -- while the creature stays a registered combatant and not yet in its combat position: walk to the combat manager's preferred position when it is in navigation line of sight, else (unless on a rotating combat ring) close in on the enemy, else idle; false when an action fails, true once in position or no longer runnable
// Grade: DIFFER(408v407) -- inlined LowerBound loads first into eax then copies to edi (ours loads edi directly); the diameter fmul/fadd are scheduled before the pushes (retail interleaves them); eax vs edx for the slot-14 vtable load
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0096EC10, length 407 bytes (+ int3 padding).
// Callees: CThingCreatureBase::IsRegisteredCombatant @ 0x00662410, IsEnemyCreatureInCombatPosition @ 0x0096E700, the brain
// world accessor @ 0x008FCFC0 (combat manager +0x4c, twice), CCombatManager::GetPreferredPos @ 0x006EA770 (C3DVector by
// hidden pointer), CTCCreatureNavigation::IsPosNavLineOfSightTo @ 0x006CBA20 (interface 7, its LowerBound inlined),
// AIAction_MoveToPos @ 0x0097EC00, CCombatManager::IsCreatureOnRotatingCombatRing @ 0x006E9700 (autoname label only),
// CBaseIntelligentPointer ctor @ 0x00A01B10 / GetPItem @ 0x00A01B50 / SetPItem @ 0x00A01B90 (the by-value
// CIntelligentPointer<const CThing> argument, vtable 0x01237F1C), AIAction_MoveToThingProximity @ 0x0097E380,
// AIAction_DoNothing @ 0x0097E200, own virtual slot 14 IsStillRunnable(bool). Floats 0.8333333f @ 0x012925CC, 1.0f @ 0x0122DED8.
// Shape: the navigation component lookup inlines the CVectorMap::LowerBound binary search (this TU's copy is inline);
// its out variable is function-scope, so the never-written miss value is read into ebx once at entry; the HasTC bit is
// bound to a const reference (frame byte); the `arrived` flag is zeroed once before the loop.
// Layout: engine/CAIStateGroup_MeleeCombatNearby.h flattens the base; local model with its PEnemy (+0x14).
struct CTCBase;

struct C3DVector {
    float X, Y, Z;
    C3DVector() { }
    C3DVector(const C3DVector& o) : X(o.X), Y(o.Y), Z(o.Z) { }
};

struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type)
    {
        SEntry* it = first;
        long count = last - first;
        while (count > 0) {
            long half = count / 2;
            SEntry* middle = it + half;
            if (middle->type < *type) {
                it = middle + 1;
                count = count - half - 1;
            } else
                count = half;
        }
        return it;
    }
};

struct CPhysicsBody {
    unsigned char _pad[0x34];
    float m_radius;                             // +0x34
    float GetDiameter() const { return m_radius * 2.0f; }
};

struct CThing {
    unsigned char _pad0[0x20];
    unsigned long m_presentBits[9];             // +0x20 std::bitset of interface types
    CInterfaceMap m_interfaces;                 // +0x44
    unsigned char _pad4c[0x14];
    CPhysicsBody* m_pBody;                      // +0x60
    unsigned char _pad64[0x2d];
    unsigned char m_flags91;                    // +0x91 bit 0: dead
    unsigned char HasTC(long interfaceType) const { return (unsigned char)((m_presentBits[interfaceType >> 5] >> (interfaceType & 31)) & 1); }
    template<class T> __forceinline bool GetInterface(T** ppOut, long interfaceType)
    {
        const unsigned char& present = HasTC(interfaceType);
        if (present) {
            long type = interfaceType;
            CInterfaceMap* map = &m_interfaces;
            CInterfaceMap::SEntry* it = map->LowerBound(&type);
            *ppOut = (T*)((it == map->last || interfaceType < it->type) ? map->last : it)->component;
        }
        return present != 0;
    }
};

struct CThingCreatureBase : CThing {
    bool IsRegisteredCombatant();               // 0x00662410
};

struct CBaseIntelligentPointer {
    virtual void v00();
    CThing* m_pItem;                            // +0x04
    CBaseIntelligentPointer();                  // 0x00A01B10
    CThing* GetPItem() const;                   // 0x00A01B50
    void SetPItem(CThing* item);                // 0x00A01B90
};
struct CIntelligentPointerCreature : CBaseIntelligentPointer {
    virtual void v00();
};
struct CIntelligentPointerConstThing : CBaseIntelligentPointer {     // CIntelligentPointer<const CThing>, vtable 0x01237F1C
    virtual void v00();
    CIntelligentPointerConstThing(const CIntelligentPointerCreature& other) : CBaseIntelligentPointer() { SetPItem(other.GetPItem()); }
};

struct CTCCreatureNavigation {                  // interface 7
    bool IsPosNavLineOfSightTo(const C3DVector& pos);      // 0x006CBA20
};

struct CCombatManager {
    C3DVector GetPreferredPos(CThingCreatureBase* creature);        // 0x006EA770
    bool IsCreatureOnRotatingCombatRing(CThingCreatureBase* creature);  // 0x006E9700
};
struct CWorld {
    unsigned char _pad0[0x4c];
    CCombatManager* CombatManager;              // +0x4c
    CCombatManager* GetPCombatManager() const { return CombatManager; }
};

extern const float g_combatProximityScale;     // 0x012925CC (0.8333333f)

struct CAIBrain {
    unsigned char _pad[0x18];
    CThingCreatureBase* PCreature;              // +0x18
};

struct CAIStateGroupBase {
    virtual ~CAIStateGroupBase();               // slot 0
    virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04(); virtual void v05();
    virtual void v06(); virtual void v07(); virtual void v08(); virtual void v09(); virtual void v10();
    virtual void v11(); virtual void v12(); virtual void v13();
    virtual bool IsStillRunnable(bool checkEnemy);  // slot 14 (+0x38)
    CAIBrain* PBrain;                           // +0x04
    long ScriptAIPriority;                      // +0x08
    void* Name_PStringData;                     // +0x0c
    unsigned char Terminated;                   // +0x10
    unsigned char Priority;                     // +0x11
    unsigned char Enabled;                      // +0x12
    unsigned char _pad_0x13;
    CWorld* GetPWorld();                        // 0x008FCFC0
    long AIAction_MoveToPos(long repeat, C3DVector pos, bool* arrived, long movement, bool a, bool b,
                            CThing* avoid, bool c, float speed);        // 0x0097EC00
    long AIAction_MoveToThingProximity(long repeat, CIntelligentPointerConstThing target, float proximity,
                                       bool* arrived, long movement, bool a, bool b, bool c, bool d, float e);   // 0x0097E380
    long AIAction_DoNothing();                  // 0x0097E200
};

struct CAIStateGroup_MeleeCombatNearby : CAIStateGroupBase {
    CIntelligentPointerCreature PEnemy;         // +0x14
    bool IsEnemyCreatureInCombatPosition();     // 0x0096E700
    bool MoveToEnemyCreatureCombatPosition();
};

bool CAIStateGroup_MeleeCombatNearby::MoveToEnemyCreatureCombatPosition()
{
    bool arrived = false;
    CTCCreatureNavigation* navigation;
    do {
        if (!PBrain->PCreature->IsRegisteredCombatant())
            return false;
        if (IsEnemyCreatureInCombatPosition())
            return true;
        CThingCreatureBase* creature = PBrain->PCreature;
        CWorld* world = GetPWorld();
        C3DVector pos = world->GetPCombatManager()->GetPreferredPos(creature);
        PBrain->PCreature->GetInterface(&navigation, 7);
        long result;
        if (navigation->IsPosNavLineOfSightTo(pos))
            result = AIAction_MoveToPos(2, pos, &arrived, 5, true, true, 0, true, 0.0f);
        else {
            CThingCreatureBase* self = PBrain->PCreature;
            CWorld* world = GetPWorld();
            if (!world->GetPCombatManager()->IsCreatureOnRotatingCombatRing(self)) {
                float diameter = PBrain->PCreature->m_pBody->GetDiameter();
                result = AIAction_MoveToThingProximity(2, PEnemy, diameter * 0.8333333f + 1.0f,
                                                       &arrived, 5, true, true, true, true, 0.0f);
            }
            else
                result = AIAction_DoNothing();
        }
        if (result != 4 && result != 2)
            return false;
    } while (IsStillRunnable(true));
    return true;
}
