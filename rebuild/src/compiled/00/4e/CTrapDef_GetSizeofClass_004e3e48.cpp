// Leaf: ?GetSizeofClass@CTrapDef@@UAEKXZ
#pragma optimize("s",on)
class CTrapDef { public: virtual unsigned long GetSizeofClass(); };
unsigned long CTrapDef::GetSizeofClass()
{
    return 100;
}