// CTCShop::GetWantedItemsList() const -- the def indices of every stocked item whose price multiplier is above the shop def's wanted threshold (an empty list when the thing has no shop def)
// Leaf: ?GetWantedItemsList@CTCShop@@QBE?AV?$vector@JV?$allocator@J@std@@@std@@XZ
// Naming wave 27 (family vl2, DECISIONS_W26 D6): the vector is STLport 4.6 std::vector<long, allocator<long> > (: _Vector_base, the
//   _STLP_alloc_proxy end pointer): the copy ctor 0x00514160 ??0?$vector@JV?$allocator@J@std@@@std@@QAE@ABV01@@Z and the protected
//   grow-insert 0x00440990 take their own rows' spellings, and the row its Ego_r / FableWin Leaf. Was CLongVector (CLongVectorBase /
//   CLongAllocProxy, InsertGrow(.., const SScalarTag&, unsigned long, bool)).
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
class CDefPointeeBase { public:
    virtual void Destroy();                         // slot 0
    virtual void Delete();                          // slot 1 (+0x04)
    long m_refCount;                                // +0x04
    void AddReference() { ++m_refCount; }
    void ReleaseReference() { if (--m_refCount == 0) Delete(); }
};
// agentNF15: the pointee is held non-const whatever T is (engine/CDefPointer.h stores CDefPointeeBase*), so the
//   CDefPointer<const T> out slots of the const def lookups keep the landed member code.
template <class T> struct CDefNonConst { typedef T Type; };
template <class T> struct CDefNonConst<const T> { typedef T Type; };
template <class T> class CDefPointer { public:
    typename CDefNonConst<T>::Type* Object;
    CDefPointer() : Object(0) { }
    CDefPointer(T* p) : Object(p) { if (Object) Object->AddReference(); }
    CDefPointer(const CDefPointer& o) : Object(o.Object) { if (Object) Object->AddReference(); }
    ~CDefPointer() { if (Object) Object->ReleaseReference(); }
    typename CDefNonConst<T>::Type* operator->() const { return Object; }
    bool IsValid() const { return Object != 0; }
};
class CShopDef : public CDefPointeeBase { public:
    unsigned char _pad08[0xa0 - 0x08];
    float MinPriceMultiplierForItemToBeWanted;      // +0xa0 (PDB +0xb0)
};
class CDefClassBase : public CDefPointeeBase { public:
    template <class T> bool GetSubDef(CDefPointer<const T>* out) const;   // 0x005130A0 GetSubDef<CShopDef> (Ego_r.pdb member template)
};
void* FindDef_CDefClassBase_GetSubDef_1();
#pragma auto_inline(off)
template <> __declspec(noinline) bool CDefClassBase::GetSubDef<CShopDef>(CDefPointer<const CShopDef>* out) const
{
    void* def = FindDef_CDefClassBase_GetSubDef_1();
    if (!def)
        return false;
    *(void**)out = def;
    return true;
}
#pragma auto_inline(on)

namespace std {
template <class _Tp> class allocator { public: allocator() { } };   // STLport allocator<long>: empty
struct __true_type { __true_type() { } };
template <class _Value, class _Tp, class _MaybeReboundAlloc>
class _STLP_alloc_proxy : public _MaybeReboundAlloc { public:      // _STLP_alloc_proxy<long*, long, allocator<long> >
    _Value _M_data;
    _STLP_alloc_proxy(const _MaybeReboundAlloc& __a, _Value __p) : _MaybeReboundAlloc(__a), _M_data(__p) { }
};
template <class _Tp, class _Alloc> class _Vector_base { public:    // _Vector_base<long, allocator<long> >
    _Tp* _M_start;
    _Tp* _M_finish;
    _STLP_alloc_proxy<_Tp*, _Tp, _Alloc> _M_end_of_storage;
    _Vector_base(const _Alloc& __a) : _M_start(0), _M_finish(0), _M_end_of_storage(__a, 0) { }
};
template <class _Tp, class _Alloc = allocator<_Tp> > class vector : public _Vector_base<_Tp, _Alloc> {   // 12 bytes
public:
    vector() : _Vector_base<_Tp, _Alloc>(_Alloc()) { }
    vector(const vector<_Tp, _Alloc>& __x);         // 0x00514160
    ~vector() { if (this->_M_start) free(this->_M_start); }
protected:
    void _M_insert_overflow(_Tp* __position, const _Tp& __x, const __true_type&, unsigned int __fill_len, bool __atend);   // 0x00440990
public:
    void push_back(const _Tp& __x)
    {
        if (this->_M_finish != this->_M_end_of_storage._M_data) {
            new (this->_M_finish) _Tp(__x);
            ++this->_M_finish;
        }
        else
            _M_insert_overflow(this->_M_finish, __x, __true_type(), 1, true);
    }
};
}
#pragma auto_inline(off)
static long* CopyTrivial(const long* first, const long* last, long* result)
{
    if (first == last)
        return result;
    unsigned int n = (const char*)last - (const char*)first;
    return (long*)((char*)memmove(result, first, n) + n);
}
// Same-TU bodies of the out-of-line vector<long> members (growth insert 0x00440990, copy ctor 0x00514160).
template <class _Tp, class _Alloc>
__declspec(noinline) void std::vector<_Tp, _Alloc>::_M_insert_overflow(_Tp* position, const _Tp& x, const __true_type&, unsigned int n, bool atEnd)
{
    unsigned long oldSize = this->_M_finish - this->_M_start;
    unsigned long len = oldSize + (oldSize < n ? n : oldSize);
    _Tp* newStart = len ? (_Tp*)malloc(len * sizeof(_Tp)) : 0;
    _Tp* newFinish = CopyTrivial(this->_M_start, position, newStart);
    for (unsigned long i = n; i != 0; --i)
        *newFinish++ = x;
    if (!atEnd)
        newFinish = CopyTrivial(position, this->_M_finish, newFinish);
    if (this->_M_start)
        free(this->_M_start);
    this->_M_start = newStart;
    this->_M_finish = newFinish;
    this->_M_end_of_storage._M_data = newStart + len;
}
template <class _Tp, class _Alloc>
__declspec(noinline) std::vector<_Tp, _Alloc>::vector(const vector<_Tp, _Alloc>& other) : _Vector_base<_Tp, _Alloc>(_Alloc())
{
    unsigned long length = other._M_finish - other._M_start;
    this->_M_start = length ? (_Tp*)malloc(length * sizeof(_Tp)) : 0;
    this->_M_finish = this->_M_start;
    this->_M_end_of_storage._M_data = this->_M_start + length;
    this->_M_finish = CopyTrivial(other._M_start, other._M_finish, this->_M_start);
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
    std::vector<long> GetWantedItemsList() const;
};

std::vector<long> CTCShop::GetWantedItemsList() const
{
    std::vector<long> wanted;
    CDefPointer<const CShopDef> shopDef;
    bool noShopDef = !ParentThing->GetPDef().IsValid() || !ParentThing->GetPDef()->GetSubDef(&shopDef);
    if (noShopDef)
        return wanted;
    for (SStockNode* it = Stock.header->left; it != Stock.header; it = StockIncrement(it))
        if (it->second.PriceMultiplier > shopDef->MinPriceMultiplierForItemToBeWanted)
            wanted.push_back(it->first);
    return wanted;
}