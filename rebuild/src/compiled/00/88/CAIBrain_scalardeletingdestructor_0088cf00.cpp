class CAIBrain { public:
    void* vftable;
};

void __fastcall CAIBrain_dtor(CAIBrain* brain);
void operator delete(void*);

void* __fastcall CAIBrain_scalar_deleting_destructor(CAIBrain* brain, int edx_unused, unsigned int flags)
{
    CAIBrain_dtor(brain);
    if (flags & 1)
        operator delete(brain);
    return brain;
}