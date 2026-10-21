// CTCShop::GetWantedItemsList() const -- the def indices of every stocked item whose price multiplier is above the shop def's wanted threshold (an empty list when the thing has no shop def)
// Retail: Fable.exe (TLC, VC7.1) @ 0x00774100, 432 bytes (+ int3 padding), `ret 4` (vector<long> by hidden pointer).
// Callees: CDefClassBase::GetSubDef<CShopDef> 0x005130A0 (landed spelling, CTCShop::GetName 0x00772610); vector<long>
//   copy ctor 0x00514160 (the by-value return: no NRVO in VC7.1); vector<long>::_M_insert_overflow 0x00440990 (push_back
//   growth); free 0x00BFEA14 (the local vector's storage); the defs' virtual slot 1 on release.
// Layout: CThing PDef +0x70 (landed FindDefaultStockItem 0x00772250); CTCShop Stock map<long, CShopItem> +0x34 (PDB +0x3c;
//   retail STLport vectors before it are 12 bytes); CShopDef MinPriceMultiplierForItemToBeWanted +0xa0 (PDB +0xb0: two
//   16-byte PDB maps are 12 bytes in retail); map node key +0x10, CShopItem +0x14 (PriceMultiplier +0x18).
// Shape: the def test is a named bool over two by-value CDefPointer copies of the thing's def (`inc [def+4]` per copy, the
//   conditional-temporary flags in ebx, the bool in a frame byte); the vector is the STLport _Vector_base/_STLP_alloc_proxy
//   structure; the loop inlines push_back and the tree increment. The out-of-line vector members get same-TU
//   `auto_inline(off)` STLport bodies (growth insert and copy ctor) so VC7.1 keeps the vector's finish pointer in ebp
//   across the loop (spilling `this`) and keeps the empty-vector knowledge on the early return (no free there).
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
};
struct CDefClassBase : CDefPointeeBase {
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