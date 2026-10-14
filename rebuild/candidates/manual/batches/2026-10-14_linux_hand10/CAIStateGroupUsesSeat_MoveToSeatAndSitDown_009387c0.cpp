// CAIStateGroup_UsesSeat::MoveToSeatAndSitDown(EMovementType) -- walk to the seat, then sit
// Retail: Fable.exe (TLC, VC7.1) function @ 0x009387C0, length 43 bytes (+ int3 padding).
// Callees: CAIStateGroup_UsesSeat::MoveToSeat(EMovementType) @ 0x009385F0,
// CAIStateGroup_UsesSeat::SitInSeat() @ 0x00938380.
// Shape: `MoveToSeat(type) && SitInSeat()` -- the int-typed `&&` of two bool calls materialises as
// `mov eax,1` / shared `xor eax,eax` (full register); the movement type is a plain 4-byte value
// re-pushed from the argument slot (`ret 4`). The base is modelled locally.
typedef long EMovementType;

struct CAIStateGroupBase {
    virtual ~CAIStateGroupBase();
    unsigned char _pad[0x10];                   // PBrain, ScriptAIPriority, Name, flags (+0x4..+0x13)
};

struct CAIStateGroup_UsesSeat : CAIStateGroupBase {
    bool MoveToSeat(EMovementType movementType);    // 0x009385F0
    bool SitInSeat();                               // 0x00938380
    bool MoveToSeatAndSitDown(EMovementType movementType);
};

bool CAIStateGroup_UsesSeat::MoveToSeatAndSitDown(EMovementType movementType)
{
    return MoveToSeat(movementType) && SitInSeat();
}
