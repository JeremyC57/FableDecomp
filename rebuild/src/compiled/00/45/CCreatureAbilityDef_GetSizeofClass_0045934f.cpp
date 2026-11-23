// Leaf: ?GetSizeofClass@CCreatureAbilityDef@@UAEKXZ
#pragma optimize("s",on)
class CCreatureAbilityDef { public: virtual unsigned long GetSizeofClass(); };
unsigned long CCreatureAbilityDef::GetSizeofClass()
{
    return 64;
}