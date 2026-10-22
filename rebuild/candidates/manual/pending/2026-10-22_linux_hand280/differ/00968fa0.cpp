// CAIStateGroup_TavernKeeper::FillJug() -- with the jug alive: pick it up, find the bar tap near the keeper, then for seven seconds play the tap's DRAW animation (counting the draws) while the keeper plays FILL_JUG_LOOP; mark the jug full (3) and put it down; the keeper is pinned throughout; returns whether every action succeeded
// Leaf: FillJug
struct CTCBase;
struct C3DVector { float X, Y, Z; };
struct CRightHandedSet { float m[6]; };
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
    template<class T> __forceinline bool GetInterface(T** ppOut, long interfaceType)
    {
        if (m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31))) {
            long type = interfaceType;
            CInterfaceMap* map = &m_interfaces;
            CInterfaceMap::SEntry* it = map->LowerBound(&type);
            *ppOut = (T*)((it == map->last || interfaceType < it->type) ? map->last : it)->component;
            return true;
        }
        return false;
    }
};
struct CBaseIntelligentPointer {
    virtual void v00();
    CThing* m_pItem;
    CBaseIntelligentPointer();                  // 0x00A01B10
    CThing* GetPItem() const;                   // 0x00A01B50
    void SetPItem(CThing* item);                // 0x00A01B90
    CBaseIntelligentPointer& operator=(const CBaseIntelligentPointer& other) { SetPItem(other.GetPItem()); return *this; }
    ~CBaseIntelligentPointer();                 // 0x00A01C10
};
struct CCharString {
    void* m_pData;
    CCharString(const char* text, long length = -1);    // 0x0099EBF0
    CCharString(const CCharString& other);          // 0x0099EC30
    ~CCharString();                                 // 0x0099EAE0
};
float __fastcall GFFloatRandom(float range, unsigned long* seed);   // 0x004978A0
struct CThingCreatureBase;
struct CCreatureActionBase {
    virtual void v00();
    unsigned char _body[0xa4];
    ~CCreatureActionBase();                     // 0x00693EF0
};
struct CCreatureAction_PlayIntoLoopOutOfAnimation : CCreatureActionBase {
    unsigned char _anim[0x14];
    CCharString LoopAnimName;                   // +0xbc
    CCharString OutOfAnimName;                  // +0xc0
    CCreatureAction_PlayIntoLoopOutOfAnimation(CThingCreatureBase* creature, const CCharString& into,
        const CCharString& loop, const CCharString& outOf, float loopTime, bool b, long c, long d);   // 0x008427E0
};
struct CAIBrain {
    virtual void v00(); virtual void v01();
    virtual void WaitForNextUpdate();           // slot 2 (+0x08)
    unsigned char _pad[0x18 - 4];
    CThing* PCreature;                          // +0x18
};
struct CAIStateGroupBase {
    virtual ~CAIStateGroupBase();
    CAIBrain* PBrain;                           // +0x04
    long ScriptAIPriority;                      // +0x08
    void* Name_PStringData;                     // +0x0c
    unsigned char Terminated;                   // +0x10
    unsigned char Priority;
    unsigned char Enabled;
    unsigned char _pad_0x13;
    void Terminate();                           // 0x008FCF40
    bool IsStopping();                          // 0x008FD010
    long AIAction_DoNothing();                  // 0x0097E200
    long AIAction_MoveToPosProximity(long repeat, C3DVector pos, float radius, bool* arrived, long movement, bool b1,
        bool b2, bool b3, CThing* faceThing, bool b4, float f);    // 0x0097E890
    CThing* GetCreature() const { return PBrain->PCreature; }
    CThingCreatureBase* GetCreatureBase() const { return (CThingCreatureBase*)PBrain->PCreature; }
    unsigned long* GetPWorldSeed();             // 0x008FCFD0
    void SetPushableByHero(bool pushable);      // 0x0097E330
    long AIAction_PerformAction(CCreatureActionBase* action);   // 0x0097D8B0
};
struct CThingCreatureBase;
struct CSoundPair {
    long FirstSound;
    long LastSound;
    CSoundPair() : FirstSound(0), LastSound(0) { }
};
struct CCreatureAction_PlayAnimation : CCreatureActionBase {   // vtable 0x01273A0C, retail sizeof 0xb4
    unsigned char _members[0xb4 - 0xa8];
    CCreatureAction_PlayAnimation(CThingCreatureBase* creature, const CCharString& animation, const CSoundPair& sound,
        bool useMovement, bool stayOnLastFrame, bool looping, long c, long d);   // 0x008424F0
};
struct CIntelligentPointerThingMember : CBaseIntelligentPointer {   // CIntelligentPointer<CThing>, vtable 0x01238C6C
    virtual void v00();
};
struct CCreatureAction_PickUpJugToFill : CCreatureActionBase {   // vtable 0x01275504
    CIntelligentPointerThingMember PJug;        // +0xa8
    CCreatureAction_PickUpJugToFill(CThingCreatureBase* creature, CThing* jug);   // 0x00844140
};
struct CCreatureAction_PutDownJugFromFilling : CCreatureActionBase {   // vtable 0x01275634
    CIntelligentPointerThingMember PJug;        // +0xa8
    CCreatureAction_PutDownJugFromFilling(CThingCreatureBase* creature, CThing* jug);   // 0x008441D0
};
struct C2DBoxI {
    long Left, Top, Right, Bottom;
};
struct CThingFilter_HasDefName {
    CCharString DefName;
    CThingFilter_HasDefName(const CCharString& name) : DefName(name) { }
};
struct CThingSearchTools {
    bool GetNearestThingInRoughArea(const C2DBoxI& area, const CThingFilter_HasDefName& filter, const C3DVector& pos,
        CThing** out);                          // 0x009697E0 (manifest GetAllThingsInRoughArea<...> label is propagated)
};
struct CEngineWorld {
    unsigned char _pad[0x20];
    CThingSearchTools* PSearchTools;            // +0x20
};
struct CEngine {
    unsigned char _pad[0x24];
    CEngineWorld* PWorld;                       // +0x24
};
extern CEngine* g_pEngine;                      // 0x013B86A0
extern long g_framesPerSecond;                  // 0x01375550
long __fastcall GFGetCurrentFrame();            // 0x0049D870
struct CTCAnimatingObject {                     // interface 0x83 on the tap
    void PlayAnimation(const CCharString& name, long index, bool b);   // 0x007F90B0
};
#pragma pack(push, 1)
struct CTCTavernJug {                           // interface 0xbb on the jug
    void* __vftable;
    void* m_pParentThing;
    unsigned char _pad_0x08[0x4];
    long Fullness;                              // +0x0c
    bool OnBar;                                 // +0x10
};
#pragma pack(pop)
struct CThingPos {
    const C3DVector& GetPosition() const;       // 0x004C73D0
};
__forceinline bool IS_THING_ALIVE(const CBaseIntelligentPointer& p)
{
    if (p.GetPItem() != 0) {
        if (!(p.GetPItem()->m_flags91 & 1))
            return true;
    }
    return false;
}
struct CAIStateGroup_TavernKeeper : CAIStateGroupBase {
    CBaseIntelligentPointer PJug;               // +0x14 CIntelligentPointer<CThingObject>
    CBaseIntelligentPointer PBar;               // +0x1c
    CBaseIntelligentPointer PPub;               // +0x24
    CBaseIntelligentPointer PPayee;             // +0x2c
    long AIAction_PerformActionAndWait(CCreatureActionBase* action, bool wait);   // 0x0097DB70
    bool FillJug();
};

bool CAIStateGroup_TavernKeeper::FillJug()
{
    if (!IS_THING_ALIVE(PJug))
        return false;
    SetPushableByHero(false);
    bool failed = AIAction_PerformActionAndWait(&CCreatureAction_PickUpJugToFill(GetCreatureBase(), PJug.GetPItem()), false) != 2;
    if (failed) {
        SetPushableByHero(true);
        return false;
    }
    const C3DVector& pos = ((CThingPos*)GetCreature())->GetPosition();
    long x = (long)pos.X;
    long y = (long)pos.Y;
    C2DBoxI area;
    area.Left = x - 2;
    area.Top = y - 2;
    area.Right = x + 2;
    area.Bottom = y + 2;
    long deadline;
    CTCAnimatingObject* tapAnimation;
    {
        CThing* tap;
        {
            CThingSearchTools* search = g_pEngine->PWorld->PSearchTools;
            CCharString tapName("OBJECT_GENERIC_BARTAP_01");
            search->GetNearestThingInRoughArea(area, CThingFilter_HasDefName(tapName), C3DVector(((CThingPos*)GetCreature())->GetPosition()), &tap);
        }
        long fps = g_framesPerSecond;
        deadline = GFGetCurrentFrame() + fps * 7;
        tapAnimation = 0;
        tap->GetInterface(&tapAnimation, 0x83);
    }
    long draws = 0;
    for (;;) {
        if (tapAnimation) {
            tapAnimation->PlayAnimation("DRAW", draws++, false);
        }
        bool stopped = AIAction_PerformActionAndWait(&CCreatureAction_PlayAnimation(GetCreatureBase(), "FILL_JUG_LOOP",
            CSoundPair(), false, true, false, 0, 0), false) == 1;
        if (stopped) {
            SetPushableByHero(true);
            return false;
        }
        if (deadline <= GFGetCurrentFrame())
            break;
    }
    if (!IS_THING_ALIVE(PJug))
        return false;
    CTCTavernJug* jug;
    PJug.GetPItem()->GetInterface(&jug, 0xbb);
    jug->Fullness = 3;
    if (AIAction_PerformActionAndWait(&CCreatureAction_PutDownJugFromFilling(GetCreatureBase(), PJug.GetPItem()), false) != 2) {
        SetPushableByHero(true);
        return false;
    }
    SetPushableByHero(true);
    return true;
}
