class CTCBoastUI { public:
    char pad0[0x18];
    unsigned char f18;
    char pad19[0x0f];
    unsigned char f28;
    unsigned char f29;
    unsigned char f2a;
};

void __fastcall RequestActivate(CTCBoastUI *boastUI)
{
    if (!boastUI->f2a && !boastUI->f18 && !boastUI->f28)
        boastUI->f2a = 1;
}