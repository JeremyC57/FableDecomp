// CAIStateGroup_Gaze::Update() -- take a free gaze marker, claim it, walk (or sometimes run) to it, look the way it faces (jittered when the marker is not the creature's current look target), then idle 3..8 seconds; terminate at the end
// Grade: DIFFER(806v802) -- frame 0x38 v 0x34 and ebp unused (retail caches g_FramesPerSecond in ebp), this-member pointer
//   in ebx (retail edi), the normal-path def release keeps a null test and the split decrement (retail `dec [ecx+4]`, no
//   test), MoveToPos argument register order; flow and calls otherwise line up.
// Retail: Fable.exe (TLC, VC7.1) @ 0x00918B90, 816 bytes. ?Update@CAIStateGroup_Gaze@@MAEXXZ (Ego_r.pdb).
// Callees: FindFreeGazePos 0x00918880, CBaseIntelligentPointer ctor 0x00A01B10 / GetPItem 0x00A01B50 / SetPItem 0x00A01B90 /
//   dtor 0x00A01C10 (CIntelligentPointer<CThingGameObject> vtable 0x0127B4D8), Terminate 0x008FCF40,
//   CTCCreatureNavigation::GetSubDef<CCreatureNavigationDef> 0x006CD2F0, CVectorMap::LowerBound 0x0040F020,
//   AssignCreatureToTask 0x00916640, the marker placement's virtual slot 70 (+0x118), the state group's slot 14 (+0x38),
//   GetPWorld 0x008FCFC0, CWorld random seed 0x0049D810, GFFloatRandom 0x004978A0, C3DVector::RotateAboutZ 0x00A14040,
//   GetRandomPointWithinUnitSphere 0x00A146C0, CThing::GetPosition 0x004C73D0, AIAction_MoveToPosProximity 0x0097E890,
//   AIAction_LookAtPos 0x0097D580, GetPWorldSeed 0x008FCFD0, GFROR13 0x00497890, CWorld::GetWorldFrame 0x0049D870,
//   AIAction_DoNothing 0x0097E200. Floats 0.1f @ 0x0122EE58, 0.25f @ 0x0123078C; FPS global 0x01375550.
// Layout: CAIStateGroup_Gaze GazeMarker +0x14, ChanceOfRunning +0x1c, MinJourneyForRunning +0x20 (Ego_r.pdb); creature
//   navigation component +0x70 and look-at pointer +0x170, thing placement +0x60 (position +0x0c), marker user +0x0c,
//   navigation def flag +0x34 (retail operands).
struct CTCBase;
struct C3DVector {
    float X, Y, Z;
    C3DVector() { }
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) { }
    C3DVector(const C3DVector& o) : X(o.X), Y(o.Y), Z(o.Z) { }
    C3DVector operator+(const C3DVector& o) const { return C3DVector(X + o.X, Y + o.Y, Z + o.Z); }
    C3DVector operator-(const C3DVector& o) const { return C3DVector(X - o.X, Y - o.Y, Z - o.Z); }
    float GetLength() const;
    void RotateAboutZ(float angle);             // 0x00A14040
};
#include <math.h>
inline float C3DVector::GetLength() const { return sqrtf(X * X + Y * Y + Z * Z); }
C3DVector __fastcall GetRandomPointWithinUnitSphere(unsigned long* seed);   // 0x00A146C0
float __fastcall GFFloatRandom(float range, unsigned long* seed);   // 0x004978A0
unsigned long __fastcall GFROR13(unsigned long value);    // 0x00497890
extern unsigned long g_FramesPerSecond;         // 0x01375550
struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);       // 0x0040F020
};
#pragma auto_inline(off)
CInterfaceMap::SEntry* CInterfaceMap::LowerBound(const long* type)
{
    SEntry* it = first;
    int len = int(last - first);
    while (len > 0) {
        int half = len / 2;
        SEntry* middle = it + half;
        if (middle->type < *type) {
            it = middle + 1;
            len = len - half - 1;
        } else
            len = half;
    }
    return it;
}
#pragma auto_inline(on)
struct CThingPlacement {
    virtual void v00();
    virtual void v01();
    virtual void v02();
    virtual void v03();
    virtual void v04();
    virtual void v05();
    virtual void v06();
    virtual void v07();
    virtual void v08();
    virtual void v09();
    virtual void v0a();
    virtual void v0b();
    virtual void v0c();
    virtual void v0d();
    virtual void v0e();
    virtual void v0f();
    virtual void v10();
    virtual void v11();
    virtual void v12();
    virtual void v13();
    virtual void v14();
    virtual void v15();
    virtual void v16();
    virtual void v17();
    virtual void v18();
    virtual void v19();
    virtual void v1a();
    virtual void v1b();
    virtual void v1c();
    virtual void v1d();
    virtual void v1e();
    virtual void v1f();
    virtual void v20();
    virtual void v21();
    virtual void v22();
    virtual void v23();
    virtual void v24();
    virtual void v25();
    virtual void v26();
    virtual void v27();
    virtual void v28();
    virtual void v29();
    virtual void v2a();
    virtual void v2b();
    virtual void v2c();
    virtual void v2d();
    virtual void v2e();
    virtual void v2f();
    virtual void v30();
    virtual void v31();
    virtual void v32();
    virtual void v33();
    virtual void v34();
    virtual void v35();
    virtual void v36();
    virtual void v37();
    virtual void v38();
    virtual void v39();
    virtual void v3a();
    virtual void v3b();
    virtual void v3c();
    virtual void v3d();
    virtual void v3e();
    virtual void v3f();
    virtual void v40();
    virtual void v41();
    virtual void v42();
    virtual void v43();
    virtual void v44();
    virtual void v45();
    virtual C3DVector GetForward() const;       // slot 70 (+0x118)
    unsigned char _pad04[0x0c - 4];
    C3DVector Position;                         // +0x0c
};
struct CThing {
    unsigned char _pad0[0x20];
    unsigned long m_presentBits[9];             // +0x20
    CInterfaceMap m_interfaces;                 // +0x44
    unsigned char _pad4c[0x60 - 0x4c];
    CThingPlacement* PPlacement;                // +0x60
    const C3DVector& GetPosition() const;       // 0x004C73D0
    bool TestTC(long type) const { return ((m_presentBits[type >> 5] >> (type & 31)) & 1) != 0; }
    template<class T> __forceinline void GetInterfaceR(T** ppOut, long type, bool& has)
    {
        has = TestTC(type);
        if (has) {
            long key = type;
            CInterfaceMap* map = &m_interfaces;
            CInterfaceMap::SEntry* it = map->LowerBound(&key);
            *ppOut = (T*)(it == map->last || type < it->type ? map->last : it)->component;
        }
    }
};
struct CThingGameObject : CThing { };
struct CBaseIntelligentPointer {
    virtual void v00();
    CThing* m_pItem;
    CBaseIntelligentPointer();                  // 0x00A01B10
    ~CBaseIntelligentPointer();                 // 0x00A01C10
    CThing* GetPItem() const;                   // 0x00A01B50
    void SetPItem(CThing* item);                // 0x00A01B90
};
template<class T> struct CIntelligentPointer : CBaseIntelligentPointer {
    virtual void v01();
    CIntelligentPointer(T* item) { SetPItem(item); }
    CIntelligentPointer& operator=(const CIntelligentPointer& o) { SetPItem(o.GetPItem()); return *this; }
};
struct CCreatureNavigationDef {
    virtual void v00();
    virtual void Destroy();                     // slot 1
    long RefCount;                              // +0x04
    unsigned char _pad08[0x34 - 8];
    bool AvoidOtherCreatures;                   // +0x34
};
template<class T> struct CDefPointer {
    T* Object;
    CDefPointer() : Object(0) { }
    ~CDefPointer() { if (Object) { if (--Object->RefCount == 0) Object->Destroy(); } }
    T* operator->() const { return Object; }
};
struct CTCCreatureNavigation {
    bool GetSubDef(CDefPointer<CCreatureNavigationDef>& out);   // 0x006CD2F0
};
struct CTCGazeMarker {                          // interface 0x2F
    unsigned char _pad00[0x0c];
    CBaseIntelligentPointer User;               // +0x0c
    void AssignCreatureToTask(CThing* creature);   // 0x00916640
};
struct CThingCreatureBase : CThing {
    unsigned char _pad64[0x70 - 0x64];
    CTCCreatureNavigation* PNavigation;         // +0x70
    unsigned char _pad74[0x170 - 0x74];
    CBaseIntelligentPointer LookAtThing;        // +0x170
};
struct CAIBrain {
    unsigned char _pad00[0x18];
    CThingCreatureBase* Creature;               // +0x18
};
struct CWorld {
    unsigned long* GetPRandomSeed();            // 0x0049D810
    static long GetWorldFrame();                // 0x0049D870
};
enum EAIActionRepeat { AI_ACTION_REPEAT_ONCE = 0, AI_ACTION_REPEAT_UNTIL_DONE = 1, AI_ACTION_REPEAT_POLL = 2 };
enum EAIActionReturn { AI_ACTION_STOPPING = 1, AI_ACTION_DONE = 2, AI_ACTION_RUNNING = 4 };
enum EMovementType { MOVEMENT_DEFAULT, MOVEMENT_WALK = 3, MOVEMENT_RUN = 5 };
struct CAIStateGroupBase {
    virtual void v00(); virtual void v04(); virtual void v08(); virtual void v0c(); virtual void v10(); virtual void v14();
    virtual void v18(); virtual void v1c(); virtual void v20(); virtual void v24(); virtual void v28(); virtual void v2c();
    virtual void v30(); virtual void v34();
    virtual CThing* GetLookTarget();            // slot 14 (+0x38)
    CAIBrain* PBrain;                           // +0x04
    unsigned char _pad08[0x0c];
    CThingCreatureBase* GetCreature() const { return PBrain->Creature; }
    CWorld* GetPWorld() const;                  // 0x008FCFC0
    unsigned long* GetPWorldSeed() const;       // 0x008FCFD0
    void Terminate();                           // 0x008FCF40
    unsigned long Random(unsigned long range) const
    {
        unsigned long* seed = GetPWorldSeed();
        *seed = *seed * 0x24a1 + 0x24df;
        *seed = GFROR13(*seed);
        return *seed % range;
    }
    EAIActionReturn AIAction_MoveToPosProximity(EAIActionRepeat repeat, C3DVector pos, float proximity, bool& moving,
                                                EMovementType anim, bool b1, bool b2, bool b3, const CThing* target,
                                                bool b4, float f);     // 0x0097E890
    EAIActionReturn AIAction_LookAtPos(EAIActionRepeat repeat, C3DVector pos, long lookFlags, float tolerance);   // 0x0097D580
    EAIActionReturn AIAction_DoNothing();       // 0x0097E200
};
struct CAIStateGroup_Gaze : CAIStateGroupBase {
    CIntelligentPointer<CThingGameObject> GazeMarker;   // +0x14
    float ChanceOfRunning;                      // +0x1c
    float MinJourneyForRunning;                 // +0x20
    CThingGameObject* FindFreeGazePos();        // 0x00918880
    void Update();
};

void CAIStateGroup_Gaze::Update()
{
    GazeMarker = CIntelligentPointer<CThingGameObject>(FindFreeGazePos());
    if (!GazeMarker.GetPItem()) {
        Terminate();
        return;
    }
    CDefPointer<CCreatureNavigationDef> navDef;
    GetCreature()->PNavigation->GetSubDef(navDef);
    CTCGazeMarker* marker;
    bool has;
    GazeMarker.GetPItem()->GetInterfaceR(&marker, 0x2f, has);
    if (marker->User.GetPItem()) {
        Terminate();
        return;
    }
    marker->AssignCreatureToTask(GetCreature());
    CThingPlacement* placement = GazeMarker.GetPItem()->PPlacement;
    C3DVector pos = placement->Position;
    C3DVector facing = placement->GetForward();
    if (GetLookTarget() == GetCreature()->LookAtThing.GetPItem()) {
        float angle = GFFloatRandom(0.2f, GetPWorld()->GetPRandomSeed()) - 0.1f;
        facing.RotateAboutZ(angle);
        C3DVector jitter = GetRandomPointWithinUnitSphere(GetPWorld()->GetPRandomSeed());
        pos = C3DVector(pos.X + jitter.X * 0.25f, pos.Y + jitter.Y * 0.25f, pos.Z);
    }
    EMovementType movement = MOVEMENT_WALK;
    if (GFFloatRandom(1.0f, GetPWorld()->GetPRandomSeed()) <= ChanceOfRunning
        && (GetCreature()->GetPosition() - pos).GetLength() > MinJourneyForRunning)
        movement = MOVEMENT_RUN;
    bool moving;
    while (AIAction_MoveToPosProximity(AI_ACTION_REPEAT_POLL, pos, 0.0f, moving, movement, true, false,
                                       navDef->AvoidOtherCreatures, 0, true, 0.0f) == AI_ACTION_RUNNING)
        ;
    while (AIAction_LookAtPos(AI_ACTION_REPEAT_POLL, pos + facing, 0x11, 0.00277778f) == AI_ACTION_RUNNING)
        ;
    long endFrame = CWorld::GetWorldFrame() + (Random(6) + 3) * g_FramesPerSecond;
    while (CWorld::GetWorldFrame() < endFrame && AIAction_DoNothing() == AI_ACTION_RUNNING)
        ;
    Terminate();
}
