// CAIStateGroup_TavernServer::FillTankard() -- with a live tankard and jug: pin the server, claim the tankard, play the pour action, wait up to 4 seconds while the serving point stays valid, fill the tankard, use a jug serving, play the second action, free the bar point and unpin
// Leaf: FillTankard
// Retail: Fable.exe (TLC, VC7.1) @ 0x00967B40, 468 bytes.
// Callees: IsAlive via CBaseIntelligentPointer::GetPItem @ 0x00A01B50 (PTankard +0x2c, PJug +0x14), Terminate @ 0x008FCF40,
//   SetPushableByHero @ 0x0097E330, CVectorMap::LowerBound @ 0x0040F020 (inlined GetInterface 0xba = +0x34 bit 26 tankard,
//   0xbb = +0x34 bit 27 jug; the miss path reads the never-written key slot), tankard user setter @ 0x0094B400 (manifest
//   "CVillageTaskCarryCrate::AssignCreatureToTask" is a folded label), the two one-argument creature actions @ 0x00842A80 /
//   0x00842AF0 (both labelled "CCreatureAction_AlertToPlayer"; sizeof 0xa8 from the 0xac frame), ~CCreatureActionBase
//   @ 0x00693EF0, AIAction_PerformActionAndWait @ 0x0097DB70, AIAction_DoNothing @ 0x0097E200, CWorld::GetWorldFrame (static)
//   @ 0x0049D870 (manifest "GFGetPreMainMemoryUsed"), frames-per-second global @ 0x01375550, IsServingPointValid @ 0x00967250,
//   FreeBarPoint @ 0x009670A0.
// Layout: engine/CAIStateGroup_TavernServer.h (PJug +0x14 .. PPub +0x34) over the landed local CAIStateGroupBase model;
//   tankard fill level (float) and jug serving count at +0x0c of their components (descriptive names).
// Shape: every failure path written out in full (`FreeBarPoint(); SetPushableByHero(true); return false;`) instead of a
//   shared goto label -- VC7.1 then cross-jumps the copies and keeps the first one as the fall-through of the first test
//   (a goto to one shared block gets that block sunk below the loop); the frames-per-second global read into a local
//   before the GetWorldFrame call; the creature through an inline getter (`mov eax,[esi+4] ; mov eax,[eax+18h]`).
struct CTCBase;
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
    unsigned char _pad1[0x91 - 0x4c];
    unsigned char m_flags91;                    // +0x91 bit 0: dead
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
struct CBaseIntelligentPointer {
    virtual void v00();
    CThing* m_pItem;
    CBaseIntelligentPointer();                  // 0x00A01B10
    CThing* GetPItem() const;                   // 0x00A01B50
    void SetPItem(CThing* item);                // 0x00A01B90
    ~CBaseIntelligentPointer();                 // 0x00A01C10
};
__forceinline bool IS_THING_ALIVE(const CBaseIntelligentPointer& p)
{
    if (p.GetPItem() != 0) {
        if (!(p.GetPItem()->m_flags91 & 1))
            return true;
    }
    return false;
}
struct CTCTankard {                             // interface 0xba
    unsigned char _pad[0xc];
    float FillLevel;                            // +0x0c
    void SetUser(CThing* user);                 // 0x0094B400
};
struct CTCJug {                                 // interface 0xbb
    unsigned char _pad[0xc];
    long Servings;                              // +0x0c
};
struct CThingAICreature;
struct CCreatureActionBase {
    virtual void v00();
    unsigned char _body[0xa4];
    ~CCreatureActionBase();                     // 0x00693EF0
};
struct CCreatureAction_FillTankardFromJug : CCreatureActionBase {
    CCreatureAction_FillTankardFromJug(CThingAICreature* creature);   // 0x00842A80
};
struct CCreatureAction_PutDownJug : CCreatureActionBase {
    CCreatureAction_PutDownJug(CThingAICreature* creature);           // 0x00842AF0
};
struct CWorld { static long GetWorldFrame(); };  // 0x0049D870
extern long g_FramesPerSecond;                  // 0x01375550
struct CAIBrain {
    unsigned char _pad[0x18];
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
    CThingAICreature* GetAICreature() const { return (CThingAICreature*)PBrain->PCreature; }
    void SetPushableByHero(bool pushable);      // 0x0097E330
    long AIAction_DoNothing();                  // 0x0097E200
    long AIAction_PerformActionAndWait(CCreatureActionBase* action, bool b);   // 0x0097DB70
};
struct CAIStateGroup_TavernServer : CAIStateGroupBase {
    CBaseIntelligentPointer PJug;               // +0x14
    CBaseIntelligentPointer PBar;               // +0x1c
    CBaseIntelligentPointer PTable;             // +0x24
    CBaseIntelligentPointer PTankard;           // +0x2c
    CBaseIntelligentPointer PPub;               // +0x34
    void FreeBarPoint();                        // 0x009670A0
    bool IsServingPointValid();                 // 0x00967250
    bool FillTankard();
};

bool CAIStateGroup_TavernServer::FillTankard()
{
    if (!IS_THING_ALIVE(PTankard) || !IS_THING_ALIVE(PJug)) {
        Terminate();
        return false;
    }
    SetPushableByHero(false);
    CTCTankard* tankard;
    PTankard.GetPItem()->GetInterface(&tankard, 0xba);
    tankard->SetUser(PBrain->PCreature);
    if (AIAction_PerformActionAndWait(&CCreatureAction_FillTankardFromJug(GetAICreature()), false) != 2) {
        FreeBarPoint();
        SetPushableByHero(true);
        return false;
    }
    long fps = g_FramesPerSecond;
    long endFrame = CWorld::GetWorldFrame() + fps * 4;
    do {
        if (AIAction_DoNothing() != 4) {
            FreeBarPoint();
            SetPushableByHero(true);
            return false;
        }
    } while (endFrame > CWorld::GetWorldFrame() && IsServingPointValid());
    tankard->FillLevel = 1.0f;
    CTCJug* jug;
    PJug.GetPItem()->GetInterface(&jug, 0xbb);
    jug->Servings--;
    if (AIAction_PerformActionAndWait(&CCreatureAction_PutDownJug(GetAICreature()), false) != 2) {
        FreeBarPoint();
        SetPushableByHero(true);
        return false;
    }
    FreeBarPoint();
    SetPushableByHero(true);
    return true;
}