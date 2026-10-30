// NScript::CV_FishingCompetitionScript::Alloc(CScriptDataBase* data, CGameScriptInterface& game) -- script factory: `new CV_FishingCompetitionScript(data, game)` with the constructor inlined (registered at 0x00CD88CB as "S_VFC" / "V_FishingCompetition")
// Leaf: ?Alloc@CV_FishingCompetitionScript@NScript@@SIPAVCScriptBase@@PAVCScriptDataBase@@AAVCGameScriptInterface@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00E81EB0, 86 bytes, static __fastcall (data in ecx, game in edx). PDB:
//   ?Alloc@CV_FishingCompetitionScript@NScript@@SIPAVCScriptBase@@PAVCScriptDataBase@@AAVCGameScriptInterfaceBase@@@Z.
// Callees: operator new 0x00BFEA1A (0x80 bytes), CScriptBase::CScriptBase 0x00CB8110, CCharString::CCharString() 0x0099E4B0. Vtable 0x012EA478.
// Inline constructor: the Winner string (+0x68) and the empty SizeComment vector (+0x74..+0x7C).
// Layout: PDB (Ego_r) offsets minus 0x14 (the retail script base is 0x14 bytes smaller), STLport vectors 12 bytes (PDB 16).
// Model: the landed quest-script Alloc (NScript::CQ_HeroSoulsArenaScript::Alloc 0x00D80BA0): CLevelScriptBase<T> is the
//   `__declspec(novtable)` argument-carrying base (Game +0x40, MasterScriptData +0x44, stored before the class's own vtable
//   store), the CGameScriptInterface model is the landed 88-slot one (RegisterTimer = slot 87, +0x15C).
// CGameScriptInterface is the retail game-script interface from the shared headers (FABLE_RETAIL_NSCRIPT opt-in:
//   CGameScriptInterface : CGameScriptInterfaceBase : CBaseClassNonCopyable : CBaseClass, vtable 0x01260F0C, 918 slots).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CGameScriptInterface.h"
extern CGameScriptInterface* g_pGameScriptInterface;    // 0x0143E8F8
class CScriptDataBase;
class CScriptBase { public:                                    // 0x40 bytes, the quest-script root
    virtual ~CScriptBase();
    unsigned char _body[0x3c];                          // +0x04..+0x3f
    CScriptBase();                                      // 0x00CB8110
};
template <class TData>
struct __declspec(novtable) CLevelScriptBase : CScriptBase {
    CGameScriptInterface* Game;                         // +0x40
    TData*                MasterScriptData;             // +0x44
    CLevelScriptBase(CScriptDataBase* data, CGameScriptInterface& game) : CScriptBase(), Game(&game), MasterScriptData((TData*)data) { }
};
class CCharString { public: void* PStringData; CCharString(); };   // default ctor 0x0099E4B0
template <class T> struct allocator {                  // STLport allocator<T>: empty
    allocator() { }
};
template <class T> struct _STLP_alloc_proxy : allocator<T> {
    T* _M_data;
    _STLP_alloc_proxy(const allocator<T>& a, T* p) : allocator<T>(a), _M_data(p) { }
};
template <class T> struct _Vector_base {
    T*                    _M_start;
    T*                    _M_finish;
    _STLP_alloc_proxy<T>  _M_end_of_storage;
    _Vector_base(const allocator<T>& a) : _M_start(0), _M_finish(0), _M_end_of_storage(a, 0) { }
};
template <class T> struct vector : _Vector_base<T> {   // STLport vector<T>, 12 bytes in retail
    vector() : _Vector_base<T>(allocator<T>()) { }
};
namespace NScript {
class CQ_SunnyvaleMasterData;
class CV_FishingCompetitionScript : public CLevelScriptBase<CQ_SunnyvaleMasterData> {
public:
    bool BountyHuntActive;                              // +0x48
    bool OrganiserRanAway;
    bool OrganiserKilled;
    bool CompetitionRunning;
    bool CompetitionWon;
    bool RodOfChampsGiven;
    bool CommentIRanAway;
    bool CompetitorsAttacked;
    bool CompetitorsJustGotAttacked;                    // +0x50
    long AttackCounter;                                 // +0x54
    long AttackDayCount;                                // +0x58
    long HitCount;                                      // +0x5c
    bool DoneIntro;                                     // +0x60
    bool RodHintGiven;                                  // +0x61
    long PrizeNumber;                                   // +0x64
    CCharString Winner;                                 // +0x68
    float BiggestFish;                                  // +0x6c
    float BiggestCompetitorFish;                        // +0x70
    vector<long> SizeComment;                           // +0x74
    CV_FishingCompetitionScript(CScriptDataBase* data, CGameScriptInterface& game) : CLevelScriptBase<CQ_SunnyvaleMasterData>(data, game) { }
    virtual ~CV_FishingCompetitionScript();
    static CScriptBase* __fastcall Alloc(CScriptDataBase* data, CGameScriptInterface& game);
};
CScriptBase* __fastcall CV_FishingCompetitionScript::Alloc(CScriptDataBase* data, CGameScriptInterface& game)
{
    return new CV_FishingCompetitionScript(data, game);
}
}