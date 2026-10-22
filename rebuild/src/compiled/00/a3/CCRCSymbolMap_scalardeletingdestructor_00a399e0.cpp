struct CCRCSymbolMap {
    void* vftable;
};

void __fastcall CCRCSymbolMap_dtor(CCRCSymbolMap* map);
void operator delete(void*);

void* __fastcall CCRCSymbolMap_scalar_deleting_destructor(CCRCSymbolMap* map, int edx_unused, unsigned int flags)
{
    CCRCSymbolMap_dtor(map);
    if (flags & 1)
        operator delete(map);
    return map;
}