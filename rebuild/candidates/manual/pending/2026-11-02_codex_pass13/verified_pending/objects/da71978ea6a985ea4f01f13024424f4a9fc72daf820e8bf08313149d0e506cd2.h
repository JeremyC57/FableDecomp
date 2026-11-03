#ifndef FABLE_ENGINE_CSHADERBASE_RENDER2D_H
#define FABLE_ENGINE_CSHADERBASE_RENDER2D_H
#include "rebuild_abi.h"
class CCharString;

// Original Ego_r and FableWin declarations: evidence/009da9f0_*_types.json and *_dependencies.json.
class CShaderBase {
protected: CShaderBase();
protected: virtual ~CShaderBase();
protected: virtual CCharString GetDebugName() const;
private: virtual unsigned long CountReferences()=0;
};
#endif
