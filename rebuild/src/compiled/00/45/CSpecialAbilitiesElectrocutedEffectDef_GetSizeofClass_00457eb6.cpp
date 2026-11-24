// Leaf: ?GetSizeofClass@CSpecialAbilitiesElectrocutedEffectDef@@UAEKXZ
#pragma optimize("s",on)
class CSpecialAbilitiesElectrocutedEffectDef { public: char pad[0x68];
    public: virtual unsigned long GetSizeofClass();
};

unsigned long CSpecialAbilitiesElectrocutedEffectDef::GetSizeofClass()
{
    const CSpecialAbilitiesElectrocutedEffectDef* self = this;
    (void)self;
    return 0x6c;
}