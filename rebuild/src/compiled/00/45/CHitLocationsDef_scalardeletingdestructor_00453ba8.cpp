#pragma optimize("s",on)
class CHitLocationsDef { public:
    int f0;
    int Parent;
    void* f8;
    void* scalar_deleting_destructor(unsigned int flags);
};

extern void __fastcall CHitLocationsDef_dtor(CHitLocationsDef* def);

void* CHitLocationsDef::scalar_deleting_destructor(unsigned int flags)
{
    CHitLocationsDef_dtor(this);
    if (flags & 1)
        operator delete(this);
    return this;
}