struct CNavNavigableLeafNode {
    void* vftable;
};

void __fastcall CNavNavigableLeafNode_dtor(CNavNavigableLeafNode* node);
void operator delete(void*);

void* __fastcall CNavNavigableLeafNode_scalar_deleting_destructor(CNavNavigableLeafNode* node, int edx_unused, unsigned int flags)
{
    CNavNavigableLeafNode_dtor(node);
    if (flags & 1)
        operator delete(node);
    return node;
}