// CTCInventoryItem::OnCreate() -- take the inventory-item sub-def of the parent thing's definition into PDef
// Leaf: OnCreate
// Retail: Fable.exe (TLC, VC7.1) @ 0x005D9380, 44 bytes (plain `ret`). Vtable 0x01240004 (CTCInventoryItem, rtti-vtables.tsv) slot 4
// (catalog "vfunc_4"; slot 4 is OnCreate in the sibling CTC vtables, e.g. CTCStockItem 0x007F4E80, CTCSmashableBase 0x007EEC90).
// Callees: CDefClassBase::GetSubDef<CInventoryItemDef>(CDefPointer&) @ 0x005BADE1 (landed; the result ignored), the def's virtual
// slot 1 on the last release (a sibling-call `jmp`).
// Layout: engine/CTCInventoryItem.h: PDef_Object +0x0c (CDefPointer<CInventoryItemDef const>); the header names the CTCBase parent
// thing (+0x04) "slot", so it is read through an accessor. CThing's definition pointer at +0x70 (retail).
// Shape: `GetDef()` is an inline by-value copy of the thing's def pointer (null-tested add-ref into esi, null-tested release).
#include "engine/CTCInventoryItem.h"

struct CInventoryItemDef;

struct CDefPointeeBase {
    virtual void v00();
    virtual void Destroy();                        // slot 1
    long m_refCount;                               // +0x04
};

template <class T> struct CDefPointer {
    T* Object;
    CDefPointer(const CDefPointer& other) : Object(other.Object) { if (Object) ++Object->m_refCount; }
    ~CDefPointer() { if (Object && --Object->m_refCount == 0) Object->Destroy(); }
    T* operator->() const { return Object; }
};

struct CDefClassBase : CDefPointeeBase {
    bool GetSubDef(CDefPointer<const CInventoryItemDef>& out);   // 0x005BADE1
};

struct CThing {
    unsigned char              _pad00[0x70];
    CDefPointer<CDefClassBase> PDef;               // +0x70
    CDefPointer<CDefClassBase> GetDef() const { return PDef; }
};

struct CTCInventoryItem_Methods : CTCInventoryItem {
    CThing* GetThing() const { return (CThing*)slot; }   // CTCBase::ParentThing (+0x04)
    void OnCreate();
};

void CTCInventoryItem_Methods::OnCreate()
{
    GetThing()->GetDef()->GetSubDef(*(CDefPointer<const CInventoryItemDef>*)&PDef_Object);
}
