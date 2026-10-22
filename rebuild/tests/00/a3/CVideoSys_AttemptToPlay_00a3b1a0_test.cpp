#include <cstdio>

struct IPlayer;
struct IPlayerVtbl {
    int (__stdcall *slot0)(IPlayer*);
    int (__stdcall *slot1)(IPlayer*);
    int (__stdcall *slot2)(IPlayer*);
    int (__stdcall *slot3)(IPlayer*);
    int (__stdcall *slot4)(IPlayer*);
    int (__stdcall *slot5)(IPlayer*);
    int (__stdcall *slot6)(IPlayer*);
    int (__stdcall *slot7)(IPlayer*);
};
struct IPlayer { IPlayerVtbl* vtbl; };
struct CVideoSys {
    void* GBuilder;
    IPlayer* MControl;
    char pad[0x28-0x08];
    int  State;
};

// Exercise the reconstructed function from the linked source object; the
// behavior exe links this test obj together with CVideoSys_AttemptToPlay_
// 00a3b1a0.cpp (Invoke-VerifiedLeaf), so redefining it here collides (LNK2005).
bool __fastcall AttemptToPlay(CVideoSys* sys);

static int g_ret;
static int __stdcall busy(IPlayer*) { return g_ret; }

int main()
{
    IPlayerVtbl vt; vt.slot7 = busy;
    IPlayer pl; pl.vtbl = &vt;
    CVideoSys vs; vs.MControl = &pl;

    g_ret = 0; vs.State = 0;
    bool r1 = AttemptToPlay(&vs);
    if (!(r1 && vs.State == 1)) { printf("FAIL1\n"); return 1; }

    g_ret = 5; vs.State = 99;
    bool r2 = AttemptToPlay(&vs);
    if (!(!r2 && vs.State == 99)) { printf("FAIL2\n"); return 1; }

    printf("FABLETLC_CVIDEOSYS_ATTEMPT_TO_PLAY PASS\n");
    return 0;
}