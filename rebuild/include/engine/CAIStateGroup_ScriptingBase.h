#ifndef FABLE_ENGINE_CAISTATEGROUP_SCRIPTINGBASE_H
#define FABLE_ENGINE_CAISTATEGROUP_SCRIPTINGBASE_H
#include "engine/CAIStateGroupBase.h"
template<class TBase>class CAIStateGroup_ScriptingBase:public TBase {protected:void Fired();long NextFrameFireable;};
#endif
