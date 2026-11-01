// CTCWife::IsOfferingSex() const -- the wife offers sex once she has been in love with her husband at home for longer than the def's delay
// Naming wave 18: the global stand-in g_FramesPerSecond (0x01375550) is the protected static long CMainGameComponent::ConstantFPS, read through the
//   inline static accessor GetConstantFPS() as retail code outside the class does (name-only: the bytes are unchanged).
// Retail: Fable.exe (TLC, VC7.1) function @ 0x006CEEA0, length 76 bytes (+ int3 padding).
// Callees: CTCBase::GetWorld() @ 0x00686A70 (returns g->+0x24; byte-identical twin of 0x00686A80; manifest
// "CViewBase::GetDisplayEngine" is propagated) -- its result only reaches the static frames-per-second getter, so the
// call stays and nothing reads eax; CWorld::GetFrame() @ 0x0049D870 (manifest "GFGetPreMainMemoryUsed", propagated).
// Global 0x01375550 = frames per second (long, 15).
// Ego_r.pdb: ?IsOfferingSex@CTCWife@@QBE_NXZ. Layout: engine/CTCWife.h (PHeroMarriageDef +0x0c,
// FrameEnteredLoveWithHusbandPresentAtHome +0x60); engine/CHeroMarriageDef.h SecsInLoveAndHusbandPresentBeforeSexOffered
// +0xb0.
// Shape: the frames-per-second value is a long local (homed for `fild`) read before the frame call, the def pointer a
// local taken before the call too (kept in edi across it), the elapsed frames another long local; `elapsed > fps * secs` compares with `fcompp ; test ah,5 ; jp`, the bool result materialised with full-register
// `mov eax,1` / `xor eax,eax` tails.
#define CTCWife CTCWifeLayout
#include "engine/CTCWife.h"
#undef CTCWife
class CWorld;

class CTCBase
{
protected:
    CWorld& GetWorld() const;   // 0x00686A70
};
class CTCWife : public CTCBase, public CTCWifeLayout
{
};
#include "engine/CHeroMarriageDef.h"

class CMainGameComponent {
protected:
    static long ConstantFPS;                             // 0x01375550 ?ConstantFPS@CMainGameComponent@@1JA (protected static)
public:
    static long __fastcall GetConstantFPS() { return ConstantFPS; }   // Ego_r / FableWin ?GetConstantFPS@CMainGameComponent@@SIJXZ (inline)
};

class CWorld { public:
    static long __fastcall GetFrame();                            // 0x0049D870
    static long GetFramesPerSecond() { return CMainGameComponent::GetConstantFPS(); }
};

struct CTCWife_Sex : CTCWife {
    const CHeroMarriageDef* GetDef() const { return (const CHeroMarriageDef*)PHeroMarriageDef_Object; }
    bool IsOfferingSex() const;
};

bool CTCWife_Sex::IsOfferingSex() const
{
    long framesPerSecond = GetWorld().GetFramesPerSecond();
    const CHeroMarriageDef* def = GetDef();
    long framesInLove = CWorld::GetFrame() - FrameEnteredLoveWithHusbandPresentAtHome;
    return framesInLove > framesPerSecond * def->SecsInLoveAndHusbandPresentBeforeSexOffered;
}