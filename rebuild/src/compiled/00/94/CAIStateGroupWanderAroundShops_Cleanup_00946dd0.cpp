// CAIStateGroup_WanderAroundShops::Cleanup() -- schedule the next buy a few seconds from now
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00946DD0, length 53 bytes (+ int3 padding).
// Callees: GFGetFrameCount @ 0x0049D870 (__fastcall, no arguments), __ftol2 @ 0x00BFEA70 (the
// float-to-long conversion of the sum).
// Globals: frames-per-second long @ 0x01375550, the buy delay in seconds (float @ 0x0138CE2C).
// Shape: `NextFrameBuy = (long)(GFGetFrameCount() + fps * delay)` where the fps global is read into
// a local BEFORE the frame-count call (retail spills it to [esp+8] first, the frame to [esp+4]);
// `fild frame ; fild fps ; fmul delay ; faddp` is the natural left-to-right sum. NextFrameBuy is
// +0x24 (engine header CAIStateGroup_WanderAroundShops.h). Frame 8, result through __ftol2.
extern long g_framesPerSecond;                  // 0x01375550
extern float g_wanderShopsBuyDelay;             // 0x0138CE2C
long __fastcall GFGetFrameCount();              // 0x0049D870

class CAIStateGroupBase { public:
    virtual ~CAIStateGroupBase();
    unsigned char _pad[0x10];                   // PBrain, ScriptAIPriority, Name, flags (+0x4..+0x13)
};

struct CAIStateGroup_WanderAroundShops : CAIStateGroupBase {
    unsigned char Shop[8];                      // +0x14 CIntelligentPointer<CThing>
    unsigned char PrettyThing[8];               // +0x1c CIntelligentPointer<CThing>
    long NextFrameBuy;                          // +0x24
    virtual void Cleanup();
};

void CAIStateGroup_WanderAroundShops::Cleanup()
{
    long fps = g_framesPerSecond;
    NextFrameBuy = (long)(GFGetFrameCount() + fps * g_wanderShopsBuyDelay);
}