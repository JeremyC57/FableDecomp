// Behaviour test for CTCShop::GetWantedItemsList (0x00774100): items above the wanted threshold, in key order; refs balanced.
#include <new>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#ifndef _MSC_VER
#define __declspec(x)
#define __forceinline inline
#endif
class CDefPointeeBase { public:
    virtual void Destroy();                         // slot 0
    virtual void Delete();                          // slot 1 (+0x04)
    long m_refCount;                                // +0x04
    void AddReference() { ++m_refCount; }
    void ReleaseReference() { if (--m_refCount == 0) Delete(); }
};
template <class T> class CDefPointer { public:
    T* Object;
    CDefPointer() : Object(0) { }
    CDefPointer(T* p) : Object(p) { if (Object) Object->AddReference(); }
    CDefPointer(const CDefPointer& o) : Object(o.Object) { if (Object) Object->AddReference(); }
    ~CDefPointer() { if (Object) Object->ReleaseReference(); }
    T* operator->() const { return Object; }
    bool IsValid() const { return Object != 0; }
};
class CShopDef : public CDefPointeeBase { public:
    unsigned char _pad08[0xa0 - 0x08];
    float MinPriceMultiplierForItemToBeWanted;      // +0xa0 (PDB +0xb0)
};
class CDefClassBase : public CDefPointeeBase { public:
    bool GetSubDef(CDefPointer<CShopDef>* out);     // 0x005130A0
};
void* FindDef_CDefClassBase_GetSubDef_1();
#pragma auto_inline(off)
__declspec(noinline) bool CDefClassBase::GetSubDef(CDefPointer<CShopDef>* out)
{
    void* def = FindDef_CDefClassBase_GetSubDef_1();
    if (!def)
        return false;
    *(void**)out = def;
    return true;
}
#pragma auto_inline(on)

struct CLongAllocator {                             // STLport allocator<long>: empty
    CLongAllocator() { }
};
struct CLongAllocProxy : CLongAllocator {           // _STLP_alloc_proxy<long*, long, allocator<long> >
    long* m_data;
    CLongAllocProxy(const CLongAllocator& alloc, long* p) : CLongAllocator(alloc), m_data(p) { }
};
struct CLongVectorBase {                            // _Vector_base<long, allocator<long> >
    long* first;
    long* last;
    CLongAllocProxy end;
    CLongVectorBase(const CLongAllocator& alloc) : first(0), last(0), end(alloc, 0) { }
};
struct SScalarTag { SScalarTag() { } };
struct CLongVector : CLongVectorBase {              // retail STLport vector<long> (12 bytes)
    CLongVector() : CLongVectorBase(CLongAllocator()) { }
    CLongVector(const CLongVector& other);          // 0x00514160
    ~CLongVector() { if (first) free(first); }
    void InsertGrow(long* where, const long& value, const SScalarTag& tag, unsigned long count, bool atEnd);   // 0x00440990
    void push_back(const long& value)
    {
        if (last != end.m_data) {
            new (last) long(value);
            ++last;
        }
        else
            InsertGrow(last, value, SScalarTag(), 1, true);
    }
};
#pragma auto_inline(off)
static long* CopyTrivial(const long* first, const long* last, long* result)
{
    if (first == last)
        return result;
    unsigned int n = (const char*)last - (const char*)first;
    return (long*)((char*)memmove(result, first, n) + n);
}
// Same-TU bodies of the out-of-line vector<long> members (growth insert 0x00440990, copy ctor 0x00514160).
void CLongVector::InsertGrow(long* position, const long& x, const SScalarTag&, unsigned long n, bool atEnd)
{
    unsigned long oldSize = last - first;
    unsigned long len = oldSize + (oldSize < n ? n : oldSize);
    long* newStart = len ? (long*)malloc(len * sizeof(long)) : 0;
    long* newFinish = CopyTrivial(first, position, newStart);
    for (unsigned long i = n; i != 0; --i)
        *newFinish++ = x;
    if (!atEnd)
        newFinish = CopyTrivial(position, last, newFinish);
    if (first)
        free(first);
    first = newStart;
    last = newFinish;
    end.m_data = newStart + len;
}
CLongVector::CLongVector(const CLongVector& other) : CLongVectorBase(CLongAllocator())
{
    unsigned long length = other.last - other.first;
    first = length ? (long*)malloc(length * sizeof(long)) : 0;
    last = first;
    end.m_data = first + length;
    last = CopyTrivial(other.first, other.last, first);
}
#pragma auto_inline(on)

class CShopItem { public:
    float Quantity;                                 // +0x00
    float PriceMultiplier;                          // +0x04
};
struct SStockNode {                                 // STLport map<long, CShopItem> node
    long color;
    SStockNode* parent;                             // +0x04
    SStockNode* left;                               // +0x08
    SStockNode* right;                              // +0x0c
    long first;                                     // +0x10
    CShopItem second;                               // +0x14
};
static inline SStockNode* StockIncrement(SStockNode* node)
{
    if (node->right != 0) {
        node = node->right;
        while (node->left != 0)
            node = node->left;
    } else {
        SStockNode* y = node->parent;
        while (node == y->right) {
            node = y;
            y = y->parent;
        }
        if (node->right != y)
            node = y;
    }
    return node;
}
struct CStockMap {                                  // map<long, CShopItem> (retail 12 bytes)
    SStockNode* header;
    unsigned long _pad[2];
};
class CThing { public:
    unsigned char _pad00[0x70];
    CDefClassBase* PDef;                            // +0x70
    CDefPointer<CDefClassBase> GetPDef() const { return CDefPointer<CDefClassBase>(PDef); }
};
class CTCShop { public:
    virtual void v00();
    CThing* ParentThing;                            // +0x04
    unsigned char _pad08[0x34 - 0x08];
    CStockMap Stock;                                // +0x34
    CLongVector GetWantedItemsList() const;
};

CLongVector CTCShop::GetWantedItemsList() const
{
    CLongVector wanted;
    CDefPointer<CShopDef> shopDef;
    bool noShopDef = !ParentThing->GetPDef().IsValid() || !ParentThing->GetPDef()->GetSubDef(&shopDef);
    if (noShopDef)
        return wanted;
    for (SStockNode* it = Stock.header->left; it != Stock.header; it = StockIncrement(it))
        if (it->second.PriceMultiplier > shopDef->MinPriceMultiplierForItemToBeWanted)
            wanted.push_back(it->first);
    return wanted;
}

void CDefPointeeBase::Destroy() { }
void CDefPointeeBase::Delete() { }
static CShopDef* g_shopDef = 0;
void* FindDef_CDefClassBase_GetSubDef_1() { return g_shopDef; }
static int g_deleted = 0;
struct CTestThingDef : CDefClassBase { void Destroy() { } void Delete() { ++g_deleted; } };
struct CTestShopDef : CShopDef { void Destroy() { } void Delete() { ++g_deleted; } };
static bool Fail() { std::printf("00774100_TEST FAIL\n"); return true; }
int main()
{
    SStockNode header, a, b, c;
    std::memset(&header, 0, sizeof(header)); std::memset(&a, 0, sizeof(a));
    std::memset(&b, 0, sizeof(b)); std::memset(&c, 0, sizeof(c));
    header.parent = &b; header.left = &a; header.right = &c;
    b.parent = &header; b.left = &a; b.right = &c;
    a.parent = &b; c.parent = &b;
    a.first = 1; a.second.PriceMultiplier = 2.0f;
    b.first = 2; b.second.PriceMultiplier = 0.5f;
    c.first = 3; c.second.PriceMultiplier = 3.0f;
    CTestThingDef thingDef; thingDef.m_refCount = 7;
    CTestShopDef shopDef; shopDef.m_refCount = 5; shopDef.MinPriceMultiplierForItemToBeWanted = 1.0f;
    CThing thing; thing.PDef = &thingDef;
    CTCShop* shop = (CTCShop*)std::malloc(sizeof(CTCShop));
    shop->ParentThing = &thing; shop->Stock.header = &header;
    g_shopDef = &shopDef;
    {
        CLongVector v = shop->GetWantedItemsList();
        if (v.last - v.first != 2 || v.first[0] != 1 || v.first[1] != 3) return Fail(), 1;
    }
    if (thingDef.m_refCount != 7 || shopDef.m_refCount != 4 || g_deleted != 0) return Fail(), 1;
    shopDef.MinPriceMultiplierForItemToBeWanted = 2.5f;      // only item 3 is wanted now
    {
        CLongVector v = shop->GetWantedItemsList();
        if (v.last - v.first != 1 || v.first[0] != 3) return Fail(), 1;
    }
    g_shopDef = 0;                                              // no shop sub-def: empty list
    {
        CLongVector v = shop->GetWantedItemsList();
        if (v.first != v.last) return Fail(), 1;
    }
    thing.PDef = 0;                                             // no thing def: empty list
    {
        CLongVector v = shop->GetWantedItemsList();
        if (v.first != v.last) return Fail(), 1;
    }
    if (thingDef.m_refCount != 7 || shopDef.m_refCount != 3) return Fail(), 1;
    std::free(shop);
    std::printf("00774100_TEST PASS\n");
    return 0;
}