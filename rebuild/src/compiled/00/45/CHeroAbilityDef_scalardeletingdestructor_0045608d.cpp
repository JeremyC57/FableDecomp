#pragma optimize("s",on)
extern "C" void __cdecl fable_operator_delete(void* p);

class CHeroAbilityDef { public:
    int f0;
    void CHeroAbilityDef_dtor();
    void* scalar_deleting_destructor(unsigned int flags);
};

void* CHeroAbilityDef::scalar_deleting_destructor(unsigned int flags)
{
    this->CHeroAbilityDef_dtor();
    if (flags & 1)
        fable_operator_delete(this);
    return this;
}