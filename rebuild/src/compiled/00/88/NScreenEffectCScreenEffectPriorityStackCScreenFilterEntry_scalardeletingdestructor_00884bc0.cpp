struct CScreenFilterEntry {
    void* vftable;
};

void __fastcall CScreenFilterEntry_dtor(CScreenFilterEntry* entry);
void operator delete(void*);

void* __fastcall CScreenFilterEntry_scalar_deleting_destructor(CScreenFilterEntry* entry, int edx_unused, unsigned int flags)
{
    CScreenFilterEntry_dtor(entry);
    if (flags & 1)
        operator delete(entry);
    return entry;
}