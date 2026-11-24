// Leaf: ?GetSizeofClass@CVillagerInteractionsDef@@UAEKXZ
#pragma optimize("s",on)
class CVillagerInteractionsDef { public: char pad[0x44];
    public: virtual unsigned long GetSizeofClass();
};

unsigned long CVillagerInteractionsDef::GetSizeofClass()
{
    const CVillagerInteractionsDef* self = this;
    (void)self;
    return 0x48;
}