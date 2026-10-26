#pragma optimize("s",on)

class CSpecialAbilitiesThunderLightningStormDef { public:
    void* vftable;
    void real_dtor();
    void* scalar_deleting_destructor(unsigned int flags);
};

void* CSpecialAbilitiesThunderLightningStormDef::scalar_deleting_destructor(unsigned int flags)
{
    this->real_dtor();
    if (flags & 1) {
        operator delete(this);
    }
    return this;
}