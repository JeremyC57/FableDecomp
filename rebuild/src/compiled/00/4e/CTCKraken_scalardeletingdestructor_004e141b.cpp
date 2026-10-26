#pragma optimize("s", on)
extern void __fastcall CTCKraken_Destruct(void* kraken);
void* __fastcall CTCKraken_ScalarDeletingDestructor(void* kraken, int, unsigned int flags)
{
    CTCKraken_Destruct(kraken);
    if (flags & 1)
        operator delete(kraken);
    return kraken;
}