// Leaf: ?GetSizeofClass@COpinionPersonalityDef@@UAEKXZ
#pragma optimize("s",on)
class COpinionPersonalityDef { public: char _pad[0x50];
    public: virtual unsigned long GetSizeofClass();
};

unsigned long COpinionPersonalityDef::GetSizeofClass()
{
    const COpinionPersonalityDef* self = this;
    (void)self;
    return 0x54;
}