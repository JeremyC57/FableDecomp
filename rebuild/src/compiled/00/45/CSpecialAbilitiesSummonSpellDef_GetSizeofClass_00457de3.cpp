// Leaf: ?GetSizeofClass@CSpecialAbilitiesSummonSpellDef@@UAEKXZ
#pragma optimize("s",on)
typedef unsigned long ulong;

class CSpecialAbilitiesSummonSpellDef { public: virtual unsigned long GetSizeofClass(); };
unsigned long CSpecialAbilitiesSummonSpellDef::GetSizeofClass()
{
    void* self = (void*)this;
    (void)self;
    return 0x6c;
}