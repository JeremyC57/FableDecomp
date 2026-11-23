// Leaf: ?GetSizeofClass@CCreatureGeneratorDef@@UAEKXZ
#pragma optimize("s",on)
class CCreatureGeneratorDef { public: virtual unsigned long GetSizeofClass(); };
unsigned long CCreatureGeneratorDef::GetSizeofClass()
{
    return 64;
}