// CAIStateGroup_MoveGoodsBase::CAIStateGroup_MoveGoodsBase() -- base state group, no pick-up action yet, not dropping.
// Retail: Fable.exe (TLC, VC7.1) function @ 0x008FDA5D, length 26 bytes.
// Callees: CAIStateGroupBase::CAIStateGroupBase() @ 0x008FD255. Vtable 0x012898DC (rtti-vtables:
// CAIStateGroup_MoveGoodsBase).
// Shape: PickUpGoodsActionID (+0x18) = -1 is stored BEFORE the vtable store, so it is an init-list member (the
// vtable store sinks below init-list stores); DroppingGoods (+0x14) = false after it is a body assignment.
// Member names and offsets from the engine header. Pragma: `#pragma optimize("s", on)` (the `or [mem],-1` store
// is the size-optimised form of the -1 initialiser).
#pragma optimize("s", on)
struct CAIStateGroupBase {
    virtual void v00();                         // (a non-virtual destructor keeps the deleting destructor out of the TU)
    unsigned char _pad[0x10];                   // +0x4..+0x13
    CAIStateGroupBase();                        // 0x008FD255
};

struct CAIStateGroup_MoveGoodsBase : CAIStateGroupBase {   // vtable 0x012898DC
    bool DroppingGoods;                         // +0x14
    long PickUpGoodsActionID;                   // +0x18
    CAIStateGroup_MoveGoodsBase();
};

CAIStateGroup_MoveGoodsBase::CAIStateGroup_MoveGoodsBase()
    : CAIStateGroupBase()
    , PickUpGoodsActionID(-1)
{
    DroppingGoods = false;
}
