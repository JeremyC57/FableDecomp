// NThingShot::CreateShot(long shot_def_index, CThing* shooter, CThing* weapon, const CThing* target, float accuracy, float damage_multiplier, float buildup_time, const CDefPointer<const CMeleeCombatAbilityDef>& ability, const C3DVector* pos, const CRightHandedSet* orientation) -- create a shot from a shot def: at the given position and orientation (else where the shooter fires from), make the def's object thing (an OBJECT) or AI creature (a CREATURE: given creature mode 57, the orientation and the script name "<shooter>_creature_shot"), add the def's shot-definition TC and Init it with the def, shooter, weapon, target, the three floats and the ability; returns the shot (0 when shots are off)
// Grade: DIFFER(936v938) -- calls, constants, frame and the class-name compares match (esi lever below); left: (1) block order of the OBJECT / CREATURE else-if chain (ours lays the null-string CREATURE test and creature body right after the null-string OBJECT test, retail the OBJECT body; g2 without the same-TU operator== body had retail's order but a second null test); (2) shot / shot def swap ebx<->ebp (c2prio shot_def 71 > shot 66; retail needs shot above)
// Leaf: ?CreateShot@NThingShot@@YIPAVCThing@@JPAV2@0PBV2@MMMABV?$CDefPointer@$$CBVCMeleeCombatAbilityDef@@@@PBVC3DVector@@PBVCRightHandedSet@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x007C0AD0, 938 bytes (+6 int3 to 0x007C0E80), __fastcall (shot_def_index in ecx, shooter in edx,
//   eight stack words), `ret 20h`.
// Identity (proven, two sources): Ego_r.pdb / FableWin.pdb
//   ?CreateShot@NThingShot@@YIPAVCThing@@JPAV2@0PBV2@MMMABV?$CDefPointer@$$CBVCMeleeCombatAbilityDef@@@@PBVC3DVector@@PBVCRightHandedSet@@@Z
//   (Ego_r donor 0x007BF280, the def-index overload beside the landed thing overload 0x007C0E80 = donor 0x007BF690); 2 registers + 8
//   stack words = `ret 20h`; it calls NThingShot::GetCreatureFirePosAndOrientation 0x007BF000 when pos or orientation is null.
// Callees: GetCreatureFirePosAndOrientation 0x007BF000, CMainGameComponent::PeekDefinitionManager 0x00415FBC,
//   CDefinitionManager::GetDef<CThingShotDef> 0x0078B230 (same-TU body), CDefinitionManager::GetDefClassTypeNameFromGlobalIndex 0x009ACCE0,
//   CDefStringTable::GetString 0x009D49B0, CBasicString<char>::operator==(const char*) 0x004115A0 ("OBJECT" 0x01235FF4, "CREATURE"
//   0x01236054; CCharString::operator==(const char*) expanded, its null arm the strcmp intrinsic against ""), CThing::GetPlayer
//   0x004C7A30 (CPlayer::GetNumber inline), CThingObject::Create 0x00703210, CThingAICreatureInit::CThingAICreatureInit 0x004FACD0,
//   CThingAICreature::Create 0x00833800, CVectorMap<..>::LowerBound 0x0040F020 (GetTC of TCI_ENTITY_MODE_MANAGER 49),
//   CTCCreatureModeManager::AddMode 0x0073AE10, the physics TC's slot 75 SetRHSet, CCharString(const char*, long) 0x0099EBF0 /
//   CCharString() 0x0099E4B0 / operator= 0x0099EFB0 / operator+ 0x0099F600 (+ "_" 0x01244DB4) / 0x0099F570 / ~CCharString 0x0099EAE0
//   ("creature_shot" 0x0126E264), CThing::AddTC 0x004C9D60, NThingShot::CTCShotDefinitionBase::Init 0x007C0690.
// Data: the shots-enabled switch 0x0137573F (descriptive name, as in the landed 0x007C0E80).
// Header overlay (FABLE_HDR_OVERLAY=$A/hdr): engine/CThingAICreatureInit.h (new), FABLE_RETAIL_PLAYER_GETNUMBER,
//   FABLE_RETAIL_CDEFINITIONMANAGER_CLASSTYPENAME, FABLE_RETAIL_CTHING_GETSCRIPTNAME, FABLE_RETAIL_CTCSHOTDEFINITIONBASE_INIT.
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CCHARSTRING_CONCAT_STRING
#define FABLE_RETAIL_CCHARSTRING_FORCEINLINE_TEXT_EQ
#define FABLE_RETAIL_CDEFSTRINGTABLE
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHING_GETTC_EXPANDED
#define FABLE_RETAIL_CTHING_GETSCRIPTNAME
#define FABLE_RETAIL_CTHING_BULLETTIMEOVERRIDE
#define FABLE_RETAIL_CTHING_STATEACCESS
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_CTHINGCREATUREBASE
#define FABLE_RETAIL_CTHINGAICREATURE
#define FABLE_RETAIL_CTHINGOBJECT_CREATE
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_PLAYER
#define FABLE_RETAIL_PLAYER_GETNUMBER
#define FABLE_RETAIL_CTCPHYSICSBASE
#define FABLE_RETAIL_CTCCREATUREMODEMANAGER
#define FABLE_RETAIL_CRIGHTHANDEDSET
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_CRESOURCELIST
#define FABLE_RETAIL_CDEFINITIONMANAGER
#define FABLE_RETAIL_CDEFINITIONMANAGER_GETDEF_GLOBAL_BODY
#define FABLE_RETAIL_CDEFINITIONMANAGER_CLASSTYPENAME
#define FABLE_RETAIL_CGAMEDEFINITIONMANAGER
#define FABLE_RETAIL_CMAINGAMECOMPONENT
#define FABLE_RETAIL_CTCSHOTDEFINITIONBASE_INIT
#include "fable_string.h"
#include "engine/CDefString.h"
#include "engine/C3DVector.h"
#include "engine/CRightHandedSet.h"
#include "engine/CThing.h"
#include "engine/CPlayer.h"
#include "engine/CThingObject.h"
#include "engine/CThingAICreature.h"
#include "engine/CThingAICreatureInit.h"
#include "engine/CTCPhysicsBase.h"
#include "engine/CTCCreatureModeManager.h"
#include "engine/CThingShotDef.h"
#include "engine/CGameDefinitionManager.h"
#include "engine/CMainGameComponent.h"
#include "engine/NThingShot_CTCShotDefinitionBase.h"

// CBasicString<T>::operator==(const T*) is a template member, so its body is in this TU as in retail (the COMDAT the linker kept is
// 0x004115A0; the landed 0x00871290 / 0x00870FC0 carry the same body): with it in view VC7.1 keeps the class name's string data in
// esi across both comparisons (`mov ecx,esi`, no second null test).
template<> bool CBasicString<char>::operator==(const char* text) const
{
    const signed char* first = (const signed char*)PData;
    const signed char* second = (const signed char*)text;
    int result = 0;
    while (*first || *second) {
        if (*first < *second) { result = -1; break; }
        if (*first > *second) { result = 1; break; }
        ++first; ++second;
    }
    return result == 0;
}

extern bool g_bShotsEnabled;                                    // 0x0137573F (descriptive)

namespace NThingShot {
void __fastcall GetCreatureFirePosAndOrientation(const CThing& creature, C3DVector& pos, CRightHandedSet& orientation);   // 0x007BF000

CThing* __fastcall CreateShot(long shot_def_index, CThing* shooter, CThing* weapon, const CThing* target, float accuracy,
                              float damage_multiplier, float buildup_time, const CDefPointer<const CMeleeCombatAbilityDef>& ability,
                              const C3DVector* pos, const CRightHandedSet* orientation)
{
    if (!g_bShotsEnabled)
        return 0;
    CThing* shot = 0;
    C3DVector shot_pos;
    CRightHandedSet shot_orientation;
    if (pos && orientation) {
        shot_pos = *pos;
        shot_orientation = *orientation;
    } else {
        GetCreatureFirePosAndOrientation(*shooter, shot_pos, shot_orientation);
    }
    CDefPointer<const CThingShotDef> shot_def;
    const CGameDefinitionManager& definitions = CMainGameComponent::Get()->PeekDefinitionManager();
    definitions.GetDef(shot_def_index, &shot_def);
    long object_def = shot_def->ObjectDef;
    CCharString class_name = definitions.GetDefClassTypeNameFromGlobalIndex(object_def).GetString();
    if (class_name == "OBJECT") {
        shot = CThingObject::Create(object_def, shot_pos, shooter->GetPlayer().GetNumber(), &shot_orientation, 0, "");
    } else if (class_name == "CREATURE") {
        shot = CThingAICreature::Create(object_def, shot_pos, shooter->GetPlayer().GetNumber(),
                                        CThingAICreatureInit(0, 0, false, "", "", true));
        CTCCreatureModeManager* modes;
        shot->GetTC(&modes);
        modes->AddMode(57);
        shot->GetPhysicsTC()->SetRHSet(shot_orientation);
        CCharString suffix("creature_shot");
        CCharString script_name;
        if (shooter && !shooter->IsDead())
            script_name = shooter->GetScriptName() + "_" + suffix;
        else
            script_name = suffix;
        shot->SetScriptName(script_name);
    }
    CTCShotDefinitionBase* shot_tc =
        (CTCShotDefinitionBase*)shot->AddTC(GDefStringTable.GetString(shot_def->ShotDefinitionClassName_TablePos), true, 0);
    shot_tc->Init(shot_def, shooter, weapon, target, accuracy, damage_multiplier, buildup_time, ability);
    return shot;
}
}
