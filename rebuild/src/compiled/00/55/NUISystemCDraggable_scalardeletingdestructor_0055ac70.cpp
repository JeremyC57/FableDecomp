struct CDraggable {
    void* vftable;
};

void __fastcall CDraggable_dtor(CDraggable* draggable);
void operator delete(void*);

void* __fastcall CDraggable_scalar_deleting_destructor(CDraggable* draggable, int edx_unused, unsigned int flags)
{
    CDraggable_dtor(draggable);
    if (flags & 1)
        operator delete(draggable);
    return draggable;
}