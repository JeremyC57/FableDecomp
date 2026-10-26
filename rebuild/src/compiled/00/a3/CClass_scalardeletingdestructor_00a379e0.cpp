class CClass { public:
    void* vftable;
};

void __fastcall CClass_dtor(CClass* object);
void operator delete(void*);

void* __fastcall CClass_scalar_deleting_destructor(CClass* object, int edx_unused, unsigned int flags)
{
    CClass_dtor(object);
    if (flags & 1)
        operator delete(object);
    return object;
}