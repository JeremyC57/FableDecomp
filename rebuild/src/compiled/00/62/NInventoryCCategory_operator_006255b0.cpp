// NInventory::CCategory::operator=(const CCategory& other) -- memberwise copy of an inventory category: associated-thing pointer, item vector, category def pointer, inventory, indices and flags
// Leaf: ??4CCategory
// Retail: Fable.exe (TLC, VC7.1) function @ 0x006255B0, length 120 bytes (+8 int3 padding), `ret 4`, returns this.
// Label correction: the catalog row says "CTCHero::Copy" (CTCHero::Copy is `void Copy(const CTCBase&)`), but the body returns
// `this` and is called per element of a 0x2C-byte record array (the copy loop at 0x00625830 steps both pointers by 0x2C):
// the record is NInventory::CCategory (Ego_r.pdb, 0x30 with a 16-byte vector; 0x2C in retail).
// Callees: CBaseIntelligentPointer::GetPItem @ 0x00A01B50 / SetPItem @ 0x00A01B90, the out-of-line
// vector<NInventory::CItem>::operator= @ 0x005C1F42 (20-byte items).
// Layout: PAssociatedThing +0x00 (CIntelligentPointer<CThing>, vptr + item), ItemsInCategory +0x08, PDef +0x14 (CDefPointer:
// release through slot 1, cleared inside the null test, then AddRef of the new def), PInventory +0x18, GlobalDefIndex +0x1C,
// SelectedItemIndex +0x20, HighlightedItemIndex +0x24, Selected +0x28, DisplayItems +0x29.
struct CBaseObject;
class CBaseIntelligentPointer {
public:
    virtual ~CBaseIntelligentPointer();
    CBaseObject* PData;
    CBaseObject* GetPItem() const;              // 0x00A01B50
    void SetPItem(CBaseObject* item);           // 0x00A01B90
};
template <class T> class CIntelligentPointer : public CBaseIntelligentPointer {
public:
    CIntelligentPointer& operator=(const CIntelligentPointer& o) { SetPItem(o.GetPItem()); return *this; }
};
struct CDefPointeeBase {
    virtual void Destroy();
    virtual void Delete();                      // slot 1
    long m_refCount;                            // +0x04
};
template <class T> struct CDefPointer {
    CDefPointeeBase* Object;
    CDefPointer& operator=(const CDefPointer& o)
    {
        if (Object != o.Object) {
            CDefPointeeBase* object = Object;
            if (object) {
                if (--object->m_refCount == 0)
                    object->Delete();
                Object = 0;
            }
            Object = o.Object;
            if (Object)
                ++Object->m_refCount;
        }
        return *this;
    }
};
namespace NInventory {
struct CItem { unsigned char _body[0x14]; };
}
struct CItemVector {                            // STLport vector<NInventory::CItem> (12 bytes)
    NInventory::CItem* _M_start;
    NInventory::CItem* _M_finish;
    NInventory::CItem* _M_end_of_storage;
    CItemVector& operator=(const CItemVector& o);   // 0x005C1F42
};
class CThing;
class CTCBase;
struct CInventoryCategoryDef;
class CTCInventoryBase;

namespace NInventory {
struct CCategory {
    CIntelligentPointer<CThing>              PAssociatedThing;      // +0x00
    CItemVector                              ItemsInCategory;       // +0x08
    CDefPointer<const CInventoryCategoryDef> PDef;                  // +0x14
    CTCInventoryBase*                        PInventory;            // +0x18
    long                                     GlobalDefIndex;        // +0x1c
    long                                     SelectedItemIndex;     // +0x20
    long                                     HighlightedItemIndex;  // +0x24
    bool                                     Selected;              // +0x28
    bool                                     DisplayItems;          // +0x29
    CCategory& operator=(const CCategory& other);
};

CCategory& CCategory::operator=(const CCategory& other)
{
    PAssociatedThing = other.PAssociatedThing;
    ItemsInCategory = other.ItemsInCategory;
    PDef = other.PDef;
    PInventory = other.PInventory;
    GlobalDefIndex = other.GlobalDefIndex;
    SelectedItemIndex = other.SelectedItemIndex;
    HighlightedItemIndex = other.HighlightedItemIndex;
    Selected = other.Selected;
    DisplayItems = other.DisplayItems;
    return *this;
}
}