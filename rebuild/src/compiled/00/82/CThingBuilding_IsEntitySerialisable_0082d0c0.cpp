// CThingBuilding::IsEntitySerialisable() const -- a building is serialised unless an owner thing created it
// Leaf: ?IsEntitySerialisable@CThingBuilding@@UBE_NXZ
// Retail: Fable.exe @ 0x0082D0C0, 16 bytes, thiscall plain `ret`, bool in al.
// Identity (proven): vtable slot 22 (+0x58) of CThingBuilding 0x0124509C. The classes that override slot 22 in
// rtti-vtables.tsv (CThing, CThingVillage, CThingHolySite, CThingSwitch, CThingMarker, CThingPhysicalSwitch,
// CThingObject, CThingBuilding, CThingAICreature, CThingDeadCreature, CThingTrackNode) are exactly the eleven classes
// Ego_r.pdb / FableWin.pdb give ?IsEntitySerialisable@<Class>@@UBE_NXZ; no other PDB virtual has that class set.
// Label correction: catalog "CThingBuilding::vfunc_22".
// Callees: none.
// Layout: CThing's second flag byte. Ego_r.pdb CThing packs Dead, CreatedByOwnerThingCreation, IncludeInThingScans ...
// into the byte at PDB +161; retail CThing sits 0x10 lower there (STLport CVectorMap 0x10 and list 4 bytes instead of
// Dinkumware 0x14 / 0xC), so the byte is +0x91 and CreatedByOwnerThingCreation is bit 1 (the CThing setter 0x004C7880
// writes the next byte, +0x92, the same way). No engine header for CThing / CThingBuilding: local partial view.
// Shape: `xor eax,eax ; mov al,[ecx+91h] ; shr eax,1 ; not eax ; and eax,1` is `!bitfield` over an unsigned char bitfield.
class CThing {
public:
    virtual ~CThing();
    unsigned char _pad004[0x91 - 4];
    unsigned char Dead : 1;                                 // +0x91 bit 0
    unsigned char CreatedByOwnerThingCreation : 1;          // +0x91 bit 1
    unsigned char IncludeInThingScans : 1;
    unsigned char Activated : 1;
    unsigned char GloballyLoaded : 1;
    unsigned char InLimbo : 1;
    unsigned char KillLocked : 1;
    unsigned char CreatedBySerialisation : 1;
    virtual bool IsEntitySerialisable() const;
};

class CThingBuilding : public CThing {
public:
    virtual bool IsEntitySerialisable() const;
};

bool CThingBuilding::IsEntitySerialisable() const
{
    return !CreatedByOwnerThingCreation;
}