#include <cstdio>

struct CTCActivationReceptorBase {
    char pad0[0x0c];
    unsigned char Activated;
    char pad1[0x20 - 0x0d];
    unsigned char f20;
    char pad2[0x28 - 0x21];
    void* f28;
};

static int g_helper_calls = 0;
void* __fastcall helper_49d870(CTCActivationReceptorBase* receptor)
{
    (void)receptor;
    ++g_helper_calls;
    return (void*)0xDEADBEEF;
}

void __fastcall Activate(CTCActivationReceptorBase* receptor)
{
    receptor->Activated = 1;
    if (receptor->f20)
        receptor->f28 = helper_49d870(receptor);
}

int main()
{
    CTCActivationReceptorBase a;
    a.Activated = 0; a.f20 = 0; a.f28 = (void*)0x11;
    Activate(&a);
    bool okA = (a.Activated == 1) && (a.f28 == (void*)0x11) && (g_helper_calls == 0);

    CTCActivationReceptorBase b;
    b.Activated = 0; b.f20 = 1; b.f28 = (void*)0x22;
    Activate(&b);
    bool okB = (b.Activated == 1) && (b.f28 == (void*)0xDEADBEEF) && (g_helper_calls == 1);

    if (okA && okB)
        printf("007f76e0_TEST PASS\n");
    else
        printf("FAIL A=%d B=%d\n", okA, okB);
    return 0;
}