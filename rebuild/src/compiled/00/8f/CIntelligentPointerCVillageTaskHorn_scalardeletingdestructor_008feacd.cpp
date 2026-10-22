#pragma optimize("s", on)
extern void __fastcall CIntelligentPointer_Destruct(void* pointer);
extern void __cdecl OperatorDelete(void* object);
void* __fastcall CIntelligentPointer_ScalarDeletingDestructor(void* pointer, int, unsigned int flags)
{
    CIntelligentPointer_Destruct(pointer);
    if (flags & 1)
        OperatorDelete(pointer);
    return pointer;
}