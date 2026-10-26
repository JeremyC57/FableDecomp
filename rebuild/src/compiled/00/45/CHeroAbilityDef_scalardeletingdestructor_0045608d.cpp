#pragma optimize("s",on)

class CHeroAbilityDef { public:
    int f0;
    void CHeroAbilityDef_dtor();
    void* scalar_deleting_destructor(unsigned int flags);
};

void* CHeroAbilityDef::scalar_deleting_destructor(unsigned int flags)
{
    this->CHeroAbilityDef_dtor();
    if (flags & 1)
        operator delete(this);
    return this;
}