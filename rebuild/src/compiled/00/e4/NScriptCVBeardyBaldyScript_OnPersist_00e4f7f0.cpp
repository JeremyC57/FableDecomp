// NScript::CV_BeardyBaldyScript::OnPersist(CPersistContext& context) -- save/load the Beardy Baldy quest: the finished / activated / complete flags, the quest phase and its requirement flags, the three required hair styles, the kill flags, the acquired-style flags, the wrong-combination count and the attack flag, each under its member name
// Leaf: ?OnPersist@CV_BeardyBaldyScript@NScript@@UAEXAAVCPersistContext@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00E4F7F0, 549 bytes, thiscall, `ret 4`.
// Identity (proven): the row is referenced once, from slot 4 (OnPersist, 0x012E57A0) of the RTTI-less level-script vtable
//   0x012E5790 [dtor 0x00E54290, RegisterMain 0x00E4F730 (landed NScript::CV_BeardyBaldyScript::RegisterMain), Main 0x00E4FA20,
//   Init 0x00E4F620 (landed CV_BeardyBaldyScript::Init), OnPersist 0x00E4F7F0], stored by the ctor at 0x00E4F5CD; Ego_r.pdb
//   ?OnPersist@CV_BeardyBaldyScript@NScript@@UAEXAAVCPersistContext@@@Z. Every key string is the Ego_r.pdb member at that
//   retail offset + 0x14. Catalog label sub_00e4f7f0.
// Callees: CPersistContext::Transfer<bool> 0x004045C0, Transfer<long> 0x00410BE0, Transfer<CCharString> 0x004109A0,
//   GetDefaultVal<CCharString> 0x0040E1E0 (the two-argument Transfer inline), ~CCharString 0x0099EAE0.
// Strings (paired by hand with the members): "QuestFinished" 0x012C6FBC (+0x48), "QuestActivated" 0x012E2778 (+0x4a),
//   "QuestComplete" 0x012E2788 (+0x49), "QuestPhase" 0x012E59EC (+0x4c), "InitialisePhase" 0x012E59DC (+0x50),
//   "IntroComplete" 0x012E59CC (+0x51), "Phase1RequirementsComplete" 0x012E59B0 (+0x52), "Phase2RequirementsComplete"
//   0x012E5994 (+0x53), "Phase3RequirementsComplete" 0x012E5978 (+0x54), "RequiredHairdo" 0x012E5968 (+0x58),
//   "RequiredBeard" 0x012E5958 (+0x5c), "RequiredTash" 0x012E5948 (+0x60), "BeardyBaldyKilled" 0x012E5934 (+0x68),
//   "BarberLadyKilled" 0x012E5920 (+0x69), "AcquiredNewHairdo" 0x012E590C (+0x64), "AcquiredNewBeard" 0x012E58F8 (+0x65),
//   "AcquiredNewTash" 0x012E58E8 (+0x66), "AllHairChanged" 0x012E58D8 (+0x67), "IncorrectHairComboCount" 0x012E58C0 (+0x6c),
//   "AttackedByHero" 0x012E58B0 (+0x70).
// Layout: retail NScript headers (FABLE_RETAIL_NSCRIPT, CScriptBase 0x40); CV_BeardyBaldyScript members at Ego_r.pdb - 0x14 (no
//   containers before them).
// Shape: the landed CV_FishingCompetitionScript::OnPersist 0x00E7E210 model: one Transfer per member in retail's order, the
//   CCharString default from GetDefaultVal, every other default a temporary.
class CCharString {
public:
    void* PStringData;
    ~CCharString();                                     // 0x0099EAE0
};
class CPersistContext {
protected:
    template <class T> static T __fastcall GetDefaultVal();   // the CCharString instance is 0x0040E1E0
public:
    template <class T> void Transfer(const char* name, T& value, const T& defaultValue);
    template <class T> void Transfer(const char* name, T& value) { Transfer(name, value, GetDefaultVal<T>()); }
};
template <class T> class CCountedPointer {
public:
    T* Data;
    void* Info;
};
#define FABLE_RETAIL_NSCRIPT
#include "engine/CTimer.h"
#include "engine/CLevelScriptBase.h"

namespace NScript {
class CQ_SunnyvaleMasterData;
class CV_BeardyBaldyScript : public CLevelScriptBase<CQ_SunnyvaleMasterData> {
public:
    bool QuestFinished;                                 // +0x48
    bool QuestComplete;                                 // +0x49
    bool QuestActivated;                                // +0x4a
    long QuestPhase;                                    // +0x4c
    bool InitialisePhase;                               // +0x50
    bool IntroComplete;                                 // +0x51
    bool Phase1RequirementsComplete;                    // +0x52
    bool Phase2RequirementsComplete;                    // +0x53
    bool Phase3RequirementsComplete;                    // +0x54
    CCharString RequiredHairdo;                         // +0x58
    CCharString RequiredBeard;                          // +0x5c
    CCharString RequiredTash;                           // +0x60
    bool AcquiredNewHairdo;                             // +0x64
    bool AcquiredNewBeard;                              // +0x65
    bool AcquiredNewTash;                               // +0x66
    bool AllHairChanged;                                // +0x67
    bool BeardyBaldyKilled;                             // +0x68
    bool BarberLadyKilled;                              // +0x69
    bool BeardyBaldyLeft;                               // +0x6a
    long IncorrectHairComboCount;                       // +0x6c
    bool AttackedByHero;                                // +0x70
    CTimer RandomSpeechTimer;                           // +0x74
    virtual void OnPersist(CPersistContext& context);
};

void CV_BeardyBaldyScript::OnPersist(CPersistContext& context)
{
    context.Transfer("QuestFinished", QuestFinished, false);
    context.Transfer("QuestActivated", QuestActivated, false);
    context.Transfer("QuestComplete", QuestComplete, false);
    context.Transfer("QuestPhase", QuestPhase, 0L);
    context.Transfer("InitialisePhase", InitialisePhase, false);
    context.Transfer("IntroComplete", IntroComplete, false);
    context.Transfer("Phase1RequirementsComplete", Phase1RequirementsComplete, false);
    context.Transfer("Phase2RequirementsComplete", Phase2RequirementsComplete, false);
    context.Transfer("Phase3RequirementsComplete", Phase3RequirementsComplete, false);
    context.Transfer("RequiredHairdo", RequiredHairdo);
    context.Transfer("RequiredBeard", RequiredBeard);
    context.Transfer("RequiredTash", RequiredTash);
    context.Transfer("BeardyBaldyKilled", BeardyBaldyKilled, false);
    context.Transfer("BarberLadyKilled", BarberLadyKilled, false);
    context.Transfer("AcquiredNewHairdo", AcquiredNewHairdo, false);
    context.Transfer("AcquiredNewBeard", AcquiredNewBeard, false);
    context.Transfer("AcquiredNewTash", AcquiredNewTash, false);
    context.Transfer("AllHairChanged", AllHairChanged, false);
    context.Transfer("IncorrectHairComboCount", IncorrectHairComboCount, 0L);
    context.Transfer("AttackedByHero", AttackedByHero, false);
}
}   // namespace NScript