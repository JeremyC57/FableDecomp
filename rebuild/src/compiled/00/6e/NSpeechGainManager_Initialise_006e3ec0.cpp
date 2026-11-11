// NSpeechGainManager::Initialise(const CWorld& world) -- reset the speech gain manager for a new world: clear the last update frame, size the per-layer speaker counts and response-rate tables to the three dialogue layers, drop every active dialogue line, and turn each layer's attack / release times from the global def into per-frame gain deltas
// Leaf: ?Initialise@NSpeechGainManager@@YIXABVCWorld@@@Z
// Inline-budget: FABLE_FORCEINLINE on the header's CResponseTimes(float, float) (engine/NSpeechGainManager.h) -- retail expands the
//   constructor at all three sites (0x006E3F6E, 0x006E3FDE, 0x006E4051) yet calls vector<CResponseTimes>::erase 0x006E4EA0 from the
//   inline resize; the plain inline constructor stays a call, and the threshold scan shows no flag can split them (default / T75:
//   both called; /d2inlT77: erase expanded, constructor called; T80 / T100: both expanded), so the constructor is forced.
// Retail: Fable.exe (TLC, VC7.1) @ 0x006E3EC0, 526 bytes (+2 int3), `__fastcall` (ecx = world), plain `ret`.
// Identity: Ego_r.pdb / FableWin.pdb ?Initialise@NSpeechGainManager@@YIXABVCWorld@@@Z (namespace function, const CWorld&); its only
//   caller is 0x004A76C0 (world setup). Same TU as the landed NSpeechGainManager::Update 0x006E37D0 / UpdateDialogueElement
//   0x006E3440 / GetTargetGain 0x006E2B70 (the statics below). The catalog's "CWorld_InitGuiTreePane" is a placeholder.
// Callees: vector<unsigned short>::_M_fill_insert 0x005149F0 (the shared COMDAT), _List_base<CActiveDialogueInfo>::clear
//   0x006E4B90, vector<CResponseTimes>::erase(first, last) 0x006E4EA0 and ::_M_fill_insert 0x006E5240 (both inside the inline resize).
// Layout: CWorld::DefinitionManager +0x10, CGameDefinitionManager global def +0xD0 (PeekGlobalDef, inline), CGlobalDef::DialogueLayers
//   +0x10C (retail; PDB +0x118), CDialogueLayerDef 0x40 bytes (attack +0x28, release +0x2C).
// Shape: STLport's inline resize for both tables (the unsigned short shrink folds to `finish = start + 6`); the three response
//   entries are explicit statements assigning a CResponseTimes temporary (built at [esp+4]/[esp+8], copied by two dword moves
//   with the vector start re-read); the release delta is `frame * (-1 / release)` (retail `fld -1.0; fdiv st(1); fmul frame`;
//   `-1 / release * frame` is reassociated to `frame / release * -1`).
// Headers: FABLE_HDR_OVERLAY=$A/hdr (CGameDefinitionManager.h, _quarantine/CGlobalDef.h opt-ins, new engine/NSpeechGainManager.h).
#define FABLE_RETAIL_CWORLD
#define FABLE_RETAIL_CMAINGAMECOMPONENT
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CDEFINITIONMANAGER
#define FABLE_RETAIL_CRESOURCELIST
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_CTCBOASTINGCROWDCONTROL
#define FABLE_RETAIL_CGAMEDEFINITIONMANAGER_GLOBALDEF
#define FABLE_RETAIL_CGLOBALDEF_DIALOGUELAYERS
#define FABLE_RETAIL_STLP_LIST_CLEAR_OUTOFLINE
#include "engine/NSpeechGainManager.h"
#include "engine/_quarantine/CGlobalDef.h"
#include "engine/CGameDefinitionManager.h"
#include "engine/CWorld.h"

namespace NSpeechGainManager {

enum { DIALOGUE_LAYER_COUNT = 3 };

void __fastcall Initialise(const CWorld& world)
{
    g_LastUpdateFrame = 0;
    g_LayerSpeakerCounts.resize(DIALOGUE_LAYER_COUNT, 0);
    g_ActiveDialogue.clear();
    g_ResponseTimes.resize(DIALOGUE_LAYER_COUNT);
    const std::vector<CDialogueLayerDef>& layers = world.PeekDefinitionManager().PeekGlobalDef().DialogueLayers;
    g_ResponseTimes[0] = CResponseTimes(layers[0].ResponseTimeSecsAttack, layers[0].ResponseTimeSecsRelease);
    g_ResponseTimes[1] = CResponseTimes(layers[1].ResponseTimeSecsAttack, layers[1].ResponseTimeSecsRelease);
    g_ResponseTimes[2] = CResponseTimes(layers[2].ResponseTimeSecsAttack, layers[2].ResponseTimeSecsRelease);
}

}