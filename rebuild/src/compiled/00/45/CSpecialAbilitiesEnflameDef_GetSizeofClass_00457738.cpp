// Leaf: ?GetSizeofClass@CSpecialAbilitiesEnflameDef@@UAEKXZ
class CSpecialAbilitiesEnflameDef { public: char pad[0xa4];
    public: virtual unsigned long GetSizeofClass();
};

unsigned long CSpecialAbilitiesEnflameDef::GetSizeofClass()
{
    const CSpecialAbilitiesEnflameDef* self = this;
    (void)self;
    return 0xa8;
}