// CThingPhysical::CanBlockNavigatingCreature() -- true when the physics TC reports a navigation blocker and ThingTypeFlags lacks bit 3
// Retail: Fable.exe 0x006A4D90, 36 bytes. Callee: CTCPhysicsBase vtable slot 57 (+0xE4, 0x006B11D0 /
// CTCPhysicsStandard 0x00724C90).
// Retail CThing sits 0x10 below the PDB layout from +0x48 on (4-byte STLport list, narrower vector map):
// PhysicsTC is +0x60 (PDB +0x70) and ThingTypeFlags +0x6c (PDB +0x7c). The generated CThingPhysical.h
// mixes PDB and retail offsets in that range, so the class is modelled locally with the PDB names.
// Shape: the three tests share one `xor al,al` exit -- the && chain in an if with an explicit true/false
// return (bool result, `mov al,1`); the TC pointer is a named local so it loads straight into ecx.
struct CTCPhysicsBase {
    virtual void v000(); virtual void v001(); virtual void v002(); virtual void v003(); virtual void v004(); virtual void v005(); virtual void v006(); virtual void v007(); virtual void v008(); virtual void v009(); virtual void v010(); virtual void v011(); virtual void v012(); virtual void v013(); virtual void v014(); virtual void v015(); virtual void v016(); virtual void v017(); virtual void v018(); virtual void v019(); virtual void v020(); virtual void v021(); virtual void v022(); virtual void v023(); virtual void v024(); virtual void v025(); virtual void v026(); virtual void v027(); virtual void v028(); virtual void v029(); virtual void v030(); virtual void v031(); virtual void v032(); virtual void v033(); virtual void v034(); virtual void v035(); virtual void v036(); virtual void v037(); virtual void v038(); virtual void v039(); virtual void v040(); virtual void v041(); virtual void v042(); virtual void v043(); virtual void v044(); virtual void v045(); virtual void v046(); virtual void v047(); virtual void v048(); virtual void v049(); virtual void v050(); virtual void v051(); virtual void v052(); virtual void v053(); virtual void v054(); virtual void v055(); virtual void v056();
    virtual bool IsNavigationBlocker();                  // slot 57 (+0xE4)
};

struct CThingPhysical {
    unsigned char   _pad00[0x60];
    CTCPhysicsBase* PhysicsTC;                           // +0x60
    unsigned char   _pad64[0x8];
    unsigned long   ThingTypeFlags;                      // +0x6c
    bool CanBlockNavigatingCreature();
};

bool CThingPhysical::CanBlockNavigatingCreature()
{
    CTCPhysicsBase* physics = PhysicsTC;
    if (physics && physics->IsNavigationBlocker() && !(ThingTypeFlags & 8))
        return true;
    return false;
}