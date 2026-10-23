// CAIStateGroup_VillageTaskSecuritySweep::Update() -- accept the sweep task, draw a weapon, react to a perceived thing, walk to the sweep point, then repeatedly look around it at random angles and wander near it until a move fails
// Grade: DIFFER(1052v1053) -- first pass: esi/edi role swap for the task pointer vs village/perception, frame 0x178 v 0x180, the task position copies go through 'add eax,0x10', the looks loop (entered only when Random(5)+2 <= 0, as retail) is rotated with an extra jmp, and the end-frame fps loads are scheduled differently
struct CTCBase;
struct C3DVector {
    float X, Y, Z;
    C3DVector() { }
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) { }
    __forceinline C3DVector operator+(const C3DVector& o) const { return C3DVector(X + o.X, Y + o.Y, Z + o.Z); }
    __forceinline C3DVector operator-(const C3DVector& o) const { return C3DVector(X - o.X, Y - o.Y, Z - o.Z); }
    void RotateAboutZ(float angle);             // 0x00A14040
};
struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);       // 0x0040F020
    const SEntry* LowerBound(const long* type) const;   // 0x004365B0
};
struct CThing {
    unsigned char _pad0[0x10];
    C3DVector Position;                         // +0x10
    unsigned char _pad1c[4];
    unsigned long m_presentBits[9];             // +0x20
    CInterfaceMap m_interfaces;                 // +0x44
    unsigned char _pad4c[0x91 - 0x4c];
    unsigned char m_flags91;                    // +0x91
    template<class T> __forceinline bool GetInterface(T** ppOut, long interfaceType)
    {
        bool found = (m_presentBits[interfaceType >> 5] & (1u << (interfaceType & 31))) != 0;
        if (found) {
            long type = interfaceType;
            CInterfaceMap* map = &m_interfaces;
            CInterfaceMap::SEntry* it = map->LowerBound(&type);
            *ppOut = (T*)((it == map->last || interfaceType < it->type) ? map->last : it)->component;
        }
        return found;
    }
    template<class T> __forceinline bool GetInterfaceC(T** ppOut, long interfaceType) const
    {
        bool found = (m_presentBits[interfaceType >> 5] & (1u << (interfaceType & 31))) != 0;
        if (found) {
            long type = interfaceType;
            const CInterfaceMap* map = &m_interfaces;
            const CInterfaceMap::SEntry* it = map->LowerBound(&type);
            *ppOut = (T*)((it == map->last || interfaceType < it->type) ? map->last : it)->component;
        }
        return found;
    }
};
struct CSweepPoint : CThing { };
// the task thing: look target +0x20, look-angle range +0x2c, wander radius +0x1c (fields of the marker thing)
struct CTaskMarker {
    unsigned char _pad0[0x10];
    C3DVector Position;                         // +0x10
    float WanderRadius;                         // +0x1c
    C3DVector LookTarget;                       // +0x20
    float LookAngleRange;                       // +0x2c
};
struct CBaseIntelligentPointer {
    virtual void v00();
    CThing* m_pItem;                            // +0x04
    CBaseIntelligentPointer();                  // 0x00A01B10
    ~CBaseIntelligentPointer();                 // 0x00A01C10
    CThing* GetPItem() const;                   // 0x00A01B50
    void SetPItem(CThing* item);                // 0x00A01B90
};
struct CIntelligentPointerConstThing : CBaseIntelligentPointer {   // vtable 0x01237F1C
    virtual void v00();
    CIntelligentPointerConstThing(CThing* item) { SetPItem(item); }
};
struct CTCPerception {                          // interface 0x34
    unsigned char _pad[0x2d];
    bool Alerted;                               // +0x2d
    unsigned char _pad2e[3];
    bool Investigating;                         // +0x31
    bool Interested;                            // +0x32
    CThing* GetPerceivedThing();                // 0x007446A0
};
struct CTCCreatureNavigation {                  // interface 7
    bool GetNearbyWanderPosition(const C3DVector& centre, float radius, C3DVector* result, bool onMap);   // 0x006CB6E0
};
struct CTCVillage {
    bool SetCrateTaskAsAcceptedByThing(CThing* task, CThing* creature);   // 0x00658586
};
struct CTCVillager;
CTCVillage* __fastcall GFGetPVillageInterface(CTCVillager* villager);    // 0x007EE540
bool __fastcall IsThingCarryingWeapon(const CThing* thing);   // 0x006A1540
float __fastcall GFFloatRandom(float range, unsigned long* seed);   // 0x004978A0
unsigned long __fastcall GFROR13(unsigned long value);   // 0x00497890
struct CWorld {
    static unsigned long GetWorldFrame();       // 0x0049D870
};
extern long g_framesPerSecond;                  // 0x01375550
struct CCombatActionBase {                      // retail sizeof 0x114
    virtual void v00();
    unsigned char _pad04[0x110];
    ~CCombatActionBase();                       // 0x0062E900
};
struct CCreatureAction_UnsheatheWeapons : CCombatActionBase {
    unsigned char _pad114[4];
    CCreatureAction_UnsheatheWeapons(CThing* creature);   // 0x008B8590
};
struct CAIBrain {
    unsigned char _pad[0x18];
    CThing* PCreature;                          // +0x18
};
struct CAIStateGroupBase {
    virtual ~CAIStateGroupBase();
    CAIBrain* PBrain;                           // +0x04
    unsigned char _pad08[0x0c];
    CThing* GetCreature() const { return PBrain->PCreature; }
    unsigned long* GetPWorldSeed();             // 0x008FCFD0
    void Terminate();                           // 0x008FCF40
    long AIAction_PerformActionAndWait(CCombatActionBase* action, bool wait);   // 0x0097DB70
    long AIAction_LookAtThing(long repeat, CIntelligentPointerConstThing thing, long flags, float speed);   // 0x0097D6F0
    long AIAction_LookAtPos(long repeat, C3DVector pos, long flags, float speed);   // 0x0097D580
    long AIAction_MoveToPos(long repeat, C3DVector pos, bool* reached, long movement, bool a, bool b,
        CThing* avoid, bool c, float speed);    // 0x0097EC00
};
struct CAIStateGroup_VillageTaskSecuritySweep : CAIStateGroupBase {
    CTCVillager* PVillager;                     // +0x14
    CBaseIntelligentPointer PTaskThing;         // +0x18
    CTaskMarker* Task() const { return (CTaskMarker*)PTaskThing.GetPItem(); }
    virtual void Update();
};

void CAIStateGroup_VillageTaskSecuritySweep::Update()
{
    if (!PTaskThing.GetPItem()) {
        Terminate();
        return;
    }
    CTCVillage* village = GFGetPVillageInterface(PVillager);
    if (!village->SetCrateTaskAsAcceptedByThing(PTaskThing.GetPItem(), GetCreature())) {
        Terminate();
        return;
    }
    if (!IsThingCarryingWeapon(GetCreature()))
        AIAction_PerformActionAndWait(&CCreatureAction_UnsheatheWeapons(GetCreature()), false);
    CTCPerception* perception = 0;
    GetCreature()->GetInterfaceC(&perception, 0x34);
    long result;
    CThing* seen = perception->GetPerceivedThing();
    if (((seen && !(seen->m_flags91 & 1) && perception->Interested) || perception->Alerted || perception->Investigating)
        && (result = AIAction_LookAtThing(1, CIntelligentPointerConstThing(perception->GetPerceivedThing()), 0x23, 0.0027777f)) == 1)
        ;
    else
        result = 4;
    bool reached = false;
    while (result == 4) {
        if (!PTaskThing.GetPItem()) {
            Terminate();
            return;
        }
        result = AIAction_MoveToPos(2, Task()->Position, &reached, 5, true, true, 0, true, 0.0f);
    }
    if (result != 2) {
        Terminate();
        return;
    }
    unsigned long* seed = GetPWorldSeed();
    do {
        *seed = *seed * 0x24A1U + 0x24DFU;
        *seed = GFROR13(*seed);
        long looks = *seed % 5 + 2;
        while (looks <= 0) {
            --looks;
            if (result == 1)
                break;
            if (!PTaskThing.GetPItem()) {
                Terminate();
                return;
            }
            float angle = GFFloatRandom(Task()->LookAngleRange, seed);
            angle = angle - Task()->LookAngleRange * 0.5f;
            C3DVector centre = Task()->Position;
            C3DVector look = Task()->LookTarget;
            C3DVector dir = look - centre;
            dir.RotateAboutZ(angle);
            C3DVector target = Task()->Position + dir;
            long fps = g_framesPerSecond;
            *seed = *seed * 0x24A1U + 0x24DFU;
            *seed = GFROR13(*seed);
            unsigned long r = *seed % 3;
            unsigned long end = CWorld::GetWorldFrame() + g_framesPerSecond * r + fps * 2;
            do
                result = AIAction_LookAtPos(2, target, 0xb, 0.0027777f);
            while (CWorld::GetWorldFrame() < end && result != 1);
        }
        if (!PTaskThing.GetPItem()) {
            Terminate();
            return;
        }
        CTCCreatureNavigation* navigation;
        GetCreature()->GetInterface(&navigation, 7);
        CTaskMarker* task = Task();
        C3DVector centre = Task()->Position;
        C3DVector wander;
        if (navigation->GetNearbyWanderPosition(centre, task->WanderRadius, &wander, false))
            result = AIAction_MoveToPos(1, wander, &reached, 5, true, true, 0, true, 0.0f);
    } while (result != 1);
    Terminate();
}
