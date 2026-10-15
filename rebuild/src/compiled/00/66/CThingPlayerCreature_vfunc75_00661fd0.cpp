// CThingPlayerCreature::vfunc_75() -- bit 6 of the creature def's flag byte at +0x148
// Retail: Fable.exe 0x00661FD0, 18 bytes, no callees.
// The member at +0xe0 is CThingCreatureBase::PDef (CDefPointer<CThingCreatureDef>, Ego_r.pdb +0xfc; the
// retail creature sits 0x1c lower there: CThing -0x10, CThingPhysical list -8, a 12-byte vector -4).
// Shape: `mov al ; shr al,6 ; and al,1` = a one-bit unsigned char bitfield read through the def pointer's
// inline operator-> (no AddRef: the member is used in place).
struct CThingCreatureDef {
    unsigned char _pad00[0x148];
    unsigned char _flags148_0 : 6;
    unsigned char Flag148Bit6 : 1;                       // +0x148 bit 6
};

template <class T> struct CDefPointer {
    T* m_pDef;
    T* operator->() const { return m_pDef; }
};

struct CThingPlayerCreature {
    unsigned char                   _pad00[0xe0];
    CDefPointer<CThingCreatureDef>  PDef;                // +0xe0
    unsigned char vfunc_75();
};

unsigned char CThingPlayerCreature::vfunc_75()
{
    return PDef->Flag148Bit6;
}