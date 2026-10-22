struct CReactionMatch_Sphere {
    void* vftable;
};

void __fastcall CReactionMatch_Sphere_dtor(CReactionMatch_Sphere* sphere);
void operator delete(void*);

void* __fastcall CReactionMatch_Sphere_scalar_deleting_destructor(CReactionMatch_Sphere* sphere, int edx_unused, unsigned int flags)
{
    CReactionMatch_Sphere_dtor(sphere);
    if (flags & 1)
        operator delete(sphere);
    return sphere;
}