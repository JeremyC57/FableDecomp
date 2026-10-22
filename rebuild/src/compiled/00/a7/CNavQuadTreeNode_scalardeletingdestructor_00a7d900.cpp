struct CNavQuadTreeNode {
    void* vftable;
};

void __fastcall CNavQuadTreeNode_dtor(CNavQuadTreeNode* node);
void operator delete(void*);

void* __fastcall CNavQuadTreeNode_scalar_deleting_destructor(CNavQuadTreeNode* node, int edx_unused, unsigned int flags)
{
    CNavQuadTreeNode_dtor(node);
    if (flags & 1)
        operator delete(node);
    return node;
}