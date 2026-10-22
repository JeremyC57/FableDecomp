#pragma optimize("s",on)
extern void __fastcall CCombatTypeDef_dtor(void* def);

struct CCombatTypeDef {
    int f0;
    int Parent;
    void* f8;
    void* scalar_deleting_destructor(unsigned int flags);
};

void* CCombatTypeDef::scalar_deleting_destructor(unsigned int flags)
{
    CCombatTypeDef_dtor(this);
    if (flags & 1)
        ::operator delete(this);
    return this;
}