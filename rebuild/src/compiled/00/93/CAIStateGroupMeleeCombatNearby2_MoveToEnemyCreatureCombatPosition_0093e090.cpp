// CAIStateGroup_MeleeCombatNearby2::MoveToEnemyCreatureCombatPosition() -- keep moving (to within 0.5) towards the combat manager's preferred position for the creature, refreshing it every fifth frame (UID-staggered) or after 6 frames; tells the manager when the position proves unreachable; false when the creature stops being a combatant or a move fails, true once in position or no longer runnable
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0093E090, length 428 bytes (+ int3 padding).
// Callees: the brain world accessor @ 0x008FCFC0 (combat manager +0x4c, three times), CCombatManager::GetPreferredPos @ 0x006EA770
// (C3DVector by hidden pointer), CWorld::GetWorldFrame (static) @ 0x0049D870, own virtual slot 14 IsStillRunnable(bool),
// IsEnemyCreatureInCombatPosition @ 0x0093DCD0, CThingCreatureBase::IsRegisteredCombatant @ 0x00662410 (twice), __aullrem
// @ 0x00BFEBB0, CBaseIntelligentPointer::GetPItem @ 0x00A01B50 (PEnemy +0x14), AIAction_MoveToPosProximity @ 0x0097E890,
// CCombatManager @ 0x006E9D10 (autoname label only), CVectorMap::LowerBound inlined (navigation interface 7, result unused).
// Shape: the UID is read through a named creature local before the static frame call (edi:ebx), the last frame lives in
// ebp; the inline navigation lookup has no consumer, so only its binary-search loop survives.
// Layout: engine/CAIStateGroup_MeleeCombatNearby2.h flattens the base; local model with its PEnemy (+0x14); CThing UID +0x18.
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
    unsigned char _pad0[0x18];
    unsigned __int64 UID;                       // +0x18
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
    void OnCombatPositionUnreachable(CThingCreatureBase* creature);     // 0x006E9D10 (autoname label only; called when the move returns 3)
};
struct CWorld {
    unsigned char _pad0[0x4c];
    CCombatManager* CombatManager;              // +0x4c
    CCombatManager* GetPCombatManager() const { return CombatManager; }
    static long __fastcall GetWorldFrame();     // 0x0049D870
};

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
    long AIAction_MoveToPosProximity(long repeat, C3DVector pos, float radius, bool* reached, long movementType,
                                     bool a, bool b, bool c, CThing* leader, bool d, float e);   // 0x0097E890
};

struct CAIStateGroup_MeleeCombatNearby2 : CAIStateGroupBase {
    CIntelligentPointerCreature PEnemy;         // +0x14
    bool IsEnemyCreatureInCombatPosition();     // 0x0093DCD0
    bool MoveToEnemyCreatureCombatPosition();
};

bool CAIStateGroup_MeleeCombatNearby2::MoveToEnemyCreatureCombatPosition()
{
    bool arrived = false;
    CThingCreatureBase* creature = PBrain->PCreature;
    CWorld* world = GetPWorld();
    C3DVector pos = world->GetPCombatManager()->GetPreferredPos(creature);
    long lastFrame = CWorld::GetWorldFrame();
    if (IsStillRunnable(true)) {
        CTCCreatureNavigation* navigation;
        do {
            if (IsEnemyCreatureInCombatPosition())
                return true;
            if (!PBrain->PCreature->IsRegisteredCombatant())
                return false;
            CThingCreatureBase* me = PBrain->PCreature;
            unsigned __int64 uid = me->UID;
            if ((uid + CWorld::GetWorldFrame()) % 5 == 0 || CWorld::GetWorldFrame() - lastFrame > 6) {
                CThingCreatureBase* self = PBrain->PCreature;
                CWorld* w = GetPWorld();
                pos = w->GetPCombatManager()->GetPreferredPos(self);
                lastFrame = CWorld::GetWorldFrame();
            }
            PBrain->PCreature->GetInterface(&navigation, 7);
            long result = AIAction_MoveToPosProximity(2, pos, 0.5f, &arrived, 5, true, false, true, PEnemy.GetPItem(), false, 0.0f);
            if (result == 3) {
                if (PBrain->PCreature->IsRegisteredCombatant()) {
                    CThingCreatureBase* self = PBrain->PCreature;
                    CWorld* w = GetPWorld();
                    w->CombatManager->OnCombatPositionUnreachable(self);
                }
            } else if (result != 4 && result != 2)
                return false;
        } while (IsStillRunnable(true));
    }
    return true;
}