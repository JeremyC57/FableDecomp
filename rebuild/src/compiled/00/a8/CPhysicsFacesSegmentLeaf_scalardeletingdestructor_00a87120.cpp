struct CPhysicsFacesSegmentLeaf {
    void* vftable;
};

void __fastcall CPhysicsFacesSegmentLeaf_dtor(CPhysicsFacesSegmentLeaf* leaf);
void operator delete(void*);

void* __fastcall CPhysicsFacesSegmentLeaf_scalar_deleting_destructor(CPhysicsFacesSegmentLeaf* leaf, int edx_unused, unsigned int flags)
{
    CPhysicsFacesSegmentLeaf_dtor(leaf);
    if (flags & 1)
        operator delete(leaf);
    return leaf;
}