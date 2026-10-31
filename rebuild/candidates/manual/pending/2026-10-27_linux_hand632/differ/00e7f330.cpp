// NScript::CV_FishingCompetitionScript::Init() -- reset the fishing competition: clear the flags and counters, pick one of three fisher names as the winner, randomise the best competitor catch from the second prize weight and size the three-entry SizeComment vector (all -1)
// Grade: DIFFER(218v218) -- scheduling only: retail puts `mov [esi-8],ebx` (BiggestFish) after `faddp` and the resize fill temporary store after the fstp; ours schedules both one slot earlier
// Leaf: ?Init@CV_FishingCompetitionScript@NScript@@UAEXXZ
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00E7F330, 218 bytes (224 with padding), thiscall `ret`.
// Identity: Ego_r.pdb ?Init@CV_FishingCompetitionScript@NScript@@UAEXXZ (donor 0x00EDF160, after OnPersist, before
//   CBountyHuntActivationChecker::RemoveFishermen); CScriptBase slot 3.
// Callees: rand 0x00BFEB16 (x2), GFIntToCharString(long) 0x0099F830 and operator+(const char*, const CCharString&)
//   0x0099F690 (both __fastcall, CCharString by value through the hidden pointer), CCharString::operator= 0x0099EFB0,
//   ~CCharString 0x0099EAE0 (x2), std::vector<long>::resize(size_type, const long&) 0x004428F0. Data: pGlobalScriptDef
//   0x0143E90C (the landed siblings' spelling), FishPrizeWeights (vector<float>) at retail CScriptDef +0x5d0 (landed
//   GetNextPrizeWeight 0x00E7F7D0); literal "TEXT_QST_B15_FISHER_NAME_" 0x012EA64C; 0.1f 0x0122EE58, 0.25f 0x0123078C.
// Layout (retail = Ego_r.pdb CV_FishingCompetitionScript - 0x14; engine/CScriptBase.h 0x40 + Game/Data): BountyHuntActive
//   +0x48 .. CompetitorsJustGotAttacked +0x50, AttackCounter +0x54, AttackDayCount +0x58, HitCount +0x5c, DoneIntro +0x60,
//   RodHintGiven +0x61, PrizeNumber +0x64, Winner +0x68, BiggestFish +0x6c, BiggestCompetitorFish +0x70, SizeComment +0x74.
class CCharString { public:
    void* PStringData;
    CCharString(const CCharString& other);
    ~CCharString();                                // 0x0099EAE0
    const CCharString& operator=(const CCharString& other);   // 0x0099EFB0
};
CCharString __fastcall GFIntToCharString(long value);                         // 0x0099F830
CCharString __fastcall operator+(const char* left, const CCharString& right);   // 0x0099F690
extern "C" int __cdecl rand();                     // 0x00BFEB16
namespace std {
template <class _Tp> class allocator { public: allocator() { } };
template <class _Value, class _Tp, class _MaybeReboundAlloc>
class _STLP_alloc_proxy : public _MaybeReboundAlloc { public: _Value _M_data; };
template <class _Tp, class _Alloc> class _Vector_base {
public:
    _Tp* _M_start;
    _Tp* _M_finish;
    _STLP_alloc_proxy<_Tp*, _Tp, _Alloc> _M_end_of_storage;
};
template <class _Tp, class _Alloc = allocator<_Tp> >
class vector : protected _Vector_base<_Tp, _Alloc> {
public:
    typedef unsigned int size_type;
    typedef _Tp* iterator;
    iterator begin() { return this->_M_start; }
    _Tp& operator[](size_type __n) { return *(begin() + __n); }
    void resize(size_type __new_size, const _Tp& __x);   // 0x004428F0 for vector<long>
};
}
class CScriptDef { public:
    unsigned char _pad000[0x5d0];
    std::vector<float> FishPrizeWeights;           // +0x5d0
};
extern CScriptDef* pGlobalScriptDef;               // 0x0143E90C (Ego_r.pdb `pGlobalScriptDef`, a def pointer: its first word)
#define FABLE_RETAIL_NSCRIPT
#include "engine/CScriptBase.h"
class CGameScriptInterface;
namespace NScript {
class CV_FishingCompetitionScript : public CScriptBase {
public:
    CGameScriptInterface* Game;                    // +0x40
    void* Data;                                    // +0x44
    bool BountyHuntActive;                         // +0x48
    bool OrganiserRanAway;                         // +0x49
    bool OrganiserKilled;                          // +0x4a
    bool CompetitionRunning;                       // +0x4b
    bool CompetitionWon;                           // +0x4c
    bool RodOfChampsGiven;                         // +0x4d
    bool CommentIRanAway;                          // +0x4e
    bool CompetitorsAttacked;                      // +0x4f
    bool CompetitorsJustGotAttacked;               // +0x50
    long AttackCounter;                            // +0x54
    long AttackDayCount;                           // +0x58
    long HitCount;                                 // +0x5c
    bool DoneIntro;                                // +0x60
    bool RodHintGiven;                             // +0x61
    long PrizeNumber;                              // +0x64
    CCharString Winner;                            // +0x68
    float BiggestFish;                             // +0x6c
    float BiggestCompetitorFish;                   // +0x70
    std::vector<long> SizeComment;                 // +0x74
    virtual void Init();
};
} // namespace NScript
void NScript::CV_FishingCompetitionScript::Init()
{
    CompetitorsAttacked = false;
    CompetitorsJustGotAttacked = false;
    OrganiserKilled = false;
    OrganiserRanAway = false;
    CompetitionRunning = true;
    BountyHuntActive = false;
    CompetitionWon = false;
    RodOfChampsGiven = false;
    CommentIRanAway = false;
    AttackCounter = 0;
    HitCount = 0;
    Winner = "TEXT_QST_B15_FISHER_NAME_" + GFIntToCharString(rand() % 3);
    BiggestCompetitorFish = pGlobalScriptDef->FishPrizeWeights[1] * 0.25f + (rand() % 50) * 0.1f;
    DoneIntro = false;
    RodHintGiven = false;
    BiggestFish = 0.0f;
    PrizeNumber = -1;
    SizeComment.resize(3, 0);
    SizeComment[0] = -1;
    SizeComment[1] = -1;
    SizeComment[2] = -1;
}
