// NScript::CV_FishingCompetitionScript::OnPersist(CPersistContext& context) -- save/load the fishing competition: the winner's name, the two biggest-fish sizes, the intro / prize / rod-hint state, and the competition, rod-of-champions, bounty-hunt, organiser and attack flags, each under its member name
// Leaf: ?OnPersist@CV_FishingCompetitionScript@NScript@@UAEXAAVCPersistContext@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00E7E210, 337 bytes, thiscall, `ret 4`.
// Identity (proven): the row is referenced once, from slot 4 (OnPersist) of the RTTI-less level-script vtable 0x012EA478
//   (0x012EA488), which the landed NScript::CV_FishingCompetitionScript::CV_FishingCompetitionScript 0x00E7E110 and Alloc
//   0x00E81EB0 store (slot 1 the landed RegisterMain 0x00E7E150, slot 2 Main 0x00E7E370); Ego_r.pdb
//   ?OnPersist@CV_FishingCompetitionScript@NScript@@UAEXAAVCPersistContext@@@Z. Every key string is the Ego_r.pdb member at
//   that retail offset + 0x14. Catalog label sub_00e7e210.
// Callees: CPersistContext::GetDefaultVal<CCharString> 0x0040E1E0 (Ego_r ??$GetDefaultVal@VCCharString@@@CPersistContext@@KI?AVCCharString@@XZ,
//   the two-argument Transfer inline), Transfer<CCharString> 0x004109A0, ~CCharString 0x0099EAE0, Transfer<float>
//   0x00410620, Transfer<bool> 0x004045C0, Transfer<long> 0x00410BE0 (Ego_r ??$Transfer@<T>@CPersistContext@@QAEXPBDAA<T>AB<T>@Z).
// Strings (paired by hand): "Winner" 0x012EA54C, "BiggestFish" 0x0124F7DC, "BiggestCompetitorFish" 0x012EA534, "DoneIntro"
//   0x012E3CF4, "PrizeNumber" 0x012EA528, "RodHintGiven" 0x012EA518, "CompetitionWon" 0x012EA508, "RodOfChampsGiven"
//   0x012EA4F4, "BountyHuntActive" 0x012EA4E0, "OrganiserRanAway" 0x012EA4CC, "OrganiserKilled" 0x012EA4BC,
//   "CompetitorsAttacked" 0x012EA4A8.
// Layout: the landed ctor 0x00E7E110 (Ego_r.pdb - 0x14).
// Shape: the landed CV_ArcheryCompetitionScript::OnPersist 0x00E37520 / CM_AssassinTrigger::OnPersist 0x00E38D80 models:
//   one Transfer per member in retail's order; the CCharString default from GetDefaultVal, every other default a temporary
//   in the dead `context` slot (0.0f as an immediate, then ebx = 0).
class CCharString {
public:
    void* PData;
    ~CCharString();                                     // 0x0099EAE0
};
class CPersistContext {
protected:
    template <class T> static T __fastcall GetDefaultVal();   // the CCharString instance is 0x0040E1E0
public:
    template <class T> void Transfer(const char* name, T& value, const T& defaultValue);
    template <class T> void Transfer(const char* name, T& value) { Transfer(name, value, GetDefaultVal<T>()); }
};
class CScriptDataBase;
class CGameScriptInterfaceBase;
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
    CGameScriptInterfaceBase* Game;                     // +0x40
    TData*                    MasterScriptData;         // +0x44
};
namespace std {                                         // STLport 4.6 vector (12 bytes), as the landed ctor 0x00E7E110 models it
template <class _Tp> class allocator { };
template <class _Value, class _Tp, class _MaybeReboundAlloc>
class _STLP_alloc_proxy : public _MaybeReboundAlloc {
public:
    _Value _M_data;
};
template <class _Tp, class _Alloc> class _Vector_base {
public:
    _Tp* _M_start;
    _Tp* _M_finish;
    _STLP_alloc_proxy<_Tp*, _Tp, _Alloc> _M_end_of_storage;
};
template <class _Tp, class _Alloc = allocator<_Tp> > class vector : protected _Vector_base<_Tp, _Alloc> { };
}

namespace NScript {
class CQ_SunnyvaleMasterData;
class CV_FishingCompetitionScript : public CLevelScriptBase<CQ_SunnyvaleMasterData> {
public:
    bool BountyHuntActive;                              // +0x48
    bool OrganiserRanAway;                              // +0x49
    bool OrganiserKilled;                               // +0x4a
    bool CompetitionRunning;                            // +0x4b
    bool CompetitionWon;                                // +0x4c
    bool RodOfChampsGiven;                              // +0x4d
    bool CommentIRanAway;                               // +0x4e
    bool CompetitorsAttacked;                           // +0x4f
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
    std::vector<long> SizeComment;                      // +0x74
    virtual void OnPersist(CPersistContext& context);
};

void CV_FishingCompetitionScript::OnPersist(CPersistContext& context)
{
    context.Transfer("Winner", Winner);
    context.Transfer("BiggestFish", BiggestFish, 0.0f);
    context.Transfer("BiggestCompetitorFish", BiggestCompetitorFish, 0.0f);
    context.Transfer("DoneIntro", DoneIntro, false);
    context.Transfer("PrizeNumber", PrizeNumber, 0L);
    context.Transfer("RodHintGiven", RodHintGiven, false);
    context.Transfer("CompetitionWon", CompetitionWon, false);
    context.Transfer("RodOfChampsGiven", RodOfChampsGiven, false);
    context.Transfer("BountyHuntActive", BountyHuntActive, false);
    context.Transfer("OrganiserRanAway", OrganiserRanAway, false);
    context.Transfer("OrganiserKilled", OrganiserKilled, false);
    context.Transfer("CompetitorsAttacked", CompetitorsAttacked, false);
}
}   // namespace NScript