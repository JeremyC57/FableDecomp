struct CTextEntry {
    void* vftable;
};

void __fastcall CTextEntry_dtor(CTextEntry* entry);
void operator delete(void*);

void* __fastcall CTextEntry_scalar_deleting_destructor(CTextEntry* entry, int edx_unused, unsigned int flags)
{
    CTextEntry_dtor(entry);
    if (flags & 1)
        operator delete(entry);
    return entry;
}