#ifndef FABLE_ENGINE_CGAMEDEFINITIONMANAGER_H
#define FABLE_ENGINE_CGAMEDEFINITIONMANAGER_H
#include "engine/CDefinitionManager.h"
// Original class V and public CDefinitionManager base: both PDB name sets,
// native RTTI. Native ctor0044c6c2 constructs that base and zeroes d0..dc.
// Both donor TPI sets retain only a forward record for this class; therefore
// the extra native storage is deliberately unnamed until its fields are proven.
class CGameDefinitionManager : public CDefinitionManager {
public:
    static CGameDefinitionManager& __fastcall Get();
    CGameDefinitionManager();
    virtual ~CGameDefinitionManager();
    virtual void InitAndCompile();
#ifndef FABLE_RETAIL_CGAMEDEFINITIONMANAGER
    template<class T> bool GetDef(long,CDefPointer<T>&) const;
#endif // retail: no CGameDefinitionManager::GetDef in Ego_r.pdb / FableWin.pdb; callers reach CDefinitionManager::GetDef(long,CDefPointer<const T>*) (0x008EDFF0)
private:
    unsigned char _pad_0xd0[16];
};
FABLE_STATIC_ASSERT(sizeof(CGameDefinitionManager)==0xe0);
#endif
