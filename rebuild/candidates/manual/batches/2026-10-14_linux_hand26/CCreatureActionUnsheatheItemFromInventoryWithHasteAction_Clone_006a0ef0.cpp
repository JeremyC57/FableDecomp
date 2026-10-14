// CCreatureAction_UnsheatheItemFromInventoryWithHasteAction::Clone() -- new CCreatureAction_UnsheatheItemFromInventoryWithHasteAction(*this): the implicit copy constructor inlined over the out-of-line CCreatureAction_UnsheatheItemFromInventory copy, this class's vtable (0x0125CDDC), the haste action pointer at +0xb8 copied
// Retail: Fable.exe (TLC, VC7.1) function @ 0x006A0EF0, length 59 bytes.
// Callees: operator new @ 0x00BFEA1A (0xbc bytes, null-checked), CCreatureAction_UnsheatheItemFromInventory::CCreatureAction_UnsheatheItemFromInventory(const CCreatureAction_UnsheatheItemFromInventory&) @ 0x006A0E80.
// Shape: the landed Clone family (0x008C3D60) with one pointer member; sizeof = 0xbc (the base is 0xb8, as its own Clone @ 0x006A0E50 allocates).
struct CCreatureActionBase;

struct CCreatureAction_UnsheatheItemFromInventory {   // retail sizeof 0xb8
    virtual void v00();
    unsigned char _body[0xb8 - 4];
    CCreatureAction_UnsheatheItemFromInventory(const CCreatureAction_UnsheatheItemFromInventory& other);   // 0x006A0E80
};

struct CCreatureAction_UnsheatheItemFromInventoryWithHasteAction : CCreatureAction_UnsheatheItemFromInventory {   // vtable 0x0125CDDC; sizeof 0xbc
    CCreatureActionBase* m_pHasteAction;    // +0xb8
    virtual void v00();                     // implicit copy constructor
    CCreatureAction_UnsheatheItemFromInventory* Clone();
};

CCreatureAction_UnsheatheItemFromInventory* CCreatureAction_UnsheatheItemFromInventoryWithHasteAction::Clone()
{
    return new CCreatureAction_UnsheatheItemFromInventoryWithHasteAction(*this);
}
