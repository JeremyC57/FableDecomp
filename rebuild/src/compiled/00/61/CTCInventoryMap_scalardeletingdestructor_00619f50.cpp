struct CTCInventoryMap {
    void* vftable;
};

void __fastcall CTCInventoryMap_dtor(CTCInventoryMap* map);
void operator delete(void*);

void* __fastcall CTCInventoryMap_scalar_deleting_destructor(CTCInventoryMap* map, int edx_unused, unsigned int flags)
{
    CTCInventoryMap_dtor(map);
    if (flags & 1)
        operator delete(map);
    return map;
}