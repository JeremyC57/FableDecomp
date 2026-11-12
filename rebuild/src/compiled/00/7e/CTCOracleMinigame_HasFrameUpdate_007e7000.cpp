// CTCOracleMinigame::HasFrameUpdate() const -- the minigame has a frame update
// Leaf: ?HasFrameUpdate@CTCOracleMinigame@@UBE_NXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x007E7000, thiscall, `ret`.
// Identity (agentFAM03 proof, re-verified by agentFAM05): slot 8 of CTCOracleMinigame's vtable 0x012707B4 (the slot holds this body in 1 vtable(s), owner CTCOracleMinigame); the Xbox 360 Test PDB virtual order (third source) aligned on 16 TLC-named slots of that vtable puts HasFrameUpdate(void) at this slot [gap kind GS]; Ego_r/FableWin name set: ?HasFrameUpdate@CTCOracleMinigame@@UBE_NXZ; `ret` matches the signature.
// Was: CTCOracleMinigame::vfunc_8 (rebuild/src/compiled/00/7e/gapscan_sub007e7000_007e7000.cpp).
#define FABLE_RETAIL_CTCORACLEMINIGAME_VSLOTS
#define FABLE_RETAIL_CTCORACLEMINIGAME_CLASS
#include "engine/ETCInterfaceType.h"
#include "engine/CTCOracleMinigame.h"
bool CTCOracleMinigame::HasFrameUpdate() const
{
    return true;
}
