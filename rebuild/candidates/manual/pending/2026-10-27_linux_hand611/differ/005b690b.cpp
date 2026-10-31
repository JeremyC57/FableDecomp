// CTCInventoryClothing::RefreshDescriptions() -- re-texts the clothing menu: every "CLOTHING TITLE" component gets the title string and every "Clothing_Description" component the description of the item it shows
// Grade: DIFFER(290v290) -- second pass only: retail keeps the list node in edi and the text component in ebx (as in the first pass), ours swaps them (equal-use colouring). Agent611: names re-proven (FindAllChildren slot 5, CText::SetValue slot 144, GetItemDescription const slot 78, real STLport list/map templates); the map key is the CText* -> CComponent* conversion temporary (frame word, as retail). Tried: shared/separate iterators, split loop init, continue form, end-first declaration.
// Leaf: ?RefreshDescriptions@CTCInventoryClothing@@QAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x005B690B, 290 bytes, thiscall, plain `ret`, ebp frame (size-optimised TU).
// Identity (proven): Ego_r.pdb ?RefreshDescriptions@CTCInventoryClothing@@QAEXXZ next to ConstructTitleString (0x005B6881, landed
//   under the same PDB spelling) and the two literals "CLOTHING TITLE" (0x01253A84) / "Clothing_Description" (0x01253A6C).
// Callees: list<NUISystem::CComponent*>(const allocator&) 0x0042ABAF / ~list 0x0042ABCA, CCharString(const char*, long)
//   0x0099EBF0 (built straight in the by-value argument slot), CComponent slot 5 FindAllChildren(CCharString, list&) (+0x14,
//   Ego_r.pdb UAEXVCCharString@@AAV?$list...), CWideString() 0x0099AED0 / ~CWideString 0x0099B510, ConstructTitleString
//   0x005B6881, CText slot 144 SetValue(const CWideString&, bool, bool) (+0x240; landed CText::SetValue 0x0054F410 is slot
//   144 of vtable 0x01249CCC), map<CComponent*, long>::operator[] 0x0042D1B7, this's slot 78 GetItemDescription(CWideString&,
//   long, bool) const (+0x138; vtable 0x0124356C slot 78 = 0x005B5CF8, Ego_r.pdb UBEXAAVCWideString@@J_N@Z).
// Shape: one list collects both passes (it is not cleared in between); both passes walk it with the SAME function-scope
//   iterator/end locals (the end node cached in one frame word); only positive indices get a description.
// Layout: engine/CTCInventoryClothing.h flattens the vptr (`void* __vftable`), which cannot host the virtual
//   GetItemDescription call; the local model keeps its ClothingItemMap +0x160 / ClothingList +0x16C offsets.
#pragma optimize("y", off)
#pragma optimize("s", on)

typedef unsigned short wchar_t;                    // VC7.1 without /Zc:wchar_t
class CCharString {
public:
    void* PStringData;
    CCharString(const char* text, long length = -1);   // 0x0099EBF0
    CCharString(const CCharString& other);             // 0x0099EC30
    ~CCharString();                                    // 0x0099EAE0
};
class CWideString {
public:
    void* PStringData;
    CWideString();                                     // 0x0099AED0
    ~CWideString();                                    // 0x0099B510
};
namespace std {
template <class _Tp> class allocator { public: allocator() { } };
template <class _T1, class _T2> struct pair { _T1 first; _T2 second; };
template <class _Tp> struct less { };
struct _List_node_base {
    _List_node_base* _M_next;
    _List_node_base* _M_prev;
};
template <class _Tp> struct _List_node : public _List_node_base {
    _Tp _M_data;
};
struct _List_iterator_base {
    _List_node_base* _M_node;
    _List_iterator_base(_List_node_base* __x) : _M_node(__x) { }
    _List_iterator_base() { }
    void _M_incr() { _M_node = _M_node->_M_next; }
    bool operator!=(const _List_iterator_base& __y) const { return _M_node != __y._M_node; }
};
template <class _Tp> struct _List_iterator : public _List_iterator_base {
    typedef _List_node<_Tp> _Node;
    _List_iterator(_Node* __x) : _List_iterator_base(__x) { }
    _List_iterator() { }
    _List_iterator(const _List_iterator& __x) : _List_iterator_base(__x._M_node) { }
    _Tp& operator*() const { return ((_Node*)_M_node)->_M_data; }
    _List_iterator& operator++() { this->_M_incr(); return *this; }
};
template <class _Tp, class _Alloc = allocator<_Tp> > class list {
public:
    typedef _List_node<_Tp> _Node;
    typedef _List_iterator<_Tp> iterator;
    _Node* _M_node;
    explicit list(const _Alloc& __a = _Alloc());       // 0x0042ABAF
    ~list();                                           // 0x0042ABCA
    iterator begin() { return iterator((_Node*)(_M_node->_M_next)); }
    iterator end() { return iterator(_M_node); }
};
template <class _Key, class _Tp, class _Compare = less<_Key>, class _Alloc = allocator<pair<const _Key, _Tp> > >
class map {
public:
    void* _M_header; unsigned long _M_node_count; unsigned char _M_key_compare;   // STLport _Rb_tree, 12 bytes
    _Tp& operator[](const _Key& __k);                  // 0x0042D1B7
};
}
namespace NUISystem {
class CComponent {
public:
    virtual void Slot000(); virtual void Slot001(); virtual void Slot002(); virtual void Slot003(); virtual void Slot004();
    virtual void FindAllChildren(CCharString name, std::list<CComponent*>& found);   // slot 5 (+0x14)
};
class CText : public CComponent {
public:
    virtual void Slot006(); virtual void Slot007(); virtual void Slot008(); virtual void Slot009(); virtual void Slot010(); virtual void Slot011();
    virtual void Slot012(); virtual void Slot013(); virtual void Slot014(); virtual void Slot015(); virtual void Slot016(); virtual void Slot017();
    virtual void Slot018(); virtual void Slot019(); virtual void Slot020(); virtual void Slot021(); virtual void Slot022(); virtual void Slot023();
    virtual void Slot024(); virtual void Slot025(); virtual void Slot026(); virtual void Slot027(); virtual void Slot028(); virtual void Slot029();
    virtual void Slot030(); virtual void Slot031(); virtual void Slot032(); virtual void Slot033(); virtual void Slot034(); virtual void Slot035();
    virtual void Slot036(); virtual void Slot037(); virtual void Slot038(); virtual void Slot039(); virtual void Slot040(); virtual void Slot041();
    virtual void Slot042(); virtual void Slot043(); virtual void Slot044(); virtual void Slot045(); virtual void Slot046(); virtual void Slot047();
    virtual void Slot048(); virtual void Slot049(); virtual void Slot050(); virtual void Slot051(); virtual void Slot052(); virtual void Slot053();
    virtual void Slot054(); virtual void Slot055(); virtual void Slot056(); virtual void Slot057(); virtual void Slot058(); virtual void Slot059();
    virtual void Slot060(); virtual void Slot061(); virtual void Slot062(); virtual void Slot063(); virtual void Slot064(); virtual void Slot065();
    virtual void Slot066(); virtual void Slot067(); virtual void Slot068(); virtual void Slot069(); virtual void Slot070(); virtual void Slot071();
    virtual void Slot072(); virtual void Slot073(); virtual void Slot074(); virtual void Slot075(); virtual void Slot076(); virtual void Slot077();
    virtual void Slot078(); virtual void Slot079(); virtual void Slot080(); virtual void Slot081(); virtual void Slot082(); virtual void Slot083();
    virtual void Slot084(); virtual void Slot085(); virtual void Slot086(); virtual void Slot087(); virtual void Slot088(); virtual void Slot089();
    virtual void Slot090(); virtual void Slot091(); virtual void Slot092(); virtual void Slot093(); virtual void Slot094(); virtual void Slot095();
    virtual void Slot096(); virtual void Slot097(); virtual void Slot098(); virtual void Slot099(); virtual void Slot100(); virtual void Slot101();
    virtual void Slot102(); virtual void Slot103(); virtual void Slot104(); virtual void Slot105(); virtual void Slot106(); virtual void Slot107();
    virtual void Slot108(); virtual void Slot109(); virtual void Slot110(); virtual void Slot111(); virtual void Slot112(); virtual void Slot113();
    virtual void Slot114(); virtual void Slot115(); virtual void Slot116(); virtual void Slot117(); virtual void Slot118(); virtual void Slot119();
    virtual void Slot120(); virtual void Slot121(); virtual void Slot122(); virtual void Slot123(); virtual void Slot124(); virtual void Slot125();
    virtual void Slot126(); virtual void Slot127(); virtual void Slot128(); virtual void Slot129(); virtual void Slot130(); virtual void Slot131();
    virtual void Slot132(); virtual void Slot133(); virtual void Slot134(); virtual void Slot135(); virtual void Slot136(); virtual void Slot137();
    virtual void Slot138(); virtual void Slot139(); virtual void Slot140(); virtual void Slot141(); virtual void Slot142(); virtual void Slot143();
    virtual void SetValue(const CWideString& text, bool lookUpTag, bool keepSize);   // slot 144 (+0x240)
};
}
using NUISystem::CComponent;
class CTCInventoryClothing {
public:
    virtual void Slot000(); virtual void Slot001(); virtual void Slot002(); virtual void Slot003(); virtual void Slot004(); virtual void Slot005();
    virtual void Slot006(); virtual void Slot007(); virtual void Slot008(); virtual void Slot009(); virtual void Slot010(); virtual void Slot011();
    virtual void Slot012(); virtual void Slot013(); virtual void Slot014(); virtual void Slot015(); virtual void Slot016(); virtual void Slot017();
    virtual void Slot018(); virtual void Slot019(); virtual void Slot020(); virtual void Slot021(); virtual void Slot022(); virtual void Slot023();
    virtual void Slot024(); virtual void Slot025(); virtual void Slot026(); virtual void Slot027(); virtual void Slot028(); virtual void Slot029();
    virtual void Slot030(); virtual void Slot031(); virtual void Slot032(); virtual void Slot033(); virtual void Slot034(); virtual void Slot035();
    virtual void Slot036(); virtual void Slot037(); virtual void Slot038(); virtual void Slot039(); virtual void Slot040(); virtual void Slot041();
    virtual void Slot042(); virtual void Slot043(); virtual void Slot044(); virtual void Slot045(); virtual void Slot046(); virtual void Slot047();
    virtual void Slot048(); virtual void Slot049(); virtual void Slot050(); virtual void Slot051(); virtual void Slot052(); virtual void Slot053();
    virtual void Slot054(); virtual void Slot055(); virtual void Slot056(); virtual void Slot057(); virtual void Slot058(); virtual void Slot059();
    virtual void Slot060(); virtual void Slot061(); virtual void Slot062(); virtual void Slot063(); virtual void Slot064(); virtual void Slot065();
    virtual void Slot066(); virtual void Slot067(); virtual void Slot068(); virtual void Slot069(); virtual void Slot070(); virtual void Slot071();
    virtual void Slot072(); virtual void Slot073(); virtual void Slot074(); virtual void Slot075(); virtual void Slot076(); virtual void Slot077();
    virtual void GetItemDescription(CWideString& text, long item, bool b) const;   // slot 78 (+0x138)
    unsigned char _pad_0x004[0x160 - 0x004];
    std::map<CComponent*, long> ClothingItemMap;       // +0x160
    CComponent* ClothingList;                          // +0x16c
    void ConstructTitleString(CWideString& title);     // 0x005B6881
    void RefreshDescriptions();
};

void CTCInventoryClothing::RefreshDescriptions()
{
    if (!ClothingList)
        return;
    std::list<CComponent*> components;
    ClothingList->FindAllChildren("CLOTHING TITLE", components);
    std::list<CComponent*>::iterator it = components.begin();
    std::list<CComponent*>::iterator end = components.end();
    for (; it != end; ++it) {
        NUISystem::CText* text = static_cast<NUISystem::CText*>(*it);
        CWideString title;
        ConstructTitleString(title);
        text->SetValue(title, true, true);
    }
    ClothingList->FindAllChildren("Clothing_Description", components);
    for (it = components.begin(), end = components.end(); it != end; ++it) {
        NUISystem::CText* text = static_cast<NUISystem::CText*>(*it);
        long item = ClothingItemMap[text];
        if (item > 0) {
            CWideString description;
            GetItemDescription(description, item, false);
            text->SetValue(description, true, true);
        }
    }
}
