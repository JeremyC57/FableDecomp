#pragma optimize("s",on)
extern void __cdecl fable_operator_delete(void* p);

class CSpecialAbilitiesThunderLightningStormDef { public:
    void* vftable;
    void real_dtor();
    void* scalar_deleting_destructor(unsigned int flags);
};

void* CSpecialAbilitiesThunderLightningStormDef::scalar_deleting_destructor(unsigned int flags)
{
    this->real_dtor();
    if (flags & 1) {
        fable_operator_delete(this);
    }
    return this;
}