#pragma optimize("s", on)
extern void __fastcall CTCInventoryClothing_Destruct(void* clothing);
extern void __cdecl OperatorDelete(void* object);
void* __fastcall CTCInventoryClothing_ScalarDeletingDestructor(void* clothing, int, unsigned int flags)
{
    CTCInventoryClothing_Destruct(clothing);
    if (flags & 1)
        OperatorDelete(clothing);
    return clothing;
}