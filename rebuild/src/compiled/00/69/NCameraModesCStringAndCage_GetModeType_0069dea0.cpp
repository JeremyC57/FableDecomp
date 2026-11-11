// NCameraModes::CStringAndCage::GetModeType() const -- returns (EMode)3
// Leaf: ?GetModeType@CStringAndCage@NCameraModes@@UBE?AW4EMode@2@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x0069DEA0, thiscall, `ret`.
// Identity: slot 2 of NCameraModes::CStringAndCage's vtable 0x0125C404 (defined here: the slot holds this body in 1 vtable(s), all of NCameraModes::CStringAndCage or derived); the Xbox 360 Test PDB virtual order (third source) aligned on 8 TLC-named slots of that vtable puts GetModeType(void) at this slot; Ego_r/FableWin name set: ?GetModeType@CStringAndCage@NCameraModes@@UBE?AW4EMode@2@XZ; `ret` matches the signature.
// Header: engine/NCameraModes_CStringAndCage.h (opt-ins as in the landed row CGameCameraManager_SwitchIntoGeneralCase_0069e2e0.cpp).
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHING_GETTC_EXPANDED
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CCOUNTEDPOINTER_RESET_CAMERA_DECL
#define FABLE_RETAIL_CGAMECAMERAMANAGER
#define FABLE_RETAIL_CGAMECAMERAMANAGER_CAMPOINTS
#define FABLE_RETAIL_CGAMECAMERAMANAGER_GENERALCASE
#define FABLE_RETAIL_NCAMERAMODES_CBASE_MEMBERS
#define FABLE_RETAIL_NCAMERAMODES_CBASE_STRING_CAGE
#define FABLE_RETAIL_NCAMERAMODES_CBASE_TYPED_COMPOSITES
#define FABLE_RETAIL_NCAMERAMODES_CSTRINGANDCAGE_CONSTRUCT
#define FABLE_RETAIL_STLP_LIST
#include "fable_string.h"
#include "engine/CThing.h"
#include "engine/CGameCameraManager.h"
#include "engine/NCameraModes_CStringAndCage.h"
#include "engine/CTCCameraPointGeneralCase.h"
NCameraModes::EMode NCameraModes::CStringAndCage::GetModeType() const
{
    return (EMode)3;
}
