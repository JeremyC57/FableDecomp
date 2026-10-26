#pragma optimize("s",on)
extern "C" void __cdecl eng_operator_delete(void* p);

class CSpecialAbilitiesBurntEffectDef { public:
    void* vftable;
    void Dtor();
    void* scalar_deleting_destructor(unsigned int flags);
};

void* CSpecialAbilitiesBurntEffectDef::scalar_deleting_destructor(unsigned int flags)
{
    this->Dtor();
    if (flags & 1) {
        eng_operator_delete(this);
    }
    return this;
}