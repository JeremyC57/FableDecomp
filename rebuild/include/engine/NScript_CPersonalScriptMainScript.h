#ifndef FABLE_ENGINE_NSCRIPT_CPERSONALSCRIPTMAINSCRIPT_H
#define FABLE_ENGINE_NSCRIPT_CPERSONALSCRIPTMAINSCRIPT_H
#ifdef FABLE_RETAIL_PERSONALSCRIPT_MAIN_PASS26
#include "engine/CLevelScriptBase.h"
namespace NScript {
class CQ_SunnyvaleMasterData;
// The debug constructor identifies the specialization; the registered script's Main reads its two base members.
class CPersonalScriptMainScript : public CLevelScriptBase<CQ_SunnyvaleMasterData> {
public:
    virtual void Main();
};
}
#endif
#endif
