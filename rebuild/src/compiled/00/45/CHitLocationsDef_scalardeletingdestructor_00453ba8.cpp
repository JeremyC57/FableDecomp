#pragma optimize("s",on)
struct CHitLocationsDef {
    int f0;
    int Parent;
    void* f8;
    void* scalar_deleting_destructor(unsigned int flags);
};

extern void __fastcall CHitLocationsDef_dtor(CHitLocationsDef* def);
extern void __cdecl eng_operator_delete(void* p);

void* CHitLocationsDef::scalar_deleting_destructor(unsigned int flags)
{
    CHitLocationsDef_dtor(this);
    if (flags & 1)
        eng_operator_delete(this);
    return this;
}