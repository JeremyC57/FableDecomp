class CWorldMap { public:
    void* vftable;
};

void __fastcall CWorldMap_dtor(CWorldMap* map);
void operator delete(void*);

void* __fastcall CWorldMap_scalar_deleting_destructor(CWorldMap* map, int edx_unused, unsigned int flags)
{
    CWorldMap_dtor(map);
    if (flags & 1)
        operator delete(map);
    return map;
}