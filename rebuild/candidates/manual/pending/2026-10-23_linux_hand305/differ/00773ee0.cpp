// CTCShop::GetSaleItemsToPlace() -- the stocked item indices (in stock, level within the shop def's limit, placeable) in a random order; empty when the thing has no shop def
// Leaf: GetSaleItemsToPlace
// Grade: DIFFER(495v540) -- retail keeps this in ebp and tests nulls with test; ours picks a zero register in ebp (as the landed 0x774100 does) and spills this, so the early return merges into the late one
// Retail: Fable.exe (TLC, VC7.1) @ 0x00773EE0, 538 bytes, `ret 4` (vector<long> by hidden pointer).
// Callees: CDefClassBase::GetSubDef<CShopDef> 0x005130A0, CTCBase::GetDefinitionManager 0x006869D0,
//   CDefinitionManager::GetPDefFromGlobalIndex(long) 0x009AD9E0, CDefClassBase::GetSubDef<CItemDef> 0x0059350C,
//   vector<long>::push_back 0x00485FF3 (out of line here), random_shuffle<long*> 0x00777080 (fastcall), vector<long>
//   copy ctor 0x00514160, free 0x00BFEA14, growth insert 0x00440990 (reached through push_back only).
// Layout: as the landed CTCShop::GetWantedItemsList 0x00774100 (Stock map +0x34, CThing PDef +0x70); CShopDef
//   +0xa4 the item-level limit; CItemDef +0x28 its level (int), +0x3c the placeable flag.
// Shape: the landed 0x00774100 model (named bool over two by-value def pointer copies, STLport vector structure,
//   same-TU vector bodies); the per-item def is a by-value CDefPointer returned by the definition manager whose release
//   is the DecRef/GetRefCount pair with the null store.
#include <new>
extern "C" void* __cdecl malloc(unsigned int size);         // 0x00BFEA0E
extern "C" void __cdecl free(void* block);                   // 0x00BFEA14
extern "C" void* __cdecl memmove(void* dest, const void* src, unsigned int count);   // 0x00BFEAE6
struct CDefPointeeBase {
    virtual void Destroy();                         // slot 0
    virtual void Delete();                          // slot 1 (+0x04)
    long m_refCount;                                // +0x04
    void AddReference() { ++m_refCount; }
    void ReleaseReference() { if (--m_refCount == 0) Delete(); }
};
template <class T> struct CDefPointer {
    T* Object;
    CDefPointer() : Object(0) { }
    CDefPointer(T* p) : Object(p) { if (Object) Object->AddReference(); }
    CDefPointer(const CDefPointer& o) : Object(o.Object) { if (Object) Object->AddReference(); }
    ~CDefPointer() { if (Object) Object->ReleaseReference(); }
    T* operator->() const { return Object; }
    bool IsValid() const { return Object != 0; }
};
struct CShopDef : CDefPointeeBase {
    unsigned char _pad08[0xa0 - 0x08];
    float MinPriceMultiplierForItemToBeWanted;      // +0xa0 (PDB +0xb0)
    float MaxPriceToDisplay;                        // +0xa4 (PDB +0xb4)
};
struct CStockItemDef : CDefPointeeBase {
    unsigned char _pad08[0x28 - 0x08];
    long DefaultPrice;                              // +0x28
    bool DefaultIsStealable;                        // +0x2c
    bool CanOnlyBeSoldToPlayer;                     // +0x2d
    unsigned char _pad2e[0x3c - 0x2e];
    bool CanBeDisplayedInShop;                      // +0x3c
};
struct CDefClassBase : CDefPointeeBase {
    bool GetSubDef(CDefPointer<CShopDef>* out);     // 0x005130A0
    bool GetSubDef(CDefPointer<CStockItemDef>* out);   // 0x0059350C
    void DecRef() { --m_refCount; }
    long GetRefCount() const { return m_refCount; }
    void Release()
    {
        DecRef();
        if (GetRefCount() == 0)
            Delete();
    }
};
void* FindDef_CDefClassBase_GetSubDef_2();
#pragma auto_inline(off)
__declspec(noinline) bool CDefClassBase::GetSubDef(CDefPointer<CStockItemDef>* out)
{
    void* def = FindDef_CDefClassBase_GetSubDef_2();
    if (!def)
        return false;
    *(void**)out = def;
    return true;
}
#pragma auto_inline(on)
struct CDefClassPointer {                           // CDefPointer<CDefClassBase> as returned by the definition manager
    CDefClassBase* Object;
    ~CDefClassPointer()
    {
        if (Object) {
            Object->Release();
            Object = 0;
        }
    }
    CDefClassBase* operator->() const { return Object; }
};
struct CDefinitionManager {
    CDefClassPointer GetPDefFromGlobalIndex(long index);   // 0x009AD9E0
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
    void push_back(const long& value);              // 0x00485FF3 (out of line in this TU)
    void InsertGrow(long* where, const long& value, const SScalarTag& tag, unsigned long count, bool atEnd);   // 0x00440990
    void push_back_inline(const long& value)
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

struct CShopItem {
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
void __fastcall RandomShuffle(long* first, long* last);   // 0x00777080 (random_shuffle<long*>)

struct CThing {
    unsigned char _pad00[0x70];
    CDefClassBase* PDef;                            // +0x70
    CDefPointer<CDefClassBase> GetPDef() const { return CDefPointer<CDefClassBase>(PDef); }
};
struct CTCShop {
    virtual void v00();
    CThing* ParentThing;                            // +0x04
    unsigned char _pad08[0x34 - 0x08];
    CStockMap Stock;                                // +0x34
    CDefinitionManager* GetDefinitionManager() const;   // 0x006869D0
    CLongVector GetSaleItemsToPlace() const;
};

CLongVector CTCShop::GetSaleItemsToPlace() const
{
    CLongVector items;
    CDefPointer<CShopDef> shopDef;
    bool noShopDef = !ParentThing->GetPDef().IsValid() || !ParentThing->GetPDef()->GetSubDef(&shopDef);
    if (noShopDef)
        return items;
    for (SStockNode* it = Stock.header->left; it != Stock.header; it = StockIncrement(it)) {
        if (it->first > 0 && it->second.Quantity > 0.0f) {
            CDefClassPointer itemDef = GetDefinitionManager()->GetPDefFromGlobalIndex(it->first);
            CDefPointer<CStockItemDef> stockDef;
            if (itemDef->GetSubDef(&stockDef) &&
                stockDef->DefaultPrice <= shopDef->MaxPriceToDisplay && stockDef->CanBeDisplayedInShop)
                items.push_back(it->first);
        }
    }
    RandomShuffle(items.first, items.last);
    return items;
}
