// CTCSoundAtmosphere::FrameUpdate() -- when the sound manager and the world's atmos processor exist: for every atmosphere layer name of the component's CSoundAtmospheresDef, walk every instantiated SOUND_THEME def (class indices 1 .. count-1) and, where the theme's atmos-group map holds that layer name and GetGain(layer) is not -1, set that theme/layer channel to the gain at once (MixAtmos, time 0); then count the update timer down by one frame (1 / constant FPS)
// Leaf: ?FrameUpdate@CTCSoundAtmosphere@@UAEXXZ
// Flags: none
// Retail: Fable.exe (TLC, VC7.1) @ 0x007F53B0, 424 bytes, thiscall, no arguments, plain `ret`, no frame pointer, speed TU.
// Identity (proven, two sources): slot 9 (CTCBase::FrameUpdate) of BOTH the CTCSoundAtmosphere vtable 0x01271DFC and the
//   CTCSoundAtmosphereVillage vtable 0x0124111C (the village class does not override it); Ego_r.pdb / FableWin.pdb
//   ?FrameUpdate@CTCSoundAtmosphere@@UAEXXZ and neither PDB has a CTCSoundAtmosphereVillage::FrameUpdate. The manifest label
//   "CTCSoundAtmosphereVillage::FrameUpdate" names the wrong class.
// Callees: CTCBase::GetWorld() const 0x00686A70 (twice: the test, then the local); CCharString(const char*, long) 0x0099EBF0
//   ("SOUND_THEME" 0x01235E60), CCharString copy constructor 0x0099EC30, ~CCharString 0x0099EAE0; CGameDefinitionManager::Get
//   0x0044C6B0; CDefinitionManager::GetNoInstantiatedDefsOfType 0x009AD3B0; GetDef<CSoundThemeDef>(const CCharString&, long, ..)
//   0x006B25E0 (the template's body is visible, called out of line); the STLport tree's _M_find<CCharString> 0x0042B467 (called);
//   this->GetGain (virtual slot 28, +0x70); CDefClassBase::GetInstantiationName (def slot 10, +0x28) with the inline
//   CDefString::GetString (CDefStringTable::GetString 0x009D49B0 on GDefStringTable 0x013CA828); CAtmosProcess::MixAtmos 0x006B1900;
//   the def's virtual Delete (slot 1) on the last release. Globals: PSoundManager 0x013B8394, CMainGameComponent::ConstantFPS
//   0x01375550 (inline GetConstantFPS); floats -1.0f 0x0122DEE0, 1.0f 0x0122DED8.
// Shape: the atmos-group test is an inline taking the layer name BY VALUE: retail copy-constructs CCharString(*it) (0x007F5482),
//   only then reads the theme's map header word (end(), 0x007F548B), calls _M_find on the copy and destroys it before the compare
//   (`cmp ebp,ebx`). An explicit temporary at the call site reads end() before the copy (graded 422 v 424). The helper's original
//   name is in neither PDB (both list no such CSoundThemeDef member): HasAtmosGroup is a descriptive name. The tree's const find
//   body must be visible (FABLE_RETAIL_STLP_TREE_CONSTFIND_TEMPLATE_BODY) so only _M_find is called and the nodes are compared in
//   registers; with the GetDef<T> body visible (.._GETDEF_INDEX_BODY) the theme handle stays uncaptured: the def lives in esi, the
//   handle's frame word is reused for `gain`, and the release is `dec dword ptr [esi+4] ; jne` with no null test (0x007F5508).
// Layout: no shared header for CTCSoundAtmosphere / CSoundAtmospheresDef / CSoundThemeDef (the latter two exist only under
//   engine/_quarantine): local declarations on the shared CTCBase / CDefClassBase with the Ego_r.pdb member names at the retail
//   offsets -- TimeUntilNextUpdate +0x0C, PDef +0x10; CSoundAtmospheresDef::SoundAtmospheres (vector<CCharString>) +0x28;
//   CSoundThemeDef::AtmosGroupMap (multimap<CCharString,long>) +0x54.
// Headers: overlay stlport/fable_map.h FABLE_RETAIL_STLP_MULTIMAP_CONSTFIND (multimap's inline const find / end).
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CDEFSTRINGTABLE
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CRESOURCELIST
#define FABLE_RETAIL_CDEFINITIONMANAGER
#define FABLE_RETAIL_CDEFINITIONMANAGER_NOINSTANTIATED
#define FABLE_RETAIL_CGAMEDEFINITIONMANAGER
#define FABLE_RETAIL_CWORLD
#define FABLE_RETAIL_CASOUNDMANAGER_ATMOSMAP
#define FABLE_RETAIL_CDEFINITIONMANAGER_GETDEF_INDEX_BODY
#define FABLE_RETAIL_CMAINGAMECOMPONENT
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_STLP_TREE_MFIND_DECL
#define FABLE_RETAIL_STLP_MULTIMAP_CONSTFIND
#define FABLE_RETAIL_STLP_TREE_CONSTFIND_TEMPLATE_BODY
#include "fable_string.h"
#include "stlport/fable_containers.h"
#include "stlport/fable_map.h"
#include "engine/CDefString.h"
#include "engine/CDefClassBase.h"
#include "engine/CDefPointer.h"
#include "engine/CGameDefinitionManager.h"
#include "engine/CASoundManager.h"
#include "engine/CAtmosProcess.h"
#include "engine/CWorld.h"
#include "engine/CMainGameComponent.h"
#include "engine/CTCBase.h"

class CSoundThemeDef : public CDefClassBase {
public:
    unsigned char _pad[0x54 - sizeof(CDefClassBase)];
    // Inline, layer name by value (see Shape); descriptive name -- the original is in neither PDB.
    bool HasAtmosGroup(CCharString name) const { return AtmosGroupMap.find(name) != AtmosGroupMap.end(); }
    std::multimap<CCharString, long> AtmosGroupMap;         // +0x54
};
FABLE_STATIC_ASSERT(offsetof(CSoundThemeDef, AtmosGroupMap) == 0x54);
class CSoundAtmospheresDef : public CDefClassBase {
public:
    unsigned char _pad[0x28 - sizeof(CDefClassBase)];
    std::vector<CCharString> SoundAtmospheres;              // +0x28
};
FABLE_STATIC_ASSERT(offsetof(CSoundAtmospheresDef, SoundAtmospheres) == 0x28);

class CTCSoundAtmosphere : public CTCBase {
public:
    virtual void FrameUpdate();                             // 0x007F53B0
    virtual float GetGain(const CCharString& name);         // slot 28
    float TimeUntilNextUpdate;                              // +0x0C
    CDefPointer<const CSoundAtmospheresDef> PDef;           // +0x10
};

void CTCSoundAtmosphere::FrameUpdate()
{
    if (PSoundManager && GetWorld().PAtmosProcessor_Data) {
        CAtmosProcess* atmos = GetWorld().PAtmosProcessor_Data;
        for (std::vector<CCharString>::const_iterator it = PDef->SoundAtmospheres.begin(); it != PDef->SoundAtmospheres.end(); ++it) {
            long count = CGameDefinitionManager::Get().GetNoInstantiatedDefsOfType("SOUND_THEME");
            for (long i = 1; i < count; ++i) {
                CDefPointer<const CSoundThemeDef> theme;
                CGameDefinitionManager::Get().GetDef<CSoundThemeDef>("SOUND_THEME", i, &theme);
                if (theme->HasAtmosGroup(*it)) {
                    float gain = GetGain(*it);
                    if (gain != -1.0f)
                        atmos->MixAtmos(theme->GetInstantiationName().GetString(), *it, gain, 0.0f);
                }
            }
        }
    }
    TimeUntilNextUpdate -= 1.0f / CMainGameComponent::GetConstantFPS();
}