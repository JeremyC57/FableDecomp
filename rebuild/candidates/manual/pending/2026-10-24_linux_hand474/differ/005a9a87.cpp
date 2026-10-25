// CTCInventoryTrade::RefreshBuyPossibility() -- greys out every buyable item entry the hero cannot buy (too dear, no room in its inventory category and not a special case, or already owned), schedules its selection arrow and buy-actions list for deletion, then refreshes the buy counts
// Grade: DIFFER(565v567) -- register allocation + one store: retail spills `this` to [ebp-4] at entry and materialises the purse type 4 in ebx (ours keeps `this` in ebx, the 4 in edi: 2 bytes); the deletion.Method = 3 store sits after the push_back argument pushes in retail, before the last ones in ours
// Leaf: ?RefreshBuyPossibility@CTCInventoryTrade@@
// Retail: Fable.exe (TLC, VC7.1) @ 0x005A9A87, 567 bytes (0x237, no padding: /Os TU), thiscall, plain `ret`. Three
//   callers (0x005AF89D, 0x005AF8C2, 0x005AF9B8).
// Identity: Ego_r.pdb ?RefreshBuyPossibility@CTCInventoryTrade@@AAEXXZ -- the console form of RefreshBuyPossibilityPC
//   0x005A887D (the "UI_" children, plus the selection arrow); the catalog's "Inventory_PopulateBuyList" is descriptive.
// Pragmas: `#pragma optimize("y", off)` + `("s", on)` (frame pointer and `leave`, no alignment padding), as the landed
//   CTCInventoryTrade::IsItemAffordable 0x0059DA10.
// Callees: CPlayerManager::GetMainPlayer() const 0x00449960 (the main game component 0x013B86A0 +0x1C),
//   CPlayer::GetControlledCreature 0x00487DD0, CTCInventoryBase::GetTradingThing 0x005BC4C4, std::bitset::test 0x00410DE0
//   + the CVectorMap::LowerBound copies 0x004365B0 / 0x0040F020 (the inline CThing::GetInterface: the hero's purse type 4,
//   the shop type 0x53, the hero's inventory for the item's category), CTCInventoryItem::GetInventoryCategory 0x005D99D0
//   (__fastcall), CTCShop::GetCurrentBuyPrice 0x007743B0, the category inventory's CanTakeItem 0x005BF2ED,
//   CTCInventoryTrade::IsSpecialCase 0x0059C983 / AlreadyHaveSpecial 0x0059FC17 / RefreshBuyPossibilityNumbers
//   0x005A3734, the entry's MakeUnavailableDynamically (slot 93) / FindChild (slot 3) / SetDeletion (slot 62),
//   CCharString(const char*, long) 0x0099EBF0 ("UI_SELECTION_ARROW_RIGHT" 0x01237150, "UI_LIST_BUY_ACTIONS"
//   0x01230244), NUISystem::CDeletion() 0x0042BED4 / CDeletion(const CDeletion&) 0x0042CD84, list<CComponent*>::insert
//   0x0042AB8A / ~list 0x0042ABCA, _Rb_global<bool>::_M_increment 0x004292C0.
// Layout: local model (the engine header is the Ego_r layout): ItemsToBuy map<CComponent*, pair<long,long> > +0x144
//   (12-byte STLport map; the pair is the item def and the stock count); the purse's Gold +0x3c (as IsItemAffordable).
// Shape: as RefreshBuyPossibilityPC: both up-front component pointers start null and are filled by the out-parameter
//   GetInterface; per entry the category inventory pointer restarts null and is only looked up for a stocked entry;
//   the gold is read into a named local before the price call. The arrow gets a plain deletion (method 1), the actions
//   list one whose parent list holds the entry (push_back = insert(end(), x) out of line; its same-TU stand-in keeps the
//   entry enregistered, as do the LowerBound stand-ins for the lookup keys).
#pragma optimize("y", off)
#pragma optimize("s", on)

struct CCharStringData;
struct CCharString {
    CCharStringData* PStringData;
    CCharString(const char* text, long length = -1);  // 0x0099EBF0
    CCharString(const CCharString& other);            // (declared: by-value arguments are built in place)
    ~CCharString();                                   // 0x0099EAE0
};

namespace NUISystem { class CComponent; }

namespace std {

struct _List_node_base {
    _List_node_base* _M_next;                                                          // +0x00
    _List_node_base* _M_prev;                                                          // +0x04
};

template <class _Tp> struct _List_node : public _List_node_base {
    _Tp _M_data;                                                                       // +0x08
};

void* AllocateListNode(unsigned int size);                                            // stand-in allocator

struct _List_iterator_base {
    _List_node_base* _M_node;
    _List_iterator_base(_List_node_base* __x) : _M_node(__x) { }
    _List_iterator_base() { }
};

template <class _Tp> struct _List_iterator : public _List_iterator_base {
    _List_iterator(_List_node<_Tp>* __x) : _List_iterator_base(__x) { }
    _List_iterator() { }
    _List_iterator(const _List_iterator& __x) : _List_iterator_base(__x._M_node) { }
};

template <class _Tp> class list {
public:
    typedef _List_iterator<_Tp> iterator;
    _List_node_base* _M_node;                                                          // +0x00 (STLport, 4 bytes)
    ~list();                                                                           // 0x0042ABCA
    iterator end() { return iterator((_List_node<_Tp>*)_M_node); }
    iterator insert(iterator __position, const _Tp& __x);                              // 0x0042AB8A
    void push_back(const _Tp& __x) { insert(end(), __x); }
};

template <class _Tp>
__declspec(noinline) typename list<_Tp>::iterator list<_Tp>::insert(iterator __position, const _Tp& __x)
{                                                   // 0x0042AB8A (same-TU stand-in: reads x, keeps no address)
    _List_node<_Tp>* __tmp = (_List_node<_Tp>*)AllocateListNode(sizeof(_List_node<_Tp>));
    __tmp->_M_data = __x;
    __tmp->_M_next = __position._M_node;
    __tmp->_M_prev = __position._M_node->_M_prev;
    __position._M_node->_M_prev->_M_next = __tmp;
    __position._M_node->_M_prev = __tmp;
    return iterator(__tmp);
}

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

enum EDeletionMethod { DELETION_METHOD_NONE, DELETION_METHOD_1, DELETION_METHOD_2, DELETION_METHOD_3 };

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
    virtual void v058(); virtual void v059(); virtual void v060(); virtual void v061();
    virtual void SetDeletion(CDeletion deletion);                                      // slot 62 (+0xf8)
    virtual void v063();
    virtual void v064(); virtual void v065(); virtual void v066(); virtual void v067(); virtual void v068(); virtual void v069();
    virtual void v070(); virtual void v071(); virtual void v072(); virtual void v073(); virtual void v074(); virtual void v075();
    virtual void v076(); virtual void v077(); virtual void v078(); virtual void v079(); virtual void v080(); virtual void v081();
    virtual void v082(); virtual void v083(); virtual void v084(); virtual void v085(); virtual void v086(); virtual void v087();
    virtual void v088(); virtual void v089(); virtual void v090(); virtual void v091(); virtual void v092();
    virtual void MakeUnavailableDynamically();                                         // slot 93 (+0x174)
};

}

struct CPresenceBits {
    bool test(long bit);                             // 0x00410DE0 (std::bitset::test)
};
struct CInterfaceMap {
    struct SEntry { long type; void* component; };
    SEntry* first;                                   // +0x00
    SEntry* last;                                    // +0x04
    __declspec(noinline) SEntry* LowerBound(const long* type)   // 0x0040F020 / its copy 0x004365B0 (same-TU stand-in)
    {
        SEntry* lo = first;
        int len = last - first;
        while (len > 0) {
            int half = len >> 1;
            SEntry* middle = lo + half;
            if (middle->type < *type) {
                lo = middle + 1;
                len = len - half - 1;
            }
            else
                len = half;
        }
        return lo;
    }
};
struct CThing {
    unsigned char _pad_0x00[0x20];
    CPresenceBits m_present;                         // +0x20
    unsigned char _pad_0x21[0x23];
    CInterfaceMap m_interfaces;                      // +0x44
    template<class T> __forceinline bool GetInterface(T** out, long wanted)
    {
        if (m_present.test(wanted)) {
            long type = wanted;
            CInterfaceMap* map = &m_interfaces;
            CInterfaceMap::SEntry* it = map->LowerBound(&type);
            CInterfaceMap::SEntry* found = (it == map->last || wanted < it->type) ? map->last : it;
            *out = (T*)found->component;
            return true;
        }
        return false;
    }
};
struct CTCShop {                                     // interface type 0x53
    long GetCurrentBuyPrice(long itemType, long count);   // 0x007743B0
};
struct CTCGoldOwner {                                // interface type 4: the hero's purse
    unsigned char _pad00[0x3c];
    long Gold;                                       // +0x3c
};
struct CTCInventoryCategory {                        // the hero's inventory component for an item's category
    bool CanTakeItem(long itemType);                 // 0x005BF2ED
};
struct CTCInventoryItem {
    static long __fastcall GetInventoryCategory(long itemType);   // 0x005D99D0
};
struct CPlayer {
    CThing* GetControlledCreature();                 // 0x00487DD0
};
struct CPlayerManager {
    CPlayer* GetMainPlayer() const;                  // 0x00449960 (const)
};
struct CMainGameComponent {
    unsigned char   _pad00[0x1c];
    CPlayerManager* PlayerManager;                   // +0x1c
};
extern CMainGameComponent* g_pMainGameComponent;     // 0x013B86A0

class CTCInventoryTrade {
public:
    void*                   __vftable;                                                 // +0x000
    unsigned char           _pad_0x004[0x144 - 0x004];
    std::map<NUISystem::CComponent*, std::pair<long, long> > ItemsToBuy;              // +0x144
    CThing* GetTradingThing();                                                         // 0x005BC4C4 (CTCInventoryBase)
    bool IsSpecialCase(long itemType);                                                 // 0x0059C983
    bool AlreadyHaveSpecial(long itemType);                                            // 0x0059FC17
    void RefreshBuyPossibilityNumbers();                                               // 0x005A3734
    void RefreshBuyPossibility();
};

void CTCInventoryTrade::RefreshBuyPossibility()
{
    CThing* hero = g_pMainGameComponent->PlayerManager->GetMainPlayer()->GetControlledCreature();
    CTCGoldOwner* purse = 0;
    hero->GetInterface(&purse, 4);
    CTCShop* shop = 0;
    GetTradingThing()->GetInterface(&shop, 0x53);
    for (std::map<NUISystem::CComponent*, std::pair<long, long> >::iterator it = ItemsToBuy.begin();
         it != ItemsToBuy.end(); ++it) {
        NUISystem::CComponent* entry = (*it).first;
        CTCInventoryCategory* inventory = 0;
        if ((*it).second.second > 0)
            hero->GetInterface(&inventory, CTCInventoryItem::GetInventoryCategory((*it).second.first));
        long gold = purse->Gold;
        if (gold < shop->GetCurrentBuyPrice((*it).second.first, 1) ||
            ((*it).second.second > 0 && !inventory->CanTakeItem((*it).second.first) &&
             !IsSpecialCase((*it).second.first)) ||
            AlreadyHaveSpecial((*it).second.first)) {
            entry->MakeUnavailableDynamically();
            NUISystem::CComponent* arrow = entry->FindChild(CCharString("UI_SELECTION_ARROW_RIGHT"));
            if (arrow != 0) {
                NUISystem::CDeletion deletion;
                deletion.Method = NUISystem::DELETION_METHOD_1;
                arrow->SetDeletion(deletion);
            }
            NUISystem::CComponent* actions = entry->FindChild(CCharString("UI_LIST_BUY_ACTIONS"));
            if (actions != 0) {
                NUISystem::CDeletion deletion;
                deletion.Method = NUISystem::DELETION_METHOD_3;
                deletion.AssociatedParents.push_back(entry);
                actions->SetDeletion(deletion);
            }
        }
    }
    RefreshBuyPossibilityNumbers();
}
