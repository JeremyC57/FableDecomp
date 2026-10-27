// CThingFilter_SolidToThing::CThingFilter_SolidToThing(const CThing& thing) -- store the argument; the second word starts at its constant
// Leaf: ??0CThingFilter_SolidToThing@@QAE@ABVCThing@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00661620, 18 bytes, thiscall, `ret 4`, returns this.
// Identity (proven): Ego_r.pdb ??0CThingFilter_SolidToThing@@QAE@ABVCThing@@@Z, an 8-byte class {CThing* SourceThing, CTCSolidToThing* PTCSolidToThing}; its caller is CTCShotDefinitionBase::UpdateCollision 0x007BFE30. The manifest's "CThingFilter_SolidToThing::CThingFilter_SolidToThing" is propagated across
//   byte-identical bodies.
// Shape (family M182, representative 0x00661620): member initialisers in declaration order. Speed TU.
class CThing;
class CTCSolidToThing;
class CThingFilter_SolidToThing {
public:
    CThingFilter_SolidToThing(const CThing& thing);
    CThing* SourceThing;                                  // +0x00
    CTCSolidToThing* PTCSolidToThing;                     // +0x04
};
CThingFilter_SolidToThing::CThingFilter_SolidToThing(const CThing& thing)
    : SourceThing((CThing*)&thing), PTCSolidToThing(0)
{
}