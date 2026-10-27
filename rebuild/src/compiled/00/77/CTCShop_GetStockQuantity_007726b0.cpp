// CTCShop::GetStockQuantity(long item) -- look the item up in the Stock map and return its Quantity rounded up to a whole unit (0 when the shop has no such item)
// Leaf: GetStockQuantity
// Retail-asm: 0x007726E7..0x007726F6 `fld [ebp-0Ch] ; fadd [ebp-8] ; fistp dword ptr [ebp+8] ; fild [ebp+8] ; fsub [ebp-4] ; fstp [ebp-10h]` -- the engine's inline-assembly ceiling helper expanded in place (the member copied into the by-value parameter word, 0.5f / 1.0f stored as frame constants, a bare dword fistp into a long homed in the dead `item` argument slot). VC7.1 emits no bare dword fistp from C++ (`(long)x` calls __ftol2, /QIfist gives `fistp qword`, /arch:SSE2 cvttss2si: rasm1 probe family (a)); the fix-up `mov eax,value ; cmp ; jne ; dec dword ptr [ebp+8]` is part of the same block (10 instructions, one jne: rasm1 family (b)), the result read back by C++.
// Retail: Fable.exe (TLC, VC7.1) function @ 0x007726B0, 101 bytes (`ret 4`, ebp frame from the inline asm). Twin of 0x00772720
// (GetMaxStock, node +0x1c); the manifest label "GetMaxStock" on this row is propagated (it reads node +0x14 = Quantity:
// ghidra_out/labels_fixnames_corrections_2026-10-24.tsv).
// Callees: map<long, CShopItem>::find(const long&) @ 0x00777170 (iterator through the hidden pointer, `ret 8`).
// Layout: retail CTCShop Stock (STLport map<long, CShopItem>; head pointer at +0) +0x34; node {key +0x10, CShopItem +0x14}.
#pragma optimize("y", off)
class CShopItem { public:                                   // Ego_r.pdb CShopItem (0x1C)
    float Quantity;                                  // +0x00
    float PriceMultiplier;                           // +0x04
    float MaxStock;                                  // +0x08
    long  RestockPeriod;                             // +0x0c
    float RestockPeriodRandomness;                   // +0x10
    float SalesPerDay;                               // +0x14
    float SalesPerDayRandomness;                     // +0x18
};
struct SStockNode {
    unsigned char _rb[0x10];                         // colour, parent, left, right
    long      Key;                                   // +0x10
    CShopItem Item;                                  // +0x14
};
struct CStockMap {                                   // STLport map<long, CShopItem>: the header node pointer at +0
    SStockNode* m_pHead;
    struct iterator {
        SStockNode* m_pNode;
        iterator() { }
        iterator(SStockNode* node) : m_pNode(node) { }
    };
    iterator find(const long& key);                  // 0x00777170 (the same-TU instantiation; the landed 0x00772790 model)
};
// Same-TU body of the STLport map::find instantiation 0x00777170 (the landed 0x00772790 model): lower_bound walk from the
// root (header +4; left +8, right +0xc) reading the key through its const reference, then the end() test. With the body in
// the TU VC7.1 knows the key's address does not escape, so the dead `item` slot and the iterator word are reused by the helper.
#pragma auto_inline(off)
__declspec(noinline) CStockMap::iterator CStockMap::find(const long& key)
{
    SStockNode* y = m_pHead;
    SStockNode* x = *(SStockNode**)((unsigned char*)m_pHead + 4);
    while (x != 0) {
        if (!(x->Key < key)) {
            y = x;
            x = *(SStockNode**)((unsigned char*)x + 8);
        } else
            x = *(SStockNode**)((unsigned char*)x + 0xc);
    }
    if (y == m_pHead || key < y->Key)
        return iterator(m_pHead);
    return iterator(y);
}
#pragma auto_inline(on)
class CTCShop {
public:
    unsigned char _pad00[0x34];
    CStockMap     Stock;                             // +0x34
    long GetStockQuantity(long item);                // 0x007726B0
    long GetMaxStock(long item);                     // 0x00772720
};
// the engine's inline-assembly FPU ceiling helper (0x004FA9A0 is its standalone copy): VC7.1 expands the __forceinline body in
// place; the by-value parameter and the four locals become frame words, which is why the function carries an ebp frame.
// The whole 10-instruction block is asm (the bit compare and the memory decrement included: rasm1 family (b)); the result
// is read back by C++ after the block.
__forceinline long CeilToLong(float value)
{
    long  result;
    float one;
    float half;
    float back;
    half = 0.5f;
    one = 1.0f;
    __asm {
        fld   value
        fadd  half
        fistp result
        fild  result
        fsub  one
        fstp  back
        mov   eax, value
        cmp   eax, back
        jne   done
        dec   result
    done:
    }
    return result;
}
long CTCShop::GetStockQuantity(long item)
{
    CStockMap::iterator it = Stock.find(item);
    if (it.m_pNode != Stock.m_pHead)
        return CeilToLong(it.m_pNode->Item.Quantity);
    return 0;
}