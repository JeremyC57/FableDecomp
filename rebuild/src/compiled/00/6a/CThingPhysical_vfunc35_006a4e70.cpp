// CThingPhysical::vfunc_35(SPhysicsState*) -- forwards the state record to the physics TC's slot-88 filler and reports success
// Retail: Fable.exe 0x006A4E70, 21 bytes (CThingPhysical vtable 0x01244E84 slot 35).
// Callee: CTCPhysicsBase vtable slot 88 (+0x160, 0x006AFBF0), which fills flag bytes of the record.
// PhysicsTC is CThing +0x60 in retail (PDB +0x70); modelled locally (see 0x006A4D90).
// Shape: a void virtual call on the member pointer, then a constant bool true (`mov al,1 ; ret 4`).
struct SPhysicsState;

struct CTCPhysicsBase {
    virtual void v000(); virtual void v001(); virtual void v002(); virtual void v003(); virtual void v004(); virtual void v005(); virtual void v006(); virtual void v007(); virtual void v008(); virtual void v009(); virtual void v010(); virtual void v011(); virtual void v012(); virtual void v013(); virtual void v014(); virtual void v015(); virtual void v016(); virtual void v017(); virtual void v018(); virtual void v019(); virtual void v020(); virtual void v021(); virtual void v022(); virtual void v023(); virtual void v024(); virtual void v025(); virtual void v026(); virtual void v027(); virtual void v028(); virtual void v029(); virtual void v030(); virtual void v031(); virtual void v032(); virtual void v033(); virtual void v034(); virtual void v035(); virtual void v036(); virtual void v037(); virtual void v038(); virtual void v039(); virtual void v040(); virtual void v041(); virtual void v042(); virtual void v043(); virtual void v044(); virtual void v045(); virtual void v046(); virtual void v047(); virtual void v048(); virtual void v049(); virtual void v050(); virtual void v051(); virtual void v052(); virtual void v053(); virtual void v054(); virtual void v055(); virtual void v056(); virtual void v057(); virtual void v058(); virtual void v059(); virtual void v060(); virtual void v061(); virtual void v062(); virtual void v063(); virtual void v064(); virtual void v065(); virtual void v066(); virtual void v067(); virtual void v068(); virtual void v069(); virtual void v070(); virtual void v071(); virtual void v072(); virtual void v073(); virtual void v074(); virtual void v075(); virtual void v076(); virtual void v077(); virtual void v078(); virtual void v079(); virtual void v080(); virtual void v081(); virtual void v082(); virtual void v083(); virtual void v084(); virtual void v085(); virtual void v086(); virtual void v087();
    virtual void GetPhysicsState(SPhysicsState* state);  // slot 88 (+0x160)
};

struct CThingPhysical {
    unsigned char   _pad00[0x60];
    CTCPhysicsBase* PhysicsTC;                           // +0x60
    bool vfunc_35(SPhysicsState* state);
};

bool CThingPhysical::vfunc_35(SPhysicsState* state)
{
    PhysicsTC->GetPhysicsState(state);
    return true;
}