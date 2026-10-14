#pragma optimize("s", on)
// CDefClassBase::CDefClassBase(const CDefClassBase&) -- copy constructor: CResource copy, own vtable, manager / global index / three flag bits copied
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0046644C, length 78 bytes. Size-optimised TU (the /Os region).
// Callees: CResource::CResource(const CResource&) @ 0x0046649A. Vtable 0x01230BCC = CDefClassBase (rtti-vtables.tsv).
// PDB CDefClassBase (0x25 bytes): CResource base (0x1c), +0x1c PDefManager, +0x20 GlobalIndex, +0x24 uchar:1 Setup,
// Template, DefaultValsApplied. The engine header flattens the base and the bit group, so the class is modelled
// locally with the real names. Shape: each 1-bit member initialiser is the xor/and/xor read-modify-write of the
// flag byte (the first through memory, the next two through al/cl).
struct CDefinitionManager;

struct CResource {
    virtual void v00();
    unsigned char _body[0x18];                     // +0x04 .. +0x1c: list links, size, last-used frame
    CResource(const CResource& other);             // 0x0046649A
};

struct CDefClassBase : CResource {                 // vtable 0x01230BCC
    virtual void v00();
    CDefinitionManager* PDefManager;               // +0x1c
    unsigned long       GlobalIndex;               // +0x20
    unsigned char       Setup : 1;                 // +0x24 bit 0
    unsigned char       Template : 1;              // +0x24 bit 1
    unsigned char       DefaultValsApplied : 1;    // +0x24 bit 2
    CDefClassBase(const CDefClassBase& other);
};

CDefClassBase::CDefClassBase(const CDefClassBase& other)
    : CResource(other), PDefManager(other.PDefManager), GlobalIndex(other.GlobalIndex),
      Setup(other.Setup), Template(other.Template), DefaultValsApplied(other.DefaultValsApplied)
{
}
