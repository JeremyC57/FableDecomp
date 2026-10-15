// InitializeDrawing(SVideoPlayer* player, SDrawingInfo* info) -- reset the player's drawing state and surface, run the backend's begin hook, and (when an info block is supplied and the player is not busy) report the surface size and the player's frame parameters
// Leaf: InitializeDrawing
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C90CB0, length 130 bytes, `__fastcall` (player in ecx, info in edx).
// Label: the catalog's "CPlayer::InitializeDrawing" is a media/video player routine of a linked library, not the game's
// CPlayer (no `this` member use of CPlayer, a fastcall signature, a -131 error code); no direct callers or
// references (reached through a table). Every type here is descriptive.
// Callees: the surface's Reset @ 0x00C912F0, GetWidth @ 0x00C91800, GetHeight @ 0x00C917B0, the busy test
// @ 0x00C9CFD0, and slot 3 of the backend member-function table @ 0x013A3DDC (`mov eax,[g] ; call [eax+0Ch]`).
// Shape: a member-pointer call through a global table (`(player->*g_pBackend[3])()`); the four state words are zeroed
// before the reset; the eight-word info block is written field by field.
struct SSurface {
    void          Reset();                                  // 0x00C912F0
    unsigned long GetWidth();                               // 0x00C91800
    unsigned long GetHeight();                              // 0x00C917B0
};

struct SDrawingInfo {
    unsigned long Width;                                    // +0x00
    unsigned long Height;                                   // +0x04
    unsigned long Flags;                                    // +0x08
    unsigned long Frame[5];                                 // +0x0c .. +0x1c
};

struct SVideoPlayer {
    void*         vfptr;                                    // +0x00
    SSurface      Surface;                                  // +0x04
    unsigned char _pad05[0x2c - 0x05];
    unsigned long FrameA;                                   // +0x2c
    unsigned long FrameB;                                   // +0x30
    unsigned long FrameC;                                   // +0x34
    unsigned long FrameD;                                   // +0x38
    unsigned long FrameE;                                   // +0x3c
    unsigned char _pad40[0x58 - 0x40];
    unsigned long State[4];                                 // +0x58 .. +0x64
    long IsBusy();                                          // 0x00C9CFD0
};

typedef long (SVideoPlayer::*PFNPlayerHook)();
extern PFNPlayerHook* g_pPlayerBackend;                    // 0x013A3DDC

long __fastcall InitializeDrawing(SVideoPlayer* player, SDrawingInfo* info)
{
    player->State[0] = 0;
    player->State[1] = 0;
    player->State[2] = 0;
    player->State[3] = 0;
    player->Surface.Reset();
    long result = (player->*g_pPlayerBackend[3])();
    if (result != 0)
        return result;
    if (info) {
        if (player->IsBusy())
            return -131;
        info->Width = player->Surface.GetWidth();
        info->Height = player->Surface.GetHeight();
        info->Flags = 0;
        info->Frame[0] = player->FrameA;
        info->Frame[1] = player->FrameB;
        info->Frame[2] = player->FrameC;
        info->Frame[3] = player->FrameD;
        info->Frame[4] = player->FrameE;
    }
    return 0;
}