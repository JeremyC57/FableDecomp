#pragma optimize("s", on)
extern void __fastcall CTCLightningNode_Destruct(void* node);
extern void __cdecl OperatorDelete(void* object);
void* __fastcall CTCLightningNode_ScalarDeletingDestructor(void* node, int, unsigned int flags)
{
    CTCLightningNode_Destruct(node);
    if (flags & 1)
        OperatorDelete(node);
    return node;
}