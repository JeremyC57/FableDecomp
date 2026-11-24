// Leaf: ?GetSizeofClass@CSpecialAbilitiesBulletTimeDef@@UAEKXZ
class CSpecialAbilitiesBulletTimeDef { public: char pad[0x188];
    public: virtual unsigned long GetSizeofClass();
};

unsigned long CSpecialAbilitiesBulletTimeDef::GetSizeofClass()
{
    const CSpecialAbilitiesBulletTimeDef* self = this;
    (void)self;
    return 0x18c;
}