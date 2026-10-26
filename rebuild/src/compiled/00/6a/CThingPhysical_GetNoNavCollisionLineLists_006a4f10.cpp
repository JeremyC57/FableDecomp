// CThingPhysical::GetNoNavCollisionLineLists() -- the no-nav collision line lists of the thing's physics mesh, or 0 without a physics TC / mesh
// Retail: Fable.exe 0x006A4F10, 69 bytes.
// Callees: CTCPhysicsBase vtable slot 93 (+0x174, mesh parameters, base 0x006B13B0 returns 0),
// CTCPhysicsBase::GetPhysicsMeshObject 0x006B0850 (returns a 0x40-byte CPhysicsMeshObject by hidden
// pointer), CPhysicsMeshObject::GetNoNavCollisionLineLists 0x00A127C0, ~CPhysicsMeshObject 0x00A127D0.
// PhysicsTC is CThing +0x60 in retail (PDB +0x70); modelled locally (see 0x006A4D90).
// Shape: the TC pointer in a named local (esi), the mesh object a by-value temporary in the 0x40-byte
// frame whose out-of-line destructor runs after the chained call; the result rides in esi across it.
class CPhysicsMeshObject { public:
    void*         m_pMesh;                               // +0x04 counted mesh
    unsigned char _pad08[0x38];
    virtual ~CPhysicsMeshObject();                               // 0x00A127D0
    void* GetNoNavCollisionLineLists() const;            // 0x00A127C0
};

class CTCPhysicsBase { public:
    virtual void v000(); virtual void v001(); virtual void v002(); virtual void v003(); virtual void v004(); virtual void v005(); virtual void v006(); virtual void v007(); virtual void v008(); virtual void v009(); virtual void v010(); virtual void v011(); virtual void v012(); virtual void v013(); virtual void v014(); virtual void v015(); virtual void v016(); virtual void v017(); virtual void v018(); virtual void v019(); virtual void v020(); virtual void v021(); virtual void v022(); virtual void v023(); virtual void v024(); virtual void v025(); virtual void v026(); virtual void v027(); virtual void v028(); virtual void v029(); virtual void v030(); virtual void v031(); virtual void v032(); virtual void v033(); virtual void v034(); virtual void v035(); virtual void v036(); virtual void v037(); virtual void v038(); virtual void v039(); virtual void v040(); virtual void v041(); virtual void v042(); virtual void v043(); virtual void v044(); virtual void v045(); virtual void v046(); virtual void v047(); virtual void v048(); virtual void v049(); virtual void v050(); virtual void v051(); virtual void v052(); virtual void v053(); virtual void v054(); virtual void v055(); virtual void v056(); virtual void v057(); virtual void v058(); virtual void v059(); virtual void v060(); virtual void v061(); virtual void v062(); virtual void v063(); virtual void v064(); virtual void v065(); virtual void v066(); virtual void v067(); virtual void v068(); virtual void v069(); virtual void v070(); virtual void v071(); virtual void v072(); virtual void v073(); virtual void v074(); virtual void v075(); virtual void v076(); virtual void v077(); virtual void v078(); virtual void v079(); virtual void v080(); virtual void v081(); virtual void v082(); virtual void v083(); virtual void v084(); virtual void v085(); virtual void v086(); virtual void v087(); virtual void v088(); virtual void v089(); virtual void v090(); virtual void v091(); virtual void v092();
    virtual void* GetPhysicsMeshParameters();            // slot 93 (+0x174)
public:
    CPhysicsMeshObject GetPhysicsMeshObject() const;   // 0x006B0850
};

class CThingPhysical { public:
    unsigned char   _pad00[0x60];
    CTCPhysicsBase* PhysicsTC;                           // +0x60
    void* GetNoNavCollisionLineLists();
};

void* CThingPhysical::GetNoNavCollisionLineLists()
{
    CTCPhysicsBase* physics = PhysicsTC;
    if (physics && physics->GetPhysicsMeshParameters())
        return physics->GetPhysicsMeshObject().GetNoNavCollisionLineLists();
    return 0;
}