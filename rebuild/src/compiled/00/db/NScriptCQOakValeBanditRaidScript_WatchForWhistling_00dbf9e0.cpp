// CQ_OakValeBanditRaidScript::WatchForWhistling() -- thread: plays a bandit whistle on the nearest raid bandit, then each frame (while the camp path region is loaded and whistling is not stopped) plays one of three random whistles whenever the last one has finished and a bandit is alive
// Leaf: WatchForWhistling
// NScript (sweep 3): the script classes live in namespace NScript (Ego_r/FableWin name sets, e.g.
//   ?IsHeroDressedAsLady@CV_BordelloScript@NScript@@QAE_NXZ; every CV_/CQ_ script class is spelled ...@NScript@@ and
//   class-keyed); this row's class is now declared there (struct->class; already NScript). Same bytes.
class CCharString { public:
    void* PStringData;
    CCharString(const char* text, long length = -1);    // 0x0099EBF0
    ~CCharString();                                     // 0x0099EAE0
};

class CCPPointerInfo { public:                                 // counted-pointer block (Ego_r.pdb)
    unsigned long RefCount;                             // +0x00
    void (__fastcall* DeleteFunc)(void* data);          // +0x04
    void* Data;                                         // +0x08
};

#include "engine/CBaseClass.h"                         // CBaseClass() 0x0099A2D0, ~CBaseClass 0x0099A2E0

class CScriptThing;
template <class T> class CCountedPointer { public:                     // CCountedPointer<CScriptThing>
    CScriptThing*   Data;                               // +0x00
    CCPPointerInfo* Info;                               // +0x04
    CCountedPointer() : Data(0), Info(0) {}
    ~CCountedPointer()
    {
        if (Info) {
            --Info->RefCount;
            if (Info->RefCount == 0) {
                Info->DeleteFunc(Info->Data);
                delete Info;
            }
        }
        Data = 0;
        Info = 0;
    }
};

// CScriptThing: the retail model from engine/CScriptThing.h (: CBaseClass, vtable 0x01238C8C, 79 slots, 0xc).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CScriptThing.h"
// Retail expands these at this row's sites (the local model had them in class): defined inline after the header.
inline CScriptThing::CScriptThing() {}
__forceinline CScriptThing::~CScriptThing() {}
inline CScriptThing::CScriptThing(const CScriptThing& other) : CBaseClass(other), PImp(other.PImp) { }
inline CScriptThing& CScriptThing::operator=(const CScriptThing& other) { PImp = other.PImp; return *this; }

// CGameScriptInterface is the retail game-script interface from the shared headers (FABLE_RETAIL_NSCRIPT opt-in:
//   CGameScriptInterface : CGameScriptInterfaceBase : CBaseClassNonCopyable : CBaseClass, vtable 0x01260F0C, 918 slots).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CGameScriptInterface.h"

extern "C" int __cdecl rand();                          // 0x00BFEB16

namespace NScript {
}   // namespace NScript (CScriptBase is global in retail)
// CScriptBase: the retail NScript model from the shared headers (FABLE_RETAIL_NSCRIPT: CScriptBase :
//   CBaseClassNonCopyable : CBaseClass, vtable 0x012C1648, 5 slots, 0x40).
#include "engine/CScriptBase.h"
namespace NScript {

class CQ_OakValeBanditRaidScript : public CScriptBase {
public:
    CGameScriptInterface* GSI;                          // +0x40
    void*                 PMasterData;                  // +0x44
    unsigned char         _pad48[0x14];                 // BanditTimer .. HerosFollowers / gates (not touched here)
    unsigned long         SoundPlayed;                  // +0x5c (PDB 0x74)
    bool                  StopWhistling;                // +0x60 (PDB 0x78)
    void WatchForWhistling();
};

void CQ_OakValeBanditRaidScript::WatchForWhistling()
{
    SoundPlayed = GSI->PlaySoundOnThing(GSI->GetNearestWithScriptName(GSI->GetHero(), "OakValeBanditRaidBandit"), "SND_BANDIT_WHISTLE_01");
#pragma inline_depth(0)
    if (IsActiveThreadTerminating())
        return;
    for (;;) {
        if (!GSI->IsRegionLoaded("BanditCampPathEntrance")) {
            if (IsActiveThreadTerminating())
                return;
            while (!GSI->IsRegionLoaded("BanditCampPathEntrance")) {
                GSI->NewScriptFrame();
                if (IsActiveThreadTerminating())
                    return;
            }
            if (IsActiveThreadTerminating())
                return;
        }
        if (StopWhistling == true) {
            if (IsActiveThreadTerminating())
                return;
            while (StopWhistling) {
                GSI->NewScriptFrame();
                if (IsActiveThreadTerminating())
                    return;
            }
            if (IsActiveThreadTerminating())
                return;
        }
        if (!GSI->IsSoundPlaying(SoundPlayed)) {
            if (IsActiveThreadTerminating())
                return;
            if (GSI->GetNearestWithScriptName(GSI->GetHero(), "OakValeBanditRaidBandit").IsAlive()) {
                if (IsActiveThreadTerminating())
                    return;
                switch (rand() % 3) {
                case 0:
                    SoundPlayed = GSI->PlaySoundOnThing(GSI->GetNearestWithScriptName(GSI->GetHero(), "OakValeBanditRaidBandit"), "SND_BANDIT_WHISTLE_01");
                    break;
                case 1:
                    SoundPlayed = GSI->PlaySoundOnThing(GSI->GetNearestWithScriptName(GSI->GetHero(), "OakValeBanditRaidBandit"), "SND_BANDIT_WHISTLE_02");
                    break;
                case 2:
                    SoundPlayed = GSI->PlaySoundOnThing(GSI->GetNearestWithScriptName(GSI->GetHero(), "OakValeBanditRaidBandit"), "SND_BANDIT_WHISTLE_03");
                    break;
                }
            }
        }
        GSI->NewScriptFrame();
        if (IsActiveThreadTerminating())
            return;
    }
}
#pragma inline_depth()
}