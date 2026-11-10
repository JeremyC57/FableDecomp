// CAtmosProcess::Update() -- per-frame atmosphere update (when the sound system exists): while it rains, create the positional rain atmos marker on first need (its atmos player playing SOUND_THEME_RAIN), fade the RAIN atmos in once and keep the player 1 unit above the render camera; when the rain stops fade it out; then update the inside-building reverb and the day/night atmos sounds
// Leaf: ?Update@CAtmosProcess@@QAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x006B2260, 671 bytes, thiscall, plain `ret`.
// Identity (proven, two sources): Ego_r.pdb / FableWin.pdb ?Update@CAtmosProcess@@QAEXXZ (the class's only public no-argument
//   update); the landed caller CWorld::UpdateFrame 0x004A5A40 calls 0x006B2260 under this spelling on PAtmosProcessor, and the
//   body ends in the private UpdateInsideBuildings 0x006B1F30 / UpdateDayNightAtmosSounds 0x006B1A20. The catalog placeholder
//   "CGameScriptInterface_UpdateAtmosphere" is propagated.
// Callees: PSoundManager 0x013B8394 (null test only), CMainGameComponent::Get()->GetWorld() (inline), CWorld::GetEnvironment
//   (inline, +0x18), CEnvironment::GetRainStrength 0x006B2BD0 (> 0.0001f 0x0129BA3C), CBaseIntelligentPointer GetPItem 0x00A01B50 /
//   SetPItem 0x00A01B90 / ctor 0x00A01B10 / dtor 0x00A01C10 (CIntelligentPointer<CThingMarker> vtable 0x0125A860),
//   CGameDefinitionManager::Get 0x0044C6B0, CDefinitionManager::GetDefGlobalIndexFromName 0x009AD410 ("MARKER_POSITIONAL_ATMOS"
//   0x0125D4AC, "SOUND_THEME_RAIN" 0x0125D498), CWorld::GetCurrentRenderCamera() const (virtual slot 6, CCamera by value),
//   CThingMarker::Create 0x0083D460 (__fastcall), the expanded CThing::GetTC (CVectorMap LowerBound 0x0040F020, TCI_ATMOS_PLAYER
//   255), CTCAtmosPlayer::SetAtmosDefIndex 0x007E3610 / SetPos 0x007E35F0 (Ego_r donor order SetPos, SetAtmosDefIndex),
//   CAtmosProcess::MixAtmos 0x006B1900 ("RAIN" 0x0125D490), CCharString(const char*, long) 0x0099EBF0 / ~CCharString 0x0099EAE0.
// Shape: the rain theme index is a named local computed after the GetTC test (retail destroys the "SOUND_THEME_RAIN" temporary
//   before SetAtmosDefIndex and keeps the index in ebx, 0x006B23A3..0x006B23AA); with the call nested in the argument list the
//   index is pushed straight from eax and `this` moves from ebp to ebx. The common tail (UpdateInsideBuildings,
//   UpdateDayNightAtmosSounds) is written once; VC7.1 duplicates it into both arms as retail.
// Layout: no shared header for CAtmosProcess / CTCAtmosPlayer; the declarations below carry only what this body touches at the
//   landed ctor 0x006B1960's offsets (RainCurrentlyPlaying +0x08, the packed CIntelligentPointer<CThingMarker> PRainPlayer +0x0a).
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CWIDESTRING
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CCLASSFACTORY
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHING_GETTC_EXPANDED
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_CTHINGMARKER
#define FABLE_RETAIL_CTHINGMARKER_CREATE
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CINTELLIGENTPOINTER_ASSIGN
#define FABLE_RETAIL_CRESOURCELIST
#define FABLE_RETAIL_CDEFINITIONMANAGER
#define FABLE_RETAIL_CGAMEDEFINITIONMANAGER
#define FABLE_RETAIL_CWORLD
#define FABLE_RETAIL_CWORLD_ENVIRONMENT
#define FABLE_RETAIL_CENVIRONMENT
#define FABLE_RETAIL_CENVIRONMENT_RAINSTRENGTH
#define FABLE_RETAIL_CMAINGAMECOMPONENT
#define FABLE_RETAIL_CMAINGAMECOMPONENT_GETWORLD
#include "fable_string.h"
#include "engine/C3DVector.h"
#include "engine/ETCInterfaceType.h"
#include "engine/CThing.h"
#include "engine/CThingMarker.h"
#include "engine/CTCBase.h"
#include "engine/CIntelligentPointer.h"
#include "engine/CGameDefinitionManager.h"
#include "engine/CCamera.h"
#include "engine/CWorld.h"
#include "engine/CEnvironment.h"
#include "engine/CMainGameComponent.h"

class CTCAtmosPlayer : public CTCBase {
public:
    void SetPos(const C3DVector& position);                     // 0x007E35F0
    void SetAtmosDefIndex(long index);                          // 0x007E3610
};
class CASoundManager;
extern CASoundManager* PSoundManager;                           // 0x013B8394

class CMyAtmosMap {
public:
    virtual long GetBlockValue(const class C2DCoordI& block);   // 0x006B1580
};
#pragma pack(push, 1)
class CAtmosProcess {
public:
    void Update();
    void MixAtmos(const CCharString& from, const CCharString& to, float mix, float time);   // 0x006B1900
private:
    void UpdateDayNightAtmosSounds();                           // 0x006B1A20
    void UpdateInsideBuildings();                               // 0x006B1F30
public:
    long        DayNightState;                                  // +0x00
    CMyAtmosMap AtmosMap;                                       // +0x04
    bool        RainCurrentlyPlaying;                           // +0x08
    bool        AtmosSoundsMuted;                               // +0x09
    CIntelligentPointer<CThingMarker> PRainPlayer;              // +0x0a
};
#pragma pack(pop)

void CAtmosProcess::Update()
{
    if (PSoundManager) {
        if (CMainGameComponent::Get()->GetWorld().GetEnvironment().GetRainStrength() > 0.0001f) {
            if (PRainPlayer.Get() == 0) {
                long index = CGameDefinitionManager::Get().GetDefGlobalIndexFromName("MARKER_POSITIONAL_ATMOS");
                PRainPlayer = CIntelligentPointer<CThingMarker>(CThingMarker::Create(index, CMainGameComponent::Get()->GetWorld().GetCurrentRenderCamera().PeekWorldPos(), ""));
                CTCAtmosPlayer* player;
                if (PRainPlayer->GetTC(TCI_ATMOS_PLAYER, &player)) {
                    long theme = CGameDefinitionManager::Get().GetDefGlobalIndexFromName("SOUND_THEME_RAIN");
                    player->SetAtmosDefIndex(theme);
                }
            }
            if (!RainCurrentlyPlaying) {
                MixAtmos("SOUND_THEME_RAIN", "RAIN", 1.0f, 2000.0f);
                RainCurrentlyPlaying = true;
            }
            CTCAtmosPlayer* player;
            if (PRainPlayer->GetTC(TCI_ATMOS_PLAYER, &player)) {
                C3DVector position = CMainGameComponent::Get()->GetWorld().GetCurrentRenderCamera().PeekWorldPos();
                position.Z += 1.0f;
                player->SetPos(position);
            }
        } else if (RainCurrentlyPlaying) {
            MixAtmos("SOUND_THEME_RAIN", "RAIN", 0.0f, 2000.0f);
            RainCurrentlyPlaying = false;
        }
        UpdateInsideBuildings();
        UpdateDayNightAtmosSounds();
    }
}