class CEngineSceneGrid { public:
    void* vftable;
};

void __fastcall CEngineSceneGrid_dtor(CEngineSceneGrid* grid);
void operator delete(void*);

void* __fastcall CEngineSceneGrid_scalar_deleting_destructor(CEngineSceneGrid* grid, int edx_unused, unsigned int flags)
{
    CEngineSceneGrid_dtor(grid);
    if (flags & 1)
        operator delete(grid);
    return grid;
}