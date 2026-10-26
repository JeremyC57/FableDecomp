#pragma optimize("s",on)
class CSpecialAbilitiesEnflameDef { public:
    void* vftable;
    void Dtor();
    void* scalar_deleting_destructor(unsigned int flags);
};

void* CSpecialAbilitiesEnflameDef::scalar_deleting_destructor(unsigned int flags)
{
    this->Dtor();
    if (flags & 1)
        operator delete(this);
    return this;
}