// Leaf: ?GetSizeofClass@CInventoryCategoryDef@@UAEKXZ
#pragma optimize("s",on)
class CInventoryCategoryDef { public: char pad[0x54];
    public: virtual unsigned long GetSizeofClass();
};

unsigned long CInventoryCategoryDef::GetSizeofClass()
{
    CInventoryCategoryDef* self = this;
    (void)self;
    return 0x58;
}