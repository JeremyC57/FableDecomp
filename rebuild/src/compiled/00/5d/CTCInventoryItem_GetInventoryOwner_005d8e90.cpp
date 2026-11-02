// CTCInventoryItem::GetInventoryOwner() const -- the thing owning this item: the owning inventory's parent thing, else the thing with InventoryUIDToAttachTo, else 0
// Leaf: ?GetInventoryOwner@CTCInventoryItem@@QBEPBVCThing@@XZ
// Retail: Fable.exe (TLC, VC7.1) function @ 0x005D8E90, 43 bytes, thiscall, plain `ret`.
// Identity (proven): Ego_r.pdb ?GetInventoryOwner@CTCInventoryItem@@QBEPBVCThing@@XZ (donor 0x00559DF0) sits between
//   GetBoughtPrice (0x00559DD0) and OnDie (0x00559E20); retail has the same run: the landed GetBoughtPrice 0x005D8E70, this
//   row, then OnDie 0x005D8EC0 (CTCInventoryItem vtable 0x01240004 slot 13). Callers 0x0052295E, 0x006DEFD6, 0x006DF08D,
//   0x008980E6. The body reads PInventory (+0x10) and InventoryUIDToAttachTo (+0x18, unsigned __int64), the Ego_r members.
// Callees: CTCBase::GetThingManager() const 0x00686AF0 (landed ?GetThingManager@CTCBase@@IBEABVCThingManager@@XZ),
//   CThingManager::PeekPThingWithUID(unsigned __int64) const 0x0051ED60 (Ego_r ?PeekPThingWithUID@CThingManager@@QBEPBVCThing@@_K@Z:
//   the const twin before GetPThingWithUID 0x0051ED80, as in Ego_r 0x00528FD0 / 0x00529000).
// Shape: `uid > 0` on the unsigned 64-bit member gives the high-word `ja` / low-word `jbe` pair; VC7.1 pushes the uid before
//   evaluating GetThingManager(), whose result becomes ecx (`this` still in ecx for the getter).
// Layout: local polymorphic CTCBase (engine/CTCBase.h and engine/CTCInventoryItem.h flatten the vptr: header debt);
//   NInventory::CTCInventoryBase : CTCBase (ParentThing +0x04).
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
const CThing* CTCInventoryItem::GetInventoryOwner() const
{
    if (PInventory)
        return PInventory->ParentThing;
    if (InventoryUIDToAttachTo > 0)
        return GetThingManager().PeekPThingWithUID(InventoryUIDToAttachTo);
    return 0;
}