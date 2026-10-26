#pragma optimize("s",on)

class CHeroMarriageDef { public:
    void *vptr;
    void real_dtor();
    void * CHeroMarriageDef_scalar_deleting_destructor(unsigned int flags);
};

void * CHeroMarriageDef::CHeroMarriageDef_scalar_deleting_destructor(unsigned int flags)
{
    this->real_dtor();
    if (flags & 1)
        operator delete(this);
    return this;
}