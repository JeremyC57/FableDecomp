#pragma optimize("s", on)
extern void __fastcall CTCInventoryClothing_Destruct(void* clothing);
void* __fastcall CTCInventoryClothing_ScalarDeletingDestructor(void* clothing, int, unsigned int flags)
{
    CTCInventoryClothing_Destruct(clothing);
    if (flags & 1)
        operator delete(clothing);
    return clothing;
}