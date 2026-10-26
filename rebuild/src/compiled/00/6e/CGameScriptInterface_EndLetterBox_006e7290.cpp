#include "engine/CGameScriptInterface.h"  // retyped onto the PDB layout; byte parity re-verified
class CDisplayEngine { public: void EndLetterBox(); };
extern "C" void __fastcall CGameScriptInterface_EndLetterBox(CGameScriptInterface* self)
{
    self->DisplayEngine->EndLetterBox();
}