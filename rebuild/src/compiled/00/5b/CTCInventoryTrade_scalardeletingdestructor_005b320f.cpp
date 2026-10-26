#pragma optimize("s", on)
extern void __fastcall CTCInventoryTrade_Destruct(void* trade);
void* __fastcall CTCInventoryTrade_ScalarDeletingDestructor(void* trade, int, unsigned int flags)
{
    CTCInventoryTrade_Destruct(trade);
    if (flags & 1)
        operator delete(trade);
    return trade;
}