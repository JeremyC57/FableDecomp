// CTCSoundPlayer::HasFrameUpdate() const -- returns true
// Leaf: ?HasFrameUpdate@CTCSoundPlayer@@UBE_NXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x0066C2C0, thiscall, `ret`.
// Identity: slot 8 of CTCSoundPlayer's vtable 0x0125B1C4 (defined here: the slot holds this body in 1 vtable(s), all of CTCSoundPlayer or derived); the Xbox 360 Test PDB virtual order (third source) aligned on 16 TLC-named slots of that vtable puts HasFrameUpdate(void) at this slot; Ego_r/FableWin name set: ?HasFrameUpdate@CTCSoundPlayer@@UBE_NXZ; `ret` matches the signature.
// Header: engine/CTCSoundPlayer.h (opt-ins as in the landed row CTCSoundPlayer_SetVoiceName_0066a9d0.cpp).
#define FABLE_RETAIL_CTCSOUNDPLAYER_VSLOTS
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CTCSOUNDPLAYER
#define FABLE_RETAIL_CTCSOUNDPLAYER_VOICENAME
#include "fable_string.h"
#include "engine/CTCSoundPlayer.h"
bool CTCSoundPlayer::HasFrameUpdate() const
{
    return true;
}
