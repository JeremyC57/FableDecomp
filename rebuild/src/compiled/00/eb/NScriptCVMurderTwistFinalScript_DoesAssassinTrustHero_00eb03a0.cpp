// CV_MurderTwistFinalScript::DoesAssassinTrustHero() -- the assassin trusts a hero dressed in the full bandit-camp outfit, never one in the full guard outfit, and otherwise any hero whose morality category is below 3
// Retail: Fable.exe (TLC, VC7.1) @ 0x00EB03A0, 0x354 bytes, plain `ret`.
// Callees: CCharString(const char*, long) 0x0099EBF0 / ~CCharString 0x0099EAE0, IsActiveThreadTerminating 0x00CB7940;
//   CGameScriptInterface slots 70 GetHero, 158 GetHeroMoralityCategory, 187 IsWearingClothingItem.
// Shape: each outfit test is a named bool over a five-way && chain of temporaries (flag word in ebx, the bool in the
//   frame), as in the landed CV_ChapelOfEvilScript::GetVictimIsGuard 0x00E6E050 `found` local.
// NScript (sweep 3): the script classes live in namespace NScript (Ego_r/FableWin name sets, e.g.
//   ?IsHeroDressedAsLady@CV_BordelloScript@NScript@@QAE_NXZ; every CV_/CQ_ script class is spelled ...@NScript@@ and
//   class-keyed); this row's class is now declared there (wrapped, struct->class). Same bytes.
class CCharString { public:
    void* PStringData;
    CCharString(const char* text, long length = -1);    // 0x0099EBF0
    ~CCharString();                                     // 0x0099EAE0
};
class CScriptThing;
// CGameScriptInterface is the retail game-script interface from the shared headers (FABLE_RETAIL_NSCRIPT opt-in:
//   CGameScriptInterface : CGameScriptInterfaceBase : CBaseClassNonCopyable : CBaseClass, vtable 0x01260F0C, 918 slots).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CGameScriptInterface.h"
class CScriptBase { public:                                    // retail 0x40 bytes
    unsigned char _body[0x40];
    bool IsActiveThreadTerminating();                   // 0x00CB7940
};
namespace NScript {
class CV_MurderTwistFinalScript : public CScriptBase {
public:
    CGameScriptInterface* Game;                         // +0x40
    bool DoesAssassinTrustHero();
};
} // namespace NScript
using NScript::CV_MurderTwistFinalScript;

bool NScript::CV_MurderTwistFinalScript::DoesAssassinTrustHero()
{
    bool banditOutfit = Game->IsWearingClothingItem(Game->GetHero(), "OBJECT_HERO_SHIRT_BANDITCAMP")
        && Game->IsWearingClothingItem(Game->GetHero(), "OBJECT_HERO_TROUSERS_BANDITCAMP")
        && Game->IsWearingClothingItem(Game->GetHero(), "OBJECT_HERO_HAT_BANDITCAMP")
        && Game->IsWearingClothingItem(Game->GetHero(), "OBJECT_HERO_BOOTS_BANDITCAMP")
        && Game->IsWearingClothingItem(Game->GetHero(), "OBJECT_HERO_GLOVES_BANDITCAMP");
    if (banditOutfit) {
        if (IsActiveThreadTerminating())
            return false;
        return true;
    }
    bool guardOutfit = Game->IsWearingClothingItem(Game->GetHero(), "OBJECT_HERO_SHIRT_GUARD")
        && Game->IsWearingClothingItem(Game->GetHero(), "OBJECT_HERO_TROUSERS_GUARD")
        && Game->IsWearingClothingItem(Game->GetHero(), "OBJECT_HERO_HAT_GUARD")
        && Game->IsWearingClothingItem(Game->GetHero(), "OBJECT_HERO_BOOTS_GUARD")
        && Game->IsWearingClothingItem(Game->GetHero(), "OBJECT_HERO_GLOVES_GUARD");
    if (guardOutfit) {
        if (IsActiveThreadTerminating())
            return false;
        return false;
    }
    if (Game->GetHeroMoralityCategory() < 3)
        return true;
    return false;
}