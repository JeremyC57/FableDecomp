// CAtmosProcess::SetAtmosSoundsAsMuted(bool muted, float time) -- muting fades every sound theme's DAY and NIGHT atmos to silence over `time` and forgets the day/night state (so the next unmuted update re-applies it); unmuting runs the day/night update; the muted flag is then recorded
// Leaf: ?SetAtmosSoundsAsMuted@CAtmosProcess@@QAEX_NM@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x006B1D30, 507 bytes (+5 int3 to 0x006B1F30), thiscall, `ret 8`.
// Identity (proven, two sources): Ego_r.pdb / FableWin.pdb ?SetAtmosSoundsAsMuted@CAtmosProcess@@QAEX_NM@Z (the class's only (bool,
//   float) member; donor order Update / UpdateDayNightAtmosSounds / SetAtmosSoundsAsMuted / ApplyReverbSettings = retail 0x006B2260 /
//   0x006B1A20 / 0x006B1D30 / 0x006B1600); it stores AtmosSoundsMuted +0x09, the byte UpdateDayNightAtmosSounds 0x006B1A20 tests, and
//   calls that function on the unmute path. Callers 0x0049E616 / 0x004A5C99 (CWorld).
// Callees: CCharString(const char*, long) 0x0099EBF0 / ~CCharString 0x0099EAE0 ("SOUND_THEME" 0x01235E60, "DAY" 0x0125D48C,
//   "NIGHT" 0x0125D484), CGameDefinitionManager::Get 0x0044C6B0, CDefinitionManager::GetNoInstantiatedDefsOfType 0x009AD3B0,
//   CDefinitionManager::GetDef<CSoundThemeDef>(const CCharString&, long, ..) 0x006B25E0 (same-TU body), the def's
//   GetInstantiationName (virtual slot 10) with CDefStringTable::GetString 0x009D49B0, and the inline CAtmosProcess::MixAtmos:
//   CDefinitionManager::GetDef<CSoundThemeDef>(const CCharString&, ..) 0x006B2660 (FableWin.pdb
//   ??$GetDef@VCSoundThemeDef@@@CDefinitionManager@@QBE_NABVCCharString@@PAV?$CDefPointer@$$CBVCSoundThemeDef@@@@@Z), PSoundManager
//   0x013B8394 slot 40 SetChannelGain; CAtmosProcess::UpdateDayNightAtmosSounds 0x006B1A20.
// Shape: the two MixAtmos calls per theme are expanded (the class's inline body; retail calls the same body out of line from
//   UpdateDayNightAtmosSounds, whose sites sit at a quarter of this loop's frequency). Like UpdateDayNightAtmosSounds, the
//   by-index GetDef body is in the TU, so the theme handle stays uncaptured and is released without a null test.
// Header overlay (FABLE_HDR_OVERLAY=$A/hdr): engine/CAtmosProcess.h (new) with FABLE_RETAIL_CATMOSPROCESS_MIXATMOS_BODY.
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CDEFSTRINGTABLE
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CRESOURCELIST
#define FABLE_RETAIL_CDEFINITIONMANAGER
#define FABLE_RETAIL_CDEFINITIONMANAGER_NOINSTANTIATED
#define FABLE_RETAIL_CDEFINITIONMANAGER_GETDEF_INDEX_BODY
#define FABLE_RETAIL_CDEFINITIONMANAGER_GETDEF_NAME_BODY
#define FABLE_RETAIL_CGAMEDEFINITIONMANAGER
#define FABLE_RETAIL_CASOUNDMANAGER_ATMOSMAP
#define FABLE_RETAIL_CATMOSPROCESS_MIXATMOS_BODY
#include "fable_string.h"
#include "engine/CDefString.h"
#include "engine/CDefClassBase.h"
#include "engine/CDefPointer.h"
#include "engine/CGameDefinitionManager.h"
#include "engine/CASoundManager.h"
#include "engine/CAtmosProcess.h"

class CSoundThemeDef : public CDefClassBase { };               // RTTI .?AVCSoundThemeDef@@ (no members read here)

void CAtmosProcess::SetAtmosSoundsAsMuted(bool muted, float time)
{
    if (muted) {
        long count = CGameDefinitionManager::Get().GetNoInstantiatedDefsOfType("SOUND_THEME");
        for (long i = 1; i < count; ++i) {
            CDefPointer<const CSoundThemeDef> theme;
            CGameDefinitionManager::Get().GetDef<CSoundThemeDef>("SOUND_THEME", i, &theme);
            MixAtmos(theme->GetInstantiationName().GetString(), "DAY", 0.0f, time);
            MixAtmos(theme->GetInstantiationName().GetString(), "NIGHT", 0.0f, time);
        }
        DayNightState = (EDayNightState)0;
    } else {
        UpdateDayNightAtmosSounds();
    }
    AtmosSoundsMuted = muted;
}