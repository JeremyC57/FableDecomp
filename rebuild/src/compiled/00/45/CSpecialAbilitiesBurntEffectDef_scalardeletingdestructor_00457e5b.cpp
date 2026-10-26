#pragma optimize("s",on)

class CSpecialAbilitiesBurntEffectDef { public:
    void* vftable;
    void Dtor();
    void* scalar_deleting_destructor(unsigned int flags);
};

void* CSpecialAbilitiesBurntEffectDef::scalar_deleting_destructor(unsigned int flags)
{
    this->Dtor();
    if (flags & 1) {
        operator delete(this);
    }
    return this;
}