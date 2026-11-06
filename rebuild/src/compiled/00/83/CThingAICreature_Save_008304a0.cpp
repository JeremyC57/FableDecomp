// CThingAICreature::Save(CCharString& out) -- append the AI creature's level-script text after CThingCreatureBase's: the five relation UIDs (home / work building, father, mother, spouse; remapped for serialisation, only when set), the overriding brain name (or NULL), ten flag bits and the initial position, each line ending ";\r\n"
// Leaf: ?Save@CThingAICreature@@UAEXAAVCCharString@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x008304A0, 2060 bytes (public virtual, `ret 4`). The last `ret` is at 0x00830C7A; the cold
//   block after it (0x00830C7D..0x00830CAB: the non-null brain-name compare and the "NULL" temporary, ending `jmp 0x008307A2`) is
//   part of the function (VC7.1 moves the unlikely arms of the conditional expression behind the epilogue).
// Callees: CThingCreatureBase::Save 0x00661A60 (by name), CBaseIntelligentPointer::GetPItem 0x00A01B50 (twice per relation: the
//   test and the UID read), CThing::IsLevelLess 0x004C7930, CThingManager::RemapUIDForSerialisation(bool, bool, unsigned long,
//   unsigned __int64) const 0x0051E210 on CThing::PCurrentThingManager 0x013B8A1C, GFUHUGEToCharString 0x0099F930,
//   GFBoolToCharString 0x0099F9B0, GFFloatToCharString(double, long) 0x009A08B0 (all three free __fastcall with a hidden result),
//   operator+(const char*, const CCharString&) 0x0099F690, operator+(const CCharString&, const char*) 0x0099F600,
//   operator+(const CCharString&, const CCharString&) 0x0099F570, CCharString::operator+= 0x0099F0A0,
//   CCharString(const char*, long) 0x0099EBF0, ~CCharString 0x0099EAE0, CBasicString<char>::operator==(const char*) 0x004115A0.
// Names: Ego_r.pdb / FableWin.pdb ?Save@CThingAICreature@@UAEXAAVCCharString@@@Z (the catalog row) and the callees' PDB spellings
//   (?GFFloatToCharString@@YI?AVCCharString@@NJ@Z, ?GFBoolToCharString@@YI?AVCCharString@@_N@Z, ?GFUHUGEToCharString@@YI?AVCCharString@@_K@Z,
//   ?RemapUIDForSerialisation@CThingManager@@QBE_K_N0K_K@Z, ?IsLevelLess@CThing@@QBE_NXZ); the key strings are the member names.
// Layout: engine/CThingAICreature.h retail branch + FABLE_RETAIL_CTHINGAICREATURE_SAVE (hdr/): HomeBuilding +0x168 .. SpouseCreature
//   +0x188, OverridingBrainName +0x190, the flag bytes +0x1C8 / +0x1C9; CThingCreatureBase::InitialPos +0xE8; CThing::UniqueID +0x18,
//   CurrentMapNumber (short) +0x8E.
// Shape: the CThingSwitch::Save 0x0083BF90 shape -- one `out += "Key " + value + ";\r\n";` per line; the brain-name line builds its
//   prefix as an explicit CCharString (0x0099F570 is CCharString + CCharString) and picks `name == "" ? CCharString("NULL") : name`
//   (the conditional temporary is tracked by bit 0 of the flag word at [esp+0Ch]).
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CCHARSTRING_FORCEINLINE_TEXT_EQ
#define FABLE_RETAIL_CCHARSTRING_CONCAT_STRING
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHING_MANAGERACCESS
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_CTHINGCREATUREBASE
#define FABLE_RETAIL_CTHINGCREATUREBASE_SAVE
#define FABLE_RETAIL_CTHINGAICREATURE
#define FABLE_RETAIL_CTHINGAICREATURE_HOMEBUILDING
#define FABLE_RETAIL_CTHINGAICREATURE_SAVE
#define FABLE_RETAIL_CTHINGBUILDING
#define FABLE_RETAIL_CCLASSFACTORY
#define FABLE_RETAIL_CTHINGMANAGER
#define FABLE_RETAIL_C3DVECTOR
#include "fable_string.h"
#include "engine/CThing.h"
#include "engine/CThingCreatureBase.h"
#include "engine/CThingAICreature.h"
#include "engine/CThingBuilding.h"
#include "engine/CThingManager.h"

CCharString __fastcall GFUHUGEToCharString(unsigned __int64 value);            // 0x0099F930
CCharString __fastcall GFBoolToCharString(bool value);                         // 0x0099F9B0
CCharString __fastcall GFFloatToCharString(double value, long digits);         // 0x009A08B0

void CThingAICreature::Save(CCharString& out)
{
    CThingCreatureBase::Save(out);
    if (HomeBuilding.Get()) {
        unsigned __int64 uid = GetPCurrentThingManager()->RemapUIDForSerialisation(true, IsLevelLess(), GetCurrentMapNumber(), HomeBuilding->GetUniqueID());
        out += "HomeBuildingUID " + GFUHUGEToCharString(uid) + ";\r\n";
    }
    if (WorkBuilding.Get()) {
        unsigned __int64 uid = GetPCurrentThingManager()->RemapUIDForSerialisation(true, IsLevelLess(), GetCurrentMapNumber(), WorkBuilding->GetUniqueID());
        out += "WorkBuildingUID " + GFUHUGEToCharString(uid) + ";\r\n";
    }
    if (FatherCreature.Get()) {
        unsigned __int64 uid = GetPCurrentThingManager()->RemapUIDForSerialisation(true, IsLevelLess(), GetCurrentMapNumber(), FatherCreature->GetUniqueID());
        out += "FatherCreatureUID " + GFUHUGEToCharString(uid) + ";\r\n";
    }
    if (MotherCreature.Get()) {
        unsigned __int64 uid = GetPCurrentThingManager()->RemapUIDForSerialisation(true, IsLevelLess(), GetCurrentMapNumber(), MotherCreature->GetUniqueID());
        out += "MotherCreatureUID " + GFUHUGEToCharString(uid) + ";\r\n";
    }
    if (SpouseCreature.Get()) {
        unsigned __int64 uid = GetPCurrentThingManager()->RemapUIDForSerialisation(true, IsLevelLess(), GetCurrentMapNumber(), SpouseCreature->GetUniqueID());
        out += "SpouseCreatureUID " + GFUHUGEToCharString(uid) + ";\r\n";
    }
    out += CCharString("OverridingBrainName ") + (OverridingBrainName == "" ? CCharString("NULL") : OverridingBrainName) + ";\r\n";
    out += "HasInformation " + GFBoolToCharString(HasInformation) + ";\r\n";
    out += "WanderWithInformation " + GFBoolToCharString(WanderWithInformation) + ";\r\n";
    out += "WaveWithInformation " + GFBoolToCharString(WaveWithInformation) + ";\r\n";
    out += "ContinueAIWithInformation " + GFBoolToCharString(ContinueAIWithInformation) + ";\r\n";
    out += "EnableCreatureAutoPlacing " + GFBoolToCharString(EnableCreatureAutoPlacing) + ";\r\n";
    out += "AllowedToFollowHero " + GFBoolToCharString(AllowedToFollowHero) + ";\r\n";
    out += "RegionFollowingOverriddenFromScript " + GFBoolToCharString(RegionFollowingOverriddenFromScript) + ";\r\n";
    out += "RespondingToFollowAndWait " + GFBoolToCharString(RespondingToFollowAndWait) + ";\r\n";
    out += "CanBeCourted " + GFBoolToCharString(CanBeCourted) + ";\r\n";
    out += "CanBeMarried " + GFBoolToCharString(CanBeMarried) + ";\r\n";
    out += "InitialPosX " + GFFloatToCharString(InitialPos.X, 6) + ";\r\n";
    out += "InitialPosY " + GFFloatToCharString(InitialPos.Y, 6) + ";\r\n";
    out += "InitialPosZ " + GFFloatToCharString(InitialPos.Z, 6) + ";\r\n";
}