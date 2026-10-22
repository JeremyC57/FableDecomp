struct CModeScreamerSuperAttack {
    void* vftable;
};

void __fastcall CModeScreamerSuperAttack_dtor(CModeScreamerSuperAttack* mode);
void operator delete(void*);

void* __fastcall CModeScreamerSuperAttack_scalar_deleting_destructor(CModeScreamerSuperAttack* mode, int edx_unused, unsigned int flags)
{
    CModeScreamerSuperAttack_dtor(mode);
    if (flags & 1)
        operator delete(mode);
    return mode;
}