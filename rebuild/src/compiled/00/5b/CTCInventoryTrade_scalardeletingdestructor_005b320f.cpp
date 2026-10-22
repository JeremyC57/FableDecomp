#pragma optimize("s", on)
extern void __fastcall CTCInventoryTrade_Destruct(void* trade);
extern void __cdecl OperatorDelete(void* object);
void* __fastcall CTCInventoryTrade_ScalarDeletingDestructor(void* trade, int, unsigned int flags)
{
    CTCInventoryTrade_Destruct(trade);
    if (flags & 1)
        OperatorDelete(trade);
    return trade;
}