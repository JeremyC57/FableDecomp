#ifndef FABLE_ENGINE_CWORLDINIT_H
#define FABLE_ENGINE_CWORLDINIT_H
#include "rebuild_abi.h"
class CMainGameComponent; class CWorld; class CPlayerManager;
class CGameDefinitionManager; class CDisplayEngine;
// Both original PDBs complete CWorldInit: five pointers, public fields, size20.
class CWorldInit { public:
 CWorldInit();
 CMainGameComponent* Component;
 const CWorld* ScavengeWorld;
 CPlayerManager* PlayerManager;
 const CGameDefinitionManager* DefinitionManager;
 CDisplayEngine* DisplayEngine;
};
FABLE_STATIC_ASSERT(sizeof(CWorldInit)==20);
#endif
