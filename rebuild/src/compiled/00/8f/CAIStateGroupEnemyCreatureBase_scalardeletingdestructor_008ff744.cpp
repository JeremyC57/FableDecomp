// CAIStateGroup_EnemyCreatureBase::`scalar deleting destructor' (free __fastcall stand-in, unchanged leaf) -- the implicit ~CAIStateGroup_EnemyCreatureBase, then
//   operator delete when bit 0 of flags is set; returns this.
// Leaf: ?CAIStateGroup_EnemyCreatureBase_ScalarDeletingDestructor@@YIPAXPAXHI@Z
// NF471 (T3 nf303 0x008FD288, callee-only respell, ruling r470a): the call at +3 (retail 0x008FF747 -> 0x008FD769) now names the real implicit
//   destructor ??1CAIStateGroup_EnemyCreatureBase@@UAE@XZ on the shared retail class (engine/CAIStateGroup_EnemyCreatureBase.h, retail branch FABLE_RETAIL_CAISTATEGROUP_ENEMYCREATUREBASE; Ego_r / FableWin type records flag
//   ~CAIStateGroup_EnemyCreatureBase compiler-generated), not the free stand-in (HEAD declared it on a local one-member model; the symbol was already this one, now the object also emits the copy). VC7.1 emits that destructor here as a
//   pick-any COMDAT byte- and relocation-equal to retail 0x008FD769 (PTargetThing +0x14, then ~CAIStateGroupBase 0x008FD288).
#pragma optimize("s", on)
#define FABLE_RETAIL_CAISTATEGROUPBASE
#define FABLE_RETAIL_CAISTATEGROUPBASE_MEMBERS
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_CAISTATEGROUP_ENEMYCREATUREBASE

class CThing; class CWorld; class CThingAICreature; class CThingObject; class CThingCreatureBase; class CCreatureActionBase;
#include "engine/CAIStateGroupBase.h"
#include "fable_string.h"
#include "engine/C3DVector.h"
#include "engine/CIntelligentPointer.h"
#include "engine/CAIStateGroup_EnemyCreatureBase.h"
void* __fastcall CAIStateGroup_EnemyCreatureBase_ScalarDeletingDestructor(void* stateGroup, int, unsigned int flags)
{
    static_cast<CAIStateGroup_EnemyCreatureBase*>(stateGroup)->CAIStateGroup_EnemyCreatureBase::~CAIStateGroup_EnemyCreatureBase();
    if (flags & 1)
        operator delete(stateGroup);
    return stateGroup;
}
