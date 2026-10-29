// Behaviour test for 0x006821E0: CTCCreatureOpinionOfHero::SetOpinionDeedMask(long defIndex) -- reset the 0x5C-entry deed mask to all enabled; for a def index naming an OPINION_DEED_MASK def, fill the mask with the def's default and then apply each per-deed override from its DeedEnabled map
// A copy of the source model and logic (compiled separately from src/006821e0.cpp) with test stand-ins for the callees.
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <new>
#if !defined(_MSC_VER)
#define __fastcall
#define __forceinline inline
#define __declspec(x)
#define __cdecl
#define __fastcall
#define __int64 long long
#endif
#pragma intrinsic(memset)

class CDefString {
public:
    long Index;
    CDefString(long index = -1) : Index(index) { }
    bool operator==(const char* name) const;           // 0x005B3440
};
class CDefVirtual {
public:
    virtual void v00();
    virtual void Destroy() const;                      // slot 1
};
class CDefPointeeBase : public CDefVirtual {
public:
    mutable long RefCount;                             // +0x04
};
template <class T> class CDefPointer {
public:
    T* Object;
    CDefPointer() : Object(0) { }
    ~CDefPointer()
    {
        T* object = Object;
        if (object && --object->RefCount == 0)
            object->Destroy();
    }
    T* operator->() const { return Object; }
};

namespace std {
struct __false_type { __false_type() { } };
template <class _Tp> struct less { };
template <class _T1, class _T2> struct pair { _T1 first; _T2 second; };
template <class _Tp> class allocator { };
template <class _Value, class _Tp, class _MaybeReboundAlloc>
class _STLP_alloc_proxy : public _MaybeReboundAlloc {
public:
    _Value _M_data;
};

template <class _ForwardIter, class _Tp>
void __fastcall fill(_ForwardIter __first, _ForwardIter __last, const _Tp& __val)
{
    for (; __first != __last; ++__first)
        *__first = __val;
}
template <>
void __fastcall fill<bool*, bool>(bool* __first, bool* __last, const bool& __val);   // 0x00685430

template <class _Tp, class _Alloc> class _Vector_base {
public:
    _Tp* _M_start;
    _Tp* _M_finish;
    _STLP_alloc_proxy<_Tp*, _Tp, _Alloc> _M_end_of_storage;
};
template <class _Tp, class _Alloc = allocator<_Tp> > class vector : public _Vector_base<_Tp, _Alloc> {
public:
    typedef _Tp* iterator;
    typedef unsigned int size_type;
    iterator begin() { return this->_M_start; }
    iterator end() { return this->_M_finish; }
    _Tp& operator[](size_type __n) { return *(this->_M_start + __n); }
    void _M_fill_assign(size_type __n, const _Tp& __val);   // 0x006860F0 (vector<bool>)
    void assign(size_type __n, const _Tp& __val) { _M_fill_assign(__n, __val); }
    vector() { this->_M_start = this->_M_finish = this->_M_end_of_storage._M_data = 0; }   // test-only
    __forceinline vector(size_type __n, const _Tp& __val)
    {
        this->_M_start = (_Tp*)malloc(__n * sizeof(_Tp));
        this->_M_finish = this->_M_start;
        this->_M_end_of_storage._M_data = this->_M_start + __n;
        memset(this->_M_start, __val, __n);
        this->_M_finish = this->_M_start + __n;
    }
    __forceinline ~vector()
    {
        if (this->_M_start != 0)
            free(this->_M_start);
    }
};

struct _Rb_tree_node_base {
    char                _M_color;
    _Rb_tree_node_base* _M_parent;
    _Rb_tree_node_base* _M_left;
    _Rb_tree_node_base* _M_right;
};
template <class _Value> struct _Rb_tree_node : public _Rb_tree_node_base {
    _Value _M_value_field;
};
struct _Rb_tree_base_iterator {
    _Rb_tree_node_base* _M_node;
};
template <class _Value> struct _Rb_tree_iterator : public _Rb_tree_base_iterator {
    _Rb_tree_iterator() { }
    _Rb_tree_iterator(_Rb_tree_node_base* __x) { _M_node = __x; }
    const _Value& operator*() const { return ((_Rb_tree_node<_Value>*)_M_node)->_M_value_field; }
    const _Value* operator->() const { return &((_Rb_tree_node<_Value>*)_M_node)->_M_value_field; }
    _Rb_tree_iterator& operator++();                   // 0x006856C0
    bool operator!=(const _Rb_tree_iterator& __y) const { return _M_node != __y._M_node; }
};

// Same-TU stand-in for _Rb_tree_iterator<pair<const EOpinionDeedType, bool> >::operator++ (0x006856C0, compiled in this TU):
// it only moves _M_node to the in-order successor.
#pragma auto_inline(off)
template <class _Value>
__declspec(noinline) _Rb_tree_iterator<_Value>& _Rb_tree_iterator<_Value>::operator++()
{
    _Rb_tree_node_base* __x = _M_node;
    if (__x->_M_right != 0) {
        __x = __x->_M_right;
        while (__x->_M_left != 0)
            __x = __x->_M_left;
    } else {
        _Rb_tree_node_base* __y = __x->_M_parent;
        while (__x == __y->_M_right) {
            __x = __y;
            __y = __y->_M_parent;
        }
        if (__x->_M_right != __y)
            __x = __y;
    }
    _M_node = __x;
    return *this;
}
#pragma auto_inline(on)
template <class _Key, class _Tp, class _Compare = less<_Key>, class _Alloc = allocator<pair<const _Key, _Tp> > >
class map {
public:
    typedef _Rb_tree_iterator<pair<const _Key, _Tp> > const_iterator;
    _STLP_alloc_proxy<_Rb_tree_node_base*, _Rb_tree_node<pair<const _Key, _Tp> >, _Alloc> _M_header;   // +0x00
    unsigned int _M_node_count;
    _Compare _M_key_compare;
    const_iterator begin() const { return const_iterator(_M_header._M_data->_M_left); }
    const_iterator end() const { return const_iterator(_M_header._M_data); }
};
}

enum EOpinionDeedType { OPINION_DEED_TYPE_COUNT = 0x5c };

class COpinionDeedMaskDef : public CDefPointeeBase {
public:
    unsigned char _pad08[0x39 - 0x08];
    bool DeedEnabledDefault;                           // +0x39
    unsigned char _pad3a[0x3c - 0x3a];
    std::map<EOpinionDeedType, bool> DeedEnabled;      // +0x3c
};

class CDefinitionManager {
public:
    CDefString GetDefClassTypeNameFromGlobalIndex(long index) const;   // 0x009ACCE0
    template <class T> bool GetDef(long index, CDefPointer<T>* out) const;   // 0x00685E00 (COMDAT-folded)
};
#pragma auto_inline(off)
#pragma auto_inline(on)
class CGameDefinitionManager : public CDefinitionManager { };
class CWorld {
public:
    unsigned char _pad00[0x10];
    CGameDefinitionManager* DefinitionManager;         // +0x10
};
class CThing;

class CTCCreatureOpinionOfHero {
public:
    void* __vftable;                                   // +0x00
    CThing* ParentThing;                               // +0x04
    unsigned char _pad08[0x3c - 0x08];
    std::vector<bool> DeedMask;                        // +0x3c
    CWorld* GetWorld() const;                          // 0x00686A70 (CTCBase)
    void SetOpinionDeedMask(long defIndex);
};

void CTCCreatureOpinionOfHero::SetOpinionDeedMask(long defIndex)
{
    DeedMask.assign(OPINION_DEED_TYPE_COUNT, true);
    if (defIndex) {
        CGameDefinitionManager* defManager = GetWorld()->DefinitionManager;
        CDefPointer<const COpinionDeedMaskDef> def;
        if (defManager->GetDefClassTypeNameFromGlobalIndex(defIndex) == "OPINION_DEED_MASK" && defManager->GetDef(defIndex, &def)) {
            std::fill(DeedMask.begin(), DeedMask.end(), def->DeedEnabledDefault);
            std::vector<bool> applied(OPINION_DEED_TYPE_COUNT, false);
            std::map<EOpinionDeedType, bool>::const_iterator end = def->DeedEnabled.end();
            for (std::map<EOpinionDeedType, bool>::const_iterator it = def->DeedEnabled.begin(); it != end; ++it) {
                EOpinionDeedType deed = it->first;
                if (deed < OPINION_DEED_TYPE_COUNT) {
                    applied[deed] = true;
                    DeedMask[deed] = it->second;
                }
            }
        }
    }
}

// ---- test stand-ins ----
static int g_destroys = 0, g_fills = 0, g_resizes = 0;
void CDefVirtual::v00() { }
void CDefVirtual::Destroy() const { ++g_destroys; }
bool CDefString::operator==(const char* name) const
{
    const char* mine = Index == 1 ? "OPINION_DEED_MASK" : Index == 2 ? "OTHER_DEF" : "";
    return std::strcmp(mine, name) == 0;
}
namespace std {
template <>
void __fastcall fill<bool*, bool>(bool* __first, bool* __last, const bool& __val)
{
    ++g_fills;
    for (; __first != __last; ++__first)
        *__first = __val;
}
template <class _Tp, class _Alloc>
void vector<_Tp, _Alloc>::_M_fill_assign(size_type __n, const _Tp& __val)
{                                                          // test stand-in for 0x006860F0: __n copies of __val
    ++g_resizes;
    if (this->_M_start + __n > this->_M_end_of_storage._M_data) {
        if (this->_M_start) std::free(this->_M_start);
        this->_M_start = (_Tp*)std::malloc(__n * sizeof(_Tp));
        this->_M_end_of_storage._M_data = this->_M_start + __n;
    }
    for (this->_M_finish = this->_M_start; this->_M_finish != this->_M_start + __n; ++this->_M_finish)
        *this->_M_finish = __val;
}
}
static COpinionDeedMaskDef* g_maskDef = 0;
CDefString CDefinitionManager::GetDefClassTypeNameFromGlobalIndex(long index) const { return CDefString(index); }
template <class T> bool CDefinitionManager::GetDef(long index, CDefPointer<T>* out) const
{
    if (index != 1 && index != 2) return false;
    out->Object = (T*)g_maskDef;
    ++out->Object->RefCount;
    return true;
}
static CGameDefinitionManager g_defs;
static CWorld g_world;
CWorld* CTCCreatureOpinionOfHero::GetWorld() const { return &g_world; }

typedef std::_Rb_tree_node<std::pair<const EOpinionDeedType, bool> > DeedNode;
static DeedNode* Node(std::_Rb_tree_node_base* parent, int deed, bool enabled)
{
    DeedNode* n = (DeedNode*)std::malloc(sizeof(DeedNode));
    n->_M_color = 0; n->_M_parent = parent; n->_M_left = n->_M_right = 0;
    *(EOpinionDeedType*)&n->_M_value_field.first = (EOpinionDeedType)deed;
    n->_M_value_field.second = enabled;
    return n;
}

static int Fail(int line) { std::printf("006821e0_TEST FAIL (line %d)\n", line); return 1; }
#define CHECK(c) if (!(c)) return Fail(__LINE__)

int main()
{
    g_world.DefinitionManager = &g_defs;
    COpinionDeedMaskDef def; def.RefCount = 1; def.DeedEnabledDefault = false;
    DeedNode* header = Node(0, 0, false);                      // deeds 1, 3, 10 and an out-of-range 0x60
    DeedNode* root = Node(header, 3, false);
    DeedNode* left = Node(root, 1, true);
    DeedNode* right = Node(root, 0x60, true);
    DeedNode* rightLeft = Node(right, 10, true);
    root->_M_left = left; root->_M_right = right; right->_M_left = rightLeft;
    header->_M_parent = root; header->_M_left = left; header->_M_right = right;
    def.DeedEnabled._M_header._M_data = header; def.DeedEnabled._M_node_count = 4;
    g_maskDef = &def;

    CTCCreatureOpinionOfHero opinion;
    opinion.SetOpinionDeedMask(0);                             // no def: every deed enabled
    CHECK(opinion.DeedMask.end() - opinion.DeedMask.begin() == 0x5c && g_fills == 0);
    for (int i = 0; i < 0x5c; ++i) CHECK(opinion.DeedMask[i]);
    opinion.DeedMask[5] = false;
    opinion.SetOpinionDeedMask(2);                             // a def of another class: mask only reset to all enabled
    CHECK(opinion.DeedMask[5] && opinion.DeedMask[6] && g_fills == 0 && def.RefCount == 1);
    opinion.SetOpinionDeedMask(1);                             // OPINION_DEED_MASK: default false, overrides 1, 3, 10
    CHECK(g_fills == 1 && def.RefCount == 1 && g_destroys == 0);
    for (int i = 0; i < 0x5c; ++i) CHECK(opinion.DeedMask[i] == (i == 1 || i == 10));
    def.DeedEnabledDefault = true;
    opinion.SetOpinionDeedMask(1);
    for (int i = 0; i < 0x5c; ++i) CHECK(opinion.DeedMask[i] == (i != 3));
    CHECK(g_resizes == 4 && opinion.DeedMask.end() - opinion.DeedMask.begin() == 0x5c);
    def.RefCount = 0;                                          // the local holds the last reference: Destroy on exit
    opinion.SetOpinionDeedMask(1);
    CHECK(g_destroys == 1 && def.RefCount == 0);
    std::printf("006821e0_TEST PASS\n");
    return 0;
}