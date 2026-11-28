#ifndef FABLE_ENGINE_CGLOBAL_DEBUGCYCLETHROUGHSPEECHSCRIPT_H
#define FABLE_ENGINE_CGLOBAL_DEBUGCYCLETHROUGHSPEECHSCRIPT_H
#include "engine/CLevelScriptNoMasterBase.h"
namespace NScript {
class CGlobal_DebugCycleThroughSpeechScript : public CLevelScriptNoMasterBase {
public:
    virtual void RegisterMain();
    virtual void Init();
    virtual void Main();
};
}
#endif
