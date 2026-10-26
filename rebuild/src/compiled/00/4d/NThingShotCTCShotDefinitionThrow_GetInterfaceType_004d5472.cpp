// NThingShot::CTCShotDefinitionThrow::GetInterfaceType() const -- the component's interface type: 119 (0x77)
// Leaf: ?GetInterfaceType@CTCShotDefinitionThrow@NThingShot@@UBE?AW4ETCInterfaceType@@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x004D5472, 4 bytes (`push 77h; pop eax; ret`).
// Identity (sweep3 GetInterfaceType family; was "NThingShot::CTCShotDefinitionThrow::vfunc_21"): slot 21 of the NThingShot::CTCShotDefinitionThrow vtable 0x0123CDC4, directly before
//   GetClassTypeName (slot 22, named in 327 TC vtables): CTCBase declares GetInterfaceType then GetClassTypeName (Ego_r
//   0x007B0610 / 0x007B0620 for CTCAssassinRush); every one of the 244 slot-21 bodies returns a per-class constant
//   (CTCBoss = 0xAD agrees with the landed HasTC(0xad) / GetTC<CTCBoss>(0xad) callers). NThingShot::CTCShotDefinitionThrow is not in the Ego_r/FableWin name sets (a class without a PDB symbol here), so the name rests on the slot and the body alone.
// The ETCInterfaceType enumerator names are not in the name sets, so the value is written as a cast.
#pragma optimize("s", on)                                    // the TU is size-optimised (push/pop for the constant), as landed
enum ETCInterfaceType { };

namespace NThingShot {
class CTCShotDefinitionThrow {
public:
    virtual ETCInterfaceType GetInterfaceType() const;
};
}

ETCInterfaceType NThingShot::CTCShotDefinitionThrow::GetInterfaceType() const
{
    return (ETCInterfaceType)0x77;
}
