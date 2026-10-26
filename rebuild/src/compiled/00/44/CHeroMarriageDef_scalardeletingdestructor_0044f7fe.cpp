#pragma optimize("s",on)
extern void __cdecl engine_operator_delete(void *p);

class CHeroMarriageDef { public:
    void *vptr;
    void real_dtor();
    void * CHeroMarriageDef_scalar_deleting_destructor(unsigned int flags);
};

void * CHeroMarriageDef::CHeroMarriageDef_scalar_deleting_destructor(unsigned int flags)
{
    this->real_dtor();
    if (flags & 1)
        engine_operator_delete(this);
    return this;
}