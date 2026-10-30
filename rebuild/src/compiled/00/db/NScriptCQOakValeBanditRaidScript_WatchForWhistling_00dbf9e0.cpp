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
struct CCountedPointerScriptThing {                     // CCountedPointer<CScriptThing>
    CScriptThing*   Data;                               // +0x00
    CCPPointerInfo* Info;                               // +0x04
    CCountedPointerScriptThing() : Data(0), Info(0) {}
    ~CCountedPointerScriptThing()
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

class CScriptThing : public CBaseClass { public:                      // 12 bytes, vtable 0x01238C8C
    CCountedPointerScriptThing PImp;                    // +0x04
    CScriptThing() {}
    virtual __forceinline ~CScriptThing() {}            // slot 0; out of line: 0x004AA840
    virtual void v001();    virtual void v002();    virtual void v003();    virtual void v004();    virtual void v005();    virtual void v006();    virtual void v007();    virtual void v008();
    virtual void v009();    virtual void v010();    virtual void v011();    virtual void v012();    virtual void v013();    virtual void v014();    virtual void v015();    virtual void v016();
    virtual void v017();
    virtual void v018();
    virtual void v019();    virtual void v020();    virtual void v021();    virtual void v022();    virtual void v023();    virtual void v024();    virtual void v025();    virtual void v026();
    virtual void v027();    virtual void v028();    virtual void v029();    virtual void v030();    virtual void v031();    virtual void v032();    virtual void v033();    virtual void v034();
    virtual void v035();    virtual void v036();    virtual void v037();    virtual void v038();    virtual void v039();    virtual void v040();    virtual void v041();    virtual void v042();
    virtual void v043();    virtual void v044();    virtual void v045();    virtual void v046();    virtual void v047();    virtual void v048();    virtual void v049();    virtual void v050();
    virtual void v051();    virtual void v052();    virtual void v053();    virtual void v054();    virtual void v055();    virtual void v056();    virtual void v057();    virtual void v058();
    virtual void v059();    virtual void v060();    virtual void v061();    virtual void v062();    virtual void v063();    virtual void v064();    virtual void v065();    virtual void v066();
    virtual void v067();    virtual void v068();    virtual void v069();    virtual void v070();    virtual void v071();    virtual void v072();    virtual void v073();    virtual void v074();
    virtual bool IsAlive() const;                                   // slot 75 (+0x12c), 0x004AB130
};

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