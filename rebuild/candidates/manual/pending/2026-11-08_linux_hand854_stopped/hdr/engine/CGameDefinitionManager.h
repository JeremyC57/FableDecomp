#ifndef FABLE_ENGINE_CGAMEDEFINITIONMANAGER_H
#define FABLE_ENGINE_CGAMEDEFINITIONMANAGER_H
#include "engine/CDefinitionManager.h"
// Original class V and public CDefinitionManager base: both PDB name sets,
// native RTTI. Native ctor0044c6c2 constructs that base and zeroes d0..dc.
// Both donor TPI sets retain only a forward record for this class; therefore
// the extra native storage is deliberately unnamed until its fields are proven.
#ifdef FABLE_RETAIL_CGAMEDEFINITIONMANAGER_HEROSTATSDEF
class CHeroStatsDef;
#endif
#ifdef FABLE_RETAIL_CGAMEDEFINITIONMANAGER_GLOBALDEF
class CGlobalDef;                                                        // chunk 854
#endif
class CGameDefinitionManager : public CDefinitionManager {
public:
    static CGameDefinitionManager& __fastcall Get();
    CGameDefinitionManager();
    virtual ~CGameDefinitionManager();
    virtual void InitAndCompile();
#ifndef FABLE_RETAIL_CGAMEDEFINITIONMANAGER
    template<class T> bool GetDef(long,CDefPointer<T>&) const;
#endif // retail: no CGameDefinitionManager::GetDef in Ego_r.pdb / FableWin.pdb; callers reach CDefinitionManager::GetDef(long,CDefPointer<const T>*) (0x008EDFF0)
#ifdef FABLE_RETAIL_CGAMEDEFINITIONMANAGER_HEROSTATSDEF
    // Nested opt-in (chunk 794, row 0x007039A0 CThingObject::OnHit): the hero-stats definition the native ctor zeroes in the d0..dc block,
    // read at +0xDC by CTCHeroStats::ApplyPenaltyForDeath 0x0057DE14 (landed) and by OnHit (`mov edx,[eax+0DCh]` off PeekDefinitionManager).
    // No PDB layout records this class (forward record only), so the member name is descriptive. No layout change.
private:
    unsigned char _pad_0xd0[12];
public:
    const CHeroStatsDef* PHeroStatsDef;                                  // +0xdc (descriptive name)
#elif defined(FABLE_RETAIL_CGAMEDEFINITIONMANAGER_GLOBALDEF)
    // Nested opt-in (chunk 854, row 0x00507070 CWorldMap::BuildStaticMapGenerationVisibilityInfo): the global definition the native
    // ctor zeroes first in the d0..dc block (`mov eax,[eax+0D0h] ; mov ecx,[eax+58h]` off PeekDefinitionManager = CGlobalDef
    // StaticMapGenerationEstimatedMaxCameraHeightAboveLandscape; the landed 0x006E37D0 / 0x008F71A0 local models read the same +0xD0).
    // No PDB layout records this class (forward record only): member and accessor names descriptive. No layout change.
public:
    const CGlobalDef& PeekGlobalDef() const { return *PGlobalDef; }
private:
    const CGlobalDef* PGlobalDef;                                        // +0xd0 (descriptive name)
    unsigned char _pad_0xd4[12];
#else
private:
    unsigned char _pad_0xd0[16];
#endif
};
FABLE_STATIC_ASSERT(sizeof(CGameDefinitionManager)==0xe0);
#endif
