// Leaf: ?GetSizeofClass@CSpecialAbilitiesDrainLifeDef@@UAEKXZ
#pragma optimize("s",on)
class CSpecialAbilitiesDrainLifeDef { public: char pad[0x6c];
    public: virtual unsigned long GetSizeofClass();
};

unsigned long CSpecialAbilitiesDrainLifeDef::GetSizeofClass()
{
    const CSpecialAbilitiesDrainLifeDef* self = this;
    (void)self;
    return 0x70;
}