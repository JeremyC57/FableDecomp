// CTCInventory::AdjustVideoOptionsSliders() -- find "PC_VIDEO_PREFS_LIST" under the first inventory component that has it, then re-read every slider and text slider in it from the profile
// Leaf: ?AdjustVideoOptionsSliders@CTCInventory@@QAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x005838DD, 200 bytes, thiscall, plain `ret`, ebp frame.
// Identity (proven): FableWin.pdb and Ego_r.pdb ?AdjustVideoOptionsSliders@CTCInventory@@QAEXXZ; the three bodies sit in the PDBs'
//   Audio / Video / Gameplay order and build "PC_VIDEO_PREFS_LIST" (0x01250174); called from the front-end manager's
//   options handler 0x0041E6D3 (at 0x00421237). The manifest's "AdjustAudioOptionsSliders" on the Video / Gameplay twins is propagated.
// Callees: CCharString(const char*, long) 0x0099EBF0, CComponent slot 3 FindChild(CCharString) and slot 6
//   FindAllChildrenOfType(type, list&) (+0x18), list<CComponent*> ctor 0x0042ABAF / clear 0x0042A18F / dtor 0x0042ABCA,
//   CSlider::AdjustValueToProfile 0x0054A6F0, CTextSlider::AdjustValueToProfile 0x00548B50.
// Shape (family M145, representative 0x00583815): the loop counter and the found-branch list share the [ebp-8] slot; the
//   slider half is the landed CFrontEndManager::ResetSlidersToProfileValues 0x00595356 on the found list component.
//   Layout: NInventory::CTCInventoryBase::Components (vector<CComponent*>) at +0x0C as in Ego_r.pdb (CTCBase is 12 bytes).
#pragma optimize("y", off)
#pragma optimize("s", on)
class CCharString {
public:
    void* PStringData;
    CCharString(const char* text, long length = -1);      // 0x0099EBF0
    CCharString(const CCharString& other);                // 0x0099EC30
    ~CCharString();                                       // 0x0099EAE0
};
namespace std {
template <class T> class allocator { public: allocator() { } };
template <class T> struct _List_node {
    _List_node* _M_next;
    _List_node* _M_prev;
    T           _M_data;
};
template <class T> struct _List_iterator {
    _List_node<T>* _M_node;
    T& operator*() const { return _M_node->_M_data; }
    _List_iterator& operator++() { _M_node = _M_node->_M_next; return *this; }
    bool operator!=(const _List_iterator& x) const { return _M_node != x._M_node; }
};
template <class T, class A = allocator<T> > class list {
public:
    typedef _List_iterator<T> iterator;
    _List_node<T>* _M_node;
    explicit list(const A& a = A());                      // 0x0042ABAF
    ~list();                                              // 0x0042ABCA
    void clear();                                         // 0x0042A18F
    iterator begin() { iterator i; i._M_node = _M_node->_M_next; return i; }
    iterator end() { iterator i; i._M_node = _M_node; return i; }
};
template <class _Tp, class _Alloc = allocator<_Tp> > class vector {
public:
    unsigned int size() const { return unsigned int(_M_finish - _M_start); }
    _Tp& operator[](unsigned int __n) { return *(_M_start + __n); }
    _Tp* _M_start;
    _Tp* _M_finish;
    _Tp* _M_end_of_storage;
};
}
namespace NUISystem {
class CComponent {
public:
    enum EType { ETYPE_SLIDER = 0x0f, ETYPE_TEXT_SLIDER = 0x10 };   // values as pushed (names not recovered)
    virtual void _v000(); virtual void _v001(); virtual void _v002();
    virtual CComponent* FindChild(CCharString name);      // slot 3 (+0x0c)
    virtual void _v004(); virtual void _v005();
    virtual void FindAllChildrenOfType(EType type, std::list<CComponent*>& found);   // slot 6 (+0x18)
};
class CSlider : public CComponent {
public:
    void AdjustValueToProfile();                          // 0x0054A6F0
};
class CTextSlider : public CComponent {
public:
    void AdjustValueToProfile();                          // 0x00548B50
};
}
using NUISystem::CComponent;
class CTCInventory {
public:
    void*                          __vftable;             // +0x00
    unsigned char                  _pad04[0x0C - 0x04];   // CTCBase: ParentThing, flags
    std::vector<CComponent*>       Components;            // +0x0C (NInventory::CTCInventoryBase)
    void AdjustVideoOptionsSliders();
};
void CTCInventory::AdjustVideoOptionsSliders()
{
    for (unsigned int i = 0; i < Components.size(); ++i) {
        CComponent* prefs = Components[i]->FindChild(CCharString("PC_VIDEO_PREFS_LIST"));
        if (prefs) {
            std::list<CComponent*> components;
            prefs->FindAllChildrenOfType(CComponent::ETYPE_SLIDER, components);
            for (std::list<CComponent*>::iterator it = components.begin(); it != components.end(); ++it) {
                NUISystem::CSlider* slider = static_cast<NUISystem::CSlider*>(*it);
                if (slider)
                    slider->AdjustValueToProfile();
            }
            components.clear();
            prefs->FindAllChildrenOfType(CComponent::ETYPE_TEXT_SLIDER, components);
            for (std::list<CComponent*>::iterator it = components.begin(); it != components.end(); ++it) {
                NUISystem::CTextSlider* slider = static_cast<NUISystem::CTextSlider*>(*it);
                if (slider)
                    slider->AdjustValueToProfile();
            }
            return;
        }
    }
}