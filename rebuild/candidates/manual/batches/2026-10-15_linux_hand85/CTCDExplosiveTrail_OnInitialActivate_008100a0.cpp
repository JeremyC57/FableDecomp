// CTCDExplosiveTrail::OnInitialActivate() -- fetch the explosive-trail sub-def from the parent thing's def into PDef
// Leaf: OnInitialActivate
// Retail: Fable.exe (TLC, VC7.1) function @ 0x008100A0, length 44 bytes (+4 int3). CTCDExplosiveTrail vtable 0x01242B4C
// slot 6 (the CTCBase OnInitialActivate slot; manifest label "vfunc_6").
// Callees: CDefClassBase::GetSubDef<CExplosiveTrailDef>(CDefPointer<T>*) const @ 0x00810C80 (manifest label names another
// byte-identical instantiation), and the thing def's virtual slot 1 (`jmp [edx+4]`, the release's destroy as a tail jump).
// Shape: the landed CTCAISummon::OnInitialActivate (0x007FDE70) without the base-hook call: the thing's counted def
// pointer (+0x70) is copied into a local (`test ; inc [esi+4]`, the copy constructor's add-ref) that lives in esi
// through GetSubDef, then its destructor drops the count and destroys on zero.
// Layout: retail offsets -- ComponentTraps is a 4-byte STLport list (+0x1c), ThingsToIgnore a 12-byte vector (+0x20), so
// PDef sits at +0x2c (PDB 56). No engine header for the class, so a local model.
template <class T> struct CDefPointer;
struct CExplosiveTrailDef_Retail;

struct CDefClassBase_Retail
{
    virtual void v00();
    virtual void Destroy();                         // slot 1 (+0x04)
    long m_refCount;                                // +0x04
    bool GetSubDef(CDefPointer<CExplosiveTrailDef_Retail>* out) const;   // GetSubDef<CExplosiveTrailDef> @ 0x00810C80
};

template <class T>
struct CDefPointer
{
    T* Object;
    CDefPointer(const CDefPointer& other) : Object(other.Object) { if (Object) ++Object->m_refCount; }
    ~CDefPointer()
    {
        T* object = Object;
        if (object != 0)
        {
            if (--object->m_refCount == 0)
                object->Destroy();
        }
    }
    T* operator->() const { return Object; }
};

struct CThingDef : CDefClassBase_Retail {};
struct CExplosiveTrailDef_Retail : CDefClassBase_Retail {};

struct CThing
{
    unsigned char          _pad_0x00[0x70];
    CDefPointer<CThingDef> PDef;                    // +0x70
};

struct CTCDExplosiveTrail
{
    void*   __vftable;                              // +0x00 (CTCBase)
    CThing* ParentThing;                            // +0x04
    bool    ToBeRemoved;                            // +0x08
    bool    IsInGlobalUpdateTCs;                    // +0x09
    void*   PTarget[2];                             // +0x0c CIntelligentPointer<CThing const>
    long    Timer;                                  // +0x14
    bool    Terminated;                             // +0x18
    void*   ComponentTraps;                         // +0x1c list<pair<CIntelligentPointer<CThing>,long> > (4 bytes)
    void*   ThingsToIgnore[3];                      // +0x20 vector<CIntelligentPointer<CThing> > (12 bytes)
    CDefPointer<CExplosiveTrailDef_Retail> PDef;    // +0x2c
    void OnInitialActivate();
};

void CTCDExplosiveTrail::OnInitialActivate()
{
    CDefPointer<CThingDef> thingDef(ParentThing->PDef);
    thingDef->GetSubDef(&PDef);
}
