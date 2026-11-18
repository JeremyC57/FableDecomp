// CTCSpecialAbilities::SetupMultiStrike() -- give the parent thing a "CTCMultiStrike" component (activated, no params) and report success; false when it already has one
// Leaf: ?SetupMultiStrike@CTCSpecialAbilities@@AAE_NXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x0072AAF0, 66 bytes, __fastcall, plain `ret`; called from CTCSpecialAbilities::UseSpecialAbility's
//   switch (0x0072CF40, HERO_ABILITY_DOUBLE_STRIKE case). Was labelled _global::UseSpecialAbility_CTCMultiStrike.
// Identity (proven, three sources): Ego_d.pdb tc_special_abilities.obj lists CTCSpecialAbilities::SetupMultiStrike right after
//   SetupMultiArrow (retail 0x0072AAA0, the preceding function); Ego_r.pdb names ?SetupMultiStrike@CTCSpecialAbilities@@AAE_NXZ (0x007418C0)
//   right after ?SetupMultiArrow@...@@AAE_NXZ (0x00741870); the string it passes is "CTCMultiStrike" @ 0x0123D374. Ego_d: no locals.
// Callees: CCharString::CCharString(const char*, long) 0x0099EBF0 (the implicit const-reference temporary of AddTC), CThing::AddTC
//   0x004C9D60, CCharString::~CCharString 0x0099EAE0. HasTC(TCI_MULTI_STRIKE = 135) folds to `test byte [+0x30]` sign bit.
// Shape: the landed twin SetupMultiArrow 0x0072AAA0, on the opt-in set of the landed UseSpecialAbility 0x0072CF40 (same TU).
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTCSPECIALABILITIES_FULL
#define FABLE_RETAIL_CTCSPECIALABILITIES_USE
#include "engine/CThing.h"
#include "engine/CTCSpecialAbilities.h"

bool CTCSpecialAbilities::SetupMultiStrike()
{
    if (!ParentThing->HasTC(TCI_MULTI_STRIKE)) {
        ParentThing->AddTC("CTCMultiStrike", true, 0);
        return true;
    }
    return false;
}