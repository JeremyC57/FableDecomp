// Grade: DIFFER(705v785) -- the body matches through the post-loop terminate test; retail destroys the boast vector differently on each exit (loop-top return: out-of-line pair scalar-deleting dtor 0x004DA68A + base 0x004D9C5D; the two boast-arm returns: inline elements + out-of-line base; the rest fully inline) and in-body inline_depth pragmas did not move it
// NScript::CQ_BountyHuntScript::WatchForBoasts() -- each frame read the boasts made this frame: the time-limit boast (15) arms the time-limit boast and spawns WatchForQuestInfoDisplayConditions, the decapitation boast (16) arms that boast; once the time-limit boast's timer has run out, flag the master data
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00D17A80, length 785 bytes, plain `ret`.
// Callees: CCharString() 0x0099E4B0 / operator= 0x0099EFB0 / (const char*, long) 0x0099EBF0 / ~CCharString 0x0099EAE0;
// IsActiveThreadTerminating 0x00CB7940; the boast vector ctor 0x0074F680 / pair dtor 0x004DA68A / base dealloc 0x004D9C5D;
// operator new 0x00BFEA1A, CSpawnedFuncBase 0x00CDD450 (vtable 0x012C9EDC, func 0x00D17DA0
// WatchForQuestInfoDisplayConditions), AddSpawnedFunction 0x00CB7E50, free 0x00BFEA14; CGameScriptInterface slots 7, 34
// MsgOnBoastsMade, 90 GetTimer (g_pGameScriptInterface 0x0143E8F8 for the CTimer).
// Layout: Ego_r.pdb - 0x14: BoastActiveTimeLimit +0x78, BoastActiveDecapitation +0x79, BoastTime (CTimer) +0x80; the
// master data's flag +0xda (descriptive).
#include <stdlib.h>
struct CCharString {
    void* PStringData;
    CCharString();                                     // 0x0099E4B0
    CCharString(const char* text, long length = -1);   // 0x0099EBF0
    ~CCharString();                                    // 0x0099EAE0
    CCharString& operator=(const CCharString& other);  // 0x0099EFB0
};
struct SBoast {                                        // std::pair<long, CCharString>
    long        first;
    CCharString second;
};
struct CBoastAllocator { CBoastAllocator() {} };
inline void _Destroy(SBoast* first, SBoast* last)
{
    for (; first != last; ++first)
        first->~SBoast();
}
struct BoastVector {                                   // STLport vector<pair<long, CCharString> > (12 bytes)
    SBoast* _M_start;
    SBoast* _M_finish;
    SBoast* _M_end_of_storage;
    BoastVector(const CBoastAllocator& a = CBoastAllocator());   // 0x0074F680
    ~BoastVector()
    {
        _Destroy(_M_start, _M_finish);
        if (_M_start)
            free(_M_start);
    }
    unsigned long size() const { return (unsigned long)(_M_finish - _M_start); }
    SBoast& operator[](unsigned long n) { return *(_M_start + n); }
};
struct CGameScriptInterface {                      // retail vtable 0x01260F0C (ghidra_out/cgsi_slots_aligned.tsv)
    virtual void v000() const; virtual void v001() const; virtual void v002() const; virtual void v003() const; virtual void v004() const; virtual void v005() const; virtual void v006() const;
    virtual void NewScriptFrame() const;   // slot 7 (+0x1c) 0x006E7410 public: virtual void __thiscall CGameScriptInterface::NewScriptFrame(void) const
    virtual void v008() const; virtual void v009() const; virtual void v010() const; virtual void v011() const; virtual void v012() const; virtual void v013() const; virtual void v014() const; virtual void v015() const; virtual void v016() const; virtual void v017() const;
    virtual void v018() const; virtual void v019() const; virtual void v020() const; virtual void v021() const; virtual void v022() const; virtual void v023() const; virtual void v024() const; virtual void v025() const; virtual void v026() const; virtual void v027() const;
    virtual void v028() const; virtual void v029() const; virtual void v030() const; virtual void v031() const; virtual void v032() const; virtual void v033() const;
    virtual bool MsgOnBoastsMade(BoastVector& boasts) const;   // slot 34 (+0x88) 0x008A9AE0 public: virtual bool __thiscall CGameScriptInterface::MsgOnBoastsMade(class std::vector<struct std::pair<long, class CCharString>, class std::allocator<struct std::pair<long, class CCharString>>> &) const
    virtual void v035() const; virtual void v036() const; virtual void v037() const; virtual void v038() const; virtual void v039() const; virtual void v040() const; virtual void v041() const; virtual void v042() const; virtual void v043() const; virtual void v044() const;
    virtual void v045() const; virtual void v046() const; virtual void v047() const; virtual void v048() const; virtual void v049() const; virtual void v050() const; virtual void v051() const; virtual void v052() const; virtual void v053() const; virtual void v054() const;
    virtual void v055() const; virtual void v056() const; virtual void v057() const; virtual void v058() const; virtual void v059() const; virtual void v060() const; virtual void v061() const; virtual void v062() const; virtual void v063() const; virtual void v064() const;
    virtual void v065() const; virtual void v066() const; virtual void v067() const; virtual void v068() const; virtual void v069() const; virtual void v070() const; virtual void v071() const; virtual void v072() const; virtual void v073() const; virtual void v074() const;
    virtual void v075() const; virtual void v076() const; virtual void v077() const; virtual void v078() const; virtual void v079() const; virtual void v080() const; virtual void v081() const; virtual void v082() const; virtual void v083() const; virtual void v084() const;
    virtual void v085() const; virtual void v086() const; virtual void v087() const; virtual void v088() const; virtual void v089() const;
    virtual long GetTimer(long timer) const;   // slot 90 (+0x168) 0x006E7710 public: virtual long __thiscall CGameScriptInterface::GetTimer(long) const
};

extern CGameScriptInterface* g_pGameScriptInterface;    // 0x0143E8F8
struct CTimer {
    long TimerIndex;
    long GetTime() const { return g_pGameScriptInterface->GetTimer(TimerIndex); }
};
struct CSpawnedFuncBase {                              // retail 0x34 bytes
    virtual ~CSpawnedFuncBase();
    virtual void RunSuspendableProcess();
    virtual void SuspendSuspendableProcess(bool suspend);
    virtual void Restart();
    virtual void SuspendableProcess() = 0;
    unsigned char _body[0x30];
    CSpawnedFuncBase(const CCharString& name, unsigned long id);   // 0x00CDD450
};
template <class T> struct CSpawnedFunc : CSpawnedFuncBase {
    void (T::*Func)();                                 // +0x34
    T* PClass;                                         // +0x38
    __forceinline CSpawnedFunc(const CCharString& name, unsigned long id, T* pclass, void (T::*func)())
        : CSpawnedFuncBase(name, id), Func(func), PClass(pclass) {}
    virtual void SuspendableProcess() { (PClass->*Func)(); }
};
struct CScriptBase {                                   // retail 0x40 bytes
    unsigned char _body[0x40];
    bool IsActiveThreadTerminating();                  // 0x00CB7940
    void AddSpawnedFunction(CSpawnedFuncBase* func, const CCharString& regionName);   // 0x00CB7E50
    __forceinline void SpawnFunction(CSpawnedFuncBase* func, const char* regionName = "") { AddSpawnedFunction(func, CCharString(regionName)); }
};
struct CQ_BountyHuntMasterData {
    unsigned char _pad[0xda];
    bool          BoastTimeLimitExpired;               // +0xda (descriptive)
};
namespace NScript {
struct CQ_BountyHuntScript : CScriptBase {
    CGameScriptInterface*    Game;                     // +0x40
    CQ_BountyHuntMasterData* Data;                     // +0x44
    unsigned char _pad048[0x78 - 0x48];
    bool   BoastActiveTimeLimit;                       // +0x78
    bool   BoastActiveDecapitation;                    // +0x79
    long   BoastTimerID;                               // +0x7c
    CTimer BoastTime;                                  // +0x80
    void WatchForQuestInfoDisplayConditions();         // 0x00D17DA0
    void WatchForBoasts();
};

void CQ_BountyHuntScript::WatchForBoasts()
{
    CCharString boastName;
    while (!IsActiveThreadTerminating()) {
        BoastVector boasts;
        Game->MsgOnBoastsMade(boasts);
        for (unsigned long i = 0; i != boasts.size(); ++i) {
            if (IsActiveThreadTerminating())
                return;
            long boast = boasts[i].first;
            boastName = boasts[i].second;
            if (boast == 15) {
                if (!BoastActiveTimeLimit) {
                    if (IsActiveThreadTerminating())
                        return;
                    BoastActiveTimeLimit = true;
                    SpawnFunction(new CSpawnedFunc<CQ_BountyHuntScript>("WatchForQuestInfoDisplayConditions", 0, this, &CQ_BountyHuntScript::WatchForQuestInfoDisplayConditions));
                }
            } else if (boast == 16) {
                if (!BoastActiveDecapitation) {
                    if (IsActiveThreadTerminating())
                        return;
                    BoastActiveDecapitation = true;
                }
            }
            if (BoastActiveTimeLimit && BoastTime.GetTime() <= 0 && !Data->BoastTimeLimitExpired) {
                if (IsActiveThreadTerminating())
                    return;
                Data->BoastTimeLimitExpired = true;
            }
        }
        if (IsActiveThreadTerminating())
            return;
        Game->NewScriptFrame();
    }
}
}
