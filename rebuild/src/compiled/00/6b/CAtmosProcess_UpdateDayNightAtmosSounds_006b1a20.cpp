// CAtmosProcess::UpdateDayNightAtmosSounds() -- unless the atmos sounds are muted, when the game clock enters the day-sounds (or night-sounds) period, cross-fade every sound theme to its DAY (NIGHT) atmos over 2 seconds and record the new day/night state
// Leaf: ?UpdateDayNightAtmosSounds@CAtmosProcess@@AAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x006B1A20, 769 bytes, thiscall, plain `ret` (private, no arguments).
// Identity (proven, two sources): Ego_r.pdb / FableWin.pdb ?UpdateDayNightAtmosSounds@CAtmosProcess@@AAEXXZ (private); its only
//   caller is CAtmosProcess::Update 0x006B2260 (both tails, after UpdateInsideBuildings 0x006B1F30), and it writes the
//   EDayNightState word +0x00 the landed ctor 0x006B1960 zeroes (1 day, 2 night) and tests AtmosSoundsMuted +0x09 (the byte
//   SetAtmosSoundsAsMuted 0x006B1D30 stores). The catalog placeholder "CGameScriptInterface_SetSoundTheme" is propagated.
// Callees: CMainGameComponent::Get()->GetWorld().PeekGameTimeManager() (inline, +0x24 / +0x1c), CGameTimeManager::IsDaySounds
//   0x006BB1E0 / IsNightSounds 0x006BB790 (Ego_r donor order IsDaySounds just before IsNightTime 0x006BB210; IsNightSounds is
//   `!IsDaySounds` in place), CCharString(const char*, long) 0x0099EBF0 / ~CCharString 0x0099EAE0 ("SOUND_THEME" 0x01235E60,
//   "DAY" 0x0125D48C, "NIGHT" 0x0125D484), CGameDefinitionManager::Get 0x0044C6B0,
//   CDefinitionManager::GetNoInstantiatedDefsOfType 0x009AD3B0, GetDef<CSoundThemeDef>(const CCharString&, long, ..) 0x006B25E0,
//   CDefClassBase::GetInstantiationName (virtual slot 10, CDefString by value) with the inline CDefString::GetString
//   (CDefStringTable::GetString 0x009D49B0 on GDefStringTable 0x013CA828), CAtmosProcess::MixAtmos 0x006B1900, the def's
//   virtual Delete (slot 1) on the last release.
// Shape: the GetDef<T> body sits in the TU (engine/CDefinitionManager.h FABLE_RETAIL_CDEFINITIONMANAGER_GETDEF_INDEX_BODY, the
//   landed 0x006B25E0's body): the theme handle then stays uncaptured, its pointer lives in esi across both MixAtmos calls and
//   the release is `dec dword ptr [esi+4] ; jne` with no null test (0x006B1B8A), as retail.
// Layout: no shared header for CAtmosProcess / CGameTimeManager / CSoundThemeDef; the declarations below carry only what this
//   body touches, at the landed ctor's offsets (DayNightState +0x00, the CMyAtmosMap +0x04, AtmosSoundsMuted +0x09).
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CDEFSTRINGTABLE
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CRESOURCELIST
#define FABLE_RETAIL_CDEFINITIONMANAGER
#define FABLE_RETAIL_CDEFINITIONMANAGER_NOINSTANTIATED
#define FABLE_RETAIL_CDEFINITIONMANAGER_GETDEF_INDEX_BODY
#define FABLE_RETAIL_CGAMEDEFINITIONMANAGER
#define FABLE_RETAIL_CWORLD
#define FABLE_RETAIL_CMAINGAMECOMPONENT
#define FABLE_RETAIL_CMAINGAMECOMPONENT_GETWORLD
#include "fable_string.h"
#include "engine/CDefString.h"
#include "engine/CDefClassBase.h"
#include "engine/CDefPointer.h"
#include "engine/CGameDefinitionManager.h"
#include "engine/CWorld.h"
#include "engine/CMainGameComponent.h"

class CSoundThemeDef : public CDefClassBase { };               // RTTI .?AVCSoundThemeDef@@ (no members read here)

class CGameTimeManager {
public:
    bool IsDaySounds() const;                                   // 0x006BB1E0 ?IsDaySounds@CGameTimeManager@@QBE_NXZ
    bool IsNightSounds() const;                                 // 0x006BB790 ?IsNightSounds@CGameTimeManager@@QBE_NXZ
};

class CMyAtmosMap {                                             // CAtmosProcess::CMyAtmosMap, vtable 0x0125D47C (landed ctor)
public:
    virtual long GetBlockValue(const class C2DCoordI& block);   // 0x006B1580
};

class CAtmosProcess {
public:
    void MixAtmos(const CCharString& from, const CCharString& to, float mix, float time);   // 0x006B1900
private:
    void UpdateDayNightAtmosSounds();
public:
    long        DayNightState;                                  // +0x00 EDayNightState (1 day, 2 night)
    CMyAtmosMap AtmosMap;                                       // +0x04
    bool        RainCurrentlyPlaying;                           // +0x08
    bool        AtmosSoundsMuted;                               // +0x09
};

void CAtmosProcess::UpdateDayNightAtmosSounds()
{
    if (!AtmosSoundsMuted) {
        const CGameTimeManager& time = CMainGameComponent::Get()->GetWorld().PeekGameTimeManager();
        if (DayNightState != 1 && time.IsDaySounds()) {
            long count = CGameDefinitionManager::Get().GetNoInstantiatedDefsOfType("SOUND_THEME");
            for (long i = 1; i < count; ++i) {
                CDefPointer<const CSoundThemeDef> theme;
                CGameDefinitionManager::Get().GetDef<CSoundThemeDef>("SOUND_THEME", i, &theme);
                MixAtmos(theme->GetInstantiationName().GetString(), "DAY", 1.0f, 2000.0f);
                MixAtmos(theme->GetInstantiationName().GetString(), "NIGHT", 0.0f, 2000.0f);
            }
            DayNightState = 1;
        } else if (DayNightState != 2 && time.IsNightSounds()) {
            long count = CGameDefinitionManager::Get().GetNoInstantiatedDefsOfType("SOUND_THEME");
            for (long i = 1; i < count; ++i) {
                CDefPointer<const CSoundThemeDef> theme;
                CGameDefinitionManager::Get().GetDef<CSoundThemeDef>("SOUND_THEME", i, &theme);
                MixAtmos(theme->GetInstantiationName().GetString(), "NIGHT", 1.0f, 2000.0f);
                MixAtmos(theme->GetInstantiationName().GetString(), "DAY", 0.0f, 2000.0f);
            }
            DayNightState = 2;
        }
    }
}