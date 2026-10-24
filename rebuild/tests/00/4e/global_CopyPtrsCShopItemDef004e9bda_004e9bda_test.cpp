#include <cstdio>

static int g_baseAssigns = 0;
struct CDefClassBase {
    unsigned char _body[0x21];
    CDefClassBase() { for (int i = 0; i < 0x21; ++i) _body[i] = 0; }
    virtual ~CDefClassBase() { }
    CDefClassBase& operator=(const CDefClassBase& other)
    {
        for (int i = 0; i < 0x21; ++i) _body[i] = other._body[i];
        ++g_baseAssigns;
        return *this;
    }
};
struct CShopItem {
    float Quantity, PriceMultiplier, MaxStock;
    long  RestockPeriod;
    float RestockPeriodRandomness, SalesPerDay, SalesPerDayRandomness;
};
struct CShopItemDef : CDefClassBase {
    long      Type;
    CShopItem Item;
    CShopItemDef() : Type(0) { Item.Quantity = 0; Item.RestockPeriod = 0; Item.SalesPerDayRandomness = 0; }
    CShopItemDef& operator=(const CShopItemDef& o)
    {
        CDefClassBase::operator=(o);
        Type = o.Type;
        Item = o.Item;
        return *this;
    }
};
struct __false_type { __false_type() { } };
struct random_access_iterator_tag { random_access_iterator_tag() { } };

static CShopItemDef* Copy(const CShopItemDef* first, const CShopItemDef* last, CShopItemDef* result, const random_access_iterator_tag&, int*)
{
    for (int n = int(last - first); n > 0; --n) {
        *result = *first;
        ++first;
        ++result;
    }
    return result;
}
static CShopItemDef* CopyPtrs(const CShopItemDef* first, const CShopItemDef* last, CShopItemDef* result, const __false_type&)
{
    return Copy(first, last, result, random_access_iterator_tag(), (int*)0);
}

int main()
{
    CShopItemDef src[3], dst[4];
    for (int i = 0; i < 3; ++i) {
        src[i]._body[0] = (unsigned char)(i + 1); src[i]._body[0x20] = (unsigned char)(0x40 + i);
        src[i].Type = 7 + i;
        src[i].Item.Quantity = 1.0f + i; src[i].Item.RestockPeriod = 30 * i; src[i].Item.SalesPerDayRandomness = 0.25f * i;
    }
    __false_type tag;
    CShopItemDef* r = CopyPtrs(src, src + 3, dst, tag);
    bool ok = r == dst + 3 && g_baseAssigns == 3;
    for (int i = 0; i < 3; ++i)
        if (dst[i]._body[0] != i + 1 || dst[i]._body[0x20] != 0x40 + i || dst[i].Type != 7 + i
            || dst[i].Item.Quantity != 1.0f + i || dst[i].Item.RestockPeriod != 30 * i || dst[i].Item.SalesPerDayRandomness != 0.25f * i) ok = false;
    if (dst[3].Type != 0 || dst[3]._body[0] != 0) ok = false;
    if (CopyPtrs(src, src, dst + 1, tag) != dst + 1 || g_baseAssigns != 3) ok = false;
    if (!ok) { std::printf("004e9bda_TEST FAIL\n"); return 1; }
    std::printf("004e9bda_TEST PASS\n");
    return 0;
}