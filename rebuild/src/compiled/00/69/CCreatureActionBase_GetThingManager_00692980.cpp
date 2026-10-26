#include "engine/CCreatureActionBase.h"  // retyped onto the PDB layout; byte parity re-verified
struct Sub; extern void __fastcall gtm_sub(Sub*);
class CThingManager; extern CThingManager* g_thingmgr;
CThingManager* __fastcall GetThingManager(CCreatureActionBase* action)
{
    gtm_sub(reinterpret_cast<Sub*>(reinterpret_cast<char*>(action) + 8));
    return g_thingmgr;
}