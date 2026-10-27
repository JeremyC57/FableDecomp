// NScript::CV_GhostGrannyNecklaceScript::Init() -- clear the ghost granny necklace quest's flags
// Leaf: ?Init@CV_GhostGrannyNecklaceScript@NScript@@UAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00E8B880, 27 bytes, thiscall, plain `ret` (virtual, level-script vtable slot 3).
// Identity (proven): slot 3 (Init) of the RTTI-less level-script vtable 0x012EBA3C, which the landed NScript::CV_GhostGrannyNecklaceScript::CV_GhostGrannyNecklaceScript 0x00E8B850 (and the landed Alloc 0x00E906F0) stores;
//   level-script slots are 0 dtor, 1 RegisterMain, 2 Main, 3 Init, 4 OnPersist (FableWin.pdb CScriptBase order, the landed
//   level Init / OnPersist rows). Ego_r.pdb has ?Init@CV_GhostGrannyNecklaceScript@NScript@@UAEXXZ.
// Layout: Ego_r.pdb CV_GhostGrannyNecklaceScript members, each at Ego_r offset - 0x14 (retail CScriptBase is 0x40).
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
class CV_GhostGrannyNecklaceScript : public CLevelScriptBase<CQ_SunnyvaleMasterData> {
public:
    bool WifeNeededForCutscene;                         // +0x48
    bool HeroKnowsAboutNecklace;                        // +0x49
    bool NecklaceReturned;                              // +0x4a
    bool BanditKilled;                                  // +0x4b
    bool WarnedGettingCold;                             // +0x4c
    bool GuidedToBanditLocation;                        // +0x4d
    bool GhostToRest;                                   // +0x4e
    bool QuestCardGiven;                                // +0x4f
    virtual void Init();
};

void CV_GhostGrannyNecklaceScript::Init()
{
    WifeNeededForCutscene = false;
    HeroKnowsAboutNecklace = false;
    NecklaceReturned = false;
    BanditKilled = false;
    WarnedGettingCold = false;
    GuidedToBanditLocation = false;
    GhostToRest = false;
    QuestCardGiven = false;
}
}