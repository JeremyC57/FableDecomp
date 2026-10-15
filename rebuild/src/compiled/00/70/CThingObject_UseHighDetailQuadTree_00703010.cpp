// CThingObject::UseHighDetailQuadTree() -- the def's UseHighDetailQuadTree flag (bit 3 of the CThingObjectDef flag byte)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00703010, length 18 bytes (vtable 0x01264DCC slot 57).
// No callees. Layout: PDef +0xd0; CThingObjectDef bitfield byte at +0x98 (PDB +0x9c: IsFragile,
// IsClothing, IsSelfIlluminating, UseHighDetailQuadTree, Electrocutable = bits 0..4).
// The catalog label "vfunc_57" is the slot; the name is the PDB bitfield the slot reads.
// Shape: a one-bit `unsigned char` bitfield read (`mov al ; shr al,3 ; and al,1`), returned as is.
struct CThingObjectDef {
    unsigned char _pad00[0x98];
    unsigned char IsFragile : 1;                   // +0x98 bit 0
    unsigned char IsClothing : 1;                  //        bit 1
    unsigned char IsSelfIlluminating : 1;          //        bit 2
    unsigned char UseHighDetailQuadTree : 1;       //        bit 3
    unsigned char Electrocutable : 1;              //        bit 4
};
struct CDefPointerThingObjectDef {
    const CThingObjectDef* m_pDef;
    const CThingObjectDef* operator->() const { return m_pDef; }
};
struct CThingObject {
    virtual void v00();
    unsigned char             _pad04[0xcc];
    CDefPointerThingObjectDef PDef;                // +0xd0
    virtual unsigned char UseHighDetailQuadTree() const;
};

unsigned char CThingObject::UseHighDetailQuadTree() const
{
    return PDef->UseHighDetailQuadTree;
}