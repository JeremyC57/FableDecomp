struct CSingletonTarget;
extern "C" void __fastcall Singleton_Method_4374b0(CSingletonTarget* self);
extern CSingletonTarget* g_singleton_13b8790;

struct CTCHeroCentre {
    char pad[0x10];
    int CurrentGUIMode;
};

void __fastcall CTCHeroCentre__OnHeroCloseDoor(CTCHeroCentre* self)
{
    switch (self->CurrentGUIMode) {
    case 1:
        self->CurrentGUIMode = 0;
        Singleton_Method_4374b0(g_singleton_13b8790);
        break;
    }
}