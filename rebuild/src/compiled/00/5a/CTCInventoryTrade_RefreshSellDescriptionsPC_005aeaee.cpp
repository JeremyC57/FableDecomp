// CTCInventoryTrade::RefreshSellDescriptionsPC(NUISystem::CComponent* parent) -- replaces the description child of every sellable item entry with a freshly built trade description and adds it to the PC description viewport
// Leaf: ?RefreshSellDescriptionsPC@CTCInventoryTrade@@
// Retail: Fable.exe (TLC, VC7.1) @ 0x005AEAEE, 272 bytes (0x110, no padding: /Os TU), thiscall `ret 4`. One caller
//   (0x005B07A8). Twin of RefreshBuyDescriptionsPC 0x005AE9DE (ItemsToBuy, the buy description name, buying=true).
// Identity: Ego_r.pdb ?RefreshSellDescriptionsPC@CTCInventoryTrade@@QAEXPAVCComponent@NUISystem@@@Z (one CComponent*
//   argument, `ret 4`); the catalog's "CTCInventoryTrade_DisplaySellItem" is descriptive.
// Pragmas: `#pragma optimize("s", on)` + `#pragma optimize("y", off)` (frame pointer and `leave`, no alignment padding:
//   the size-optimised GUI TU).
// Callees: CCharString(const char*, long) 0x0099EBF0, CCharString(const CCharString&) 0x0099EC30, NUISystem::CDeletion()
//   0x0042BED4 / CDeletion(const CDeletion&) 0x0042CD84, list<CComponent*>::~list 0x0042ABCA (the inlined ~CDeletion),
//   CTCInventoryTrade::ConstructTradeDescription 0x005AD571, CCountedPointer<CComponent>(CComponent*) 0x00429C15 /
//   ~CCountedPointer 0x004291DE, _Rb_global<bool>::_M_increment 0x004292C0 (out of line under /Os); virtual
//   CComponent::FindChild (slot 3), AddChild (slot 60), SetDeletion (slot 62), SetName (slot 83), AddPositionChild
//   (slot 124), CViewport::AddFakeChild (slot 142); "PC_DESCRIPTION_VIEWPORT" 0x012370F4, the sell-item description
//   name CCharString 0x013B8B7C (GetSellItemDescriptionString).
// Layout: local model -- the engine header CTCInventoryTrade.h is the Ego_r layout (16-byte maps, 12-byte lists), retail
//   STLport containers are 12 bytes: ItemsToBuy map<CComponent*, pair<long,long> > +0x144, ItemsToSell +0x150,
//   PCurrentOpenTradeMenu +0x224 (Ego_r +0x274 less nine 4-byte container shrinks). NUISystem::CDeletion is
//   { EDeletionMethod Method; list<CComponent*> AssociatedParents; } (8 bytes).
// Shape: the viewport lookup takes a by-value CCharString built in its argument slot; the loop walks the map with the
//   out-of-line `_M_increment`; each entry's old description is found by name (a by-value copy of the static name),
//   marked for deletion through a by-value CDeletion copy, and replaced by a counted pointer to the new description,
//   which is position-added to `parent`, renamed, added as the entry's child and handed to the viewport. The scope's
//   counted pointer and deletion object die at the end of the `if` (the deletion's dtor is only its list member).
#pragma optimize("s", on)
#pragma optimize("y", off)

class CCharString {
public:
    void* PStringData;
    CCharString(const char* text, long length = -1);                                  // 0x0099EBF0
    CCharString(const CCharString& other);                                             // 0x0099EC30
    ~CCharString();                                                                    // 0x0099EAE0
};

template <class T> class CCountedPointer {
public:
    T*    Data;                                                                        // +0x00
    void* PInfo;                                                                       // +0x04 (CCPPointerInfo*)
    CCountedPointer(T* data);                                                          // 0x00429C15
    ~CCountedPointer();                                                                // 0x004291DE
    T* operator->() const { return Data; }
    T* Get() const { return Data; }
};

namespace NUISystem { class CComponent; }

namespace std {

template <class _Tp> class list {
public:
    void* _M_node;                                                                     // +0x00 (STLport, 4 bytes)
    ~list();                                                                           // 0x0042ABCA
};

struct _Rb_tree_node_base {
    bool                _M_color;                                                      // +0x00
    _Rb_tree_node_base* _M_parent;                                                     // +0x04
    _Rb_tree_node_base* _M_left;                                                       // +0x08
    _Rb_tree_node_base* _M_right;                                                      // +0x0c
};

template <class _Dummy> class _Rb_global {
public:
    static _Rb_tree_node_base* _M_increment(_Rb_tree_node_base* __x);                  // 0x004292C0
};

template <class _T1, class _T2> struct pair {
    _T1 first;
    _T2 second;
};

template <class _Value> struct _Rb_tree_node : public _Rb_tree_node_base {
    _Value _M_value_field;                                                             // +0x10
};

template <class _Value> struct _Rb_tree_iterator {
    _Rb_tree_node_base* _M_node;
    _Rb_tree_iterator() { }
    _Rb_tree_iterator(_Rb_tree_node_base* __x) { _M_node = __x; }
    _Value& operator*() const { return ((_Rb_tree_node<_Value>*)_M_node)->_M_value_field; }
    _Value* operator->() const { return &(operator*()); }
    _Rb_tree_iterator& operator++() { _M_node = _Rb_global<bool>::_M_increment(_M_node); return *this; }
    bool operator!=(const _Rb_tree_iterator& __y) const { return _M_node != __y._M_node; }
};

template <class _Key, class _Tp> class map {
public:
    typedef pair<const _Key, _Tp> value_type;
    typedef _Rb_tree_iterator<value_type> iterator;
    _Rb_tree_node_base* _M_header;                                                     // +0x00
    unsigned int        _M_node_count;                                                 // +0x04
    unsigned char       _M_key_compare;                                                // +0x08
    iterator begin() { return iterator(_M_header->_M_left); }
    iterator end() { return iterator(_M_header); }
};

}

namespace NUISystem {

enum EDeletionMethod { DELETION_METHOD_NONE, DELETION_METHOD_1 };

class CDeletion {
public:
    EDeletionMethod                 Method;                                            // +0x00
    std::list<CComponent*>          AssociatedParents;                                 // +0x04
    CDeletion();                                                                       // 0x0042BED4
    CDeletion(const CDeletion& other);                                                 // 0x0042CD84
};

class CComponent {
public:
    virtual void v000(); virtual void v001(); virtual void v002();
    virtual CComponent* FindChild(CCharString name);                                   // slot 3 (+0x0c)
    virtual void v004(); virtual void v005(); virtual void v006(); virtual void v007(); virtual void v008(); virtual void v009();
    virtual void v010(); virtual void v011(); virtual void v012(); virtual void v013(); virtual void v014(); virtual void v015();
    virtual void v016(); virtual void v017(); virtual void v018(); virtual void v019(); virtual void v020(); virtual void v021();
    virtual void v022(); virtual void v023(); virtual void v024(); virtual void v025(); virtual void v026(); virtual void v027();
    virtual void v028(); virtual void v029(); virtual void v030(); virtual void v031(); virtual void v032(); virtual void v033();
    virtual void v034(); virtual void v035(); virtual void v036(); virtual void v037(); virtual void v038(); virtual void v039();
    virtual void v040(); virtual void v041(); virtual void v042(); virtual void v043(); virtual void v044(); virtual void v045();
    virtual void v046(); virtual void v047(); virtual void v048(); virtual void v049(); virtual void v050(); virtual void v051();
    virtual void v052(); virtual void v053(); virtual void v054(); virtual void v055(); virtual void v056(); virtual void v057();
    virtual void v058(); virtual void v059();
    virtual void AddChild(CCountedPointer<CComponent>& child);                         // slot 60 (+0xf0)
    virtual void v061();
    virtual void SetDeletion(CDeletion deletion);                                      // slot 62 (+0xf8)
    virtual void v063();
    virtual void v064(); virtual void v065(); virtual void v066(); virtual void v067(); virtual void v068(); virtual void v069();
    virtual void v070(); virtual void v071(); virtual void v072(); virtual void v073(); virtual void v074(); virtual void v075();
    virtual void v076(); virtual void v077(); virtual void v078(); virtual void v079(); virtual void v080(); virtual void v081();
    virtual void v082();
    virtual void SetName(const CCharString& name);                                     // slot 83 (+0x14c)
    virtual void v084(); virtual void v085(); virtual void v086(); virtual void v087(); virtual void v088();
    virtual void v089(); virtual void v090(); virtual void v091(); virtual void v092(); virtual void v093(); virtual void v094();
    virtual void v095(); virtual void v096(); virtual void v097(); virtual void v098(); virtual void v099(); virtual void v100();
    virtual void v101(); virtual void v102(); virtual void v103(); virtual void v104(); virtual void v105(); virtual void v106();
    virtual void v107(); virtual void v108(); virtual void v109(); virtual void v110(); virtual void v111(); virtual void v112();
    virtual void v113(); virtual void v114(); virtual void v115(); virtual void v116(); virtual void v117(); virtual void v118();
    virtual void v119(); virtual void v120(); virtual void v121(); virtual void v122(); virtual void v123();
    virtual void AddPositionChild(CComponent* child);                                  // slot 124 (+0x1f0)
    virtual void v125(); virtual void v126(); virtual void v127(); virtual void v128(); virtual void v129(); virtual void v130();
    virtual void v131(); virtual void v132(); virtual void v133();
};

class CViewport : public CComponent {
public:
    virtual void v134(); virtual void v135(); virtual void v136(); virtual void v137(); virtual void v138(); virtual void v139();
    virtual void v140(); virtual void v141();
    virtual void AddFakeChild(CComponent* child);                                      // slot 142 (+0x238)
};

}

extern CCharString g_SellItemDescriptionString;                                       // 0x013B8B7C

class CTCInventoryTrade {
public:
    void*                   __vftable;                                                 // +0x000
    unsigned char           _pad_0x004[0x144 - 0x004];
    std::map<NUISystem::CComponent*, std::pair<long, long> > ItemsToBuy;              // +0x144
    std::map<NUISystem::CComponent*, std::pair<long, long> > ItemsToSell;             // +0x150
    unsigned char           _pad_0x15c[0x224 - 0x15c];
    NUISystem::CComponent*  PCurrentOpenTradeMenu;                                     // +0x224
    NUISystem::CComponent* ConstructTradeDescription(long defIndex, unsigned long count, bool buying,
                                                     bool special);                    // 0x005AD571
    void RefreshSellDescriptionsPC(NUISystem::CComponent* parent);
};

void CTCInventoryTrade::RefreshSellDescriptionsPC(NUISystem::CComponent* parent)
{
    NUISystem::CViewport* viewport =
        (NUISystem::CViewport*)PCurrentOpenTradeMenu->FindChild(CCharString("PC_DESCRIPTION_VIEWPORT"));
    for (std::map<NUISystem::CComponent*, std::pair<long, long> >::iterator it = ItemsToSell.begin();
         it != ItemsToSell.end(); ++it) {
        NUISystem::CComponent* oldDescription = (*it).first->FindChild(g_SellItemDescriptionString);
        if (oldDescription != 0) {
            NUISystem::CDeletion deletion;
            deletion.Method = NUISystem::DELETION_METHOD_1;
            oldDescription->SetDeletion(deletion);
            CCountedPointer<NUISystem::CComponent> description(ConstructTradeDescription((*it).second.first, 1, false, false));
            parent->AddPositionChild(description.Get());
            description->SetName(g_SellItemDescriptionString);
            (*it).first->AddChild(description);
            viewport->AddFakeChild(description.Get());
        }
    }
}