// NScript::CV_TalentlessBardScript::IsHeroDressedAsAWoman() -- true when the hero wears a full plain, good or evil dress suit (shirt, gloves and trousers of one set), each set checked with short-circuit
// Leaf: ?IsHeroDressedAsAWoman@CV_TalentlessBardScript@NScript@@QAE_NXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00EDA5E0, 732 bytes, thiscall, plain `ret`, no EH frame.
// Identity (proven): called from NScript::CV_TalentlessBardScript::ProcessSongsClothes 0x00ED9BB0; Ego_r.pdb lists the bard
//   script's dress checks in the order IsHeroDressedInArmour / IsHeroDressedInWillRobe / IsHeroDressedAsApprentice /
//   IsHeroDressedAsAWoman, which is the retail address order 0x00ED9CF0 / 0x00EDA0C0 / 0x00EDA490 / 0x00EDA5E0; the strings
//   are the DRESS clothing items. Catalog label ProcessSongsClothes_ApplyDressCode is wrong.
// Callees: CCharString(const char*, long) 0x0099EBF0 / ~CCharString 0x0099EAE0 (each under its construction flag);
//   CGameScriptInterface slot 70 GetHero (+0x118), slot 187 IsWearingClothingItem (+0x2EC).
// Data: OBJECT_HERO_{SHIRT,GLOVES,TROUSERS}_DRESS 0x012E417C / 0x012E4120 / 0x012E41D4, ..._DRESS_GOOD 0x012E415C / 0x012E4100 /
//   0x012E41B4, ..._DRESS_EVIL 0x012E413C / 0x012E40E0 / 0x012E4194.
// Layout: Game +0x40 (CLevelScriptBase).
// Shape: the landed twin IsHeroDressedInWillRobe 0x00EDA0C0 shape with three-item chains: three named bools (plain, good, evil),
//   `return plain || good || evil;`.
class CCharString {
public:
    class CCharStringData* PStringData;
    CCharString(const char* text, long length = -1);    // 0x0099EBF0
    ~CCharString();                                     // 0x0099EAE0
};
#define FABLE_RETAIL_NSCRIPT
#include "engine/CLevelScriptBase.h"
#include "engine/CGameScriptInterfaceBase.h"

namespace NScript {
class CQ_SunnyvaleMasterData;
class CV_TalentlessBardScript : public CLevelScriptBase<CQ_SunnyvaleMasterData> {
public:
    bool IsHeroDressedAsAWoman();
};

bool CV_TalentlessBardScript::IsHeroDressedAsAWoman()
{
    bool plain = Game->IsWearingClothingItem(Game->GetHero(), "OBJECT_HERO_SHIRT_DRESS")
        && Game->IsWearingClothingItem(Game->GetHero(), "OBJECT_HERO_GLOVES_DRESS")
        && Game->IsWearingClothingItem(Game->GetHero(), "OBJECT_HERO_TROUSERS_DRESS");
    bool good = Game->IsWearingClothingItem(Game->GetHero(), "OBJECT_HERO_SHIRT_DRESS_GOOD")
        && Game->IsWearingClothingItem(Game->GetHero(), "OBJECT_HERO_GLOVES_DRESS_GOOD")
        && Game->IsWearingClothingItem(Game->GetHero(), "OBJECT_HERO_TROUSERS_DRESS_GOOD");
    bool evil = Game->IsWearingClothingItem(Game->GetHero(), "OBJECT_HERO_SHIRT_DRESS_EVIL")
        && Game->IsWearingClothingItem(Game->GetHero(), "OBJECT_HERO_GLOVES_DRESS_EVIL")
        && Game->IsWearingClothingItem(Game->GetHero(), "OBJECT_HERO_TROUSERS_DRESS_EVIL");
    return plain || good || evil;
}
}   // namespace NScript