// Leaf: ?GetSizeofClass@CSpecialAbilitiesForcePushDataDef@@UAEKXZ
#pragma optimize("s",on)
typedef unsigned long ulong;

class CSpecialAbilitiesForcePushDataDef { public: virtual unsigned long GetSizeofClass(); };
unsigned long CSpecialAbilitiesForcePushDataDef::GetSizeofClass()
{
    const void* self = (void*)this;
    (void)self;
    return 0x2c;
}