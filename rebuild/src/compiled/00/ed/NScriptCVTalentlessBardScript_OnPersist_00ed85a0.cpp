// NScript::CV_TalentlessBardScript::OnPersist(CPersistContext& context) -- persist the talentless-bard quest state: the finished / reward / hurt flags, the count of things sung about, the intro / attacked / following flags and the song-thing availability bits
// Leaf: ?OnPersist@CV_TalentlessBardScript@NScript@@UAEXAAVCPersistContext@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00ED85A0, 213 bytes, thiscall, `ret 4`.
// Identity (proven): slot 4 (OnPersist) of the RTTI-less level-script vtable 0x012F3428, which the landed constructor
//   NScript::CV_TalentlessBardScript::CV_TalentlessBardScript 0x00ED76E0 stores; the eight keys are Ego_r.pdb
//   CV_TalentlessBardScript member names at the touched offsets (Ego_r - 0x14; - 0x18 after the SongItems vector).
//   Level-script slots: 0 dtor, 1 RegisterMain, 2 Main, 3 Init, 4 OnPersist. Ego_r.pdb has
//   ?OnPersist@CV_TalentlessBardScript@NScript@@UAEXAAVCPersistContext@@@Z.
// Layout: Ego_r.pdb CV_TalentlessBardScript members, each at Ego_r offset - 0x14 (retail CScriptBase is 0x40).
// Callees: CPersistContext::Transfer<bool> 0x004045C0, Transfer<unsigned long> 0x004106F0, TransferVector<bool> 0x00CDCF80
//   (??$TransferVector@_N@CPersistContext@@QAEXPBDAAV?$vector@_NV?$allocator@_N@std@@@std@@@Z). Keys 0x012F36EC ..
//   0x012F3678 are Ego_r.pdb member names ("AttackedByHero" 0x012E58B0 is a shared pooled string).
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
namespace std {
template <> class vector<bool, allocator<bool> > {     // STLport 4.6 bit vector: start / finish bit iterators, end of storage (0x14)
public:
    unsigned int* _M_start_p;
    unsigned int _M_start_offset;
    unsigned int* _M_finish_p;
    unsigned int _M_finish_offset;
    unsigned int* _M_end_of_storage;
};
}
#include "engine/CBaseClass.h"                         // CBaseClass() 0x0099A2D0, ~CBaseClass 0x0099A2E0 (slot 0), implicit copy
#include "engine/CCountedPointer.h"
// CScriptThing: the retail model from engine/CScriptThing.h (: CBaseClass, vtable 0x01238C8C, 79 slots, 0xc).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CScriptThing.h"
// CTimer: the retail model from engine/CTimer.h (Ego_r.pdb CTimer {long TimerIndex}, 0x4; the setter is the retail
//   operator=(long), Ego_r 0x00CCE720). Bodies the row expands inline follow the include.
#include "engine/CTimer.h"
class CPersistContext {
protected:
    template <class T> static T __fastcall GetDefaultVal();
public:
    template <class T> void Transfer(const char* name, T& value, const T& defaultValue);
    template <class T> void Transfer(const char* name, T& value) { Transfer(name, value, GetDefaultVal<T>()); }
    template <class T> void TransferVector(const char* name, std::vector<T>& value);
};
// The unsigned long and bool defaults are inline (retail stores 0 into the stack slot before each call).
template <> inline unsigned long __fastcall CPersistContext::GetDefaultVal<unsigned long>() { return 0; }
template <> inline bool __fastcall CPersistContext::GetDefaultVal<bool>() { return false; }
class CGameScriptInterface;
class CPersistContext;
// CScriptBase / CLevelScriptBase<T>: the retail NScript model from the shared headers (FABLE_RETAIL_NSCRIPT: CScriptBase :
//   CBaseClassNonCopyable : CBaseClass, vtable 0x012C1648, 5 slots, 0x40; CLevelScriptBase<T> Game +0x40, MasterScriptData +0x44).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CLevelScriptBase.h"
class CGameScriptInterfaceBase;

namespace NScript {
class CQ_SunnyvaleMasterData;
class CV_TalentlessBardScript : public CLevelScriptBase<CQ_SunnyvaleMasterData> {
public:
    class QuestData {                                   // Ego_r.pdb, 8 bytes
    public:
        CCharString name;
        CCharString tag;
    };
    bool bHurtBard;                                     // +0x48
    bool bBard_finished;                                // +0x49
    unsigned long lNumber_of_things_sung_about;         // +0x4c
    bool bRewardGiven;                                  // +0x50
    CScriptThing stBard;                                // +0x54
    bool BardIntroComplete;                             // +0x60
    bool AttackedByHero;                                // +0x61
    bool BardFollowing;                                 // +0x62
    CTimer BardWanderTime;                              // +0x64
    std::vector<CCharString> SongItems;                 // +0x68
    QuestData QuestInfo[12];                            // +0x74
    std::vector<bool> SongThingAvailable;               // +0xd4
    CTimer RandomSpeechTimer;                           // +0xe8
    virtual void OnPersist(CPersistContext& context);
};

void CV_TalentlessBardScript::OnPersist(CPersistContext& context)
{
    context.Transfer("bBard_finished", bBard_finished);
    context.Transfer("bRewardGiven", bRewardGiven);
    context.Transfer("bHurtBard", bHurtBard);
    context.Transfer("lNumber_of_things_sung_about", lNumber_of_things_sung_about);
    context.Transfer("AttackedByHero", AttackedByHero);
    context.Transfer("BardIntroComplete", BardIntroComplete);
    context.Transfer("BardFollowing", BardFollowing);
    context.TransferVector("SongThingAvailable", SongThingAvailable);
}
}