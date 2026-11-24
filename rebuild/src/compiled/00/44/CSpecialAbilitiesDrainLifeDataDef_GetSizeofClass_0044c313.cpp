// Leaf: ?GetSizeofClass@CSpecialAbilitiesDrainLifeDataDef@@UAEKXZ
#pragma optimize("s",on)
class CSpecialAbilitiesDrainLifeDataDef { public: unsigned char pad[0x28];
    public: virtual unsigned long GetSizeofClass();
};

unsigned long CSpecialAbilitiesDrainLifeDataDef::GetSizeofClass()
{
    const CSpecialAbilitiesDrainLifeDataDef* self = this;
    (void)self;
    return sizeof(CSpecialAbilitiesDrainLifeDataDef);
}