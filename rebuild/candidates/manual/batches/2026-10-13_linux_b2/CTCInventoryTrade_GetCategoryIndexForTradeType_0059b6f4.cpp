// CTCInventoryTrade::GetCategoryIndexForTradeType()
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0059B6F4, oracle length 35 bytes.
// Grade: candidate (authored from the retail bytes; byte parity NOT yet proven -- run
//        tools/decomp_pipeline/verify_and_land.py on this batch's payload to grade it).
//
// Retail bytes:  mov ecx,[ecx+170h] ; xor eax,eax ; L: cmp [eax*8+0137CCDCh],ecx ; je F ; inc eax ; cmp eax,2 ; jl L ; or eax,-1 ; ret ; F: mov eax,[eax*8+0137CCE0h] ; ret
// Looks the inventory screen's CategoryIdent up in a two-row {ident, index} table in .data at
// 0x0137CCDC: {11 -> 0, 12 -> 1}; anything else yields -1. Retail keeps CategoryIdent at +0x170
// where engine/CTCInventoryTrade.h (Ego_r.pdb) has it at +0x19C: the shipped class is 0x2C bytes
// smaller before that member, so a retail-offset view of `this` is declared here.

struct CTradeTypeCategory {
    long Ident;          // EInventoryCategoryIdentifier value
    long CategoryIndex;  // trade category index
};

extern CTradeTypeCategory g_TradeTypeCategories[2];    // 0x0137CCDC: {11,0}, {12,1}

struct CTCInventoryTrade_Retail {
    unsigned char _pad_0x000[0x170];
    long          CategoryIdent;      // +0x170 (PDB +0x19C)
    long GetCategoryIndexForTradeType();
};

long CTCInventoryTrade_Retail::GetCategoryIndexForTradeType()
{
    long ident = this->CategoryIdent;
    for (long i = 0; i < 2; ++i) {
        if (g_TradeTypeCategories[i].Ident == ident)
            return g_TradeTypeCategories[i].CategoryIndex;
    }
    return -1;
}
