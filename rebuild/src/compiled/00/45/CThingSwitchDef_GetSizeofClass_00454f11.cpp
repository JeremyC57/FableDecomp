// Leaf: ?GetSizeofClass@CThingSwitchDef@@UAEKXZ
#pragma optimize("s",on)
class CThingSwitchDef { public: virtual unsigned long GetSizeofClass(); };
unsigned long CThingSwitchDef::GetSizeofClass()
{
    void* self = (void*)this;
    (void)self;
    return 0x74;
}