// Leaf: ?GetSizeofClass@CDragonActionNapalmDef@@UAEKXZ
#pragma optimize("s",on)
typedef unsigned long ulong;

class CDragonActionNapalmDef { public: virtual unsigned long GetSizeofClass(); };
unsigned long CDragonActionNapalmDef::GetSizeofClass()
{
    const void* self = (void*)this;
    (void)self;
    return 0x38;
}