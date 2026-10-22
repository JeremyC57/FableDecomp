struct CHoverable {
    void* vftable;
};

void __fastcall CHoverable_dtor(CHoverable* hoverable);
void operator delete(void*);

void* __fastcall CHoverable_scalar_deleting_destructor(CHoverable* hoverable, int edx_unused, unsigned int flags)
{
    CHoverable_dtor(hoverable);
    if (flags & 1)
        operator delete(hoverable);
    return hoverable;
}