// Leaf: ?GetSizeofClass@CCreatureModeDef@@UAEKXZ
#pragma optimize("s",on)
class CCreatureModeDef { public: virtual unsigned long GetSizeofClass(); };
unsigned long CCreatureModeDef::GetSizeofClass()
{
    return 64;
}