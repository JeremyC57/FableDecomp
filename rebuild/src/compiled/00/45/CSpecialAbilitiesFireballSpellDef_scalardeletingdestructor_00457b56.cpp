#pragma optimize("s",on)

class CSpecialAbilitiesFireballSpellDef { public:
    void* vftable;
    void dtor();
    void* scalar_deleting_destructor(unsigned int flags);
};

void* CSpecialAbilitiesFireballSpellDef::scalar_deleting_destructor(unsigned int flags)
{
    this->dtor();
    if (flags & 1)
        operator delete(this);
    return this;
}