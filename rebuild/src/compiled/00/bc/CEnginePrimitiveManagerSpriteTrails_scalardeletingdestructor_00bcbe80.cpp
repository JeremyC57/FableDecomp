class CEnginePrimitiveManagerSpriteTrails { public:
    void* vftable;
};

void __fastcall CEnginePrimitiveManagerSpriteTrails_dtor(CEnginePrimitiveManagerSpriteTrails* trails);
void operator delete(void*);

void* __fastcall CEnginePrimitiveManagerSpriteTrails_scalar_deleting_destructor(CEnginePrimitiveManagerSpriteTrails* trails, int edx_unused, unsigned int flags)
{
    CEnginePrimitiveManagerSpriteTrails_dtor(trails);
    if (flags & 1)
        operator delete(trails);
    return trails;
}