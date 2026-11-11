#ifndef FABLE_ENGINE_CATMOSPROCESS_H
#define FABLE_ENGINE_CATMOSPROCESS_H
// CAtmosProcess (new shared header, chunk 1025, row 0x006B1D30 SetAtmosSoundsAsMuted). Ego_r.pdb members (struct_layouts_egor.tsv,
// sizeof 0x1C) at the retail offsets of the landed TU (ctor 0x006B1960, Update 0x006B2260, UpdateDayNightAtmosSounds 0x006B1A20):
// retail has the nested CMyAtmosMap object at +0x04 (vtable 0x0125D47C; RTTI .?AVCMyAtmosMap@CAtmosProcess@@ : CASoundManager::CAtmosMap)
// that Ego_r's PDB view leaves out, so DayNightState +0x00, AtmosMap +0x04, RainCurrentlyPlaying +0x08, AtmosSoundsMuted +0x09,
// PRainPlayer +0x0A (the byte-packed CIntelligentPointer<CThingMarker>), InsideBuilding +0x14, ReverbType +0x18, ReverbLevel +0x1C,
// sizeof 0x20. Member functions: the Ego_r.pdb / FableWin.pdb name set (donor 0x006EE0A0..0x006EE150), declarations only, except
// MixAtmos under FABLE_RETAIL_CATMOSPROCESS_MIXATMOS_BODY: inline in the class (retail keeps the out-of-line COMDAT 0x006B1900 that
// UpdateDayNightAtmosSounds and CTCSoundAtmosphereVillage 0x007F53B0 call, and expands it in SetAtmosSoundsAsMuted 0x006B1DE8 / 0x006B1E84).
// EDayNightState: Ego_r.pdb type of DayNightState; its enumerators are in no name set on disk (values 0 unset, 1 day, 2 night from
// the landed rows), so the enum is declared without them. The PRainPlayer member is opaque unless FABLE_RETAIL_CATMOSPROCESS_RAINPLAYER
// (which needs FABLE_RETAIL_CINTELLIGENTPOINTER and a complete CThingMarker for its member functions).
// Needs FABLE_RETAIL_CASOUNDMANAGER_ATMOSMAP (engine/CASoundManager.h, the map's base).
#include "rebuild_abi.h"
#include "fable_string.h"
#ifndef FABLE_RETAIL_CASOUNDMANAGER_ATMOSMAP
#define FABLE_RETAIL_CASOUNDMANAGER_ATMOSMAP
#endif
#include "engine/CASoundManager.h"
#ifdef FABLE_RETAIL_CATMOSPROCESS_RAINPLAYER
#include "engine/CIntelligentPointer.h"
#endif
#ifdef FABLE_RETAIL_CATMOSPROCESS_MIXATMOS_BODY
#include "engine/CGameDefinitionManager.h"
class CSoundThemeDef;
#endif
enum EReverbEnvironmentType;                                                // MS forward enum (Ego_r.pdb; defined, without enumerators, in engine/CThingBuildingDef.h)
class C2DCoordI;
class CThingMarker;
enum EDayNightState { };
class CAtmosProcess {                                                       // natural alignment: the packed pointer class sits at +0x0a
public:
    class CMyAtmosMap : public CASoundManager::CAtmosMap {                  // vtable 0x0125D47C
    public:
        virtual long GetBlockValue(const C2DCoordI& block);                 // slot 0 0x006B1580
        virtual float GetBlockHeight(const C2DCoordI& block);               // slot 1 0x006B15C0
    };
    CAtmosProcess();                                                        // 0x006B1960
    ~CAtmosProcess();
    void Update();                                                          // 0x006B2260
    void SetAtmosSoundsAsMuted(bool muted, float time);                     // 0x006B1D30
    void RegisterAtmosPalette();
#ifdef FABLE_RETAIL_CATMOSPROCESS_MIXATMOS_BODY
    // Ego_r.pdb ?MixAtmos@CAtmosProcess@@QAEXABVCCharString@@0MM@Z: when the sound manager exists, look the atmos theme up by name
    // (GetDef<CSoundThemeDef>(const CCharString&, ..) 0x006B2660; the handle only checks it) and set the channel gain (slot 40).
    void MixAtmos(const CCharString& from, const CCharString& to, float mix, float time)
    {
        if (PSoundManager) {
            CDefPointer<const CSoundThemeDef> theme;
            CGameDefinitionManager::Get().GetDef(from, &theme);
            PSoundManager->SetChannelGain(from, to, mix, time);
        }
    }
#else
    void MixAtmos(const CCharString& from, const CCharString& to, float mix, float time);   // 0x006B1900
#endif
private:
    void UpdateDayNightAtmosSounds();                                       // 0x006B1A20
    void ApplyReverbSettings();                                             // 0x006B1600
    void UpdateInsideBuildings();                                           // 0x006B1F30
    EDayNightState DayNightState;                                           // +0x00
    CMyAtmosMap    AtmosMap;                                                // +0x04
    bool           RainCurrentlyPlaying;                                    // +0x08
    bool           AtmosSoundsMuted;                                        // +0x09
#ifdef FABLE_RETAIL_CATMOSPROCESS_RAINPLAYER
    CIntelligentPointer<CThingMarker> PRainPlayer;                          // +0x0a
#else
    unsigned char  PRainPlayer[0x8];                                        // +0x0a CIntelligentPointer<CThingMarker>
#endif
    long           InsideBuilding;                                          // +0x14
    EReverbEnvironmentType ReverbType;                                      // +0x18
    float          ReverbLevel;                                             // +0x1c
public:
    static void CheckRetailLayout() {
        FABLE_STATIC_ASSERT(offsetof(CAtmosProcess, AtmosMap) == 0x04);
        FABLE_STATIC_ASSERT(offsetof(CAtmosProcess, AtmosSoundsMuted) == 0x09);
        FABLE_STATIC_ASSERT(offsetof(CAtmosProcess, PRainPlayer) == 0x0a);
        FABLE_STATIC_ASSERT(offsetof(CAtmosProcess, InsideBuilding) == 0x14);
    }
};
FABLE_STATIC_ASSERT(sizeof(CAtmosProcess) == 0x20);
#endif // FABLE_ENGINE_CATMOSPROCESS_H
