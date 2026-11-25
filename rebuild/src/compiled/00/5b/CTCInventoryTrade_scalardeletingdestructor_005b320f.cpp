#pragma optimize("s", on)
// Naming (NF423 gen, 2026-11-25): the callee 0x0059EC48 now binds its PDB spelling ??1CTCInventoryTrade@@UAE@XZ (the T1 tracker target; the self row's
//   Leaf / PDB name sets), called qualified on a declaration of the class (no body, no layout); was the free stand-in ?CTCInventoryTrade_Destruct@@YIXPAX@Z.
class CTCInventoryTrade { public: virtual ~CTCInventoryTrade(); };
void* __fastcall CTCInventoryTrade_ScalarDeletingDestructor(void* trade, int, unsigned int flags)
{
    reinterpret_cast<CTCInventoryTrade*>(trade)->CTCInventoryTrade::~CTCInventoryTrade();
    if (flags & 1)
        operator delete(trade);
    return trade;
}