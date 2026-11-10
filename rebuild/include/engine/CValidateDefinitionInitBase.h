#ifndef FABLE_ENGINE_CVALIDATEDEFINITIONINITBASE_H
#define FABLE_ENGINE_CVALIDATEDEFINITIONINITBASE_H
#include "rebuild_abi.h"
class CDefinitionManager;
class CValidateDefinitionInit;
class CValidateDefinitionInitBase {
public:
 virtual ~CValidateDefinitionInitBase();
 const CDefinitionManager & Manager;
 CValidateDefinitionInitBase(const CValidateDefinitionInitBase & p0);
 CValidateDefinitionInitBase(const CDefinitionManager & p0);
};
FABLE_STATIC_ASSERT(sizeof(CValidateDefinitionInitBase)==8);
#endif
