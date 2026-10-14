// CCreatureAction_UnsheatheItemFromInventoryWithHasteAction::CCreatureAction_UnsheatheItemFromInventoryWithHasteAction(CCreatureActionBase*, CThingCreatureBase*, const CCharString&, CThing*, long, long, CCreatureActionBase*) -- forwards the last six arguments to the CCreatureAction_UnsheatheItemFromInventory constructor, stores the haste action at +0xb8, own vtable (0x0125CDDC)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0069F1F0, length 62 bytes.
// Callees: CCreatureAction_UnsheatheItemFromInventory::CCreatureAction_UnsheatheItemFromInventory(CThingCreatureBase*, const CCharString&, CThing*, long, long, CCreatureActionBase*) @ 0x0069F0F0 (manifest prototype).
// Shape: the six forwarded arguments are pushed right to left straight from their slots (loads interleaved with the pushes); the
// first argument is read after the base call and stored before the vtable (an init-list member whose plain store sinks the vtable
// store below it, as 0x008CCB80). Retail CCreatureAction_UnsheatheItemFromInventory is 0xb8 bytes. `ret 1Ch`: seven arguments.
struct CThingCreatureBase;
struct CThing;
struct CCharString;
struct CCreatureActionBase;

struct CCreatureAction_UnsheatheItemFromInventory {   // retail sizeof 0xb8
    virtual void v00();
    unsigned char _body[0xb8 - 4];
    CCreatureAction_UnsheatheItemFromInventory(CThingCreatureBase* owner, const CCharString& itemName, CThing* item, long a, long b, CCreatureActionBase* followUp);   // 0x0069F0F0
};

struct CCreatureAction_UnsheatheItemFromInventoryWithHasteAction : CCreatureAction_UnsheatheItemFromInventory {   // vtable 0x0125CDDC
    CCreatureActionBase* m_pHasteAction;    // +0xb8
    virtual void v00();
    CCreatureAction_UnsheatheItemFromInventoryWithHasteAction(CCreatureActionBase* hasteAction, CThingCreatureBase* owner, const CCharString& itemName, CThing* item, long a, long b, CCreatureActionBase* followUp);
};

CCreatureAction_UnsheatheItemFromInventoryWithHasteAction::CCreatureAction_UnsheatheItemFromInventoryWithHasteAction(CCreatureActionBase* hasteAction, CThingCreatureBase* owner, const CCharString& itemName, CThing* item, long a, long b, CCreatureActionBase* followUp)
    : CCreatureAction_UnsheatheItemFromInventory(owner, itemName, item, a, b, followUp)
    , m_pHasteAction(hasteAction)
{
}
