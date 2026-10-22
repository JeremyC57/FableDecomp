struct CTriangleList {
    void* vftable;
};

void __fastcall CTriangleList_dtor(CTriangleList* list);
void operator delete(void*);

void* __fastcall CTriangleList_scalar_deleting_destructor(CTriangleList* list, int edx_unused, unsigned int flags)
{
    CTriangleList_dtor(list);
    if (flags & 1)
        operator delete(list);
    return list;
}