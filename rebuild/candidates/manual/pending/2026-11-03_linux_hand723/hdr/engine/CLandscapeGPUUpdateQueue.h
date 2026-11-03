#ifndef FABLE_ENGINE_CLANDSCAPEGPUUPDATEQUEUE_H
#define FABLE_ENGINE_CLANDSCAPEGPUUPDATEQUEUE_H
// Hand-written (chunk 723): CLandscapeGPUUpdateQueue : CBaseClassNonCopyable : CBaseClass (RTTI). Retail size 4 (vptr only): Ego_r.pdb
// adds a list<CLandscapeGPUUpdateTask*> TaskList at +0x04, but retail's CEngineLandscapeRenderer places MeshBuilder right after it
// (~CEngineLandscapeRenderer 0x00B67920: `lea ecx,[esi+10h] ; call ~CEngineLandscapeMeshBuilder` then `lea ecx,[esi+0Ch] ; call
// ~CBaseClassNonCopyable`, no list teardown), so the TLC build has no TaskList. Destructor: the implicit one (no vtable store).
#include "rebuild_abi.h"
#include "engine/CBaseClassNonCopyable.h"
class CLandscapeGPUUpdateQueue : public CBaseClassNonCopyable {
};
FABLE_STATIC_ASSERT(sizeof(CLandscapeGPUUpdateQueue) == 0x4);

#endif // FABLE_ENGINE_CLANDSCAPEGPUUPDATEQUEUE_H
