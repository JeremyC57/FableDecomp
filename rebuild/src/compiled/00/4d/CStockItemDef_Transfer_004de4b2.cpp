// CStockItemDef::Transfer(CPersistContext& context) -- persist the def's fields one by one (DefaultPrice, DefaultIsStealable, SellersBanter, BuyersPhrase, CanOnlyBeSoldToPlayer, ChanceOfBecomingAvailable, CanBeDisplayedInShop)
// Leaf: ?Transfer@CStockItemDef@@UAEXAAVCPersistContext@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004DE4B2, 90 bytes (size-optimised def TU, odd entry), thiscall, `ret 4`.
// Identity: CStockItemDef::Transfer(CPersistContext&) -- proven; evidence: the catalogue row is this virtual (manifest
//   ?Transfer@CStockItemDef@@UAEXAAVCPersistContext@@@Z, Ego_r.pdb name set); the seven transferred offsets and their callee
//   overloads match Ego_r.pdb CStockItemDef's members and types exactly (retail offset = Ego_r offset). Family G061 (seven def Transfers of seven fields; the G057 recipe).
// Callees: CPersistContext::Transfer(unsigned long&) 0x00431020, Transfer(long&) 0x00431102, Transfer(float&) 0x00431061,
//   Transfer(bool&) 0x0043314A (each bound under that name by landed defs).
// Shape: context in edi and this in esi for the whole body; the last field's address is `add esi, N` in place.
#pragma optimize("s", on)
class CPersistContext {
public:
    void Transfer(unsigned long& value);                   // 0x00431020
    void Transfer(long& value);                            // 0x00431102
    void Transfer(float& value);                           // 0x00431061
    void Transfer(bool& value);                            // 0x0043314A
    template <class T> void Transfer(T& value);            // enum / class instances in this TU
};
class CStockItemDef {                                              // retail fields from +0x28
public:
    virtual void Transfer(CPersistContext& context);
    unsigned char _base[0x24];                    // +0x04 def base body
    long DefaultPrice;                       // +0x28
    bool DefaultIsStealable;                 // +0x2c
    bool CanOnlyBeSoldToPlayer;              // +0x2d
    unsigned long SellersBanter;             // +0x30
    unsigned long BuyersPhrase;              // +0x34
    float ChanceOfBecomingAvailable;         // +0x38
    bool CanBeDisplayedInShop;               // +0x3c
};
void CStockItemDef::Transfer(CPersistContext& context)
{
    context.Transfer(DefaultPrice);
    context.Transfer(DefaultIsStealable);
    context.Transfer(SellersBanter);
    context.Transfer(BuyersPhrase);
    context.Transfer(CanOnlyBeSoldToPlayer);
    context.Transfer(ChanceOfBecomingAvailable);
    context.Transfer(CanBeDisplayedInShop);
}