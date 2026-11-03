// CUserProfileManager::CUserProfileManager() -- members per Ego_r.pdb (profile name, vectors, tutorial queue, 50 save-game names, score-board strings), then the display-mode list: enumerate the display manager's modes from the config defaults' minimum resolution (1024x768 without a def) and keep one entry per size and depth; then SetDefaultValues / SetDefaultValuesForNewProfile, the profile name "Default", and registration as a device-reset callback
// Grade: DIFFER(1015v977) -- retail calls std::__uninitialized_copy<CDisplayModeDesc*> 0x0040DDC0 out of line inside the inlined first push_back growth (and leaves the second push_back's _M_insert_overflow 0x004124C0 out of line); VC7.1 here inlines the copy, which shifts the small stack temporaries by one slot (122 diff lines). With the copy out of line (FABLE_STLP_OUTOFLINE_UNINIT_COPY) VC7.1 also inlines the second growth (1164 bytes). 10 attempts: the same-TU 0x004083E0 body fixed the def release (dec [ecx+4] form, no zero store), C2DExtentsI's FableWin user copy ctor gave the in-place by-value arguments, a scoped named "Default" CWideString gave the lea/dtor order, modes before def and the for-scoped iterator the frame.
// Leaf: ??0CUserProfileManager@@QAE@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x0040CEC0, 977 bytes, thiscall, no EH frame; only caller CTBaseSingleton<CUserProfileManager>::Get 0x0040D2A0.
// Identity: Ego_r.pdb / FableWin.pdb ??0CUserProfileManager@@QAE@XZ.
// Callees: CBaseClassNonCopyable ctor 0x0099A2F0, CWideString ctors 0x0099AED0 / 0x0099B720 (copy) / 0x0099B6B0 (const wchar_t*),
//   ~CWideString 0x0099B510, operator= 0x0099B7D0, _Deque_base<ETutorialCategory>::_M_initialize_map 0x0040F4A0, malloc / free,
//   GetConfigOptionsDefaultsDef 0x004083E0, GFGetSystemManager 0x009A4EC0, CDisplayManager::EnumerateDisplayModes 0x009C02D0 /
//   AddDeviceResetCallback 0x009C0190, __uninitialized_copy<CDisplayModeDesc> 0x0040DDC0, vector<CDisplayModeDesc>::
//   _M_insert_overflow 0x004124C0, SetDefaultValues 0x0040C790, SetDefaultValuesForNewProfile 0x00409B70.
// Header work (hdr/): fable_user_profile.h FABLE_RETAIL_CUSERPROFILEMANAGER_MEMBERS (the opaque blocks as Ego_r members),
//   stlport/fable_containers.h FABLE_STLP_DEQUE (+ the copy opt-in), engine/C2DExtentsI.h FABLE_RETAIL_C2DEXTENTSI,
//   engine/CDisplayManager.h EnumerateDisplayModes / AddDeviceResetCallback.
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CUSERPROFILEMANAGER
#define FABLE_RETAIL_CSTARTUPDEFINITIONMANAGER
#define FABLE_RETAIL_CGAMECOMPONENT
#define FABLE_RETAIL_CMAINGAMECOMPONENT
#define FABLE_RETAIL_CRESOURCELIST
#define FABLE_RETAIL_CDEFINITIONMANAGER
#define FABLE_RETAIL_CUSERPROFILEMANAGER_MEMBERS
#define FABLE_STLP_DEQUE
#define FABLE_RETAIL_C2DEXTENTSI
#define FABLE_RETAIL_CARRAY
#define FABLE_RETAIL_CDISPLAYMANAGER
#define FABLE_RETAIL_CSYSTEMMANAGER
#define FABLE_RETAIL_CTEXTURE
#define FABLE_RETAIL_CPIXELFORMAT
#define FABLE_RETAIL_C2DBOXF
#define FABLE_RETAIL_SCOPED_PTR
#include "stlport/fable_containers.h"
#include "fable_string.h"
#include "fable_user_profile.h"
#include "engine/CParentDefClassBase.h"
#include "engine/CDefPointer.h"
#include "engine/CArray.h"
#include "engine/CSystemManager.h"
#include "engine/CDisplayManager.h"
#include "fable_game.h"
#include "engine/CGameDefinitionManager.h"
#include "engine/CStartupDefinitionManager.h"


class CNewFrontendGameComponent {
private:
    static CNewFrontendGameComponent* m_pNewFrontendGameComponent;   // 0x013B871C (bound spelling; the data symbol's PDB name is unproven)
public:
    static CNewFrontendGameComponent* __fastcall Get() { return m_pNewFrontendGameComponent; }   // FableWin ?Get@CNewFrontendGameComponent@@SIPAV1@XZ (inline)
};

class CConfigOptionsDefaultsDef : public CParentDefClassBase {      // Ego_r.pdb members at retail offsets (Ego_r - 4), sizeof 0x74
public:
    unsigned long Antialiasing;                                      // +0x3c
    long          ResolutionWidth;                                   // +0x40
    long          ResolutionHeight;                                  // +0x44
    long          BitDepth;                                          // +0x48
    long          MinResolutionWidth;                                // +0x4c
    long          MinResolutionHeight;                               // +0x50
    float         TextureDetail;                                     // +0x54
    float         MaxTextureDetail;                                  // +0x58
    float         ShadowDetail;                                      // +0x5c
    float         MaxShadowDetail;                                   // +0x60
    float         MeshDetail;                                        // +0x64
    float         MaxMeshDetail;                                     // +0x68
    float         EffectsDetail;                                     // +0x6c
    float         MaxEffectsDetail;                                  // +0x70
};

template<class T> bool CDefinitionManager::GetDef(long index, CDefPointer<const T>* result) const   // 0x00410890 for T = CConfigOptionsDefaultsDef
{
    if (index <= 0) return false;
    CDefPointer<const CDefClassBase> definition = GetPDefFromGlobalIndex(index);
    if (!definition.IsNull()) { result->SetDynamicCast(definition); return true; }
    return false;
}

CDefPointer<const CConfigOptionsDefaultsDef> CUserProfileManager::GetConfigOptionsDefaultsDef() const
{
    CDefPointer<const CConfigOptionsDefaultsDef> result;
    if (CMainGameComponent::Get())
    {
        long index = CGameDefinitionManager::Get().GetGlobalIndexFromClassIndex("CONFIG_OPTIONS_DEFAULTS_DEF", 1);
        CGameDefinitionManager::Get().CDefinitionManager::GetDef(index, &result);
    }
    else if (CNewFrontendGameComponent::Get())
    {
        long index = CStartupDefinitionManager::Get().GetGlobalIndexFromClassIndex("CONFIG_OPTIONS_DEFAULTS_DEF", 1);
        CStartupDefinitionManager::Get().GetDef(index, &result);
    }
    return result;
}

// Declaration-only explicit specialisation of the growth (0x004124C0): its body is in another TU, so neither push_back expands it.
template<> void std::vector<CDisplayModeDesc, std::allocator<CDisplayModeDesc> >::_M_insert_overflow(CDisplayModeDesc* position,
    const CDisplayModeDesc& x, const std::__false_type&, unsigned int fillLength, bool atEnd);
CUserProfileManager::CUserProfileManager()
    : UseNewProfileValues(false), SoundsMuted(false), ApplyVideoValuesOnLoad(true), DisplayDialogOnOptionsChanged(true),
      SaveGameNames(50), DisplayTutorialsDuringCutscenes(false), VideoChangesAccepted(true)
{
    CArray<CDisplayModeDesc> modes;
    CDefPointer<const CConfigOptionsDefaultsDef> def = GetConfigOptionsDefaultsDef();
    C2DExtentsI minResolution(1024, 768);
    if (!def.IsNull())
        minResolution = C2DExtentsI(def->MinResolutionWidth, def->MinResolutionHeight);
    GFGetSystemManager()->GetDisplayManager()->EnumerateDisplayModes(modes, minResolution, C2DExtentsI(0x7fff, 0x7fff));
    CDisplayModeDesc previous = *modes.begin();
    for (CArray<CDisplayModeDesc>::iterator it = modes.begin(); it != modes.end(); ++it) {
        if (it->Dimensions.X != previous.Dimensions.X || it->Dimensions.Y != previous.Dimensions.Y || it->BitsPerPixel != previous.BitsPerPixel)
            DisplayModes.push_back(previous);
        previous = *it;
    }
    if (!DisplayModes.empty()) {
        const CDisplayModeDesc& last = DisplayModes.back();
        if (last.Dimensions.X != previous.Dimensions.X || last.Dimensions.Y != previous.Dimensions.Y || last.BitsPerPixel != previous.BitsPerPixel)
            DisplayModes.push_back(previous);
    }
    SetDefaultValues();
    SetDefaultValuesForNewProfile();
    {
        CWideString defaultProfileName(L"Default");
        ProfileName = defaultProfileName;
    }
    PBackBufferTexture = 0;
    GFGetSystemManager()->GetDisplayManager()->AddDeviceResetCallback(this);
}
