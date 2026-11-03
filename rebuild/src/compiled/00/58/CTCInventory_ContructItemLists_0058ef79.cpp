// CTCInventory::ContructItemLists(map<long,long>& items, NUISystem::CList* list, NUISystem::CComponent* owner, CCountedPointer<NUISystem::CComponent>& entry) -- build one inventory entry per merged item, the guild seal first
// Leaf: ContructItemLists@CTCInventory
// Naming wave 18: the global stand-in g_pMainGameComponent (0x013B86A0) is the protected static CMainGameComponent::PMainGameComponent, read through the
//   inline static accessor Get() as retail code outside the class does (name-only: the bytes are unchanged).
// Retail: Fable.exe (TLC, VC7.1) @ 0x0058EF79, 157 bytes, `ret 10h`.
// Name: FableWin.pdb ?ContructItemLists@CTCInventory@@QAEXAAV?$map@JJ...@std@@PAVCList@NUISystem@@PAVCComponent@5@AAV?$CCountedPointer@VCComponent@NUISystem@@@@@Z
// (sic, "Contruct"); the catalog "ConstructItemCategoryList" is a different method. Only caller 0x0058F5C3; the PC twin
// ContructItemListsPC is 0x00590198 (ConstructItemEntryPC).
// Callees: CCharString(const char*, long) @ 0x0099EBF0 / ~CCharString @ 0x0099EAE0, CMainGameComponent::
// PeekDefinitionManager() const @ 0x00415FBC (a jmp thunk; catalog "GFGetPreMainMemoryUsed") on CMainGameComponent::Get()
// @ 0x013B86A0, CDefinitionManager::GetDefGlobalIndexFromName(const CCharString&) @ 0x009AD410, map<long,long>::find
// @ 0x00592F92 (node pointer), CTCInventory::ConstructItemEntry(long, long, CList*, CComponent*, CCountedPointer&)
// @ 0x0058A0C7, the STLport tree successor _Rb_global<bool>::_M_increment @ 0x004292C0 (`__cdecl`).
// Body: find the def index of "OBJECT_GUILD_SEAL_1" (string 0x01251B4C) in the map, construct that entry first,
// then walk the map from begin() to end() and construct every other entry (key = def index, value = count).
// Shape: frame-pointer size TU (`optimize("y", off)` + `optimize("s", on)`). The name is a named CCharString in its
// own block around the find (it takes the top frame word and dies right after the find; as a bare temporary it sinks
// below the `this` spill and the key word), and the loop's end() is cached in a local (ebx; the loop enters at its
// bottom test, where re-reading items.end() each pass adds a top test). The found node stays in edi.
// Model: `this` is unused beyond the member call (engine/CTCInventory.h is the PDB layout, not needed here).
#pragma optimize("y", off)
#pragma optimize("s", on)

class CCharString { public:
    void* PStringData;
    CCharString(const char* text, long length = -1);   // 0x0099EBF0
    ~CCharString();                                     // 0x0099EAE0
};

class CDefinitionManager { public:
    long GetDefGlobalIndexFromName(const CCharString& name) const;   // 0x009AD410
};
class CGameDefinitionManager : public CDefinitionManager
{
};
class CMainGameComponent {
protected:
    static CMainGameComponent* PMainGameComponent;      // 0x013B86A0 ?PMainGameComponent@CMainGameComponent@@1PAV1@A (protected static; ESTABLISHED_SPELLINGS)
public:
    static CMainGameComponent* __fastcall Get() { return PMainGameComponent; }   // Ego_r / FableWin ?Get@CMainGameComponent@@SIPAV1@XZ (inline)
private: public:
public:
    const CGameDefinitionManager& PeekDefinitionManager() const;   // 0x00415FBC
};

template <class T> class CCountedPointer { public:
    T*    m_pObject;
    void* m_pCount;
};

namespace NUISystem {
class CComponent;
class CList;
}

namespace std {
struct _Rb_tree_node_base {
    bool                _M_color;
    _Rb_tree_node_base* _M_parent;
    _Rb_tree_node_base* _M_left;
    _Rb_tree_node_base* _M_right;
};
template <class _Dummy> struct _Rb_global {
    static _Rb_tree_node_base* __cdecl _M_increment(_Rb_tree_node_base* x);   // 0x004292C0
};
template <class V> struct _Rb_tree_node : _Rb_tree_node_base {
    V _M_value_field;                                   // +0x10
};
template <class T1, class T2> struct pair {
    T1 first;
    T2 second;
};
template <class V> struct _Rb_tree_iterator {
    _Rb_tree_node_base* _M_node;
    _Rb_tree_iterator(_Rb_tree_node_base* x) : _M_node(x) {}
    V& operator*() const { return static_cast<_Rb_tree_node<V>*>(_M_node)->_M_value_field; }
    V* operator->() const { return &static_cast<_Rb_tree_node<V>*>(_M_node)->_M_value_field; }
    _Rb_tree_iterator& operator++() { _M_node = _Rb_global<bool>::_M_increment(_M_node); return *this; }
    bool operator==(const _Rb_tree_iterator& y) const { return _M_node == y._M_node; }
    bool operator!=(const _Rb_tree_iterator& y) const { return _M_node != y._M_node; }
};
template <class _Tp> struct less { };                  // STLport 4.6 _function.h
template <class _Pair> struct _Select1st { };          // (map's KeyOfValue)
template <class _Tp> class allocator { };
// STLport 4.6.1 _tree.h: find / _M_find are member templates (owner decision 2026-11-03), _M_find private and const.
template <class K, class V, class KoV, class C, class A> class _Rb_tree {
public:
    typedef _Rb_tree_iterator<V> iterator;
    _Rb_tree_node_base* _M_header;                      // +0x00
    unsigned int        _M_node_count;                  // +0x04
    C                   _M_key_compare;                 // +0x08
    template <class _KT> iterator find(const _KT& k) { return iterator(_M_find(k)); }
    iterator begin() { return iterator(_M_header->_M_left); }
    iterator end() { return iterator(_M_header); }
private:
    template <class _KT> _Rb_tree_node<V>* _M_find(const _KT& k) const;   // 0x00592F92
};
template <class K, class T, class C = less<K>, class A = allocator<pair<const K, T> > > class map {
public:
    typedef pair<const K, T> value_type;
    typedef _Rb_tree<K, value_type, _Select1st<value_type>, C, A> _Rep_type;
    typedef typename _Rep_type::iterator iterator;
    _Rep_type _M_t;
    iterator find(const K& k) { return _M_t.find(k); }
    iterator begin() { return _M_t.begin(); }
    iterator end() { return _M_t.end(); }
};
}

class CTCInventory {
public:
    void ConstructItemEntry(long defIndex, long count, NUISystem::CList* list, NUISystem::CComponent* owner,
                            CCountedPointer<NUISystem::CComponent>& entry);                 // 0x0058A0C7
    void ContructItemLists(std::map<long, long>& items, NUISystem::CList* list, NUISystem::CComponent* owner,
                           CCountedPointer<NUISystem::CComponent>& entry);
};

void CTCInventory::ContructItemLists(std::map<long, long>& items, NUISystem::CList* list, NUISystem::CComponent* owner,
                                     CCountedPointer<NUISystem::CComponent>& entry)
{
    std::map<long, long>::iterator seal(0);
    {
        CCharString name("OBJECT_GUILD_SEAL_1", -1);
        seal = items.find(CMainGameComponent::Get()->PeekDefinitionManager().GetDefGlobalIndexFromName(name));
    }
    if (seal != items.end())
        ConstructItemEntry(seal->first, seal->second, list, owner, entry);
    std::map<long, long>::iterator last = items.end();
    for (std::map<long, long>::iterator it = items.begin(); it != last; ++it)
        if (it != seal)
            ConstructItemEntry(it->first, it->second, list, owner, entry);
}