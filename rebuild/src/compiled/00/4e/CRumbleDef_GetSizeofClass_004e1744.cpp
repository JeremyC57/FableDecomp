// Leaf: ?GetSizeofClass@CRumbleDef@@UAEKXZ
#pragma optimize("s",on)
class CRumbleDef { public: virtual unsigned long GetSizeofClass(); };
unsigned long CRumbleDef::GetSizeofClass()
{
    return 64;
}