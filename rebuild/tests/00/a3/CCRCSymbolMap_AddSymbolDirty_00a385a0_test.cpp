// Behaviour test for 0x00A385A0 (source body copied from src/00a385a0.cpp; out-of-line callees stubbed below).
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#ifdef _MSC_VER
inline void* operator new(unsigned int, void* p) { return p; }
#else
#include <new>
#define __fastcall
#define __cdecl
#define __stdcall
#define __fastcall
#define __forceinline inline
#define __declspec(x)
#endif

struct CCharStringData {                                    // CBasicString<char> head of the shared block
    const char*   PData;                                    // +0x00
    unsigned long StringLength;                             // +0x04
};
extern const char g_emptyLiteral_0129aaf4[];                // 0x0129AAF4: ""
class CCharString {
public:
    CCharStringData* PStringData;
    unsigned long GetLength() const { return PStringData == 0 ? 0 : PStringData->StringLength; }
    const char* GetPString() const { return PStringData == 0 ? g_emptyLiteral_0129aaf4 : PStringData->PData; }
};

unsigned long __fastcall Calc_004014a0(unsigned long seed, const void* data, unsigned long length);   // 0x004014A0: CRC-32

class CBase {
public:
    virtual ~CBase();
};

namespace _STL {
struct __true_type { __true_type() { } };
template <class _T1, class _T2> inline void _Construct(_T1* __p, const _T2& __val) { new (__p) _T1(__val); }
template <class _T1, class _T2> struct pair {
    _T1 first;
    _T2 second;
    pair(const _T1& __a, const _T2& __b) : first(__a), second(__b) { }
};
template <class _Tp> class allocator { public: allocator() { } };
template <class _Tp, class _Alloc = allocator<_Tp> > class vector {
public:
    _Tp* _M_start;                                          // +0x00
    _Tp* _M_finish;                                         // +0x04
    _Tp* _M_end_of_storage;                                 // +0x08
    _Tp* end() const { return _M_finish; }
    void _M_insert_overflow(_Tp* __position, const _Tp& __x, const __true_type&, unsigned int __fill_len, bool __atend);   // 0x009B61F0
    void push_back(const _Tp& __x)
    {
        if (_M_finish != _M_end_of_storage) {
            _Construct(_M_finish, __x);
            ++_M_finish;
        }
        else
            _M_insert_overflow(_M_finish, __x, __true_type(), 1UL, true);
    }
};
template <class _Tp> struct less { };
struct _Rb_tree_node_base;
struct _Rb_tree_iterator { _Rb_tree_node_base* _M_node; };
template <class _Key, class _Compare = less<_Key>, class _Alloc = allocator<_Key> > class set {
public:
    void*        _M_header;                                 // +0x00
    unsigned int _M_node_count;                             // +0x04
    _Compare     _M_key_compare;                            // +0x08
    pair<_Rb_tree_iterator, bool> insert(const _Key& __x);  // 0x00514C10
};

#pragma auto_inline(off)
// Same-TU stand-in for 0x009B61F0 (constructs in place and adopts the end; never touches the tag or keeps `__x`).
template <class _Tp, class _Alloc>
__declspec(noinline) void vector<_Tp, _Alloc>::_M_insert_overflow(_Tp* __position, const _Tp& __x, const __true_type&, unsigned int __fill_len, bool __atend)
{
    _Construct(__position, __x);
    _M_finish = __position + __fill_len;
    if (__atend)
        _M_end_of_storage = _M_finish;
}
// Same-TU stand-in for 0x00514C10 (reads the key, never keeps its address).
template <class _Key, class _Compare, class _Alloc>
__declspec(noinline) pair<_Rb_tree_iterator, bool> set<_Key, _Compare, _Alloc>::insert(const _Key& __x)
{
    _Rb_tree_iterator __it;
    __it._M_node = (_Rb_tree_node_base*)_M_header;
    _M_node_count += (unsigned int)(__x != 0);
    return pair<_Rb_tree_iterator, bool>(__it, true);
}
#pragma auto_inline(on)
}

template <class K, class V> class CKeyPairCompareLess { };

template <class K, class V, class C = CKeyPairCompareLess<K, V> > class CVectorMap {   // retail 16 bytes
public:
    typedef _STL::pair<K, V> value_type;
    _STL::vector<value_type> Data;                          // +0x00
    C                        Compare;                       // +0x0C (empty comparator)
    bool                     Dirty;                         // +0x0D
    const value_type* LowerBound(const K& key) const;       // 0x009B21F0
    const value_type* End() const { return Data.end(); }
    const value_type* Find(const K& key) const
    {
        const value_type* it = LowerBound(key);
        if (it == Data.end() || key < it->first)
            return Data.end();
        return it;
    }
    __forceinline void Insert(const K& key, const V& value)
    {
        Data.push_back(value_type(key, value));
        Dirty = true;
    }
};

// Layout: Ego_r.pdb CCRCSymbolMap with the retail 16-byte CVectorMap and 12-byte STLport set (the landed constructor
// 0x00A38500): SLONGMap +0x04 (Dirty +0x11), NamesUsed +0x14, CheckForDuplicateSymbols +0x20.
class CCRCSymbolMap : public CBase {                        // vtable 0x0129CF84
public:
    CVectorMap<unsigned long, long> SLONGMap;               // +0x04
    _STL::set<unsigned long>        NamesUsed;              // +0x14
    bool                            CheckForDuplicateSymbols;   // +0x20
    void AddSymbolDirty(const CCharString& name, long value);
    bool FindSymbol(const CCharString& name, long& value) const;
};

void CCRCSymbolMap::AddSymbolDirty(const CCharString& name, long value)
{
    if (CheckForDuplicateSymbols)
        NamesUsed.insert(Calc_004014a0(0, name.GetPString(), name.GetLength()));
    SLONGMap.Insert(Calc_004014a0(0, name.GetPString(), name.GetLength()), value);
}

// ---- test harness ----
const char g_emptyLiteral_0129aaf4[] = "";
static int g_calc = 0;
unsigned long __fastcall Calc_004014a0(unsigned long seed, const void* data, unsigned long length)
{
    ++g_calc;
    unsigned long h = seed ^ 0x1234u;
    const unsigned char* p = (const unsigned char*)data;
    for (unsigned long i = 0; i < length; ++i) h = h * 31u + p[i];
    return h;
}
CBase::~CBase() { }
int main()
{
    int ok = 1;
    static unsigned char room[8 * sizeof(_STL::pair<unsigned long, long>)];
    CCRCSymbolMap* m = (CCRCSymbolMap*)malloc(sizeof(CCRCSymbolMap));
    memset(m, 0, sizeof(CCRCSymbolMap));
    typedef _STL::pair<unsigned long, long> TPair;
    m->SLONGMap.Data._M_start = m->SLONGMap.Data._M_finish = (TPair*)room;
    m->SLONGMap.Data._M_end_of_storage = (TPair*)room + 1;
    CCharStringData data; data.PData = "ALPHA"; data.StringLength = 5;
    CCharString name; name.PStringData = &data;
    CCharString empty; empty.PStringData = 0;
    unsigned long crcA = 0x1234u; for (int i = 0; i < 5; ++i) crcA = crcA * 31u + (unsigned char)"ALPHA"[i];
    m->CheckForDuplicateSymbols = false;
    m->AddSymbolDirty(name, 42);                   // in place
    if (g_calc != 1 || m->NamesUsed._M_node_count != 0 || !m->SLONGMap.Dirty) ok = 0;
    m->SLONGMap.Dirty = false;
    m->CheckForDuplicateSymbols = true;
    m->AddSymbolDirty(empty, -7);                  // full: the growth path, plus the used-names insert
    if (g_calc != 3 || m->NamesUsed._M_node_count != 1 || !m->SLONGMap.Dirty) ok = 0;
    TPair* p = m->SLONGMap.Data._M_start;
    if (m->SLONGMap.Data._M_finish - p != 2) ok = 0;
    if (p[0].first != crcA || p[0].second != 42 || p[1].first != 0x1234u || p[1].second != -7) ok = 0;
    printf("00a385a0_TEST %s\n", ok ? "PASS" : "FAIL");
    return ok ? 0 : 1;
}