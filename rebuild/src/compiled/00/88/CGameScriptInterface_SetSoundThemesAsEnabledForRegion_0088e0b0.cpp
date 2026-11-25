#include <cstddef>

class CCharString;
class CWorld;
class CCategory;

namespace NInventory
{
class CTCInventoryBase
{
public:
};
}

// Naming (NF438 gen, 2026-11-25): callee-only rebind of this row's calls to their PDB spellings (T1 tracker; the target
//   self rows' Leaf / PDB name sets), through a bodiless declaration (no layout; rulings r2/r3):
//   0x004FB490 ?GetRegion@CWorldMap@@QAEAAVCRegion@@J@Z (was ?GetCategoryWithIndex@CTCInventoryBase@NInventory@@QAEPAVCCategory@@J@Z)
class CRegion;
class CWorldMap
{
public:
    long GetRegionNumberFromName(const CCharString& regionName) const;   // 0x004FC210 (NF313: the PDB QBE const-reference form, both PDBs)
 public: CRegion& GetRegion(long); };

namespace
{
struct CWorldVTableOverlay
{
    unsigned char m_Pad00[0x30];
    NInventory::CTCInventoryBase* (__fastcall* m_GetTCInventoryBase)(CWorld*); // 0x30
    CWorldMap* (__fastcall* m_GetWorldMap)(CWorld*);                          // 0x34
};

struct CWorldOverlay
{
    CWorldVTableOverlay* m_pVTable; // 0x00
};

struct CGameScriptInterfaceOverlay
{
    unsigned char m_Pad00[0x04];
    CWorld* m_pWorld; // 0x04
};

struct CCategorySoundThemesEnabledOverlay
{
    unsigned char m_Pad00[0x56];
    unsigned char m_SoundThemesEnabled; // 0x56
};
}

// CGameScriptInterface is the retail game-script interface from the shared headers (FABLE_RETAIL_NSCRIPT opt-in:
//   CGameScriptInterface : CGameScriptInterfaceBase : CBaseClassNonCopyable : CBaseClass, vtable 0x01260F0C, 918 slots).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CGameScriptInterface.h"

void CGameScriptInterface::SetSoundThemesAsEnabledForRegion(
    const CCharString& theName,
    bool enabled) const
{
    const CGameScriptInterfaceOverlay* const self =
        reinterpret_cast<const CGameScriptInterfaceOverlay*>(this);

    const CCharString* const pName = &theName;
    CWorld* const pWorld = self->m_pWorld;
    const long regionNumber =
        reinterpret_cast<CWorldOverlay*>(pWorld)->m_pVTable->m_GetWorldMap(pWorld)
            ->GetRegionNumberFromName(*pName);
    if (regionNumber > 0)
    {
        CWorld* const pWorld2 = self->m_pWorld;
        CCategory* const pCategory =
            reinterpret_cast<CCategory*>(&reinterpret_cast<CWorldMap*>(reinterpret_cast<CWorldOverlay*>(pWorld2)->m_pVTable->m_GetTCInventoryBase(pWorld2))->GetRegion(regionNumber));
        reinterpret_cast<CCategorySoundThemesEnabledOverlay*>(pCategory)->m_SoundThemesEnabled =
            static_cast<unsigned char>(enabled);
    }
}