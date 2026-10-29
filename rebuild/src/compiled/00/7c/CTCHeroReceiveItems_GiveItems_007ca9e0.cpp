// CTCHeroReceiveItems::GiveItems(const vector<pair<long, CCharString> >& items, CThing& container, bool sendMessage) -- create each positive-def item at the hero, put it in limbo and queue it for display, then remember the container and start the display at its position
// Leaf: GiveItems
// Retail: Fable.exe (TLC, VC7.1) function @ 0x007CA9E0, 334 bytes (+ int3 padding), `ret 0Ch`, `and esp,-8` frame.
//   Ego_r.pdb: `?GiveItems@CTCHeroReceiveItems@@QAEXABV?$vector@U?$pair@JVCCharString@@@std@@...@std@@AAVCThing@@_N@Z`.
// Callees: CThing::GetPos @ 0x004C73D0, CThingObject::Create(long, const C3DVector&, long, const CRightHandedSet*,
//   unsigned long, const CCharString&) @ 0x00703210 (static __fastcall), CThing::SetInLimbo(bool) @ 0x004C8CF0, the item
//   list's out-of-line insert(iterator, const value_type&) @ 0x007CBBC0 (manifest "Inventory_AddItemNode"), the
//   CBaseIntelligentPointer ctor/GetPItem/SetPItem/dtor @ 0x00A01B10 / 0x00A01B50 / 0x00A01B90 / 0x00A01C10
//   (CIntelligentPointer<CThingObject> vtable 0x0125A858, CIntelligentPointer<CThing> 0x01238C6C),
//   CTCHeroReceiveItems::PrepareToShowItems @ 0x007CA670.
// Shape: Create's stack arguments are pushed before the GetPos call that feeds edx; the queued element is
//   `push_back(value_type(CIntelligentPointer<CThingObject>(item), false))` (temporary pointer, pair built by the
//   pointer's copy ctor, inline push_back = insert(end(), x) with the iterator built in its argument slot -- the
//   iterator needs a user copy ctor for that `push ecx ; mov eax,esp ; mov [eax],edx` shape); the
//   container is stored by assigning a CIntelligentPointer<CThing> temporary.
// Layout: retail CTCHeroReceiveItems as the landed ctor 0x007CA830 (Items +0x0c, SendMessage +0x28, PContainer +0x29;
//   the intelligent-pointer class is 1-aligned); retail CThing dead bit +0x91.
#include <stddef.h>
class CThing;
class CThingObject;
class CBaseObject;
class CRightHandedSet;

class C3DVector { public: float X, Y, Z; };
class CCharString {
public:
    void* m_pData;
};

class CThing {
public:
    unsigned char _pad00[0x91];
    unsigned char m_flags91;                            // +0x91 bit 0: dead
    const C3DVector& GetPos() const;               // 0x004C73D0
    void SetInLimbo(bool inLimbo);                      // 0x004C8CF0
};
class CThingObject : public CThing {
public:
    static CThingObject* __fastcall Create(long defIndex, const C3DVector& position, long player,
                                           const CRightHandedSet* orientation, unsigned long uid,
                                           const CCharString& scriptName);   // 0x00703210
};

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
    CBaseObject* GetPItem() const;                           // 0x00A01B50
    void SetPItem(CThing* item);                        // 0x00A01B90
};
template <class T> class CIntelligentPointer : public CBaseIntelligentPointer { public:   // vtables 0x0125A858 / 0x01238C6C
    virtual void v00();
    CIntelligentPointer() { }
    CIntelligentPointer(T* item) { SetPItem((CThing*)item); }
    CIntelligentPointer(const CIntelligentPointer& other) { SetPItem(((CThing*)other.GetPItem())); }
    CIntelligentPointer& operator=(const CIntelligentPointer& other)
    {
        CThing* item = ((CThing*)other.GetPItem());
        SetPItem(item);
        return *this;
    }
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
struct SGiveItem { long DefIndex; CCharString ScriptName; };   // pair<long, CCharString>
struct CGiveItemVector {                                // STLport vector<pair<long, CCharString> >
    SGiveItem* _M_start;
    SGiveItem* _M_finish;
    SGiveItem* _M_end_of_storage;
    unsigned int size() const { return _M_finish - _M_start; }
    const SGiveItem& operator[](unsigned int n) const { return _M_start[n]; }
};

class CTCHeroReceiveItems : public CTCBase { public:
    CItemList Items;                                    // +0x0c
    unsigned char PickedUpItems[0xc];                   // +0x10 STLport vector<long>
    CIntelligentPointer<CThingObject> PCurrentItem;     // +0x1c
    bool PersistCurrentItem;                            // +0x24
    bool ReceivingItems;                                // +0x25
    bool DisplayingItems;                               // +0x26
    bool ForceClear;                                    // +0x27
    bool SendMessage;                                   // +0x28
    CIntelligentPointer<CThing> PContainer;             // +0x29
    void PrepareToShowItems(const C3DVector& position); // 0x007CA670
    void GiveItems(const CGiveItemVector& items, CThing& container, bool sendMessage);
};

void CTCHeroReceiveItems::GiveItems(const CGiveItemVector& items, CThing& container, bool sendMessage)
{
    for (unsigned int i = 0; i < items.size(); ++i) {
        if (items[i].DefIndex > 0) {
            CThingObject* item = CThingObject::Create(items[i].DefIndex, ParentThing->GetPos(), 5, 0, 0,
                                                      items[i].ScriptName);
            if (item && !(item->m_flags91 & 1)) {
                item->SetInLimbo(true);
                Items.push_back(SItemEntry(CIntelligentPointer<CThingObject>(item), false));
            }
        }
    }
    SendMessage = sendMessage;
    PContainer = CIntelligentPointer<CThing>(&container);
    PrepareToShowItems(((CThing*)PContainer.GetPItem())->GetPos());
}