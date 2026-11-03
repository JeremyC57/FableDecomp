// CUserProfileManager::GetMaxMeshDetail() const -- the highest selectable level: the config-detection cap when it set one, else the config-options defaults def's MaxMeshDetail, else 0
// Leaf: ?GetMaxMeshDetail@CUserProfileManager@@QBEMXZ
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004085A0, 67 bytes, thiscall, plain `ret` (float in st0).
// Identity: Ego_r.pdb / FableWin.pdb ?GetMaxMeshDetail@CUserProfileManager@@QBEMXZ (Ego_r 0x00406E00). The four 67-byte getters 0x004084B0 /
//   0x00408500 / 0x00408550 / 0x004085A0 (all labelled "GetMaxMeshDetail" in the catalog: propagated) read CConfigOptionsDefaultsDef
//   +0x70 / +0x58 / +0x60 / +0x68 = MaxEffectsDetail / MaxTextureDetail / MaxShadowDetail / MaxMeshDetail (Ego_r.pdb members, retail
//   = Ego_r - 4), and Ego_r lists GetMaxEffectsDetail 0x00406D40, GetMaxTextureDetail 0x00406D80, GetMaxShadowDetail 0x00406DC0,
//   GetMaxMeshDetail 0x00406E00 in that order: field and order agree.
// Callees: ?GetConfigOptionsDefaultsDef@CUserProfileManager@@QBE?AV?$CDefPointer@$$CBVCConfigOptionsDefaultsDef@@@@XZ 0x004083E0
//   (`this` passes through in ecx; the result comes back in the one-word frame slot), the def's Delete (vtable slot 1) on the last
//   release. Data: the cap 0x01375070 (GFConfigDetection 0x004017B0 writes MaxDetail - 1 there; established spelling), 0.0f 0x0122DEDC.
// Lever: this file holds the retail TU's 0x004083E0 body (graded RELOCATION_MATCH at 0x004083E0 from this same file) and the
//   CDefinitionManager::GetDef<T> template it calls. With 0x004083E0 only declared, VC7.1 assumes the callee may keep the result
//   slot's address and gives the copied member a second frame word (84v67, 14 probes); with the body in the TU it reuses the def's
//   slot as retail does. Non-null path first (GOTCHAS chunk 651).
// Layout: fable_user_profile.h FABLE_RETAIL_CUSERPROFILEMANAGER (chunk 719 adds the five member declarations), engine/CDefPointer.h,
//   engine/CParentDefClassBase.h; CConfigOptionsDefaultsDef restated (only engine/_quarantine has it) on the retail def chain.
//   0x013B871C is read through FableWin's inline CNewFrontendGameComponent::Get() (bound static spelling; PDB data name unproven).
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CGAMECOMPONENT
#define FABLE_RETAIL_CMAINGAMECOMPONENT
#define FABLE_RETAIL_SCOPED_PTR
#define FABLE_RETAIL_CRESOURCELIST
#define FABLE_RETAIL_CDEFINITIONMANAGER
#define FABLE_RETAIL_CUSERPROFILEMANAGER
#define FABLE_RETAIL_CSTARTUPDEFINITIONMANAGER
#include "fable_string.h"
#include "fable_game.h"
#include "fable_user_profile.h"
#include "engine/CParentDefClassBase.h"
#include "engine/CDefPointer.h"
#include "engine/CGameDefinitionManager.h"
#include "engine/CStartupDefinitionManager.h"                       // retail branch: public CDefinitionManager base + Get (hdr/)
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

extern long g_MaxDetailA;                                            // 0x0137507C config-detection cap: max effects detail (-1 = none; established spelling)
extern long g_MaxDetailD;                                            // 0x01375074 max texture detail
extern long g_MaxDetailB;                                            // 0x01375078 max shadow detail
extern long g_MaxDetailC;                                            // 0x01375070 max mesh detail

// The retail TU's own out-of-line callee (0x004083E0, landed): defined here so VC7.1 sees that it does not keep the address of
// the caller's result slot (the lever: with only a declaration the caller gives the copied member its own frame word).
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

float CUserProfileManager::GetMaxEffectsDetail() const
{
    if (g_MaxDetailA != -1)
        return (float)g_MaxDetailA;
    CDefPointer<const CConfigOptionsDefaultsDef> def = GetConfigOptionsDefaultsDef();
    if (!def.IsNull())
        return def->MaxEffectsDetail;
    return 0.0f;
}

float CUserProfileManager::GetMaxTextureDetail() const
{
    if (g_MaxDetailD != -1)
        return (float)g_MaxDetailD;
    CDefPointer<const CConfigOptionsDefaultsDef> def = GetConfigOptionsDefaultsDef();
    if (!def.IsNull())
        return def->MaxTextureDetail;
    return 0.0f;
}

float CUserProfileManager::GetMaxShadowDetail() const
{
    if (g_MaxDetailB != -1)
        return (float)g_MaxDetailB;
    CDefPointer<const CConfigOptionsDefaultsDef> def = GetConfigOptionsDefaultsDef();
    if (!def.IsNull())
        return def->MaxShadowDetail;
    return 0.0f;
}

float CUserProfileManager::GetMaxMeshDetail() const
{
    if (g_MaxDetailC != -1)
        return (float)g_MaxDetailC;
    CDefPointer<const CConfigOptionsDefaultsDef> def = GetConfigOptionsDefaultsDef();
    if (!def.IsNull())
        return def->MaxMeshDetail;
    return 0.0f;
}