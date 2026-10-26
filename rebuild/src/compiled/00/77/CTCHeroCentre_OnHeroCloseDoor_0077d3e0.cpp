class CPlayerGui {
public:
    void Open();   // 0x004374B0
};
struct CSingletonTarget;
extern CSingletonTarget* g_singleton_13b8790;

class CTCHeroCentre { public:
    char pad[0x10];
    int CurrentGUIMode;
};

void __fastcall CTCHeroCentre__OnHeroCloseDoor(CTCHeroCentre* self)
{
    switch (self->CurrentGUIMode) {
    case 1:
        self->CurrentGUIMode = 0;
        ((CPlayerGui*)(g_singleton_13b8790))->Open();
        break;
    }
}