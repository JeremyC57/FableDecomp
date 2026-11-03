// NScript::CV_DemonDoorsScript::Main() -- original script entry logic.
// Leaf: ?Main@CV_DemonDoorsScript@NScript@@UAEXXZ
#define FABLE_RETAIL_CENTITYSCRIPTBINDING
#define FABLE_RETAIL_CSCRIPTTHING
#define FABLE_RETAIL_NSCRIPT
#define FABLE_RETAIL_CLEVELSCRIPTBASE
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_CCHARSTRING
#include "engine/CV_DemonDoorsScript.h"
#include "engine/CGameScriptInterfaceBase.h"
#include "engine/CEntityScriptBinding.h"

void NScript::CV_DemonDoorsScript::Main()
{
    AddEntityScriptBinding(new CEntityScriptBinding<CV_DemonDoorsScript>("DF_ShineLight", this, &CDF_ShineLight::Alloc, 0));
    AddEntityScriptBinding(new CEntityScriptBinding<CV_DemonDoorsScript>("DF_ThunderHelmet", this, &CDF_ThunderHelmet::Alloc, 0));
    AddEntityScriptBinding(new CEntityScriptBinding<CV_DemonDoorsScript>("DF_CombatMult", this, &CDF_CombatMult::Alloc, 0));
    AddEntityScriptBinding(new CEntityScriptBinding<CV_DemonDoorsScript>("DF_Dragon", this, &CDF_Dragon::Alloc, 0));
    AddEntityScriptBinding(new CEntityScriptBinding<CV_DemonDoorsScript>("DF_SummonsCreatures", this, &CDF_SummonsCreatures::Alloc, 0));
    AddEntityScriptBinding(new CEntityScriptBinding<CV_DemonDoorsScript>("DF_BeEvil", this, &CDF_BeEvil::Alloc, 0));
    AddEntityScriptBinding(new CEntityScriptBinding<CV_DemonDoorsScript>("DF_DoDamage", this, &CDF_DoDamage::Alloc, 0));
    AddEntityScriptBinding(new CEntityScriptBinding<CV_DemonDoorsScript>("DF_EatPies", this, &CDF_EatPies::Alloc, 0));
    AddEntityScriptBinding(new CEntityScriptBinding<CV_DemonDoorsScript>("DF_GiveGift", this, &CDF_GiveGift::Alloc, 0));
    AddEntityScriptBinding(new CEntityScriptBinding<CV_DemonDoorsScript>("DF_DressAsBandit", this, &CDF_DressAsBandit::Alloc, 0));
    AddEntityScriptBinding(new CEntityScriptBinding<CV_DemonDoorsScript>("DF_ThreeSuits", this, &CDF_ThreeSuits::Alloc, 0));
    AddEntityScriptBinding(new CEntityScriptBinding<CV_DemonDoorsScript>("DF_MarryLadyGrey", this, &CDF_MarryLadyGrey::Alloc, 0));
    AddEntityScriptBinding(new CEntityScriptBinding<CV_DemonDoorsScript>("DF_SexMad", this, &CDF_SexMad::Alloc, 0));
    AddEntityScriptBinding(new CEntityScriptBinding<CV_DemonDoorsScript>("DF_KeyHoarder", this, &CDF_KeyHoarder::Alloc, 0));
    PostAddScriptedEntities();
    Game.PostAddScriptedEntities();
}
