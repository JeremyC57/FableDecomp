#pragma optimize("s", on)
extern void __fastcall CTCKraken_Destruct(void* kraken);
extern void __cdecl OperatorDelete(void* object);
void* __fastcall CTCKraken_ScalarDeletingDestructor(void* kraken, int, unsigned int flags)
{
    CTCKraken_Destruct(kraken);
    if (flags & 1)
        OperatorDelete(kraken);
    return kraken;
}