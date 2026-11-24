// Leaf: ?CanRelocateData@CLandscapeBackgroundTreeNode@@EBE_NJ@Z
struct Inner {
    char pad[0x5c];
    unsigned char flag;
};
class CLandscapeBackgroundTreeNode { public:
    char pad[0xa0];
    Inner* inner;
private:
    virtual bool CanRelocateData(long) const;
};
bool CLandscapeBackgroundTreeNode::CanRelocateData(long a) const
{
    CLandscapeBackgroundTreeNode* node = const_cast<CLandscapeBackgroundTreeNode*>(this);
    (void)a;
    return node->inner->flag == 0;
}
// Flatten route (flatten_pilot v4, NF324): CLandscapeBackgroundTreeNode_CanRelocateData -> CLandscapeBackgroundTreeNode::CanRelocateData, the RTTI-placed PDB member; class model: the row's local class model.
