#pragma optimize("s",on)
extern void __cdecl eng_operator_delete(void* p);

class CSpecialAbilitiesFireballSpellDef { public:
    void* vftable;
    void dtor();
    void* scalar_deleting_destructor(unsigned int flags);
};

void* CSpecialAbilitiesFireballSpellDef::scalar_deleting_destructor(unsigned int flags)
{
    this->dtor();
    if (flags & 1)
        eng_operator_delete(this);
    return this;
}