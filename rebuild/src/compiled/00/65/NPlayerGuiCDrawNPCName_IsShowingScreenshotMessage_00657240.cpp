// NPlayerGui::CDrawNPCName::IsShowingScreenshotMessage() -- true while a screenshot message was posted (counter > 0) within the message duration of the current frame
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00657240, length 58 bytes (+ int3 padding).
// Callees: the frame counter getter @ 0x004167C8 on the global display engine pointer at 0x013B86A0 (a thunk to `return g_frame` @ 0x0049D870; the manifest
// label "GFGetPreMainMemoryUsed" is propagated). The duration is the float constant at 0x0123003C (a float pool literal; its value is not
// recoverable from the masked relocation and is written as 10 frames here).
// Shape: the frame difference is spilled to a 4-byte local for `fild`, `fabs`, then `fcomp [pool] ; fnstsw ; test ah,5 ; jp` is the ordered
// `<`; the results are `mov al,1` / `xor al,al` with a shared `pop esi ; pop ecx` epilogue.
#include <math.h>

class CDisplayEngine { public:
    long GetFrame();                               // 0x004167C8
};
extern CDisplayEngine* g_pDisplayEngine_013b86a0;  // 0x013B86A0

namespace NPlayerGui {

class CDrawNPCName { public:
    unsigned char _pad[0x28];                      // CDrawBase, Name (CWideString at +0x28 in retail)
    void*         Name;                            // +0x28 CWideString
    long          ScreenShotMessageCounter;        // +0x2c
    bool IsShowingScreenshotMessage();
};

bool CDrawNPCName::IsShowingScreenshotMessage()
{
    if (ScreenShotMessageCounter > 0) {
        long framesSince = g_pDisplayEngine_013b86a0->GetFrame() - ScreenShotMessageCounter;
        if (fabsf((float)framesSince) < 10.0f)
            return true;
    }
    return false;
}

}