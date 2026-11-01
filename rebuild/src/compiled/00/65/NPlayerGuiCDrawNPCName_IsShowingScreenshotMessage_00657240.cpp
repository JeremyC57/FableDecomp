// NPlayerGui::CDrawNPCName::IsShowingScreenshotMessage() -- true while a screenshot message was posted (counter > 0) within the message duration of the current frame
// Naming wave 18: the global stand-in g_pMainGameComponent (0x013B86A0) is the protected static CMainGameComponent::PMainGameComponent, read through the
//   inline static accessor Get() as retail code outside the class does (name-only: the bytes are unchanged).
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00657240, length 58 bytes (+ int3 padding).
// Callees: CMainGameComponent::GetCurrentWorldFrame @ 0x004167C8 on CMainGameComponent::Get() 0x013B86A0 (a thunk to `return g_frame` @ 0x0049D870; the manifest
// label "GFGetPreMainMemoryUsed" is propagated). The duration is the float constant at 0x0123003C (a float pool literal; its value is not
// recoverable from the masked relocation and is written as 10 frames here).
// Shape: the frame difference is spilled to a 4-byte local for `fild`, `fabs`, then `fcomp [pool] ; fnstsw ; test ah,5 ; jp` is the ordered
// `<`; the results are `mov al,1` / `xor al,al` with a shared `pop esi ; pop ecx` epilogue.
#include <math.h>

class CMainGameComponent {
protected:
    static CMainGameComponent* PMainGameComponent;      // 0x013B86A0 ?PMainGameComponent@CMainGameComponent@@1PAV1@A (protected static; ESTABLISHED_SPELLINGS)
public:
    static CMainGameComponent* __fastcall Get() { return PMainGameComponent; }   // Ego_r / FableWin ?Get@CMainGameComponent@@SIPAV1@XZ (inline)
private: public:
    long GetCurrentWorldFrame() const;             // 0x004167C8 (Ego_r.pdb ?GetCurrentWorldFrame@CMainGameComponent@@QBEJXZ)
};

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
        long framesSince = CMainGameComponent::Get()->GetCurrentWorldFrame() - ScreenShotMessageCounter;
        if (fabsf((float)framesSince) < 10.0f)
            return true;
    }
    return false;
}

}