// CTCInventoryClothing::OnItemSelected(long itemType, const CEmptyContext& context) -- for a clothing item: collect the owner's equipped clothing defs (non-hidden ones), deselect everything, reselect every item of the outfit's set that is in the inventory plus the collected ones, then refresh the selected clothes and dress the preview/owner thing
// Grade: DIFFER(488v460) -- first-pass model (leaf OnItemSelected); retail keeps this in edi / zero in ebx (ours swapped), puts the interface key in the dead itemType argument slot [ebp+8] and the TC pointer chain in esi; frame 4 bytes larger. One shape tried (row mislabelled in the catalog).
// Retail: Fable.exe @ 0x005B48A2, 460 bytes (`leave ; ret 8`). Frame-pointer size TU: optimize("y",off) + ("s",on).
// Label correction: the catalog row says CTCShop::GetName; the only reference is CTCInventoryClothing vtable 0x0124356C
//   slot 33 (+0x84, CTCInventoryBase::OnItemSelected as the landed CCategory::SelectItemOfType calls it).
// Callees: CTCBase::GetDefinitions 0x006869D0, CDefinitionManager::GetDef 0x0042AF3C, GetSubDef 0x005935C4 / 0x0057FF92,
//   bitset::test 0x00410DE0, LowerBound 0x0040F020, list<long>(alloc) 0x0042AB58 / ~list 0x0042AB73, GetItems 0x00706800,
//   vector<long>::push_back 0x00485FF3, DeselectAll 0x005BCCF1, _Rb_tree increment 0x004292C0,
//   GetNumberOfItemsOfTypeInInventory 0x005BDF08, SelectItemWithDefIndex 0x005BDFB1, SetUpSelectedClothes 0x005B4795,
//   SetThingAsWearingSelectedClothes 0x005B406D, free 0x00BFEA14.
// Layout (local model): CThing TC bitset +0x20, interface map +0x44; CTCInventoryClothing preview thing +0x158, use-preview
//   flag +0x15c; the clothing def's rb-tree header pointer +0x28 (value at node +0x14); the sub def's hidden flag +0x44.
#pragma optimize("y", off)
#pragma optimize("s", on)
extern "C" void __cdecl free(void* block);          // 0x00BFEA14
struct CDefBase {
    virtual void v00();
    virtual void Destroy();                         // slot 1
    long RefCount;                                  // +0x04
    void Release() { if (--RefCount == 0) Destroy(); }
};
struct CClothingSetNode { CClothingSetNode* _M_left_dummy[2]; CClothingSetNode* _M_left; unsigned char _pad[0x8]; long DefIndex; };
CClothingSetNode* __cdecl RbTreeIncrement(CClothingSetNode* node);   // 0x004292C0
struct CClothingSetDef : CDefBase {
    unsigned char _pad08[0x20];
    CClothingSetNode* SetHeader;                    // +0x28
};
struct CClothingSubDef : CDefBase {
    unsigned char _pad08[0x3c];
    bool Hidden;                                    // +0x44
};
template <class T> struct CDefPtr {
    T* Object;
    CDefPtr() : Object(0) {}
    ~CDefPtr() { if (Object) Object->Release(); }
    T* operator->() const { return Object; }
};
struct CDef : CDefBase {
    bool GetSubDef(CDefPtr<CClothingSetDef>* out);  // 0x005935C4
    bool GetSubDef(CDefPtr<CClothingSubDef>* out);  // 0x0057FF92
};
struct CDefinitionManager {
    bool GetDef(long index, CDefPtr<CDef>* out);    // 0x0042AF3C
};
struct CLongList {                                  // STLport list<long>
    struct SNode { SNode* _M_next; SNode* _M_prev; long _M_data; };
    SNode* _M_node;
    struct allocator_type {};
    CLongList(const allocator_type& alloc);         // 0x0042AB58
    ~CLongList();                                   // 0x0042AB73
};
struct CLongVector {
    long* _M_start; long* _M_finish; long* _M_end_of_storage;
    CLongVector() : _M_start(0), _M_finish(0), _M_end_of_storage(0) {}
    ~CLongVector() { if (_M_start) free(_M_start); }
    void push_back(const long& x);                  // 0x00485FF3
};
struct CTCBase;
struct CWearer { void GetItems(CLongList* out, long mode); };   // 0x00706800
struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first; SEntry* last; SEntry* end;
    SEntry* LowerBound(const long& key);            // 0x0040F020
};
struct CTCBitset { bool test(unsigned long bit) const; };   // 0x00410DE0
struct CThing {
    unsigned char _pad00[0x20];
    CTCBitset     TCBits;                           // +0x20
    unsigned char _pad24[0x20];
    CInterfaceMap Interfaces;                       // +0x44
};
struct CEmptyContext {};
struct CTCInventoryClothing {
    void*   __vftable;
    CThing* Owner;                                  // +0x04
    unsigned char _pad08[0x150];
    CThing* PreviewThing;                           // +0x158
    bool    UsePreviewThing;                        // +0x15c
    CDefinitionManager* GetDefinitions();           // 0x006869D0
    void DeselectAll();                             // 0x005BCCF1
    long GetNumberOfItemsOfTypeInInventory(long defIndex);   // 0x005BDF08
    void SelectItemWithDefIndex(long defIndex);     // 0x005BDFB1
    void SetUpSelectedClothes();                    // 0x005B4795
    void SetThingAsWearingSelectedClothes(CThing* thing);    // 0x005B406D
    void OnItemSelected(long itemType, const CEmptyContext& context);
};

void CTCInventoryClothing::OnItemSelected(long itemType, const CEmptyContext& context)
{
    CDefPtr<CDef> def;
    GetDefinitions()->GetDef(itemType, &def);
    CDefPtr<CClothingSetDef> setDef;
    if (def->GetSubDef(&setDef))
    {
        CThing* owner = Owner;
        CLongVector worn;
        if (owner->TCBits.test(0x5e))
        {
            long key = 0x5e;
            CInterfaceMap::SEntry* it = owner->Interfaces.LowerBound(key);
            CWearer* wearer = (CWearer*)(it == owner->Interfaces.last || key < it->type ? owner->Interfaces.last : it)->component;
            CLongList items = CLongList(CLongList::allocator_type());
            wearer->GetItems(&items, 2);
            for (CLongList::SNode* node = items._M_node->_M_next; node != items._M_node; node = node->_M_next)
            {
                CDefPtr<CDef> itemDef;
                CDefPtr<CClothingSubDef> subDef;
                if (GetDefinitions()->GetDef(node->_M_data, &itemDef))
                {
                    if (itemDef->GetSubDef(&subDef) && !subDef->Hidden)
                        worn.push_back(node->_M_data);
                }
            }
        }
        DeselectAll();
        for (CClothingSetNode* n = setDef->SetHeader->_M_left; n != setDef->SetHeader; n = RbTreeIncrement(n))
        {
            if (GetNumberOfItemsOfTypeInInventory(n->DefIndex) > 0)
                SelectItemWithDefIndex(n->DefIndex);
        }
        for (long* p = worn._M_start; p != worn._M_finish; ++p)
            SelectItemWithDefIndex(*p);
    }
    SetUpSelectedClothes();
    if (UsePreviewThing)
    {
        if (PreviewThing)
            SetThingAsWearingSelectedClothes(PreviewThing);
    }
    else
        SetThingAsWearingSelectedClothes(Owner);
}
