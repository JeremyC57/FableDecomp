struct CTable {
    void* vftable;
};

void __fastcall CTable_dtor(CTable* table);
void operator delete(void*);

void* __fastcall CTable_scalar_deleting_destructor(CTable* table, int edx_unused, unsigned int flags)
{
    CTable_dtor(table);
    if (flags & 1)
        operator delete(table);
    return table;
}