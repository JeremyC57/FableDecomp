// CTCInventoryItem::GetStartingRightHandedSet() -- the item def's starting orientation as a right-handed set
// Retail 0x005D92F0, 45 bytes (+3 int3), ret 4 (hidden CRightHandedSet result). Callers:
// CTCHeroReceiveItems::GlobalUpdate (0x007CB2D9, 0x007CB3F6).
// Shape: PDef (CDefPointer<const CInventoryItemDef>, +0x0C) is copied into a local -- inline add-ref
// `if (p) ++p->refCount` under a null test -- the def's Orientation (+0x54) goes to the
// `__fastcall` GetRHSetFromOrientation 0x005D8EF0 (hidden result in ecx = our hidden result, the
// orientation in edx), then the copy is released (`if (--refCount == 0) Destroy()`, vtable slot 1,
// no null test) and the hidden pointer is returned.
// engine/CTCInventoryItem.h flattens the polymorphic CTCBase into a POD: local model, header offsets.

class CRightHandedSet { public:
    float m_v[6];
    CRightHandedSet(const CRightHandedSet& other);
};
CRightHandedSet __fastcall GetRHSetFromOrientation(long orientation);    // 0x005D8EF0

class CInventoryItemDef { public:
    virtual void v00();
    virtual void Destroy();                        // slot 1
    long refCount;                                 // +0x04
    unsigned char _pad_08[0x4C];
    long Orientation;                              // +0x54 EInventoryObjectOrientation (Ego_r.pdb)
    void AddRef() { ++refCount; }
    void Release() { if (--refCount == 0) Destroy(); }
};

struct CInventoryItemDefPointer {                  // CDefPointer<const CInventoryItemDef>
    CInventoryItemDef* Object;
    CInventoryItemDefPointer(const CInventoryItemDefPointer& other) : Object(other.Object) { if (Object) Object->AddRef(); }
    ~CInventoryItemDefPointer() { Object->Release(); }
    CInventoryItemDef* operator->() const { return Object; }
};

class CThing;
struct CTCInventoryBase;

class CTCBase { public:
    virtual void v00();
    CThing* ParentThing;                           // +0x04
    bool    ToBeRemoved;                           // +0x08
    bool    IsInGlobalUpdateTCs;                   // +0x09
};

class CTCInventoryItem : public CTCBase { public:
    CInventoryItemDefPointer PDef;                 // +0x0C
    CTCInventoryBase* PInventory;                  // +0x10
    CRightHandedSet GetStartingRightHandedSet();
};

CRightHandedSet CTCInventoryItem::GetStartingRightHandedSet()
{
    CInventoryItemDefPointer def(PDef);
    return GetRHSetFromOrientation(def->Orientation);
}