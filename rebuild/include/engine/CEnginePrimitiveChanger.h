#ifndef FABLE_ENGINE_CENGINEPRIMITIVECHANGER_H
#define FABLE_ENGINE_CENGINEPRIMITIVECHANGER_H
#ifdef FABLE_RETAIL_CENGINEPRIMITIVECHANGER_PASS22
#include "engine/CEnginePrimitive.h"
#include "engine/CIVCountedPointer.h"
#include "engine/CEngineMeshEffect.h"
class CEnginePrimitiveChanger:public CEnginePrimitive {
public:
 long ActiveIndex;
 long InitialSize;
 bool PreloadAll;
 CIVCountedPointer<CEngineMeshEffect> MeshEffect;
 CEnginePrimitiveChanger(long active_index,long initial_size,bool preload_all,EEngineInterpolationSet interp_set)
  :CEnginePrimitive((EEnginePrimitiveType)2,0,interp_set),ActiveIndex(active_index),InitialSize(initial_size),PreloadAll(preload_all){}
};
FABLE_STATIC_ASSERT(sizeof(CEnginePrimitiveChanger)==28);
#endif
#endif
// Retail opt-in (Codex pass22): original both-PDB class/constructor; actual inline draw constructor stores,all members retained.
