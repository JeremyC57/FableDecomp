// CTCInventoryWeapons::RefreshAugmentationsLeftDescriptions() -- with the weapons list open, collect the augmentation items of the inventory, then rewrite the text of every "Augmentations_Left_Description_M" (melee) and "..._R" (ranged) component under the weapons list
// Leaf: RefreshAugmentationsLeftDescriptions
// Retail: Fable.exe (TLC, VC7.1) function @ 0x005C69AA, 380 bytes, plain `ret`. Size TU with a frame pointer:
//   `#pragma optimize("y", off)` + `("s", on)` (`leave`, `pop ecx` after free).
// Callees: CTCInventoryBase::GetAllItems(vector<pair<long, long> >&) @ 0x005BF482, CTCInventoryItem::IsAugmentation(long)
//   @ 0x005D9870 (static `__fastcall`), the item vector's push_back @ 0x005947B6, list<CComponent*>::list(const
//   allocator&) @ 0x0042ABAF and its dtor @ 0x0042ABCA (label "CFileInstaller::CActiveFile::OnReadFinished" is
//   propagated), CCharString(const CCharString&) @ 0x0099EC30 (the by-value name), the weapons list's virtual slot 5
//   (+0x14, find the children named ...), CWideString() @ 0x0099AED0 / ~CWideString @ 0x0099B510 (labels say
//   CCharString: byte-identical bodies), ConstructAugmentationsLeftDescription(CWideString&, vector<pair<long, long> >&,
//   bool) @ 0x005C4F42, the component's virtual slot 144 (+0x240, set text (text, true, true)), the CRT free thunk
//   @ 0x00BFEA14.
// Data: 0x013B8BB8 "Augmentations_Left_Description_M", 0x013B8BBC "Augmentations_Left_Description_R" (CCharString).
// Layout: retail CTCInventoryWeapons WeaponsList +0x17C (as the landed ctor 0x005C3947).
#pragma optimize("y", off)
#pragma optimize("s", on)

extern "C" void __cdecl free(void* block);                 // 0x00BFEA14

struct CCharStringData;
struct CCharString {
    CCharStringData* PStringData;
    CCharString(const CCharString& other);                  // 0x0099EC30
    ~CCharString();                                         // 0x0099EAE0
};
extern CCharString g_AugmentationsLeftDescriptionM;         // 0x013B8BB8
extern CCharString g_AugmentationsLeftDescriptionR;         // 0x013B8BBC
struct CWideStringData;
struct CWideString {
    CWideStringData* PStringData;
    CWideString();                                          // 0x0099AED0
    ~CWideString();                                         // 0x0099B510
};

template <class T1, class T2> struct pair {
    T1 first;
    T2 second;
};
typedef pair<long, long> CItem;                             // (def index, count)
struct CItemVector {                                        // STLport vector<pair<long, long> > (12 bytes)
    CItem* _M_start;
    CItem* _M_finish;
    CItem* _M_end_of_storage;
    CItemVector() : _M_start(0), _M_finish(0), _M_end_of_storage(0) { }
    ~CItemVector()
    {
        if (_M_start)
            free(_M_start);
    }
    unsigned long size() const { return _M_finish - _M_start; }
    CItem& operator[](unsigned long n) { return _M_start[n]; }
    void push_back(const CItem& x);                         // 0x005947B6
};
struct CTCInventoryItem {
    static bool __fastcall IsAugmentation(long defIndex);   // 0x005D9870
};

namespace NUISystem { struct CComponent; }
template <class T> struct CAllocator { CAllocator() { } };
struct SComponentNode {
    SComponentNode* _M_next;
    SComponentNode* _M_prev;
    NUISystem::CComponent* _M_data;                         // +0x08
};
struct CComponentList {                                     // STLport list<CComponent*> (release: node pointer)
    SComponentNode* _M_node;
    explicit CComponentList(const CAllocator<NUISystem::CComponent*>& a = CAllocator<NUISystem::CComponent*>());   // 0x0042ABAF
    ~CComponentList();                                      // 0x0042ABCA
};
namespace NUISystem {
struct CComponent {
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04();
    virtual void FindChildren(CCharString name, CComponentList& out);   // slot 5 (+0x14)
    virtual void v006(); virtual void v007(); 
    virtual void v008(); virtual void v009(); virtual void v010(); virtual void v011(); virtual void v012(); virtual void v013(); virtual void v014(); virtual void v015(); 
    virtual void v016(); virtual void v017(); virtual void v018(); virtual void v019(); virtual void v020(); virtual void v021(); virtual void v022(); virtual void v023(); 
    virtual void v024(); virtual void v025(); virtual void v026(); virtual void v027(); virtual void v028(); virtual void v029(); virtual void v030(); virtual void v031(); 
    virtual void v032(); virtual void v033(); virtual void v034(); virtual void v035(); virtual void v036(); virtual void v037(); virtual void v038(); virtual void v039(); 
    virtual void v040(); virtual void v041(); virtual void v042(); virtual void v043(); virtual void v044(); virtual void v045(); virtual void v046(); virtual void v047(); 
    virtual void v048(); virtual void v049(); virtual void v050(); virtual void v051(); virtual void v052(); virtual void v053(); virtual void v054(); virtual void v055(); 
    virtual void v056(); virtual void v057(); virtual void v058(); virtual void v059(); virtual void v060(); virtual void v061(); virtual void v062(); virtual void v063(); 
    virtual void v064(); virtual void v065(); virtual void v066(); virtual void v067(); virtual void v068(); virtual void v069(); virtual void v070(); virtual void v071(); 
    virtual void v072(); virtual void v073(); virtual void v074(); virtual void v075(); virtual void v076(); virtual void v077(); virtual void v078(); virtual void v079(); 
    virtual void v080(); virtual void v081(); virtual void v082(); virtual void v083(); virtual void v084(); virtual void v085(); virtual void v086(); virtual void v087(); 
    virtual void v088(); virtual void v089(); virtual void v090(); virtual void v091(); virtual void v092(); virtual void v093(); virtual void v094(); virtual void v095(); 
    virtual void v096(); virtual void v097(); virtual void v098(); virtual void v099(); virtual void v100(); virtual void v101(); virtual void v102(); virtual void v103(); 
    virtual void v104(); virtual void v105(); virtual void v106(); virtual void v107(); virtual void v108(); virtual void v109(); virtual void v110(); virtual void v111(); 
    virtual void v112(); virtual void v113(); virtual void v114(); virtual void v115(); virtual void v116(); virtual void v117(); virtual void v118(); virtual void v119(); 
    virtual void v120(); virtual void v121(); virtual void v122(); virtual void v123(); virtual void v124(); virtual void v125(); virtual void v126(); virtual void v127(); 
    virtual void v128(); virtual void v129(); virtual void v130(); virtual void v131(); virtual void v132(); virtual void v133(); virtual void v134(); virtual void v135(); 
    virtual void v136(); virtual void v137(); virtual void v138(); virtual void v139(); virtual void v140(); virtual void v141(); virtual void v142(); virtual void v143(); 
    
    virtual void SetText(const CWideString& text, bool a, bool b);      // slot 144 (+0x240)
};
}

class CThing;
namespace NInventory {
struct CTCInventoryBase {
    void*         __vftable;                                // +0x00
    CThing*       ParentThing;                              // +0x04 CTCBase
    unsigned char _pad08[0x17c - 0x08];
    void GetAllItems(CItemVector& items) const;             // 0x005BF482
};
}

struct CTCInventoryWeapons : NInventory::CTCInventoryBase {
    NUISystem::CComponent* WeaponsList;                     // +0x17C
    void ConstructAugmentationsLeftDescription(CWideString& text, CItemVector& augmentations, bool melee);   // 0x005C4F42
    void RefreshAugmentationsLeftDescriptions();
};

void CTCInventoryWeapons::RefreshAugmentationsLeftDescriptions()
{
    if (WeaponsList) {
        CItemVector items;
        CItemVector augmentations;
        GetAllItems(items);
        for (unsigned long i = 0; i < items.size(); ++i)
            if (CTCInventoryItem::IsAugmentation(items[i].first))
                augmentations.push_back(items[i]);
        CComponentList components;
        WeaponsList->FindChildren(g_AugmentationsLeftDescriptionM, components);
        for (SComponentNode* n = components._M_node->_M_next, *end = components._M_node; n != end; n = n->_M_next) {
            NUISystem::CComponent* component = n->_M_data;
            CWideString text;
            ConstructAugmentationsLeftDescription(text, augmentations, true);
            component->SetText(text, true, true);
        }
        WeaponsList->FindChildren(g_AugmentationsLeftDescriptionR, components);
        for (SComponentNode* n = components._M_node->_M_next, *end = components._M_node; n != end; n = n->_M_next) {
            NUISystem::CComponent* component = n->_M_data;
            CWideString text;
            ConstructAugmentationsLeftDescription(text, augmentations, false);
            component->SetText(text, true, true);
        }
    }
}
