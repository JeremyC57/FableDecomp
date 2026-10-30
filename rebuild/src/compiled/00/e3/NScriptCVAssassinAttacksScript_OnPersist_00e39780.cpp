// NScript::CV_AssassinAttacksScript::OnPersist(CPersistContext& context) -- persist the assassin-attacks quest state: object count, flags, the region of the latest kill and the per-region kill counts
// Leaf: ?OnPersist@CV_AssassinAttacksScript@NScript@@UAEXAAVCPersistContext@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00E39780, 207 bytes, thiscall, `ret 4`.
// Identity (proven): slot 4 (OnPersist) of the RTTI-less level-script vtable 0x012E35B0, which the constructor 0x00E376D0
//   stores (manifest label CInputProcessManager::CInputProcessManager is wrong); its Alloc 0x00E39850 news 0xB8 bytes
//   (Ego_r.pdb CV_AssassinAttacksScript 0xD0 - 0x14 - 4 for the vector) and is registered as "S_VAA" (0x00CD80F9). The
//   keys are Ego_r.pdb CV_AssassinAttacksScript's member names. Level-script slots: 0 dtor, 1 RegisterMain, 2 Main,
//   3 Init, 4 OnPersist. Ego_r.pdb has ?OnPersist@CV_AssassinAttacksScript@NScript@@UAEXAAVCPersistContext@@@Z.
// Layout: Ego_r.pdb CV_AssassinAttacksScript members, each at Ego_r offset - 0x14 (retail CScriptBase is 0x40).
// Callees: CPersistContext::Transfer<long> 0x00410BE0, Transfer<bool> 0x004045C0, Transfer<CCharString> 0x004109A0 with
//   GetDefaultVal<CCharString> 0x0040E1E0 and ~CCharString 0x0099EAE0, TransferVector<long> 0x005815D6 (Ego_r.pdb
//   ??$TransferVector@J@CPersistContext@@QAEXPBDAAV?$vector@JV?$allocator@J@std@@@std@@@Z).
class CCharString {
public:
    void* PData;
    ~CCharString();                                     // 0x0099EAE0
};
namespace std {
template <class T> class allocator { };
template <class T, class A = allocator<T> > class vector {   // STLport 4.6: start, finish, end of storage (0xC)
public:
    T* _M_start;
    T* _M_finish;
    T* _M_end_of_storage;
};
}
class CPersistContext {
protected:
    template <class T> static T __fastcall GetDefaultVal();   // the CCharString instance is 0x0040E1E0
public:
    template <class T> void Transfer(const char* name, T& value, const T& defaultValue);
    template <class T> void Transfer(const char* name, T& value) { Transfer(name, value, GetDefaultVal<T>()); }
    template <class T> void TransferVector(const char* name, std::vector<T>& value);
};
// The long and bool defaults are inline (retail stores 0 into stack slots); see agent501's 0x00E38D80 for why the
// CCharString instance stays declaration-only.
template <> inline long __fastcall CPersistContext::GetDefaultVal<long>() { return 0; }
template <> inline bool __fastcall CPersistContext::GetDefaultVal<bool>() { return false; }
namespace NScript {
class CV_AssassinAttacksScript;
}
class CGameScriptInterface;
class CPersistContext;
// CScriptBase / CLevelScriptBase<T>: the retail NScript model from the shared headers (FABLE_RETAIL_NSCRIPT: CScriptBase :
//   CBaseClassNonCopyable : CBaseClass, vtable 0x012C1648, 5 slots, 0x40; CLevelScriptBase<T> Game +0x40, MasterScriptData +0x44).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CLevelScriptBase.h"
class CGameScriptInterfaceBase;

namespace NScript {
class CQ_SunnyvaleMasterData;
class CV_AssassinAttacksScript : public CLevelScriptBase<CQ_SunnyvaleMasterData> {
public:
    enum AA_LocationIdentifier { };                     // Ego_r.pdb nested enum
    class RegionData {                                  // Ego_r.pdb, 16 bytes
    public:
        CCharString name;
        long count;
        long max;
        bool assassinActive;
    };
    AA_LocationIdentifier RegionIndex;                  // +0x48
    long AssassinObjectCount;                           // +0x4c
    bool Activated;                                     // +0x50
    CCharString RegionOfMostRecentKill;                 // +0x54
    RegionData RegionInfo[5];                           // +0x58
    std::vector<long> RegionKillCounts;                 // +0xa8
    bool QuestComplete;                                 // +0xb4
    bool RewardsGiven;                                  // +0xb5
    bool GivenQuestCard;                                // +0xb6
    virtual void OnPersist(CPersistContext& context);
};

void CV_AssassinAttacksScript::OnPersist(CPersistContext& context)
{
    context.Transfer("AssassinObjectCount", AssassinObjectCount);
    context.Transfer("Activated", Activated);
    context.Transfer("QuestComplete", QuestComplete);
    context.Transfer("RegionOfMostRecentKill", RegionOfMostRecentKill);
    context.Transfer("RewardsGiven", RewardsGiven);
    context.Transfer("GivenQuestCard", GivenQuestCard);
    context.TransferVector("RegionKillCounts", RegionKillCounts);
}
}