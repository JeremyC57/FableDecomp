// Leaf: ?GetSizeofClass@CFactionDef@@UAEKXZ
#pragma optimize("s",on)
class CFactionDef { public: virtual unsigned long GetSizeofClass(); };
unsigned long CFactionDef::GetSizeofClass()
{
    const void* self = (void*)this;
    (void)self;
    return 0x54;
}