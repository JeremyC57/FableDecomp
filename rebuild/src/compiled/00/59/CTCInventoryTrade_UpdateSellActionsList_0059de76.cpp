// CTCInventoryTrade::UpdateSellActionsList() -- unless the shop deals in abilities, haircuts or tattoos, send every sell list's owner component a type-3 action naming the list's key component, then clear the sell-list map
// Leaf: UpdateSellActionsList@CTCInventoryTrade
// Retail: Fable.exe (TLC, VC7.1) @ 0x0059DE76, 211 bytes, plain `ret` (thiscall, no arguments).
// Name: FableWin.pdb `?UpdateSellActionsList@CTCInventoryTrade@@AAEXXZ` (private); the map is ItemSellLists (retail
// +0x20C, as the landed ctor 0x0059EB0C lays it out); the Buy twin is 0x0059DE04.
// Callees: the shop-thing getter @ 0x005BC4C4, the inline CThing interface lookup for the shopkeeper (kind 0x53: bitset
// test 0x00410DE0, non-const LowerBound 0x0040F020 -- the landed InitialiseBuyableItems 0x005A30A1 form), the shop type
// tests 0x00772590 / 0x00772490 / 0x00772510 (names unproven, as in 0x005A30A1; called on the null shop too when the
// interface is absent), then the 0x0059DE04 loop: CAction() 0x0042BED4, list push_back 0x0042B6B8, the copy ctor
// 0x0042CD84 for the by-value argument, slot 62 (+0xF8), ~list 0x0042ABCA, _M_increment 0x004292C0, map::clear 0x0042ADE5.
// Label: the catalog "Shop_CleanupCarryables" is descriptive and wrong (nothing is cleaned up but the map).
// Shape: the lookup key and the action's list share [ebp-4] (block-scoped key in the inline lookup, the action at
// [ebp-8] in the loop), `this` and the target component share [ebp-0Ch]. Frame-pointer size TU: `optimize("y", off)` +
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

class CTCBase;
enum ETCInterfaceType { };                                   // CTCBase interface type (values unused here)
template <class K, class V> class CKeyPairCompareLess;
template <class K, class V, class TCompare> class CVectorMap;
template <> class CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > { public:
    typedef std::pair<ETCInterfaceType, CTCBase*> SEntry;
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const ETCInterfaceType& type);                // 0x0040F020
};
typedef CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > CInterfaceMap;
struct CPresenceBits { bool test(long kind) const; };    // 0x00410DE0
class CThing { public:
    unsigned char  _pad00[0x20];
    CPresenceBits  m_present;                            // +0x20
    unsigned char  _pad21[0x44 - 0x21];
    CInterfaceMap  m_interfaces;                         // +0x44
    template<class T> __forceinline bool GetInterface(T** out, long kind)
    {
        if (m_present.test(kind)) {
            ETCInterfaceType key = (ETCInterfaceType)(kind);
            CInterfaceMap* map = &m_interfaces;
            CInterfaceMap::SEntry* it = map->LowerBound(key);
            *out = (T*)((void*)((it == map->last || kind < it->first) ? map->last : it)->second);
            return true;
        }
        return false;
    }
};
struct CTCShopkeeper {
    bool IsTattooShop() const;                           // 0x00772510 (name unproven)
    bool IsBarberShop() const;                           // 0x00772490 (name unproven)
    bool IsAbilityShop() const;                          // 0x00772590 (name unproven)
};

class CTCInventoryTrade {
public:
    unsigned char _pad000[0x200];                    // CTCInventoryBase (0x130) and the trade state (landed ctor 0x0059EB0C)
    CComponentMap ItemBuyLists;                      // +0x200
    CComponentMap ItemSellLists;                     // +0x20c
    CThing* GetShopThing();                          // 0x005BC4C4
    void UpdateSellActionsList();
};

void CTCInventoryTrade::UpdateSellActionsList()
{
    CTCShopkeeper* shop = 0;
    GetShopThing()->GetInterface(&shop, 0x53);
    if (shop->IsAbilityShop() || shop->IsBarberShop() || shop->IsTattooShop())
        return;
    for (CComponentMap::iterator it = ItemSellLists.begin(); it != ItemSellLists.end(); ++it) {
        NUISystem::CAction action;
        action.ActionType = NUISystem::ACTION_TYPE_3;
        action.UIActionParams.push_back(it->first);
        it->second->SetAction(action);
    }
    ItemSellLists.clear();
}