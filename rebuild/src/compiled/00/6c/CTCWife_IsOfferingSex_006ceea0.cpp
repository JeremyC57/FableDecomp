// CTCWife::IsOfferingSex() const -- the wife offers sex once she has been in love with her husband at home for longer than the def's delay
// Retail: Fable.exe (TLC, VC7.1) function @ 0x006CEEA0, length 76 bytes (+ int3 padding).
// Callees: CTCBase::GetWorld() @ 0x00686A70 (returns g->+0x24; byte-identical twin of 0x00686A80; manifest
// "CViewBase::GetDisplayEngine" is propagated) -- its result only reaches the static frames-per-second getter, so the
// call stays and nothing reads eax; GetWorldFrame() @ 0x0049D870 (manifest "GFGetPreMainMemoryUsed", propagated).
// Global 0x01375550 = frames per second (long, 15).
// Ego_r.pdb: ?IsOfferingSex@CTCWife@@QBE_NXZ. Layout: engine/CTCWife.h (PHeroMarriageDef +0x0c,
// FrameEnteredLoveWithHusbandPresentAtHome +0x60); engine/CHeroMarriageDef.h SecsInLoveAndHusbandPresentBeforeSexOffered
// +0xb0.
// Shape: the frames-per-second value is a long local (homed for `fild`) read before the frame call, the def pointer a
// local taken before the call too (kept in edi across it), the elapsed frames another long local; `elapsed > fps * secs` compares with `fcompp ; test ah,5 ; jp`, the bool result materialised with full-register
// `mov eax,1` / `xor eax,eax` tails.
#include "engine/CTCWife.h"
#include "engine/CHeroMarriageDef.h"

extern long g_FramesPerSecond;                              // 0x01375550

class CWorld { public:
    static long GetWorldFrame();                            // 0x0049D870
    static long GetFramesPerSecond() { return g_FramesPerSecond; }
};

struct CTCWife_Sex : CTCWife {
    const CWorld* GetWorld() const;                         // 0x00686A70 (CTCBase)
    const CHeroMarriageDef* GetDef() const { return (const CHeroMarriageDef*)PHeroMarriageDef_Object; }
    bool IsOfferingSex() const;
};

bool CTCWife_Sex::IsOfferingSex() const
{
    long framesPerSecond = GetWorld()->GetFramesPerSecond();
    const CHeroMarriageDef* def = GetDef();
    long framesInLove = CWorld::GetWorldFrame() - FrameEnteredLoveWithHusbandPresentAtHome;
    return framesInLove > framesPerSecond * def->SecsInLoveAndHusbandPresentBeforeSexOffered;
}