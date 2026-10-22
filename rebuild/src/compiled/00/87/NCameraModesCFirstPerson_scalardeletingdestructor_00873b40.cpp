struct CFirstPerson {
    void* vftable;
};

void __fastcall CFirstPerson_dtor(CFirstPerson* person);
void operator delete(void*);

void* __fastcall CFirstPerson_scalar_deleting_destructor(CFirstPerson* person, int edx_unused, unsigned int flags)
{
    CFirstPerson_dtor(person);
    if (flags & 1)
        operator delete(person);
    return person;
}