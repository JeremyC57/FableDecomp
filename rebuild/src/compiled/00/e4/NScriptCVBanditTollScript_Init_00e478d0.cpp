// NScript::CV_BanditTollScript::Init() -- clear the toll quest's flags and take the toll fee from the global script def
// Leaf: ?Init@CV_BanditTollScript@NScript@@UAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00E478D0, 51 bytes, thiscall, plain `ret` (virtual, level-script vtable slot 3).
// Identity (proven): slot 3 (Init) of the RTTI-less level-script vtable 0x012E4B7C, which the landed NScript::CV_BanditTollScript::CV_BanditTollScript 0x00E478A0 (and the landed Alloc 0x00E4F550) stores;
//   level-script slots are 0 dtor, 1 RegisterMain, 2 Main, 3 Init, 4 OnPersist (FableWin.pdb CScriptBase order, the landed
//   level Init / OnPersist rows). Ego_r.pdb has ?Init@CV_BanditTollScript@NScript@@UAEXXZ.
// Layout: Ego_r.pdb CV_BanditTollScript members, each at Ego_r offset - 0x14 (retail CScriptBase is 0x40).
// Data: pGlobalScriptDef 0x0143E90C, CScriptDef::TollBanditFee (Ego_r +0x464, retail +0x430: 13 Dinkumware vectors
//   (0x10) before it are 0xC in retail).
class CGameScriptInterface;
class CScriptDef {                                      // Ego_r.pdb CScriptDef; retail offsets (STLport vectors 0xC)
public:
    unsigned char _pad0[0x430];
    long TollBanditFee;                                 // +0x430 (Ego_r +0x464)
};
extern CScriptDef* pGlobalScriptDef;                    // 0x0143E90C (Ego_r.pdb `pGlobalScriptDef`)
class CPersistContext;
// CScriptBase / CLevelScriptBase<T>: the retail NScript model from the shared headers (FABLE_RETAIL_NSCRIPT: CScriptBase :
//   CBaseClassNonCopyable : CBaseClass, vtable 0x012C1648, 5 slots, 0x40; CLevelScriptBase<T> Game +0x40, MasterScriptData +0x44).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CLevelScriptBase.h"
class CGameScriptInterfaceBase;

namespace NScript {
class CQ_SunnyvaleMasterData;
class CV_BanditTollScript : public CLevelScriptBase<CQ_SunnyvaleMasterData> {
public:
    bool BanditsAngered;                                // +0x48
    bool TollPaid;                                      // +0x49
    bool AssassinateReluctantTrader;                    // +0x4a
    bool TollBanditIsAfterHero;                         // +0x4b
    bool TraderDead;                                    // +0x4c
    bool SafeTraderAppeared;                            // +0x4d
    bool LeaderDead;                                    // +0x4e
    bool BanditsFlee;                                   // +0x4f
    bool BanditsReachedMarker;                          // +0x50
    bool IsFactionSet;                                  // +0x51
    long BanditTollFee;                                 // +0x54
    bool GivenQuestCard;                                // +0x58
    virtual void Init();
};

void CV_BanditTollScript::Init()
{
    BanditsAngered = false;
    TollPaid = false;
    AssassinateReluctantTrader = false;
    TollBanditIsAfterHero = false;
    TraderDead = false;
    SafeTraderAppeared = false;
    LeaderDead = false;
    BanditsFlee = false;
    BanditsReachedMarker = false;
    IsFactionSet = false;
    BanditTollFee = pGlobalScriptDef->TollBanditFee;
    GivenQuestCard = false;
}
}