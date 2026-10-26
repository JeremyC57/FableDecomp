struct Inner {
    char pad[0x5c];
    unsigned char flag;
};
class CLandscapeBackgroundTreeNode { public:
    char pad[0xa4];
    Inner* inner;
};
bool __fastcall CLandscapeBackgroundTreeNode_CanRelocateData(CLandscapeBackgroundTreeNode* node, int /*edx*/, long a)
{
    (void)a;
    return node->inner->flag == 0;
}