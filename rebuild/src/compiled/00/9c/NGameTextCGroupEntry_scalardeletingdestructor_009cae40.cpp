struct CGroupEntry {
    void* vftable;
};

void __fastcall CGroupEntry_dtor(CGroupEntry* entry);
void operator delete(void*);

void* __fastcall CGroupEntry_scalar_deleting_destructor(CGroupEntry* entry, int edx_unused, unsigned int flags)
{
    CGroupEntry_dtor(entry);
    if (flags & 1)
        operator delete(entry);
    return entry;
}