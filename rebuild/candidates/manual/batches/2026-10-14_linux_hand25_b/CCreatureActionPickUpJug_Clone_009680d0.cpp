// CCreatureAction_PickUpJug::Clone() -- new CCreatureAction_PickUpJug(*this): the implicit copy constructor inlined over the out-of-line CCreatureAction_PickUpThingAtTable copy constructor, then this class's vtable (0x012963A4)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x009680D0, length 47 bytes.
// Callees: operator new @ 0x00BFEA1A (0xb4 bytes, null-checked), CCreatureAction_PickUpThingAtTable::CCreatureAction_PickUpThingAtTable(const CCreatureAction_PickUpThingAtTable&) @ 0x0084C9A0.
// Shape: PickUpJug adds no data to PickUpThingAtTable (sizeof 0xb4), so its copy constructor is the base copy plus the vtable replacement.
struct CCreatureActionBase {
    virtual void v00();
};

struct CCreatureAction_PickUpThingAtTable : CCreatureActionBase {   // vtable 0x012753E4; sizeof 0xb4
    unsigned char _body[0xb4 - 4];
    CCreatureAction_PickUpThingAtTable(const CCreatureAction_PickUpThingAtTable& other);   // 0x0084C9A0
};

struct CCreatureAction_PickUpJug : CCreatureAction_PickUpThingAtTable {   // vtable 0x012963A4; adds no data
    virtual void v00();
    CCreatureActionBase* Clone();
};

CCreatureActionBase* CCreatureAction_PickUpJug::Clone()
{
    return new CCreatureAction_PickUpJug(*this);
}
