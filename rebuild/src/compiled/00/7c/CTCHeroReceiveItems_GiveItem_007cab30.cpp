// CTCHeroReceiveItems::GiveItem(CThingObject& item, bool persist) -- put the item in limbo, queue it (with its flag) on the items-to-show list, then start the display at the item's position
// Leaf: ?GiveItem@CTCHeroReceiveItems
// Retail: Fable.exe (TLC, VC7.1) @ 0x007CAB30, 158 bytes (+2 int3), thiscall `ret 8`.
// Identity: Ego_r.pdb name set ?GiveItem@CTCHeroReceiveItems@@QAEXAAVCThingObject@@_N@Z (public, CThingObject&, bool):
//   six callers in other classes (0x005EB6A9, 0x006AC380, 0x006ACD6C, 0x006ACFFE, 0x00844BAD, 0x00898C27). It is the
//   one-item form of the landed GiveItems 0x007CA9E0 (same push_back shape, flag from the argument). The catalog
//   label "TCHeroReceiveItems_AddItemToInventory" names the private AddItemToInventory(CThing&, bool), which is not
//   this body (see NOTES).
// Callees: CThing::SetInLimbo(bool) @ 0x004C8CF0, CBaseIntelligentPointer ctor/GetPItem/SetPItem/dtor @ 0x00A01B10 /
//   0x00A01B50 / 0x00A01B90 / 0x00A01C10 (CIntelligentPointer<CThingObject> vtable 0x0125A858), the item list's
//   out-of-line insert(iterator, const value_type&) @ 0x007CBBC0, CThing::GetPos @ 0x004C73D0,
//   CTCHeroReceiveItems::PrepareToShowItems(const C3DVector&) @ 0x007CA670.
// Layout / shape: the landed GiveItems model (1-aligned intelligent pointer and pair, so the locals sit at odd frame
//   offsets; push_back = insert(end(), x) with the iterator built in its argument slot).
class CThing;
class CThingObject;
class CBaseObject;

class C3DVector { public: float X, Y, Z; };

class CThing {
public:
    const C3DVector& GetPos() const;               // 0x004C73D0
    void SetInLimbo(bool inLimbo);                      // 0x004C8CF0
};
class CThingObject : public CThing { };

class CTCBase { public:
    virtual void v00();
    CThing* ParentThing;                                // +0x04
    bool ToBeRemoved;                                   // +0x08
    bool IsInGlobalUpdateTCs;                           // +0x09
};
#pragma pack(push, 1)
struct CBaseIntelligentPointer {
    virtual void v00();
    CBaseObject* m_pItem;                               // +0x04
    CBaseIntelligentPointer();                          // 0x00A01B10
    virtual ~CBaseIntelligentPointer();                         // 0x00A01C10
    CThing* GetPItem() const;                           // 0x00A01B50
    void SetPItem(CThing* item);                        // 0x00A01B90
};
template <class T> class CIntelligentPointer : public CBaseIntelligentPointer { public:   // vtable 0x0125A858 for CThingObject
    virtual void v00();
    CIntelligentPointer() { }
    CIntelligentPointer(T* item) { SetPItem((CThing*)item); }
    CIntelligentPointer(const CIntelligentPointer& other) { SetPItem(other.GetPItem()); }
};
template <class T1, class T2> struct pair {
    T1 first;
    T2 second;
    pair(const T1& a, const T2& b) : first(a), second(b) { }
};
typedef pair<CIntelligentPointer<CThingObject>, bool> SItemEntry;
#pragma pack(pop)

struct SItemNode { SItemNode* _M_next; SItemNode* _M_prev; SItemEntry _M_data; };
struct CItemListIterator {                              // STLport _List_iterator
    SItemNode* _M_node;
    CItemListIterator(SItemNode* node) : _M_node(node) { }
    CItemListIterator(const CItemListIterator& other) : _M_node(other._M_node) { }
};
struct CItemList {                                      // STLport list (retail: the node pointer only)
    SItemNode* _M_node;
    CItemListIterator end() { return CItemListIterator(_M_node); }
    CItemListIterator insert(CItemListIterator position, const SItemEntry& x);   // 0x007CBBC0
    void push_back(const SItemEntry& x) { insert(end(), x); }
};

class CTCHeroReceiveItems : public CTCBase { public:
    CItemList Items;                                    // +0x0c
    void PrepareToShowItems(const C3DVector& position); // 0x007CA670
    void GiveItem(CThingObject& item, bool persist);
};

void CTCHeroReceiveItems::GiveItem(CThingObject& item, bool persist)
{
    item.SetInLimbo(true);
    Items.push_back(SItemEntry(CIntelligentPointer<CThingObject>(&item), persist));
    PrepareToShowItems(item.GetPos());
}