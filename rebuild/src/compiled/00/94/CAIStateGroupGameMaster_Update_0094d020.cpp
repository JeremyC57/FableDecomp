// CAIStateGroup_GameMaster::Update() -- adopt the work building as the tavern; when not yet seated walk to a seat, attach to the table's game; then, while the hero is not following, keep the TABLE_GAME mode in step with the game state and either offer the hero a game or run the base behaviour; terminate when anything fails
// Grade: RELOCATION_MATCH -- levers: out-parameter GetInterface for the village member (merges the HasTC test), named
//   interaction target, shared ChooseAction() tail via continue, named item in CIntelligentPointer operator=
// Retail: Fable.exe (TLC, VC7.1) @ 0x0094D020, 1040 bytes, plain ret.
// Callees: CBaseIntelligentPointer ctor 0x00A01B10 / GetPItem 0x00A01B50 / SetPItem 0x00A01B90 / dtor 0x00A01C10
//   (CIntelligentPointer<CThingBuilding> vtable 0x01244344), AIAction_DoNothing 0x0097E200, Terminate 0x008FCF40,
//   CAIStateGroup_GoToPub::WalktoSeatAndSit 0x00949580, IS_THING_ALIVE<CIntelligentPointer<CThingObject> > 0x0078B2A0 (__fastcall),
//   CVectorMap::LowerBound 0x0040F020 / const 0x004365B0 (same-TU stand-ins), CTCTavernGame::RegisterGameMaster 0x0094D430,
//   CTCVillageMember::GetPVillage 0x007EE540, GetPlayerManager 0x008FCF90, CPlayerManager::GetMainPlayer 0x00449970,
//   CPlayer::GetPControlledCreature 0x00487DC0, CTCFollowed::IsFollower 0x006D4D70, CTCRegionFollower::IsRegionFollower 0x006AEB60,
//   CTCCreatureModeManager::AddMode 0x0073AE10 / RemoveMode 0x0073ABA0, CWorld::GetFrame 0x0049D870,
//   OfferPlayerAGame 0x0094CE60, the group's virtual slots 14 (+0x38) and 15 (+0x3c), the hero component's slot 33 (+0x84).
// Layout: CAIStateGroup_GameMaster derives CAIStateGroup_GoToPub (Ego_r.pdb): PPub +0x14, PTable +0x24,
//   ActionPointIndex +0x64, Sitting +0x70, HandsOffMode +0xa0, TimeNextReactToPlayer +0xa8; retail CThingAICreature
//   WorkBuilding +0x170 (landed 0x0094C900). Modelled locally (flattened bases, no engine header for the group).

class CTCBase;
class CBaseObject { };
class CThing;
class CThingBuilding;
struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);           // 0x0040F020
    SEntry* LowerBound(const long* type) const;     // 0x004365B0
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
CInterfaceMap::SEntry* CInterfaceMap::LowerBound(const long* type) const
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

struct CBaseIntelligentPointer {
    virtual void v00();
    CThing* PItem;                              // +0x04
    CBaseIntelligentPointer();                  // 0x00A01B10
    ~CBaseIntelligentPointer();                 // 0x00A01C10
    CBaseObject* GetPItem() const;              // 0x00A01B50 (Ego_r.pdb ?GetPItem@CBaseIntelligentPointer@@QBEPAVCBaseObject@@XZ)
    void SetPItem(CBaseObject* item);           // 0x00A01B90 (Ego_r.pdb ?SetPItem@CBaseIntelligentPointer@@QAEXPAVCBaseObject@@@Z)
};
struct CBuildingPointer : CBaseIntelligentPointer {          // CIntelligentPointer<CThingBuilding>, vtable 0x01244344
    virtual void v00();
    __forceinline CBuildingPointer(CThingBuilding* item) : CBaseIntelligentPointer() { SetPItem((CBaseObject*)item); }
    __forceinline CBuildingPointer& operator=(const CBuildingPointer& other) { CBaseObject* item = other.CBaseIntelligentPointer::GetPItem(); SetPItem(item); return *this; }
    __forceinline CThingBuilding* GetPItem() const { return (CThingBuilding*)CBaseIntelligentPointer::GetPItem(); }
};
class CThingObject;
template <class T> class CIntelligentPointer : public CBaseIntelligentPointer { };
template <class T> bool __fastcall IS_THING_ALIVE(const T& p);   // 0x0078B2A0 = ??$IS_THING_ALIVE@V?$CIntelligentPointer@VCThingObject@@@@@@ (FableWin.pdb; PTable is CIntelligentPointer<CThingObject>, Ego_r.pdb)

class CThing : public CBaseObject {
public:
    unsigned char _pad0[0x20];
    unsigned long m_presentBits[9];             // +0x20
    CInterfaceMap m_interfaces;                 // +0x44
    unsigned char _pad1[0x91 - 0x4c];
    unsigned char m_flags91;                    // +0x91 bit 0: dead
    __forceinline unsigned long HasTC(long interfaceType) const
    {
        return m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31));
    }
    template<class T> __forceinline T* GetTC(long interfaceType)
    {
        long type = interfaceType;
        CInterfaceMap* map = &m_interfaces;
        CInterfaceMap::SEntry* it = map->LowerBound(&type);
        return (T*)((it == map->last || interfaceType < it->type) ? map->last : it)->component;
    }
    template<class T> __forceinline T* GetTCConst(long interfaceType) const
    {
        long type = interfaceType;
        const CInterfaceMap* map = &m_interfaces;
        CInterfaceMap::SEntry* it = map->LowerBound(&type);
        return (T*)((it == map->last || interfaceType < it->type) ? map->last : it)->component;
    }
    template<class T> __forceinline bool GetInterface(T** ppOut, long interfaceType) const
    {
        if (m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31))) {
            long type = interfaceType;
            const CInterfaceMap* map = &m_interfaces;
            const CInterfaceMap::SEntry* it = map->LowerBound(&type);
            *ppOut = (T*)(it == map->last || interfaceType < it->type ? map->last : it)->component;
            return true;
        }
        return false;
    }
};
__forceinline bool IS_THING_ALIVE(const CBaseIntelligentPointer& p)
{
    if (p.GetPItem() != 0) {
        if (!(static_cast<CThing*>(p.GetPItem())->m_flags91 & 1))
            return true;
    }
    return false;
}
class CThingAICreature : public CThing {
public:
    unsigned char _pad2[0x170 - sizeof(CThing)];
    CBuildingPointer WorkBuilding;              // +0x170
};
class CTCTavernGame { public:                          // interface 0xcf
    unsigned char _pad[0x20];
    bool GameInProgress;                        // +0x20
    void RegisterGameMaster(CThingAICreature* gameMaster);   // 0x0094D430 (Ego_r.pdb ?RegisterGameMaster@CTCTavernGame@@QAEXPAVCThingAICreature@@@Z)
};
struct CTCGameMaster {                          // interface 0xd2
    unsigned char _pad[0x0c];
    CTCTavernGame* PTavernGame;                 // +0x0c
};
class CTCCreatureModeManager { public:                 // interface 0x31
    bool AddMode(unsigned long mode);           // 0x0073AE10 (Ego_r.pdb ?AddMode@CTCCreatureModeManager@@QAE_NK@Z)
    void RemoveMode(unsigned long mode);        // 0x0073ABA0 (Ego_r.pdb ?RemoveMode@CTCCreatureModeManager@@QAEXK@Z)
};
class CTCFollowed { public:                            // interface 0xbc
    bool IsFollower(const CThing* thing) const; // 0x006D4D70 (Ego_r.pdb ?IsFollower@CTCFollowed@@QBE_NPBVCThing@@@Z; landed binding)
};
class CTCRegionFollower { public:                      // interface 0x7a
    bool IsRegionFollower(const CThing* thing) const;   // 0x006AEB60 (Ego_r.pdb ?IsRegionFollower@CTCRegionFollower@@QBE_NPBVCThing@@@Z)
};
class CTCVillage;
class CTCVillageMember { public:                       // interface 0x23
    CTCVillage* GetPVillage() const;            // 0x007EE540 (Ego_r.pdb ?GetPVillage@CTCVillageMember@@QBEPAVCTCVillage@@XZ)
};
struct CTCHeroControl {                         // interface 0x08
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04();
    virtual void v05(); virtual void v06(); virtual void v07(); virtual void v08(); virtual void v09();
    virtual void v10(); virtual void v11(); virtual void v12(); virtual void v13(); virtual void v14();
    virtual void v15(); virtual void v16(); virtual void v17(); virtual void v18(); virtual void v19();
    virtual void v20(); virtual void v21(); virtual void v22(); virtual void v23(); virtual void v24();
    virtual void v25(); virtual void v26(); virtual void v27(); virtual void v28(); virtual void v29();
    virtual void v30(); virtual void v31(); virtual void v32();
    virtual CThing* GetInteractionTarget();    // slot 33 (+0x84)
};
class CThingPlayerCreature : public CThing { };
class CPlayer {
public:
    CThingPlayerCreature* GetPControlledCreature() const;   // 0x00487DC0 (Ego_r.pdb; 65 landed bindings)
};
class CPlayerManager {
public:
    CPlayer& GetMainPlayer();                   // 0x00449970 (Ego_r.pdb ?GetMainPlayer@CPlayerManager@@QAEAAVCPlayer@@XZ)
};
class CAIBrain { public:
    unsigned char _pad[0x18];
    CThingAICreature* PCreature;                // +0x18
};
class CWorld {
public:
    static long __fastcall GetFrame();          // 0x0049D870 (Ego_r.pdb ?GetFrame@CWorld@@SIJXZ)
};
enum EAIActionReturn { AI_ACTION_DONE };

class CAIStateGroupBase { public:
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04();
    virtual void v05(); virtual void v06(); virtual void v07(); virtual void v08(); virtual void v09();
    virtual void v10(); virtual void v11(); virtual void v12(); virtual void v13();
protected:
    virtual bool GetSeat();                     // slot 14 (+0x38)
    virtual bool ChooseAction();                // slot 15 (+0x3c)
public:
    CAIBrain* PBrain;                           // +0x04
    long ScriptAIPriority;                      // +0x08
    void* Name_PStringData;                     // +0x0c
    unsigned char Terminated;                   // +0x10
    unsigned char Priority;                     // +0x11
    unsigned char Enabled;                      // +0x12
    unsigned char _pad_0x13;
    __forceinline CThingAICreature* GetCreature() const { return PBrain->PCreature; }
    void Terminate();                           // 0x008FCF40
protected:
    CPlayerManager& GetPlayerManager() const;   // 0x008FCF90 (Ego_r.pdb ?GetPlayerManager@CAIStateGroupBase@@IBEAAVCPlayerManager@@XZ)
    EAIActionReturn AIAction_DoNothing();       // 0x0097E200 (Ego_r.pdb ?AIAction_DoNothing@CAIStateGroupBase@@IAE?AW4EAIActionReturn@@XZ)
public:
};
struct CAIStateGroup_GoToPub : CAIStateGroupBase {
    CBuildingPointer PPub;                      // +0x14
    CBaseIntelligentPointer PBar;               // +0x1c
    CIntelligentPointer<CThingObject> PTable;   // +0x24
    unsigned char _pad2c[0x64 - 0x2c];
    long ActionPointIndex;                      // +0x64
    long NextFramePub;                          // +0x68
    long FrameLeavePub;                         // +0x6c
    bool Sitting;                               // +0x70
    unsigned char _pad71[0xa0 - 0x71];
    bool HandsOffMode;                          // +0xa0
    bool UnderScriptControl;                    // +0xa1
    unsigned char _pada2[2];
    long TimeBeforeAskingForBeer;               // +0xa4
protected:
    bool WalktoSeatAndSit(bool run);            // 0x00949580 (Ego_r.pdb ?WalktoSeatAndSit@CAIStateGroup_GoToPub@@IAE_N_N@Z)
public:
};
struct CAIStateGroup_GameMaster : CAIStateGroup_GoToPub {
    long TimeNextReactToPlayer;                 // +0xa8
    bool WasAllowedToFollowPlayer;              // +0xac
protected:
    bool OfferPlayerAGame();                    // 0x0094CE60 (Ego_r.pdb ?OfferPlayerAGame@CAIStateGroup_GameMaster@@IAE_NXZ)
    virtual void Update();                      // Ego_r.pdb ?Update@CAIStateGroup_GameMaster@@MAEXXZ
};

void CAIStateGroup_GameMaster::Update()
{
    PPub = CBuildingPointer(GetCreature()->WorkBuilding.GetPItem());
    if (!IS_THING_ALIVE(static_cast<const CBaseIntelligentPointer&>(PPub))) {
        Terminate();
        return;
    }
    if (ActionPointIndex == -1 && !GetSeat()) {
        AIAction_DoNothing();
        Terminate();
        return;
    }
    if (!Sitting) {
        if (!WalktoSeatAndSit(false)) {
            Terminate();
            return;
        }
        if (!IS_THING_ALIVE<CIntelligentPointer<CThingObject> >(PTable)) {
            Terminate();
            return;
        }
        CTCTavernGame* game;
        CThing* table = static_cast<CThing*>(PTable.GetPItem());
        if (table->HasTC(0xcf))
            game = table->GetTC<CTCTavernGame>(0xcf);
        game->RegisterGameMaster(GetCreature());
        CTCGameMaster* master;
        if (GetCreature()->HasTC(0xd2))
            master = GetCreature()->GetTC<CTCGameMaster>(0xd2);
        master->PTavernGame = game;
    }
    CTCVillageMember* member;
    if (!GetCreature()->GetInterface(&member, 0x23)) {
        Terminate();
        return;
    }
    if (!member->GetPVillage()) {
        Terminate();
        return;
    }
    bool result;
    do {
        CThing* hero = GetPlayerManager().GetMainPlayer().GetPControlledCreature();
        if (hero) {
            if (hero->HasTC(0xbc)) {
                CTCFollowed* followed = hero->GetTCConst<CTCFollowed>(0xbc);
                if (followed->IsFollower(GetCreature()))
                    break;
            }
            if (hero->HasTC(0x7a)) {
                CTCRegionFollower* regionFollower = hero->GetTCConst<CTCRegionFollower>(0x7a);
                if (regionFollower->IsRegionFollower(GetCreature()))
                    break;
            }
        }
        CTCTavernGame* game = 0;
        if (!IS_THING_ALIVE(static_cast<const CBaseIntelligentPointer&>(PTable)))
            break;
        CThing* table = static_cast<CThing*>(PTable.GetPItem());
        if (table->HasTC(0xcf) && (game = table->GetTC<CTCTavernGame>(0xcf)) != 0 && game->GameInProgress) {
            if (!HandsOffMode) {
                HandsOffMode = true;
                CTCCreatureModeManager* modes;
                if (GetCreature()->HasTC(0x31))
                    modes = GetCreature()->GetTC<CTCCreatureModeManager>(0x31);
                modes->AddMode(0x2d);
            }
        } else if (HandsOffMode) {
            HandsOffMode = false;
            CTCCreatureModeManager* modes;
            if (GetCreature()->HasTC(0x31))
                modes = GetCreature()->GetTC<CTCCreatureModeManager>(0x31);
            modes->RemoveMode(0x2d);
        }
        if (!game->GameInProgress && CWorld::GetFrame() > TimeNextReactToPlayer) {
            CThing* player = GetPlayerManager().GetMainPlayer().GetPControlledCreature();
            if (!player)
                break;
            CTCHeroControl* control;
            if (player->HasTC(8))
                control = player->GetTC<CTCHeroControl>(8);
            CThing* target = control->GetInteractionTarget();
            if (target == GetCreature()) {
                result = OfferPlayerAGame();
                continue;
            }
        }
        result = ChooseAction();
    } while (result);
    Terminate();
}