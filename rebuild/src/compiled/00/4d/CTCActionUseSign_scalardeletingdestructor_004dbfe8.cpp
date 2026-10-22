#pragma optimize("s", on)
extern void __fastcall CTCActionUseSign_Destruct(void* sign);
extern void __cdecl OperatorDelete(void* object);
void* __fastcall CTCActionUseSign_ScalarDeletingDestructor(void* sign, int, unsigned int flags)
{
    CTCActionUseSign_Destruct(sign);
    if (flags & 1)
        OperatorDelete(sign);
    return sign;
}