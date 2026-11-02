// CTCInventoryItem::OnDie() -- leave the owning inventory (RemoveFromInventory inlined), then the empty CTCBase::OnDie
// Leaf: ?OnDie@CTCInventoryItem@@UAEXXZ
// Retail: Fable.exe (TLC, VC7.1) function @ 0x005D8EC0, 34 bytes, thiscall, tail `jmp CTCBase::OnDie`.
// Identity (proven): the only reference is vtable 0x01240004 (RTTI .?AVCTCInventoryItem@@ : CTCBase) slot 13, OnDie in the
//   CTCBase virtual order (the landed CTCScriptedControl::OnKill 0x00713700 / CTCBase::OnKill 0x006868F0 slot 14; CTCBase's own
//   slot 13 is the landed empty CTCBase::OnDie 0x006868E0, this row's tail). Ego_r.pdb ?OnDie@CTCInventoryItem@@UAEXXZ (donor
//   0x00559E20) right after GetInventoryOwner, the order retail keeps (0x005D8E90, this row).
// Callees: NInventory::CTCInventoryBase::RemoveItemFromInventory(CThing&) 0x005BC499 (Ego_r ?RemoveItemFromInventory@
//   CTCInventoryBase@NInventory@@IAEXAAVCThing@@@Z: protected; it reads the thing's def index +0x8C and calls the inventory's
//   virtual remove), CTCBase::OnDie 0x006868E0.
// Shape: `RemoveFromInventory(); CTCBase::OnDie();` with RemoveFromInventory (retail 0x005D8E20, Ego_r QAEXXZ) a plain member
//   defined above it in the same file: VC7.1 expands it here (GOTCHAS: a callee retail inlines in one place and calls in others).
//   The base call is qualified, so the tail is a direct `jmp`.
// Layout: as GetInventoryOwner 0x005D8E90 (PInventory +0x10, CTCBase::ParentThing +0x04).
#include "engine/CBaseClassNonCopyable.h"
#define FABLE_RETAIL_DEF_CLASS_BASE
#include "engine/CDefPointer.h"                          // PDef: CDefPointer<const CInventoryItemDef> (4 bytes)
class CThing;
class CCharString;
class CPersistContext;
class CTCParamsBase;
class CEnginePrimitivePrepareInfo;
class CThingManager {
public:
    const CThing* PeekPThingWithUID(unsigned __int64 uid) const;   // 0x0051ED60
};
class CTCBase : public CBaseClassNonCopyable {               // engine/CTCBase.h flattens the vptr: local polymorphic model
protected:
    virtual void OnSerialise(CPersistContext&, bool);      // slot 1
public:
    virtual void OnRemove();
    virtual void ConstructFromParams(const CTCParamsBase&);
    virtual void OnCreate();
    virtual void InitialActivate();
    virtual void OnInitialActivate();
    virtual void RenderUpdate(float);
    virtual bool HasFrameUpdate() const;
    virtual void FrameUpdate();
    virtual void BulletTimeFrameUpdate();
    virtual void DrawPreparePrimitivesForRendering(const CEnginePrimitivePrepareInfo&);
    virtual void Copy(const CTCBase&);
    virtual void OnDie();                                  // slot 13: 0x006868E0 (empty)
    virtual void OnKill();                                 // slot 14
protected:
    CThingManager& GetThingManager();                      // 0x00686AE0
    const CThingManager& GetThingManager() const;          // 0x00686AF0
public:
    CThing* ParentThing;                                   // +0x04
    bool ToBeRemoved;                                      // +0x08
    bool IsInGlobalUpdateTCs;                              // +0x09
};
class CTCInventoryItem;
namespace NInventory {
class CTCInventoryBase : public CTCBase {
    friend class CTCInventoryItem;
protected:
    void RemoveItemFromInventory(CThing& thing);           // 0x005BC499
};
}
class CInventoryItemDef;
class CTCInventoryItem : public CTCBase {                  // vtable 0x01240004, 0x28 bytes
public:
    void RemoveFromInventory();                            // 0x005D8E20
    const CThing* GetInventoryOwner() const;
    virtual void OnDie();                                  // slot 13
    CDefPointer<const CInventoryItemDef> PDef;             // +0x0c
    NInventory::CTCInventoryBase* PInventory;              // +0x10
    unsigned __int64 InventoryUIDToAttachTo;               // +0x18
    bool DefaultItem;                                      // +0x20
};
void CTCInventoryItem::RemoveFromInventory()
{
    if (PInventory) {
        PInventory->RemoveItemFromInventory(*ParentThing);
        PInventory = 0;
    }
}
void CTCInventoryItem::OnDie()
{
    RemoveFromInventory();
    CTCBase::OnDie();
}