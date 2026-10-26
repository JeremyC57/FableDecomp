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
#include "engine/CAIStateGroup_BanditPlayDice.h"

class CWorld;
class CThingAICreature;

long __fastcall GFGetCurrentFrame();                        // 0x0049D870
unsigned long __fastcall GFROR13(unsigned long value);      // 0x00497890

inline unsigned long WorldRandomRange(unsigned long& seed, unsigned long range)
{
    seed = seed * 0x24A1 + 0x24DF;
    seed = GFROR13(seed);
    if (range == 0)
        return 0;
    return seed % range;
}

struct CAIStateGroup_BanditPlayDice_Methods : CAIStateGroup_BanditPlayDice {
    unsigned long* GetPWorldSeed();                         // 0x008FCFD0 (CAIStateGroupBase)
    void InitStateGroup(const CWorld& world, CThingAICreature& creature);
};

void CAIStateGroup_BanditPlayDice_Methods::InitStateGroup(const CWorld& world, CThingAICreature& creature)
{
    DiceGame = 0;
    WasCarryingWeapon = false;
    unsigned long joinDelay = WorldRandomRange(*GetPWorldSeed(), 3000);
    FrameToJoin = (long)(GFGetCurrentFrame() + joinDelay * 0.5f);
    FrameToInitiate = WorldRandomRange(*GetPWorldSeed(), 3000) + GFGetCurrentFrame();
}