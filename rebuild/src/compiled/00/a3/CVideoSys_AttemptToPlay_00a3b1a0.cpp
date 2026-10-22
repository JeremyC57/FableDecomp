// CVideoSys::AttemptToPlay  0x00a3b1a0
struct IPlayer;
struct IPlayerVtbl {
    // slots 0..6 unknown
    int (__stdcall *slot0)(IPlayer*);
    int (__stdcall *slot1)(IPlayer*);
    int (__stdcall *slot2)(IPlayer*);
    int (__stdcall *slot3)(IPlayer*);
    int (__stdcall *slot4)(IPlayer*);
    int (__stdcall *slot5)(IPlayer*);
    int (__stdcall *slot6)(IPlayer*);   // +0x18
    int (__stdcall *slot7)(IPlayer*);   // +0x1c
};
struct IPlayer {
    IPlayerVtbl* vtbl;
};

struct CVideoSys {
    void* GBuilder;         // +0x00
    IPlayer* MControl;      // +0x04
    char pad[0x28-0x08];
    int  State;         // +0x28
};

bool __fastcall AttemptToPlay(CVideoSys* sys)
{
    IPlayer* p = sys->MControl;
    if (p->vtbl->slot7(p) == 0) {
        sys->State = 1;
        return true;
    }
    return false;
}