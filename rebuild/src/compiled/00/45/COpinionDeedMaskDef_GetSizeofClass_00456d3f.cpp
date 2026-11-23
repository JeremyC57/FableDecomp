// Leaf: ?GetSizeofClass@COpinionDeedMaskDef@@UAEKXZ
#pragma optimize("s",on)
class COpinionDeedMaskDef { public: char pad[0x44];
    public: virtual unsigned long GetSizeofClass();
};
unsigned long COpinionDeedMaskDef::GetSizeofClass()
{
    const COpinionDeedMaskDef* self = this;
    (void)self;
    return 0x48;
}