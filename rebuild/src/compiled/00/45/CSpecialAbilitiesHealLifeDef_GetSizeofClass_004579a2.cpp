// Leaf: ?GetSizeofClass@CSpecialAbilitiesHealLifeDef@@UAEKXZ
#pragma optimize("s",on)
class CSpecialAbilitiesHealLifeDef { public: char pad[0x5c];
    public: virtual unsigned long GetSizeofClass();
};

unsigned long CSpecialAbilitiesHealLifeDef::GetSizeofClass()
{
    const CSpecialAbilitiesHealLifeDef* self = this;
    (void)self;
    return 0x60;
}