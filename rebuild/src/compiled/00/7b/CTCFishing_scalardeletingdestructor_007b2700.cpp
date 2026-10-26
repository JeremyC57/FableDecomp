class CTCFishing { public:
    void* vftable;
};

void __fastcall CTCFishing_dtor(CTCFishing* fishing);
void operator delete(void*);

void* __fastcall CTCFishing_scalar_deleting_destructor(CTCFishing* fishing, int edx_unused, unsigned int flags)
{
    CTCFishing_dtor(fishing);
    if (flags & 1)
        operator delete(fishing);
    return fishing;
}