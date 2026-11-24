// Leaf: ?GetSizeofClass@CVillagePeopleDef@@UAEKXZ
class CVillagePeopleDef { public: char pad[0xc0];
    public: virtual unsigned long GetSizeofClass();
};

unsigned long CVillagePeopleDef::GetSizeofClass()
{
    const CVillagePeopleDef* self = this;
    (void)self;
    return 0xC4;
}