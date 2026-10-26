// CTCInventory::ContructItemListsPC(map<long,long>& items, NUISystem::CList* list, NUISystem::CComponent* owner, CCountedPointer<NUISystem::CComponent>& entry) -- build one inventory entry per merged item, the guild seal first
// Leaf: ContructItemListsPC@CTCInventory
// Retail: Fable.exe (TLC, VC7.1) @ 0x00590198, 157 bytes, `ret 10h`.
// Name: FableWin.pdb ?ContructItemListsPC@CTCInventory@@QAEXAAV?$map@JJ...@std@@PAVCList@NUISystem@@PAVCComponent@5@AAV?$CCountedPointer@VCComponent@NUISystem@@@@@Z
// (sic, "Contruct"); the catalog "ConstructPCExpressionCategoryList" is propagated. Only caller 0x00590CC4 (inside the
// landed ConstructItemCategoryListPC 0x005907AB, which names this address); the console twin ContructItemLists is
// 0x0058EF79 (byte-identical but for the ConstructItemEntry callee).
// Callees: CCharString(const char*, long) @ 0x0099EBF0 / ~CCharString @ 0x0099EAE0, CMainGameComponent::
// PeekDefinitionManager() const @ 0x00415FBC (a jmp thunk; catalog "GFGetPreMainMemoryUsed") on g_pMainGameComponent
// @ 0x013B86A0, CDefinitionManager::GetDefGlobalIndexFromName(const CCharString&) @ 0x009AD410, map<long,long>::find
// @ 0x00592F92 (node pointer), CTCInventory::ConstructItemEntryPC(long, long, CList*, CComponent*, CCountedPointer&)
// @ 0x0058E208, the STLport tree successor _Rb_global<bool>::_M_increment @ 0x004292C0 (`__cdecl`).
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
class CMainGameComponent { public:
    const CDefinitionManager& PeekDefinitionManager() const;   // 0x00415FBC
};
extern CMainGameComponent* g_pMainGameComponent;               // 0x013B86A0

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
template <class K, class T> class map {
public:
    typedef pair<const K, T> value_type;
    typedef _Rb_tree_iterator<value_type> iterator;
    _Rb_tree_node_base* _M_header;                      // +0x00
    unsigned int        _M_node_count;                  // +0x04
    _Rb_tree_node_base* _M_find(const K& k);            // 0x00592F92
    iterator find(const K& k) { return iterator(_M_find(k)); }
    iterator begin() { return iterator(_M_header->_M_left); }
    iterator end() { return iterator(_M_header); }
};
}

class CTCInventory {
public:
    void ConstructItemEntryPC(long defIndex, long count, NUISystem::CList* list, NUISystem::CComponent* owner,
                            CCountedPointer<NUISystem::CComponent>& entry);                 // 0x0058E208
    void ContructItemListsPC(std::map<long, long>& items, NUISystem::CList* list, NUISystem::CComponent* owner,
                           CCountedPointer<NUISystem::CComponent>& entry);
};

void CTCInventory::ContructItemListsPC(std::map<long, long>& items, NUISystem::CList* list, NUISystem::CComponent* owner,
                                     CCountedPointer<NUISystem::CComponent>& entry)
{
    std::map<long, long>::iterator seal(0);
    {
        CCharString name("OBJECT_GUILD_SEAL_1", -1);
        seal = items.find(g_pMainGameComponent->PeekDefinitionManager().GetDefGlobalIndexFromName(name));
    }
    if (seal != items.end())
        ConstructItemEntryPC(seal->first, seal->second, list, owner, entry);
    std::map<long, long>::iterator last = items.end();
    for (std::map<long, long>::iterator it = items.begin(); it != last; ++it)
        if (it != seal)
            ConstructItemEntryPC(it->first, it->second, list, owner, entry);
}