class CTCBoss { public:
    void* vftable;
};

void __fastcall CTCBoss_dtor(CTCBoss* boss);
void operator delete(void*);

void* __fastcall CTCBoss_scalar_deleting_destructor(CTCBoss* boss, int edx_unused, unsigned int flags)
{
    CTCBoss_dtor(boss);
    if (flags & 1)
        operator delete(boss);
    return boss;
}