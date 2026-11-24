// Leaf: ?GetSizeofClass@CSoundThemeDef@@UAEKXZ
#pragma optimize("s",on)
class CSoundThemeDef { public: char pad[0x60];
    public: virtual unsigned long GetSizeofClass();
};

unsigned long CSoundThemeDef::GetSizeofClass()
{
    const CSoundThemeDef* self = this;
    return 0x64;
}