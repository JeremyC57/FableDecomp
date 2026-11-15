#ifndef FIXTURE_DEFINITION_MANAGER_H
#define FIXTURE_DEFINITION_MANAGER_H
#include "engine/CDefClassBase.h"
class CDefinitionManager {public:CDefClassBase*Effect;CDefinitionManager():Effect(0){}CDefPointer<CDefClassBase>GetPDefFromGlobalIndex(long n){assert(n==19&&Effect);Effect->AddRef();return CDefPointer<CDefClassBase>(Effect);}};
#endif
