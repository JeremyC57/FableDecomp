class CNavQuadTree { public:
    void* vftable;
};

void __fastcall CNavQuadTree_dtor(CNavQuadTree* tree);
void operator delete(void*);

void* __fastcall CNavQuadTree_scalar_deleting_destructor(CNavQuadTree* tree, int edx_unused, unsigned int flags)
{
    CNavQuadTree_dtor(tree);
    if (flags & 1)
        operator delete(tree);
    return tree;
}