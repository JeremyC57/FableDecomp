// NScript::CV_ChickenKickingScript::Init() -- reset the chicken-kicking game state
// Leaf: ?Init@CV_ChickenKickingScript@NScript@@UAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00E628B0, 33 bytes, thiscall, plain `ret` (virtual, level-script vtable slot 3).
// Identity (proven): slot 3 (Init) of the RTTI-less level-script vtable 0x012E7318, which the landed NScript::CV_ChickenKickingScript::CV_ChickenKickingScript 0x00E62830 stores;
//   level-script slots are 0 dtor, 1 RegisterMain, 2 Main, 3 Init, 4 OnPersist (FableWin.pdb CScriptBase order, the landed
//   level Init / OnPersist rows). Ego_r.pdb has ?Init@CV_ChickenKickingScript@NScript@@UAEXXZ.
// Layout: Ego_r.pdb CV_ChickenKickingScript members, each at Ego_r offset - 0x14 (retail CScriptBase is 0x40).
class CGameScriptInterface;
class CPersistContext;
class CScriptBase {                                     // retail 0x40 bytes (Ego_r 0x54: debug containers)
public:
    virtual ~CScriptBase();                             // slot 0
protected:
    virtual void RegisterMain();                        // slot 1
    virtual void Main();                                // slot 2
    virtual void Init();                                // slot 3
public:
    virtual void OnPersist(CPersistContext& context);   // slot 4
    unsigned char _body[0x3c];
};
template <class TData> class CLevelScriptBase : public CScriptBase {
public:
    CGameScriptInterface* Game;                         // +0x40
    TData*                MasterScriptData;             // +0x44
};

namespace NScript {
class CQ_SunnyvaleMasterData;
class CV_ChickenKickingScript : public CLevelScriptBase<CQ_SunnyvaleMasterData> {
public:
    bool          ChickenLanded;                        // +0x48
    long          FinalSector;                          // +0x4c
    long          DistanceBand;                         // +0x50
    bool          KnowGhostHasGone;                     // +0x54
    bool          SpectatorsCreated;                    // +0x55
    bool          RanOff;                               // +0x56
    bool          TalkedTo;                             // +0x57
    bool          SpectatorsUnderAttack;                // +0x58
    unsigned long PrizesWon;                            // +0x5c
    bool          InfoDisplayed;                        // +0x60
    virtual void Init();
};

void CV_ChickenKickingScript::Init()
{
    ChickenLanded = false;
    FinalSector = 0;
    DistanceBand = 0;
    KnowGhostHasGone = false;
    SpectatorsCreated = false;
    RanOff = false;
    TalkedTo = false;
    SpectatorsUnderAttack = false;
    PrizesWon = 0;
    InfoDisplayed = false;
}
}