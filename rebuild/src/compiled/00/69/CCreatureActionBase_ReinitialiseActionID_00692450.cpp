// Leaf: ?CCreatureActionBase_ReinitialiseActionID@@YIXPAVCCreatureActionBase@@@Z
#include "engine/CCreatureActionBase.h"  // retyped onto the PDB layout; byte parity re-verified
extern unsigned long g_actionid;
void __fastcall CCreatureActionBase_ReinitialiseActionID(CCreatureActionBase* action){ action->ActionID = g_actionid; ++g_actionid; }