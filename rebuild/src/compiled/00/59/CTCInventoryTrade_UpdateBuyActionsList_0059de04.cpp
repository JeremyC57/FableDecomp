// CTCInventoryTrade::UpdateBuyActionsList() -- send every buy list's owner component a type-3 action naming the list's key component, then clear the buy-list map
// Leaf: UpdateBuyActionsList@CTCInventoryTrade
// Retail: Fable.exe (TLC, VC7.1) @ 0x0059DE04, 114 bytes, plain `ret` (thiscall, no arguments).
// Name: FableWin.pdb `?UpdateBuyActionsList@CTCInventoryTrade@@AAEXXZ` (private); the map is ItemBuyLists (retail +0x200,
// map<NUISystem::CComponent*, NUISystem::CComponent*>, as the landed ctor 0x0059EB0C lays it out); the Sell twin
// 0x0059DE76 walks ItemSellLists (+0x20C). Callers: CTCInventoryTrade::SellItemPC 0x005AF55E, BuyItem 0x005B00B4,
// BuyItemPC 0x005B0F6A.
// Callees: NUISystem::CAction::CAction() @ 0x0042BED4 (landed) and its copy ctor @ 0x0042CD84 (the by-value argument
// built in its slot), list<CActionParam*>::push_back(const value_type&) @ 0x0042B6B8 (the manifest's "Std_List_PushFront":
// it inserts before end()), the value component's virtual slot 62 (+0xF8, a CAction by value; descriptive name as in the
// landed RefreshEntriesAmountsAndAvailabilities 0x0059DB82), the CAction's list destructor @ 0x0042ABCA, the STLport
// tree successor @ 0x004292C0 (`__cdecl`), map::clear() @ 0x0042ADE5 (the landed "CTCCarryable::OnKill").
// Label: the catalog "CTCCarryable_Cleanup" is propagated and wrong.
// Body: `for (it = ItemBuyLists.begin(); it != ItemBuyLists.end(); ++it) { NUISystem::CAction action;
// action.ActionType = 3; action.UIActionParams.push_back(it->first); it->second->SetAction(action); }
// ItemBuyLists.clear();` -- push_back's reference binds to the node's key itself (`lea eax,[esi+10h]`), so in this
// build the parameter list's element type is the map's key type: a list<CActionParam*> (both PDBs, later builds) with
// a CComponent* key converts through a frame temporary (`mov [ebp-4],eax ; lea eax,[ebp-4]`, 120 v 114), with or
// without the PDB's CAction::AddParam(CActionParam*) inline. CComponent is a CActionParam (FableWin.pdb
// GFDebugPDynamicCast<CActionParam*, CComponent*>), so the list is modelled as list<CComponent*>.
// Shape: the map in edi, the node in esi, the action at [ebp-0Ch] (list at [ebp-8]), the target component spilled to
// [ebp-4] across the argument copy and its vtable in ebx. Frame-pointer size TU: `optimize("y", off)` +
// `optimize("s", on)`.
#pragma optimize("y", off)
#pragma optimize("s", on)
#include <stddef.h>

namespace NUISystem {
class CActionParam {
public:
    virtual ~CActionParam();                         // slot 0
    long ActionParamType;                            // +0x04
};
}

namespace std {
template <class T> class list {                      // STLport list: one word, the sentinel node
public:
    void* _M_node;
    ~list();                                         // 0x0042ABCA
    void push_back(const T& x);                      // 0x0042B6B8
};
}

namespace NUISystem {
enum EActionType { ACTION_TYPE_3 = 3 };
class CComponent;
class CAction {                                      // retail 8 bytes
public:
    EActionType                   ActionType;        // +0x00
    std::list<CComponent*>        UIActionParams;    // +0x04 (see the header: the element is the map's key type here)
    CAction();                                       // 0x0042BED4
    CAction(const CAction& other);                   // 0x0042CD84
};
class CComponent : public CActionParam {
public:
    virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04(); virtual void v05(); virtual void v06();
    virtual void v07(); virtual void v08(); virtual void v09(); virtual void v10(); virtual void v11(); virtual void v12();
    virtual void v13(); virtual void v14(); virtual void v15(); virtual void v16(); virtual void v17(); virtual void v18();
    virtual void v19(); virtual void v20(); virtual void v21(); virtual void v22(); virtual void v23(); virtual void v24();
    virtual void v25(); virtual void v26(); virtual void v27(); virtual void v28(); virtual void v29(); virtual void v30();
    virtual void v31(); virtual void v32(); virtual void v33(); virtual void v34(); virtual void v35(); virtual void v36();
    virtual void v37(); virtual void v38(); virtual void v39(); virtual void v40(); virtual void v41(); virtual void v42();
    virtual void v43(); virtual void v44(); virtual void v45(); virtual void v46(); virtual void v47(); virtual void v48();
    virtual void v49(); virtual void v50(); virtual void v51(); virtual void v52(); virtual void v53(); virtual void v54();
    virtual void v55(); virtual void v56(); virtual void v57(); virtual void v58(); virtual void v59(); virtual void v60();
    virtual void v61();
    virtual void SetAction(CAction action);          // slot 62 (+0xF8) (name descriptive)
};
}

namespace std {
template <class T1, class T2> struct pair {
    T1 first;
    T2 second;
};
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
    V _M_value_field;                                // +0x10
};
template <class V> struct _Rb_tree_iterator {
    _Rb_tree_node_base* _M_node;
    _Rb_tree_iterator(_Rb_tree_node_base* x) : _M_node(x) {}
    V* operator->() const { return &static_cast<_Rb_tree_node<V>*>(_M_node)->_M_value_field; }
    _Rb_tree_iterator& operator++() { _M_node = _Rb_global<bool>::_M_increment(_M_node); return *this; }
    bool operator!=(const _Rb_tree_iterator& y) const { return _M_node != y._M_node; }
};
template <class K, class T> class map {
public:
    typedef pair<const K, T> value_type;
    typedef _Rb_tree_iterator<value_type> iterator;
    _Rb_tree_node_base* _M_header;                   // +0x00
    size_t              _M_node_count;               // +0x04
    long                _M_key_compare;              // +0x08
    iterator begin() { return iterator(_M_header->_M_left); }
    iterator end() { return iterator(_M_header); }
    void clear();                                    // 0x0042ADE5
};
}

typedef std::map<NUISystem::CComponent*, NUISystem::CComponent*> CComponentMap;

class CTCInventoryTrade {
public:
    unsigned char _pad000[0x200];                    // CTCInventoryBase (0x130) and the trade state (landed ctor 0x0059EB0C)
    CComponentMap ItemBuyLists;                      // +0x200
    CComponentMap ItemSellLists;                     // +0x20c
    void UpdateBuyActionsList();
};

void CTCInventoryTrade::UpdateBuyActionsList()
{
    for (CComponentMap::iterator it = ItemBuyLists.begin(); it != ItemBuyLists.end(); ++it) {
        NUISystem::CAction action;
        action.ActionType = NUISystem::ACTION_TYPE_3;
        action.UIActionParams.push_back(it->first);
        it->second->SetAction(action);
    }
    ItemBuyLists.clear();
}