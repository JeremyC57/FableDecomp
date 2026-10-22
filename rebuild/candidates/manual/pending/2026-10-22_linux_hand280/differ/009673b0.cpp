// CAIStateGroup_TavernServer::OnReplace() -- if the server is carrying its jug, take the live jug out of the carry slot and put it back (ReplaceJug, mark it on the bar) and drop modes 0x29/0x2a; then free the serving and bar points, clear all five intelligent pointers and make the server pushable again
// Leaf: ?OnReplace@CAIStateGroup_TavernServer
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
    ~CCharString();                                 // 0x0099EAE0
};
float __fastcall GFFloatRandom(float range, unsigned long* seed);   // 0x004978A0
struct CThingCreatureBase;
struct CTCTavernBar {                           // interface 0xb9
    bool GetRandomBarSlot(CThing& creature, C3DVector& position, CRightHandedSet& orientation, long& actionPoint, long& slot);  // 0x007BA3C0
    bool HasEmptyJugSlot(CThing* creature);     // 0x007BA8C0
};
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
    CThing* GetCreature() const { return PBrain->PCreature; }
    CThingCreatureBase* GetCreatureBase() const { return (CThingCreatureBase*)PBrain->PCreature; }
    unsigned long* GetPWorldSeed();             // 0x008FCFD0
    void SetPushableByHero(bool pushable);      // 0x0097E330
    long AIAction_PerformAction(CCreatureActionBase* action);   // 0x0097D8B0
};
struct CTCTavernGame {                          // interface 0x47 on the jug
    long GetItemID();                           // 0x00705CC0 (manifest "CTCTavernGame::GetBanter" is propagated)
};
struct CTCCarrying {                            // interface 0x46 on the creature
    CThing* FindThingByID(long id);             // 0x006A11F0
    CThing* RemoveThingInCarrySlot(long id, bool b);   // 0x006A16A0
};
#pragma pack(push, 1)
struct CTCTavernJug {                           // interface 0xbb on the jug
    void* __vftable;                            // +0x00
    void* m_pParentThing;                       // +0x04
    unsigned char _pad_0x08[0x4];
    long Fullness;                              // +0x0c
    bool OnBar;                                 // +0x10
    CRightHandedSet RHSet;                      // +0x11
    C3DVector Pos;                              // +0x29
    void ReplaceJug();                          // 0x00968090
};
#pragma pack(pop)
struct CIntelligentPointerObject : CBaseIntelligentPointer {     // CIntelligentPointer<CThingObject>, vtable 0x0125A858
    virtual void v00();
    CIntelligentPointerObject(CThing* item) : CBaseIntelligentPointer() { SetPItem(item); }
    CIntelligentPointerObject& operator=(const CIntelligentPointerObject& other) { SetPItem(other.GetPItem()); return *this; }
};
struct CIntelligentPointerBuilding : CBaseIntelligentPointer {   // CIntelligentPointer<CThingBuilding>, vtable 0x01244344
    virtual void v00();
    CIntelligentPointerBuilding(CThing* item) : CBaseIntelligentPointer() { SetPItem(item); }
    CIntelligentPointerBuilding& operator=(const CIntelligentPointerBuilding& other) { SetPItem(other.GetPItem()); return *this; }
};
struct CTCCreatureModeManager {                 // interface 0x31
    void RemoveMode(unsigned long mode);        // 0x0073ABA0
};
inline bool IS_THING_ALIVE(const CBaseIntelligentPointer& p)
{
    if (p.GetPItem() != 0) {
        if (!(p.GetPItem()->m_flags91 & 1))
            return true;
    }
    return false;
}
struct CAIStateGroup_TavernServer : CAIStateGroupBase {
    CIntelligentPointerObject PJug;             // +0x14
    CIntelligentPointerObject PBar;             // +0x1c
    CIntelligentPointerObject PTable;           // +0x24
    CIntelligentPointerObject PTankard;         // +0x2c
    CIntelligentPointerBuilding PPub;           // +0x34
    long JugIndex;                              // +0x3c
    long ServingActionPointIndex;               // +0x40
    bool IsCarryingJug();                       // 0x00966CF0
    void FreeServingPoint();                    // 0x009670A0 (manifest "CAIStateGroup_TavernKeeper::FreeBarPoint")
    void FreeBarPoint();                        // 0x00967110
    virtual void OnReplace();
};

void CAIStateGroup_TavernServer::OnReplace()
{
    if (IsCarryingJug()) {
        CTCTavernGame* game;
        if (IS_THING_ALIVE(PJug) && PJug.GetPItem()->GetInterface(&game, 0x47)) {
            long id = game->GetItemID();
            CTCCarrying* carrying;
            if (GetCreature()->GetInterface(&carrying, 0x46)) {
                CThing* jug = carrying->FindThingByID(id);
                if (jug && !(jug->m_flags91 & 1)) {
                    carrying->RemoveThingInCarrySlot(id, false);
                    CTCTavernJug* tavernJug;
                    if (jug->GetInterface(&tavernJug, 0xbb)) {
                        tavernJug->ReplaceJug();
                        tavernJug->OnBar = true;
                    }
                }
            }
        }
        CTCCreatureModeManager* modes;
        GetCreature()->GetInterface(&modes, 0x31);
        modes->RemoveMode(0x29);
        modes->RemoveMode(0x2a);
    }
    FreeServingPoint();
    FreeBarPoint();
    PJug = CIntelligentPointerObject(0);
    PBar = CIntelligentPointerObject(0);
    PTable = CIntelligentPointerObject(0);
    PPub = CIntelligentPointerBuilding(0);
    PTankard = CIntelligentPointerObject(0);
    SetPushableByHero(true);
}
