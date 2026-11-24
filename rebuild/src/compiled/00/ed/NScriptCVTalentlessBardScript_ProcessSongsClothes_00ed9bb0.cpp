// 00ed9bb0 NScript::CV_TalentlessBardScript::ProcessSongsClothes
// Leaf: ?ProcessSongsClothes@CV_TalentlessBardScript@NScript@@QAEXXZ
// Inferred: The entry store at 00ED9BB4 is the only difference from the five donor branches. VC7.1 retains a conditional temporary lifetime flag in this build.
// Flags:
#define FABLE_RETAIL_NSCRIPT
#define FABLE_RETAIL_CLEVELSCRIPTBASE
#define FABLE_RETAIL_CLEVELSCRIPTBASE_ORIGINAL_MEMBERS
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_STLP_VECTOR_PUSHBACK_OUTOFLINE
#include "engine/NScript_CV_TalentlessBardScript.h"
#include "engine/CGameScriptInterfaceBase.h"
#include "engine/TRetVoid.h"

namespace NScript {
void CV_TalentlessBardScript::ProcessSongsClothes()
{
    TRetVoid _Fail_Val;
    if (Game.IsHeroNaked()) {
        if (IsActiveThreadTerminating())
            return _Fail_Val.Return();
        true ? SongItems.push_back("TEXT_QST_163_WEAR_NOTHING") : _Fail_Val.Return();
    } else if (IsHeroDressedAsAWoman()) {
        if (IsActiveThreadTerminating())
            return _Fail_Val.Return();
        SongItems.push_back("TEXT_QST_163_WEAR_DRESS");
    } else if (IsHeroDressedInWillRobe()) {
        if (IsActiveThreadTerminating())
            return _Fail_Val.Return();
        SongItems.push_back("TEXT_QST_163_WEAR_WILL");
    } else if (IsHeroDressedInArmour()) {
        if (IsActiveThreadTerminating())
            return _Fail_Val.Return();
        SongItems.push_back("TEXT_QST_163_WEAR_ARMOUR");
    } else if (IsHeroDressedAsApprentice()) {
        if (IsActiveThreadTerminating())
            return _Fail_Val.Return();
        SongItems.push_back("TEXT_QST_163_WEAR_APPRENTICE");
    }
}
}