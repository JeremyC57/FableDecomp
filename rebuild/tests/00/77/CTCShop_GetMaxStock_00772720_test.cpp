#include <cstdio>
#ifdef _MSC_VER
#pragma optimize("y", off)
__forceinline long CeilToLong(float value)
{
    long result; float one; float half; float back;
    half = 0.5f; one = 1.0f;
    __asm {
        fld value
        fadd half
        fistp result
        fild result
        fsub one
        fstp back
        mov eax, value
        cmp eax, back
        jne done
        dec result
    done:
    }
    return result;
}
#else
#include <math.h>
inline long CeilToLong(float value) { return (long)ceil((double)value); }   // the FPU block is ceil() for the magnitudes used here
#endif
class CShopItem { public: float Quantity; float PriceMultiplier; float MaxStock; long RestockPeriod; float RestockPeriodRandomness; float SalesPerDay; float SalesPerDayRandomness; };
struct SStockNode { SStockNode* color_unused; SStockNode* parent; SStockNode* left; SStockNode* right; long Key; CShopItem Item; };
struct CStockMap {
    SStockNode* m_pHead;
    struct iterator { SStockNode* m_pNode; iterator() { } iterator(SStockNode* n) : m_pNode(n) { } };
    iterator find(const long& key)
    {
        SStockNode* y = m_pHead; SStockNode* x = m_pHead->parent;
        while (x != 0) { if (!(x->Key < key)) { y = x; x = x->left; } else x = x->right; }
        if (y == m_pHead || key < y->Key) return iterator(m_pHead);
        return iterator(y);
    }
};
class CTCShop { public:
    unsigned char _pad00[0x34]; CStockMap Stock;
    long GetStockQuantity(long item) { CStockMap::iterator it = Stock.find(item); if (it.m_pNode != Stock.m_pHead) return CeilToLong(it.m_pNode->Item.Quantity); return 0; }
    long GetMaxStock(long item) { CStockMap::iterator it = Stock.find(item); if (it.m_pNode != Stock.m_pHead) return CeilToLong(it.m_pNode->Item.MaxStock); return 0; }
};
static SStockNode g_head, g_n5, g_n9;
static CTCShop* MakeShop()
{
    static CTCShop shop;
    g_head.parent = &g_n9; g_head.left = 0; g_head.right = 0;
    g_n9.Key = 9; g_n9.left = &g_n5; g_n9.right = 0; g_n9.parent = &g_head; g_n9.Item.Quantity = 2.25f; g_n9.Item.MaxStock = 7.0f;
    g_n5.Key = 5; g_n5.left = 0; g_n5.right = 0; g_n5.parent = &g_n9; g_n5.Item.Quantity = 0.5f; g_n5.Item.MaxStock = 11.9f;
    shop.Stock.m_pHead = &g_head;
    return &shop;
}
int main()
{
    CTCShop* s = MakeShop();
    bool ok = s->GetMaxStock(9) == 7 && s->GetMaxStock(5) == 12 && s->GetMaxStock(1) == 0 && s->GetMaxStock(100) == 0;
    std::printf(ok ? "00772720_TEST PASS\n" : "00772720_TEST FAIL\n");
    return ok ? 0 : 1;
}