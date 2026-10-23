// Grade: DIFFER(1043v1035) -- retail calls control->GetInteractionTarget() before loading GetCreature() (cmp eax,[ecx+X]); ours hoists the creature load above the virtual call (+8 bytes); plus mov eax,[eax+4];mov ecx,eax (inlined lower_bound eax copy lane gap) before GetPVillage
// CAIStateGroup_GameMaster::Update() -- adopt the work building as the tavern; when not yet seated walk to a seat, attach to the table's game; then, while the hero is not following, keep the TABLE_GAME mode in step with the game state and either offer the hero a game or run the base behaviour; terminate when anything fails
// Retail: Fable.exe (TLC, VC7.1) @ 0x0094D020, 1040 bytes, plain ret.
// Callees: CBaseIntelligentPointer ctor 0x00A01B10 / GetPItem 0x00A01B50 / SetPItem 0x00A01B90 / dtor 0x00A01C10
//   (CIntelligentPointer<CThingBuilding> vtable 0x01244344), AIAction_DoNothing 0x0097E200, Terminate 0x008FCF40,
//   CAIStateGroup_GoToPub::WalktoSeatAndSit 0x00949580, IS_THING_ALIVE<CIntelligentPointer<T>> 0x0078B2A0 (__fastcall),
//   CVectorMap::LowerBound 0x0040F020 / const 0x004365B0 (same-TU stand-ins), CTCTavernGame::SetGameMaster 0x0094D430,
//   CTCVillageMember::GetPVillage 0x007EE540, GetPPlayerManager 0x008FCF90, CPlayerManager::GetMainPlayer 0x00449970,
//   CPlayer::GetHeroThing 0x00487DC0, CTCFollowed::IsAIFollower 0x006D4D70, CTCRegionFollower::IsRegionFollower 0x006AEB60,
//   CTCCreatureModeManager::AddMode 0x0073AE10 / RemoveMode 0x0073ABA0, GFGetCurrentFrame 0x0049D870,
//   OfferPlayerAGame 0x0094CE60, the group's virtual slots 14 (+0x38) and 15 (+0x3c), the hero component's slot 33 (+0x84).
// Layout: CAIStateGroup_GameMaster derives CAIStateGroup_GoToPub (Ego_r.pdb): PPub +0x14, PTable +0x24,
//   ActionPointIndex +0x64, Sitting +0x70, HandsOffMode +0xa0, TimeNextReactToPlayer +0xa8; retail CThingAICreature
//   WorkBuilding +0x170 (landed 0x0094C900). Modelled locally (flattened bases, no engine header for the group).

struct CTCBase;
struct CThing;
struct CThingBuilding;
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
    CThing* GetPItem() const;                   // 0x00A01B50
    void SetPItem(CThing* item);                // 0x00A01B90
};
struct CBuildingPointer : CBaseIntelligentPointer {          // CIntelligentPointer<CThingBuilding>, vtable 0x01244344
    virtual void v00();
    __forceinline CBuildingPointer(CThingBuilding* item) : CBaseIntelligentPointer() { SetPItem((CThing*)item); }
    __forceinline CBuildingPointer& operator=(const CBuildingPointer& other) { SetPItem(other.CBaseIntelligentPointer::GetPItem()); return *this; }
    __forceinline CThingBuilding* GetPItem() const { return (CThingBuilding*)CBaseIntelligentPointer::GetPItem(); }
};
bool __fastcall IS_THING_ALIVE_Pointer(const CBaseIntelligentPointer& p);   // 0x0078B2A0

struct CThing {
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
};
__forceinline bool IS_THING_ALIVE(const CBaseIntelligentPointer& p)
{
    if (p.GetPItem() != 0) {
        if (!(p.GetPItem()->m_flags91 & 1))
            return true;
    }
    return false;
}
struct CThingAICreature : CThing {
    unsigned char _pad2[0x170 - sizeof(CThing)];
    CBuildingPointer WorkBuilding;              // +0x170
};
struct CTCTavernGame {                          // interface 0xcf
    unsigned char _pad[0x20];
    bool GameInProgress;                        // +0x20
    void SetGameMaster(CThingAICreature* master);   // 0x0094D430
};
struct CTCGameMaster {                          // interface 0xd2
    unsigned char _pad[0x0c];
    CTCTavernGame* PTavernGame;                 // +0x0c
};
struct CTCCreatureModeManager {                 // interface 0x31
    bool AddMode(long mode);                    // 0x0073AE10
    void RemoveMode(long mode);                 // 0x0073ABA0
};
struct CTCFollowed {                            // interface 0xbc
    bool IsAIFollower(CThingAICreature* creature);   // 0x006D4D70
};
struct CTCRegionFollower {                      // interface 0x7a
    bool IsRegionFollower(CThing* thing);       // 0x006AEB60
};
struct CVillage;
struct CTCVillageMember {                       // interface 0x23
    CVillage* GetPVillage();                    // 0x007EE540
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
struct CPlayer {
    CThing* GetHeroThing();                     // 0x00487DC0
};
struct CPlayerManager {
    CPlayer* GetMainPlayer();                   // 0x00449970
};
struct CAIBrain {
    unsigned char _pad[0x18];
    CThingAICreature* PCreature;                // +0x18
};
long __fastcall GFGetCurrentFrame();            // 0x0049D870

struct CAIStateGroupBase {
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04();
    virtual void v05(); virtual void v06(); virtual void v07(); virtual void v08(); virtual void v09();
    virtual void v10(); virtual void v11(); virtual void v12(); virtual void v13();
    virtual bool v14();                         // +0x38
    virtual bool v15();                         // +0x3c
    CAIBrain* PBrain;                           // +0x04
    long ScriptAIPriority;                      // +0x08
    void* Name_PStringData;                     // +0x0c
    unsigned char Terminated;                   // +0x10
    unsigned char Priority;                     // +0x11
    unsigned char Enabled;                      // +0x12
    unsigned char _pad_0x13;
    __forceinline CThingAICreature* GetCreature() const { return PBrain->PCreature; }
    CPlayerManager* GetPPlayerManager();        // 0x008FCF90
    void Terminate();                           // 0x008FCF40
    long AIAction_DoNothing();                  // 0x0097E200
};
struct CAIStateGroup_GoToPub : CAIStateGroupBase {
    CBuildingPointer PPub;                      // +0x14
    CBaseIntelligentPointer PBar;               // +0x1c
    CBaseIntelligentPointer PTable;             // +0x24
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
    bool WalktoSeatAndSit(bool run);            // 0x00949580
};
struct CAIStateGroup_GameMaster : CAIStateGroup_GoToPub {
    long TimeNextReactToPlayer;                 // +0xa8
    bool WasAllowedToFollowPlayer;              // +0xac
    bool OfferPlayerAGame();                    // 0x0094CE60
    void Update();
};

void CAIStateGroup_GameMaster::Update()
{
    PPub = CBuildingPointer(GetCreature()->WorkBuilding.GetPItem());
    if (!IS_THING_ALIVE(PPub)) {
        Terminate();
        return;
    }
    if (ActionPointIndex == -1 && !v14()) {
        AIAction_DoNothing();
        Terminate();
        return;
    }
    if (!Sitting) {
        if (!WalktoSeatAndSit(false)) {
            Terminate();
            return;
        }
        if (!IS_THING_ALIVE_Pointer(PTable)) {
            Terminate();
            return;
        }
        CTCTavernGame* game;
        CThing* table = PTable.GetPItem();
        if (table->HasTC(0xcf))
            game = table->GetTC<CTCTavernGame>(0xcf);
        game->SetGameMaster(GetCreature());
        CTCGameMaster* master;
        if (GetCreature()->HasTC(0xd2))
            master = GetCreature()->GetTC<CTCGameMaster>(0xd2);
        master->PTavernGame = game;
    }
    if (!GetCreature()->HasTC(0x23)) {
        Terminate();
        return;
    }
    CTCVillageMember* member = GetCreature()->GetTCConst<CTCVillageMember>(0x23);
    if (!member->GetPVillage()) {
        Terminate();
        return;
    }
    bool result;
    do {
        CThing* hero = GetPPlayerManager()->GetMainPlayer()->GetHeroThing();
        if (hero) {
            if (hero->HasTC(0xbc)) {
                CTCFollowed* followed = hero->GetTCConst<CTCFollowed>(0xbc);
                if (followed->IsAIFollower(GetCreature()))
                    break;
            }
            if (hero->HasTC(0x7a)) {
                CTCRegionFollower* regionFollower = hero->GetTCConst<CTCRegionFollower>(0x7a);
                if (regionFollower->IsRegionFollower(GetCreature()))
                    break;
            }
        }
        CTCTavernGame* game = 0;
        if (!IS_THING_ALIVE(PTable))
            break;
        CThing* table = PTable.GetPItem();
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
        if (!game->GameInProgress && GFGetCurrentFrame() > TimeNextReactToPlayer) {
            CThing* player = GetPPlayerManager()->GetMainPlayer()->GetHeroThing();
            if (!player)
                break;
            CTCHeroControl* control;
            if (player->HasTC(8))
                control = player->GetTC<CTCHeroControl>(8);
            if (control->GetInteractionTarget() == GetCreature())
                result = OfferPlayerAGame();
            else
                result = v15();
        } else
            result = v15();
    } while (result);
    Terminate();
}
