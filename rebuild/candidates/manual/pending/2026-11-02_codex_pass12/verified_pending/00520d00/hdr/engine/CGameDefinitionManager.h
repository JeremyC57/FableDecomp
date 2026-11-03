#ifndef FABLE_ENGINE_CGAMEDEFINITIONMANAGER_H
#define FABLE_ENGINE_CGAMEDEFINITIONMANAGER_H
#include "engine/CDefinitionManager.h"
// Original class V and public CDefinitionManager base: both PDB name sets,
// native RTTI. Native ctor0044c6c2 constructs that base and zeroes d0..dc.
// Both donor TPI sets retain only a forward record for this class; therefore
// the extra native storage is deliberately unnamed until its fields are proven.
class CGameDefinitionManager : public CDefinitionManager {
public:
#if defined(FABLE_RETAIL_HERO_LOGBOOK) || defined(FABLE_RETAIL_UI_FACTORY) || defined(FABLE_RETAIL_ITEMS_LIST)
    static CGameDefinitionManager& __fastcall Get();
#endif
    CGameDefinitionManager();
    virtual ~CGameDefinitionManager();
    virtual void InitAndCompile();
    template<class T> bool GetDef(long,CDefPointer<T>&) const;
#if defined(FABLE_RETAIL_FACTIONS) || defined(FABLE_RETAIL_CTHINGMANAGER_LOAD) || defined(FABLE_RETAIL_ITEMS_LIST) || defined(FABLE_RETAIL_UI_FACTORY)
    using CDefinitionManager::GetDef;
#endif
private:
    unsigned char _pad_0xd0[16];
};
FABLE_STATIC_ASSERT(sizeof(CGameDefinitionManager)==0xe0);
#endif
