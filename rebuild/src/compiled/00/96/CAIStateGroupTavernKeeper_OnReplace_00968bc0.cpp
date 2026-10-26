// CAIStateGroup_TavernKeeper::OnReplace() -- finish any running action; if the keeper still holds the jug, take it out of the carry slot and put it back on the bar (reset its physics position and orientation, mark it on the bar); drop mode 0x2b, free and clear the bar point, and make the keeper pushable again
// Leaf: ?OnReplace@CAIStateGroup_TavernKeeper
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00968BC0, length 570 bytes (plain `ret`, virtual slot 10).
// Callees: CThingCreatureBase::IsPerformingAnyAction @ 0x006622A0 / FinishCurrentAction @ 0x00662650, IsCarryingJug
//   @ 0x009687B0 (manifest names it CAIStateGroup_TavernServer; called on the keeper, PJug +0x14 in both), CVectorMap::
//   LowerBound @ 0x0040F020 (inlined ternary GetInterface: 0x47 on the jug, 0x46 carrying, 0xbb tavern jug, 0x31 mode
//   manager, 0xb9 bar), 0x00705D00 (the jug's item id; manifest "CTCTavernGame::GetBanter" is propagated), CTCCarrying
//   0x006A11F0 (find by id) / RemoveThingInCarrySlot 0x006A16A0, CTCTavernJug::ReplaceJug (landed 0x00968090, inlined
//   here: physics slots 31/75/36), CTCCreatureModeManager::RemoveMode @ 0x0073ABA0, FreeBarPoint @ 0x00968950,
//   CBaseIntelligentPointer 0x00A01B10/0x00A01B50/0x00A01B90/0x00A01C10 (null temporary vtable 0x0123E7E0),
//   SetPushableByHero @ 0x0097E330.
// Layout: engine/CAIStateGroup_TavernKeeper.h (PJug +0x14, PBar +0x1c) over the landed LeanOnBar 0x009689C0 model;
//   CTCTavernJug as the landed ReplaceJug (pack 1, OnBar +0x10, RHSet +0x11, Pos +0x29); the bar point's user +0x18.
// Shape: RemoveMode(0x2b) belongs to the carrying-jug branch (the retail not-carrying `je` skips it); the ternary
//   GetInterface form puts the 0x47 component straight into ecx.
class CTCBase;
class C3DVector { public: float X, Y, Z; };
class CRightHandedSet { public: float m[6]; };
struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);       // 0x0040F020
};
class CThing { public:
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
class CCharString { public:
    void* m_pData;
    CCharString(const char* text, long length = -1);    // 0x0099EBF0
    ~CCharString();                                 // 0x0099EAE0
};
float __fastcall GFFloatRandom(float range, unsigned long* seed);   // 0x004978A0
class CThingCreatureBase;
class CTCTavernBar { public:                           // interface 0xb9
    bool GetRandomBarSlot(CThing& creature, C3DVector& position, CRightHandedSet& orientation, long& actionPoint, long& slot);  // 0x007BA3C0
    bool HasEmptyJugSlot(CThing* creature);     // 0x007BA8C0
};
class CCreatureActionBase { public:
    virtual void v00();
    unsigned char _body[0xa4];
    ~CCreatureActionBase();                     // 0x00693EF0
};
class CCreatureAction_PlayIntoLoopOutOfAnimation : public CCreatureActionBase { public:
    unsigned char _anim[0x14];
    CCharString LoopAnimName;                   // +0xbc
    CCharString OutOfAnimName;                  // +0xc0
    CCreatureAction_PlayIntoLoopOutOfAnimation(CThingCreatureBase* creature, const CCharString& into,
        const CCharString& loop, const CCharString& outOf, float loopTime, bool b, long c, long d);   // 0x008427E0
};
class CAIBrain { public:
    unsigned char _pad[0x18];
    CThing* PCreature;                          // +0x18
};
class CAIStateGroupBase { public:
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
class CTCTavernGame { public:                          // interface 0x47 on the jug
    long GetItemID();                           // 0x00705D00 (manifest "CTCTavernGame::GetBanter" is propagated)
};
class CTCCarrying { public:                            // interface 0x46 on the creature
    CThing* FindThingByID(long id);             // 0x006A11F0
    CThing* RemoveThingInCarrySlot(long id, bool b);   // 0x006A16A0
};
class CTCPhysicsBase { public:
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04();
    virtual void v05(); virtual void v06(); virtual void v07(); virtual void v08(); virtual void v09();
    virtual void v10(); virtual void v11(); virtual void v12(); virtual void v13(); virtual void v14();
    virtual void v15(); virtual void v16(); virtual void v17(); virtual void v18(); virtual void v19();
    virtual void v20(); virtual void v21(); virtual void v22(); virtual void v23(); virtual void v24();
    virtual void v25(); virtual void v26(); virtual void v27(); virtual void v28(); virtual void v29();
    virtual void v30();
    virtual void ResetPosition(const C3DVector& pos);           // slot 31 (+0x7c)
    virtual void v32(); virtual void v33(); virtual void v34(); virtual void v35();
    virtual void SetPhysicsActive(bool active);                 // slot 36 (+0x90)
    virtual void v37(); virtual void v38(); virtual void v39(); virtual void v40(); virtual void v41();
    virtual void v42(); virtual void v43(); virtual void v44(); virtual void v45(); virtual void v46();
    virtual void v47(); virtual void v48(); virtual void v49(); virtual void v50(); virtual void v51();
    virtual void v52(); virtual void v53(); virtual void v54(); virtual void v55(); virtual void v56();
    virtual void v57(); virtual void v58(); virtual void v59(); virtual void v60(); virtual void v61();
    virtual void v62(); virtual void v63(); virtual void v64(); virtual void v65(); virtual void v66();
    virtual void v67(); virtual void v68(); virtual void v69(); virtual void v70(); virtual void v71();
    virtual void v72(); virtual void v73(); virtual void v74();
    virtual void ResetOrientation(const CRightHandedSet& rhSet); // slot 75 (+0x12c)
};
struct CThingPhysicsOwner {
    unsigned char _pad[0x60];
    CTCPhysicsBase* m_pPhysics;                 // +0x60
};
#pragma pack(push, 1)
class CTCTavernJug { public:                           // interface 0xbb on the jug
    void* __vftable;                            // +0x00
    CThingPhysicsOwner* m_pParentThing;         // +0x04
    unsigned char _pad_0x08[0x4];
    long Fullness;                              // +0x0c
    bool OnBar;                                 // +0x10
    CRightHandedSet RHSet;                      // +0x11
    C3DVector Pos;                              // +0x29
    __forceinline void ReplaceJug()             // 0x00968090 (inlined here)
    {
        CTCPhysicsBase* physics = m_pParentThing->m_pPhysics;
        if (physics) {
            physics->ResetPosition(Pos);
            physics->ResetOrientation(RHSet);
            physics->SetPhysicsActive(false);
        }
    }
};
#pragma pack(pop)
struct CTCBarPoint {                            // interface 0xb9 on the bar
    unsigned char _pad[0x18];
    CBaseIntelligentPointer PUser;              // +0x18
};
struct CIntelligentPointerCreature : CBaseIntelligentPointer {   // vtable 0x0123E7E0
    virtual void v00();
    CIntelligentPointerCreature(CThing* item) : CBaseIntelligentPointer() { SetPItem(item); }
};
class CTCCreatureModeManager { public:                 // interface 0x31
    void RemoveMode(unsigned long mode);        // 0x0073ABA0
};
struct CThingCreature : CThing {
    bool IsPerformingAnyAction();               // 0x006622A0
    void FinishCurrentAction();                 // 0x00662650
};
struct CAIStateGroup_TavernKeeper : CAIStateGroupBase {
    CBaseIntelligentPointer PJug;               // +0x14 CIntelligentPointer<CThingObject>
    CBaseIntelligentPointer PBar;               // +0x1c CIntelligentPointer<CThingObject>
    CBaseIntelligentPointer PPub;               // +0x24 CIntelligentPointer<CThingBuilding>
    CBaseIntelligentPointer PPayee;             // +0x2c CIntelligentPointer<CThingAICreature>
    long JugActionPointIndex;                   // +0x34
    C3DVector JugPos;                           // +0x38
    CRightHandedSet JugRHSet;                   // +0x44
    long JugSlot;                               // +0x5c
    CThingCreature* GetAICreature() const { return (CThingCreature*)PBrain->PCreature; }
    bool IsCarryingJug();                       // 0x009687B0 (manifest CAIStateGroup_TavernServer::IsCarryingJug)
    void FreeBarPoint();                        // 0x00968950
    virtual void OnReplace();
};

void CAIStateGroup_TavernKeeper::OnReplace()
{
    if (GetAICreature()->IsPerformingAnyAction())
        GetAICreature()->FinishCurrentAction();
    if (IsCarryingJug()) {
        CTCTavernGame* game;
        PJug.GetPItem()->GetInterface(&game, 0x47);
        long id = game->GetItemID();
        CTCCarrying* carrying;
        GetCreature()->GetInterface(&carrying, 0x46);
        CThing* jug = carrying->FindThingByID(id);
        if (jug && !(jug->m_flags91 & 1)) {
            carrying->RemoveThingInCarrySlot(id, false);
            CTCTavernJug* tavernJug;
            jug->GetInterface(&tavernJug, 0xbb);
            tavernJug->ReplaceJug();
            tavernJug->OnBar = true;
        }
        CTCCreatureModeManager* modes;
        GetCreature()->GetInterface(&modes, 0x31);
        modes->RemoveMode(0x2b);
    }
    if (PBar.GetPItem() && !(PBar.GetPItem()->m_flags91 & 1)) {
        CTCBarPoint* bar;
        PBar.GetPItem()->GetInterface(&bar, 0xb9);
        FreeBarPoint();
        bar->PUser = CIntelligentPointerCreature(0);
    }
    SetPushableByHero(true);
}