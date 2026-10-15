// NUISystem::CZoomingComponent::GetDataFromDef() -- fetch the component's UI def, copy its InputDelay and ScrollingSpeed, clear the Panning / Zooming flags, then release the def
// Leaf: ?GetDataFromDef@CZoomingComponent
// Retail: Fable.exe @ 0x00546AA0, 84 bytes (+ int3 padding). Called by the landed CZoomingComponent constructor 0x00546B00.
// Callees: CComponent::LoadAndReturnDef() const (primary vtable slot 108, +0x1B0; CDefPointer<const CUIDef> returned
// through the hidden pointer = the `push ecx` frame slot), the def's virtual slot 1 (destroy) on the last release.
// Label: the manifest's "CTextContainer::GetDataFromDef" is propagated; the landed CZoomingComponent constructor
// (0x00546B00) calls this body, and its stores hit CZoomingComponent's InputDelay / ScrollingSpeed / Panning / Zooming.
// Shape: the two floats copied as dwords in member order, the two flags cleared from one zero register (`xor edx,edx`),
// which the inline release then reuses for its zero compare (`cmp esi,edx`).
// Retail layouts: CZoomingComponent = Ego_r.pdb minus 0x30 (Panning +0x178, Zooming +0x179, InputDelay +0x17C,
// ScrollingSpeed +0x180, as in the landed constructor); CUIDef minus 0x1C (ScrollingSpeed PDB +0xDC -> +0xC0,
// InputDelay PDB +0x1A0 -> +0x184). Modelled locally with the real member names.
struct CRefCounted {                               // second base of CDefBase: the count at +4 behind the vptr
    long m_count;
    void DecRef() { --m_count; }
    long GetRefCount() const { return m_count; }
};
struct CDefVirtual {
    virtual void v00();
    virtual void Destroy();                        // slot 1
};
struct C2DBoxF {
    float TLX, TLY, BRX, BRY;
};
struct CUIDef : CDefVirtual, CRefCounted {
    unsigned char _pad_0x08[0xc0 - 0x08];
    float ScrollingSpeed;                          // +0xc0
    unsigned char _pad_0xc4[0x184 - 0xc4];
    float InputDelay;                              // +0x184
    void ReleaseTemporary() { DecRef(); if (GetRefCount() == 0) Destroy(); }
};
struct CDefPointer_CUIDef {                        // CDefPointer<const NUISystem::CUIDef>
    const CUIDef* Data;
    ~CDefPointer_CUIDef() { if (Data) const_cast<CUIDef*>(Data)->ReleaseTemporary(); }
    const CUIDef* operator->() const { return Data; }
};
namespace NUISystem {
struct CComponentInterface {
    virtual void v000(); virtual void v001(); virtual void v002(); virtual void v003(); virtual void v004();
    virtual void v005(); virtual void v006(); virtual void v007(); virtual void v008(); virtual void v009();
    virtual void v010(); virtual void v011(); virtual void v012(); virtual void v013(); virtual void v014();
    virtual void v015(); virtual void v016(); virtual void v017(); virtual void v018(); virtual void v019();
    virtual void v020(); virtual void v021(); virtual void v022(); virtual void v023(); virtual void v024();
    virtual void v025(); virtual void v026(); virtual void v027(); virtual void v028(); virtual void v029();
    virtual void v030(); virtual void v031(); virtual void v032(); virtual void v033(); virtual void v034();
    virtual void v035(); virtual void v036(); virtual void v037(); virtual void v038(); virtual void v039();
    virtual void v040(); virtual void v041(); virtual void v042(); virtual void v043(); virtual void v044();
    virtual void v045(); virtual void v046(); virtual void v047(); virtual void v048(); virtual void v049();
    virtual void v050(); virtual void v051(); virtual void v052(); virtual void v053(); virtual void v054();
    virtual void v055(); virtual void v056(); virtual void v057(); virtual void v058(); virtual void v059();
    virtual void v060(); virtual void v061(); virtual void v062(); virtual void v063(); virtual void v064();
    virtual void v065(); virtual void v066(); virtual void v067(); virtual void v068(); virtual void v069();
    virtual void v070(); virtual void v071(); virtual void v072(); virtual void v073(); virtual void v074();
    virtual void v075(); virtual void v076(); virtual void v077(); virtual void v078(); virtual void v079();
    virtual void v080(); virtual void v081(); virtual void v082(); virtual void v083(); virtual void v084();
    virtual void v085(); virtual void v086(); virtual void v087(); virtual void v088(); virtual void v089();
    virtual void v090(); virtual void v091(); virtual void v092(); virtual void v093(); virtual void v094();
    virtual void v095(); virtual void v096(); virtual void v097(); virtual void v098(); virtual void v099();
    virtual void v100(); virtual void v101(); virtual void v102(); virtual void v103(); virtual void v104();
    virtual void v105(); virtual void v106(); virtual void v107();
    virtual CDefPointer_CUIDef LoadAndReturnDef() const;   // slot 108 (+0x1B0)
};
struct C2DVector { float X, Y; };
struct CZoomingComponent : CComponentInterface {
    unsigned char _pad_0x04[0x15c - 0x04];
    float     LastInputTime;                       // +0x15C
    C2DVector ZoomCenter;                          // +0x160
    C2DVector MinZoom;                             // +0x168
    C2DVector MaxZoom;                             // +0x170
    bool      Panning;                             // +0x178
    bool      Zooming;                             // +0x179
    float     InputDelay;                          // +0x17C
    float     ScrollingSpeed;                      // +0x180
    void GetDataFromDef();
};
void CZoomingComponent::GetDataFromDef()
{
    CDefPointer_CUIDef def = LoadAndReturnDef();
    InputDelay = def->InputDelay;
    ScrollingSpeed = def->ScrollingSpeed;
    Panning = false;
    Zooming = false;
}
}
