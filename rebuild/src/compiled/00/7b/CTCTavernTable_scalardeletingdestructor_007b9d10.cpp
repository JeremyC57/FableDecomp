struct CTCTavernTable {
    void* vftable;
};

void __fastcall CTCTavernTable_dtor(CTCTavernTable* table);
void operator delete(void*);

void* __fastcall CTCTavernTable_scalar_deleting_destructor(CTCTavernTable* table, int edx_unused, unsigned int flags)
{
    CTCTavernTable_dtor(table);
    if (flags & 1)
        operator delete(table);
    return table;
}