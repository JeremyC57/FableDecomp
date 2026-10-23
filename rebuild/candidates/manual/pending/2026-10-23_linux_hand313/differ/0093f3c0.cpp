// CAIStateGroup_VillageTaskPatrol::Update() -- take the village patrol task, draw a weapon, then walk the patrol track node by node, glancing at each node ahead and idling a random 0.5-1.5 seconds there, while the task stays valid
// Grade: DIFFER(1182v1185) -- first pass; register roles (ebx/ebp swap), frame 0x160 v 0x150, call scheduling
extern unsigned long g_InvSqrtMantissaTable[128];   // 0x013CE558
__forceinline float FastInvSqrt(float lengthSquared)
{
    unsigned long bits = *(unsigned long*)&lengthSquared;
    unsigned long estimate = (((0x17c - ((bits >> 23) & 0xff)) >> 1) << 23) |
                             (g_InvSqrtMantissaTable[(bits >> 17) & 0x7f] << 15);
    float r = *(float*)&estimate;
    return (3.0f - r * r * lengthSquared) * r * 0.5f;
}
struct C3DVector {
    float X, Y, Z;
    C3DVector() { }
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) { }
};
__forceinline C3DVector operator*(const C3DVector& v, const float& s) { return C3DVector(v.X * s, v.Y * s, v.Z * s); }
__forceinline C3DVector Normalise(const C3DVector& v) { return v * FastInvSqrt(v.X * v.X + v.Y * v.Y + v.Z * v.Z); }
struct CRightHandedSet {
    C3DVector Forward;
    C3DVector Up;
};
struct CTCPhysicsBase {
    virtual void v000(); virtual void v001(); virtual void v002(); virtual void v003(); virtual void v004(); virtual void v005(); virtual void v006(); virtual void v007();
    virtual void v008(); virtual void v009(); virtual void v010(); virtual void v011(); virtual void v012(); virtual void v013(); virtual void v014(); virtual void v015();
    virtual void v016(); virtual void v017(); virtual void v018(); virtual void v019(); virtual void v020(); virtual void v021(); virtual void v022(); virtual void v023();
    virtual void v024(); virtual void v025(); virtual void v026(); virtual void v027(); virtual void v028(); virtual void v029(); virtual void v030(); virtual void v031();
    virtual void v032(); virtual void v033(); virtual void v034(); virtual void v035(); virtual void v036(); virtual void v037(); virtual void v038(); virtual void v039();
    virtual void v040(); virtual void v041(); virtual void v042(); virtual void v043(); virtual void v044(); virtual void v045(); virtual void v046(); virtual void v047();
    virtual void v048(); virtual void v049(); virtual void v050(); virtual void v051(); virtual void v052(); virtual void v053(); virtual void v054(); virtual void v055();
    virtual void v056(); virtual void v057(); virtual void v058(); virtual void v059(); virtual void v060(); virtual void v061(); virtual void v062(); virtual void v063();
    virtual void v064(); virtual void v065(); virtual void v066(); virtual void v067(); virtual void v068(); virtual void v069(); virtual void v070(); virtual void v071();
    virtual const CRightHandedSet* GetMatrix(); // slot 72 (+0x120)
};
struct CThing {
    unsigned char _pad00[0x60];
    CTCPhysicsBase* PPhysics;                   // +0x60
    unsigned char _pad64[0x91 - 0x64];
    unsigned char m_flags91;                    // +0x91
    const C3DVector& GetPosition() const;       // 0x004C73D0
};
struct CThingTrackNode : CThing {
    unsigned char _pad92[0xd0 - 0x92];
    CThingTrackNode* PNextNode;                 // +0xd0
    unsigned char _pad_d4[0xed - 0xd4];
    bool IsEndNode;                             // +0xed
    CThingTrackNode* PeekTrackStartNode();      // 0x00836DA0
};
struct CVillageTaskPatrol {
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04(); virtual void v05(); virtual void v06();
    virtual bool IsValidFor(CThing* creature);  // slot 7 (+0x1c)
    unsigned char _pad04[0x14];
    bool Started;                               // +0x18
    CThingTrackNode* GetNearestNode(const C3DVector& pos);   // 0x00860A50
};
struct CBaseIntelligentPointer {
    virtual void v00();
    CThing* m_pItem;                            // +0x04
    CBaseIntelligentPointer();                  // 0x00A01B10
    ~CBaseIntelligentPointer();                 // 0x00A01C10
    CThing* GetPItem() const;                   // 0x00A01B50
    void SetPItem(CThing* item);                // 0x00A01B90
};
struct CIntelligentPointerThing : CBaseIntelligentPointer {   // CIntelligentPointer<CThing>, vtable 0x0128B090
    virtual void v00();
    CIntelligentPointerThing(CThing* item) { SetPItem(item); }
    CIntelligentPointerThing& operator=(const CIntelligentPointerThing& other) { CThing* item = other.GetPItem(); SetPItem(item); return *this; }
    CVillageTaskPatrol* Task() const { return (CVillageTaskPatrol*)GetPItem(); }
};
struct CIntelligentPointerTrackNode : CBaseIntelligentPointer {   // CIntelligentPointer<CThingTrackNode>, vtable 0x0125A894
    virtual void v00();
    CIntelligentPointerTrackNode(CThingTrackNode* item) { SetPItem(item); }
    CIntelligentPointerTrackNode& operator=(const CIntelligentPointerTrackNode& other) { CThing* item = other.GetPItem(); SetPItem(item); return *this; }
    CThingTrackNode* Node() const { return (CThingTrackNode*)GetPItem(); }
};
struct CTCVillage {
    CThing* GetPatrolTaskForThing(CThing* creature);   // 0x00658E90
    bool SetPatrolTaskAsAcceptedByThing(CThing* task, CThing* creature);   // 0x0065852D
};
struct CTCVillager;
CTCVillage* __fastcall GFGetPVillageInterface(CTCVillager* villager);    // 0x007EE540
bool __fastcall IsThingCarryingWeapon(const CThing* thing);   // 0x006A1540
float __fastcall GFFloatRandom(float range, unsigned long* seed);   // 0x004978A0
struct CWorld {
    static long GetWorldFrame();                // 0x0049D870
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
    long AIAction_LookAtPos(long repeat, C3DVector pos, long flags, float speed);   // 0x0097D580
    long AIAction_MoveToPosProximity(long repeat, C3DVector pos, float radius, bool* reached, long movementType,
        bool a, bool b, bool c, CThing* avoid, bool d, float speed);   // 0x0097E890
    long AIAction_MinionIdle();                 // 0x0097FA00
};
struct CAIStateGroup_VillageTaskPatrol : CAIStateGroupBase {
    CTCVillager* PVillager;                     // +0x14
    CIntelligentPointerThing PTaskThing;        // +0x18
    virtual void Update();
};

void CAIStateGroup_VillageTaskPatrol::Update()
{
    CTCVillage* village = GFGetPVillageInterface(PVillager);
    PTaskThing = village->GetPatrolTaskForThing(GetCreature());
    if (!PTaskThing.GetPItem() || !village->SetPatrolTaskAsAcceptedByThing(PTaskThing.GetPItem(), GetCreature())) {
        Terminate();
        return;
    }
    if (!IsThingCarryingWeapon(GetCreature()))
        AIAction_PerformActionAndWait(&CCreatureAction_UnsheatheWeapons(GetCreature()), false);
    {
        CThing* creature = GetCreature();
        CIntelligentPointerTrackNode node(PTaskThing.Task()->GetNearestNode(creature->GetPosition()));
        while (PTaskThing.GetPItem() && PTaskThing.Task()->IsValidFor(GetCreature())) {
            long result;
            do {
                if (!PTaskThing.GetPItem() || !PTaskThing.Task()->IsValidFor(GetCreature()) || !node.GetPItem() || (node.GetPItem()->m_flags91 & 1)) {
                    Terminate();
                    return;
                }
                bool reached = false;
                result = AIAction_MoveToPosProximity(2, node.GetPItem()->GetPosition(), 0.5f, &reached, 3, true, false, true, 0, true, 0.0f);
            } while (result == 4);
            if (result != 2 || !PTaskThing.GetPItem() || !PTaskThing.Task()->IsValidFor(GetCreature())) {
                Terminate();
                return;
            }
            PTaskThing.Task()->Started = true;
            if (node.Node()->IsEndNode)
                node = CIntelligentPointerTrackNode(node.Node()->PeekTrackStartNode());
            else
                node = CIntelligentPointerTrackNode(node.Node()->PNextNode);
            C3DVector facing = GetCreature()->PPhysics->GetMatrix()->Up;
            const C3DVector& from = GetCreature()->GetPosition();
            const C3DVector& to = node.GetPItem()->GetPosition();
            C3DVector dir = Normalise(C3DVector(to.X - from.X, to.Y - from.Y, 0.0f));
            if (dir.X * facing.X + dir.Y * facing.Y + dir.Z * facing.Z > 0.0f) {
                if (AIAction_LookAtPos(1, node.GetPItem()->GetPosition(), 9, 0.0027777f) == 1) {
                    Terminate();
                    return;
                }
            }
            long fps = g_framesPerSecond;
            double wait = (GFFloatRandom(1.0f, GetPWorldSeed()) + 0.5) * fps;
            long until = (long)(CWorld::GetWorldFrame() + wait);
            while (CWorld::GetWorldFrame() < until) {
                if (AIAction_MinionIdle() != 2 || !PTaskThing.GetPItem() || !PTaskThing.Task()->IsValidFor(GetCreature())) {
                    Terminate();
                    return;
                }
            }
        }
    }
    Terminate();
}
