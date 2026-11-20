// CFrontendGameComponent::ClearString @ 0x004944C0 -- clear the front end's text-input string (String[128] +0x168) and its
// length (StringPos +0x1E8).
// Leaf: ?ClearString@CFrontendGameComponent@@AAEXXZ
// Naming (NF191, 2026-11-20, q6283): Ego_r.pdb 0x0049D0E0 + FableWin.pdb 0x01A46A80 ?ClearString@CFrontendGameComponent@@AAEXXZ
//   (private; the Ego_r.pdb layout puts char String[128] at +0x168 and long StringPos at +0x1E8, as this body writes). Was the
//   uncataloged stand-in CFrontendGameComponent_004944c0::ClearTextInput on a row-local layout; now on the shared
//   engine/CFrontendGameComponent.h (opt-in FABLE_RETAIL_CFRONTENDGAMECOMPONENT_CLEARSTRING). Its body is also expanded
//   inside SetMode 0x00495FE0 / Input 0x004963A0; no retail call reaches 0x004944C0.

#define FABLE_RETAIL_CFRONTENDGAMECOMPONENT_CLEARSTRING
#include <cstring>
#include "engine/CFrontendGameComponent.h"

void CFrontendGameComponent::ClearString()
{
    std::memset(String, 0, sizeof(String));
    StringPos = 0;
}
