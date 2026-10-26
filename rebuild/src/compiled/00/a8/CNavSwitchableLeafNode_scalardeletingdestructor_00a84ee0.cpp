class CNavSwitchableLeafNode { public:
    void* vftable;
};

void __fastcall CNavSwitchableLeafNode_dtor(CNavSwitchableLeafNode* node);
void operator delete(void*);

void* __fastcall CNavSwitchableLeafNode_scalar_deleting_destructor(CNavSwitchableLeafNode* node, int edx_unused, unsigned int flags)
{
    CNavSwitchableLeafNode_dtor(node);
    if (flags & 1)
        operator delete(node);
    return node;
}