#pragma optimize("s", on)
extern void __fastcall CTCLightningNode_Destruct(void* node);
void* __fastcall CTCLightningNode_ScalarDeletingDestructor(void* node, int, unsigned int flags)
{
    CTCLightningNode_Destruct(node);
    if (flags & 1)
        operator delete(node);
    return node;
}