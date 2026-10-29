// CRegion::SetRegionDefName(const CCharString&) @ 0x006BC1D0 (retail): store the def name (+0x20, CCharString::operator=
// @ 0x0099EFB0), look its global def index up (CDefinitionManager::GetDefGlobalIndexFromName @ 0x009AD410 on the
// singleton from CDefinitionManager::Get @ 0x0044C6B0 -- the name argument is pushed before the Get call, which takes
// none) and, for a positive index, fetch the def into the def pointer at +0x24 (CDefinitionManager::GetDef<T>(long,
// CDefPointer&) @ 0x006BD060, again on Get()). __thiscall shape modelled as __fastcall with a dead edx.
// Engine headers for the callee classes (PDB data layouts, renamed out of the way): the local declarations
// below carry the retail identities the relocations bind.
#define CDefinitionManager CDefinitionManagerPdb
#include "engine/CDefinitionManager.h"
#undef CDefinitionManager
class CCharString { public:
    void* PStringData;
    const CCharString& operator=(const CCharString& other);           // 0x0099EFB0
};
struct CDefPointer { void* PDef; };
class CDefinitionManager { public:
    long GetDefGlobalIndexFromName(const CCharString& name) const;     // 0x009AD410
    bool GetDef(long index, CDefPointer& out);                   // 0x006BD060
};
class CGameDefinitionManager : public CDefinitionManager { public: static CGameDefinitionManager& __fastcall Get(); };   // 0x0044C6B0
class CRegion { public:
    char pad0[0x20];
    CCharString DefName;     // at +0x20
    CDefPointer PDef;        // at +0x24
};
void __fastcall CRegion_SetRegionDefName(CRegion* region, int /*edx*/, const CCharString& name)
{
    region->DefName = name;
    long index = CGameDefinitionManager::Get().GetDefGlobalIndexFromName(region->DefName);
    if (index > 0) {
        CGameDefinitionManager::Get().GetDef(index, region->PDef);
    }
}
