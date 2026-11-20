// CAIStateGroup_BanditPlayDice::InitStateGroup(const CWorld& world, CThingAICreature& creature) -- clears the dice game, then schedules joining within 0..1500 frames and initiating within 0..3000 frames of now
// Retail: Fable.exe (TLC, VC7.1) function @ 0x009284B0, length 160 bytes, `ret 8` (both references unused).
// Callees: CAIStateGroupBase::GetPWorldSeed @ 0x008FCFD0 (twice), GFROR13 @ 0x00497890 (__fastcall),
// GFGetCurrentFrame @ 0x0049D870 (__fastcall; the manifest's "GFGetPreMainMemoryUsed" label is propagated),
// __ftol2 @ 0x00BFEA70. Constants: 2^32 @ 0x0122DCB4 (unsigned conversion), 0.5f @ 0x012917C4.
// Shape: DiceGame and WasCarryingWeapon share one zero register; each random draw is the landed world-random
// step (`seed = seed * 0x24A1 + 0x24DF ; seed = GFROR13(seed) ; seed % 3000`, unsigned `div`) through an inline
// helper on the seed pointer, drawn BEFORE the frame call (a named local), then
// `FrameToJoin = frame + r * 0.5f` (frame converted first: `fild frame ; fild r ; jge ; fadd 2^32 ; fmul 0.5 ;
// faddp`) and `FrameToInitiate = r + frame` (`add eax,edi`).
// Model: engine/CAIStateGroup_BanditPlayDice.h (DiceGame +0x18, FrameToInitiate +0x1c, FrameToJoin +0x20,
// WasCarryingWeapon +0x24).
// Leaf: ?InitStateGroup@CAIStateGroup_BanditPlayDice@@MAEXABVCWorld@@AAVCThingAICreature@@@Z
// NF131 (2026-11-20, q5141): the stand-in CAIStateGroup_BanditPlayDice_Methods over the flat generated header gives way to the retail class
//   (engine/CAIStateGroup_BanditPlayDice.h FABLE_RETAIL_CAISTATEGROUP_BANDITPLAYDICE + _VIRTUALS on the retail engine/CAIStateGroupBase.h).
//   Own name: ?InitStateGroup@CAIStateGroup_BanditPlayDice@@MAEXABVCWorld@@AAVCThingAICreature@@@Z (Ego_r.pdb 0x0094B660 + FableWin.pdb 0x02C65740,
//   protected virtual, slot 13), was ?InitStateGroup@CAIStateGroup_BanditPlayDice_Methods@@QAEXABVCWorld@@AAVCThingAICreature@@@Z. GetPWorldSeed
//   0x008FCFD0 binds ?GetPWorldSeed@CAIStateGroupBase@@IBEPAKXZ (Ego_r 0x00847E20 + FableWin 0x0288DA10; the header member), and the local
//   WorldRandomRange helper is the shared inline GFRandom (engine/GFRandom.h, Ego_r.pdb ?GFRandom@@YIKKPAK@Z).
#define FABLE_RETAIL_CAISTATEGROUPBASE
#define FABLE_RETAIL_CAISTATEGROUPBASE_MEMBERS
#define FABLE_RETAIL_CAISTATEGROUP_BANDITPLAYDICE
#define FABLE_RETAIL_CAISTATEGROUP_BANDITPLAYDICE_VIRTUALS
#include "engine/CAIStateGroup_BanditPlayDice.h"
#include "engine/GFRandom.h"

class CWorld {
public:
    static long __fastcall GetFrame();   // 0x0049D870
};

void CAIStateGroup_BanditPlayDice::InitStateGroup(const CWorld& world, CThingAICreature& creature)
{
    DiceGame = 0;
    WasCarryingWeapon = false;
    unsigned long joinDelay = GFRandom(3000, GetPWorldSeed());
    FrameToJoin = (long)(CWorld::GetFrame() + joinDelay * 0.5f);
    FrameToInitiate = GFRandom(3000, GetPWorldSeed()) + CWorld::GetFrame();
}
