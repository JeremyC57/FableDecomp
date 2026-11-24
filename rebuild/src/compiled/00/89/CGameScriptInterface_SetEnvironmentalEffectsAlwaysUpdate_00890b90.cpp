// Leaf: ?SetEnvironmentalEffectsAlwaysUpdate@CGameScriptInterface@@UBEX_N@Z
#define FABLE_RETAIL_NSCRIPT   // alias route: engine/CGameScriptInterface.h declares SetEnvironmentalEffectsAlwaysUpdate under this class-model switch
#include "engine/CGameScriptInterface.h"  // retyped onto the PDB layout; byte parity re-verified
struct C { virtual void v0(); virtual void v1(); virtual void v2(); virtual void v3();
          virtual void v4(); virtual void v5(); virtual void SetX(bool); };
struct B { char pad[0x2c]; C* f2c; };
class CMainGameComponent { public: char pad[0x28]; B* PDisplayEngine; };
void CGameScriptInterface::SetEnvironmentalEffectsAlwaysUpdate(bool b) const{ Component->PDisplayEngine->f2c->SetX(b); }