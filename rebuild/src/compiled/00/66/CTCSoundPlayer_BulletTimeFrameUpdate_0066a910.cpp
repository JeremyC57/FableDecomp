// CTCSoundPlayer::BulletTimeFrameUpdate() -- the bullet-time frame update is the ordinary frame update (virtual, slot 9)
// Leaf: ?BulletTimeFrameUpdate@CTCSoundPlayer@@UAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x0066A910, thiscall, tail jump (no ret).
// Identity (agentFAM03 proof, re-verified by agentFAM05): slot 10 of CTCSoundPlayer's vtable 0x0125B1C4 (the slot holds this body in 1 vtable(s), owner CTCSoundPlayer); the Xbox 360 Test PDB virtual order (third source) aligned on 16 TLC-named slots of that vtable puts BulletTimeFrameUpdate(void) at this slot [gap kind GS]; Ego_r/FableWin name set: ?BulletTimeFrameUpdate@CTCSoundPlayer@@UAEXXZ; no `ret` to check (NORET).
// Was: CTCSoundPlayer::vfunc_10 (rebuild/src/compiled/00/66/CTCSoundPlayer_vfunc10_0066a910.cpp).
#define FABLE_RETAIL_CTCSOUNDPLAYER_VSLOTS3
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CTCSOUNDPLAYER
#include "fable_string.h"
#include "engine/CTCSoundPlayer.h"
void CTCSoundPlayer::BulletTimeFrameUpdate()
{
    FrameUpdate();
}
