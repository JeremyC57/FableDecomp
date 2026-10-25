// CTCCreatureOpinionOfHero::SetOpinionDeedMask(long defIndex) -- reset the 0x5C-entry deed mask to all enabled; for a def index naming an OPINION_DEED_MASK def, fill the mask with the def's default and then apply each per-deed override from its DeedEnabled map
// Leaf: ?SetOpinionDeedMask@
// Retail: Fable.exe (TLC, VC7.1) @ 0x006821E0, 256 bytes (241 without padding), thiscall `ret 4`. Ego_r.pdb
//   `?SetOpinionDeedMask@CTCCreatureOpinionOfHero@@QAEXJ@Z`; callers 0x006822FE (the CCharString overload), 0x006845F2,
//   0x0079976E, 0x00799BB7, 0x0089C2D4 (CGameScriptInterface::EntitySetOpinionDeedMask), 0x00958BF3.
// Callees: vector<bool>::resize(size_type, const bool&) 0x006860F0 (catalog "std::vector_insert_value"), CTCBase::GetWorld
//   0x00686A70 (catalog "CViewBase::GetDisplayEngine"), CDefinitionManager::GetDefClassTypeNameFromGlobalIndex 0x009ACCE0
//   (CDefString by hidden pointer), CDefString::operator==(const char*) 0x005B3440 ("OPINION_DEED_MASK" 0x01235D20),
//   CDefinitionManager::GetDef<COpinionDeedMaskDef> 0x00685E00 (COMDAT-folded, catalog "GetDef<CThingPhysicalSwitchDef>"),
//   std::fill<bool*, bool> 0x00685430 (`__fastcall`, catalog "fill<char*,char>"), malloc 0x00BFEA0E / free 0x00BFEA14 (the
//   scratch vector), the map iterator's operator++ 0x006856C0 (catalog "std::tree_node::GetNext").
// Label: catalog "CViewBase_SetOpinionDeedMask" -- this is CTCCreatureOpinionOfHero::SetOpinionDeedMask(long) (PDB name).
// Layout: retail CTCCreatureOpinionOfHero (12-byte STLport vectors): AttitudeMask +0x24, ReactionMask +0x30, DeedMask +0x3c
//   (Ego_r +0x24/+0x38/+0x4c with 20-byte vectors); COpinionDeedMaskDef DeedEnabledDefault +0x39, DeedEnabled map +0x3c (Ego_r
//   +0x3d/+0x40); CWorld DefinitionManager +0x10; the def keeps its count at +4 (`dec [ebp+4] ; jne ; call [vt+4]`).
// Shape: the def pointer lives in the dead defIndex slot (zeroed with the first call's pushes) and its slot is reused by the map
//   iterator; GetDef and the iterator's operator++ are template members compiled in this TU -- same-TU noinline stand-ins keep
//   both uncaptured, so the deed key stays in ecx across the byte stores. The deed key is a named local; the map end is a local
//   read once (edi); the scratch "applied" vector is constructed with an inline zero fill (`rep stosd`, 0x17 dwords) after the
//   std::fill and freed on the way out. The resize's `true` temporary is the high byte of the 4-byte frame word.
extern "C" void* __cdecl malloc(unsigned int size);  // 0x00BFEA0E
extern "C" void __cdecl free(void* p);               // 0x00BFEA14
extern "C" void* __cdecl memset(void* d, int c, unsigned int n);
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
    void resize(size_type __new_size, const _Tp& __x);   // 0x006860F0 (vector<bool>)
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
template <class T> __declspec(noinline) bool CDefinitionManager::GetDef(long index, CDefPointer<T>* out) const
{
    if (index < 0)
        return false;
    out->Object = (T*)(this + index);
    return true;
}
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
    DeedMask.resize(OPINION_DEED_TYPE_COUNT, true);
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