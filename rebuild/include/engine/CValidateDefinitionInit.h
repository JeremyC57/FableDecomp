#ifndef FABLE_ENGINE_CVALIDATEDEFINITIONINIT_H
#define FABLE_ENGINE_CVALIDATEDEFINITIONINIT_H
#include "engine/CValidateDefinitionInitBase.h"

class CGameDefinitionManager;
class CWorld;

// Anniversary TPI 0x7D39, Ego_d class references and retail 0x0068005B.
class CValidateDefinitionInit : public CValidateDefinitionInitBase
{
public:
    CValidateDefinitionInit(const CGameDefinitionManager&, const CWorld&);
    const CWorld& World;
};

FABLE_STATIC_ASSERT(sizeof(CValidateDefinitionInit) == 12);

#endif
