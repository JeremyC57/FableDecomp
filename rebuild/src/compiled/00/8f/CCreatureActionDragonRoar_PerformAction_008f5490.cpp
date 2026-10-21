// CCreatureAction_DragonRoar::PerformAction() -- finds the dragon's mouth dummy, drops it to ground height, and raises a short-lived physical barrier two units ahead along the facing; the dragon is registered with the barrier's driver
// Leaf: ?PerformAction@CCreatureAction_DragonRoar@@UAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x008F5490, 321 bytes (336 with padding).
// Callees: CBaseIntelligentPointer ctor/GetPItem/SetPItem/dtor 0x00A01B10/B50/B90/C10 (CIntelligentPointer<CThing>
//   vtable 0x01238C6C), CThing::GetPosition 0x004C73D0, CTCGraphicAppearance::GetDummyObject 0x004C08C0 (the
//   "DRAGON_MOUTH_DUMMY" literal 0x01285B4C; CCharString ctor 0x0099EBF0 / dtor 0x0099EAE0), the physics virtual
//   GetFacing (slot 70, +0x118, C3DVector by hidden pointer), the barrier factory (__fastcall start, end, radius)
//   0x007FD4B0, the driver's lifetime setter 0x007FD3D0 (stores (long)(g_rate * seconds) at +0x24; its catalog label
//   CAIStateGroupBuyFromSeller::BoughtSomething is a byte-identical propagated name), vector<CIntelligentPointer<CThing>>
//   ::push_back 0x004AD520.
// Shape: the landed CTCUnholyPower::OnCreate model (barrier factory, cached driver accessor -- here initialised to 0,
//   retail's xor edi,edi -- and the push_back of a CIntelligentPointer temporary); the mouth-dummy name is a scoped
//   named string; __forceinline GetGraphicAppearance()/GetPhysics() accessors keep the component in ecx; the facing
//   offset is added in place (operator+=) and C3DVector needs its user copy ctor or the Y add keeps t.Y on the x87 stack.
// Layout: local model -- retail CThing PTCDriver +0x5C, PhysicsTC +0x60, PTCGraphicAppearance +0x64 (PDB +0x6C/+0x70/
//   +0x74, as in the landed CTCUnholyPower::OnCreate and CActionEventExplosion::Dispatch); the action's PCreature +0x08.
struct CTCBase;
struct CBaseObject;
struct CThing;
struct CCharString {
    void* PStringData;
    CCharString(const char* text, long length = -1);    // 0x0099EBF0
    ~CCharString();                             // 0x0099EAE0
};
struct CBaseIntelligentPointer {
    virtual void v00();
    CThing* m_pItem;                            // +0x04
    CBaseIntelligentPointer();                  // 0x00A01B10
    ~CBaseIntelligentPointer();                 // 0x00A01C10
    CThing* GetPItem() const;                   // 0x00A01B50
    void SetPItem(CBaseObject* item);           // 0x00A01B90
};
template<class T> struct CIntelligentPointer : CBaseIntelligentPointer {   // vtable 0x01238C6C
    virtual void v00();
    CIntelligentPointer(T* item) : CBaseIntelligentPointer() { SetPItem((CBaseObject*)item); }
};
template <class T> struct SRetailAllocProxy { T* _M_data; };
template <class T> struct CRetailVector {
    T* _M_start;
    T* _M_finish;
    SRetailAllocProxy<T> _M_end_of_storage;
    void push_back(const T& value);             // 0x004AD520
};
struct C3DVector {
    float X, Y, Z;
    C3DVector() {}
    C3DVector(const C3DVector& v) : X(v.X), Y(v.Y), Z(v.Z) {}
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) {}
    C3DVector operator+(const C3DVector& o) const { return C3DVector(X + o.X, Y + o.Y, Z + o.Z); }
    C3DVector& operator+=(const C3DVector& o) { X += o.X; Y += o.Y; Z += o.Z; return *this; }
    C3DVector operator*(float s) const { return C3DVector(X * s, Y * s, Z * s); }
};
struct CRightHandedSet { C3DVector Up, Forward; };
struct CTCDriverBase {
    unsigned char _pad00[0x2c];
    CRetailVector< CIntelligentPointer<CThing> > Passengers;   // +0x2c
    void SetLifetime(float seconds);            // 0x007FD3D0
};
struct CTCGraphicAppearance {
    bool GetDummyObject(const CCharString& name, long zero1, unsigned long zero2, C3DVector* position, CRightHandedSet* orientation, float scale); // 0x004C08C0
};
struct CTCPhysicsBase {
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04(); virtual void v05(); virtual void v06(); virtual void v07(); virtual void v08(); virtual void v09();
    virtual void v10(); virtual void v11(); virtual void v12(); virtual void v13(); virtual void v14(); virtual void v15(); virtual void v16(); virtual void v17(); virtual void v18(); virtual void v19();
    virtual void v20(); virtual void v21(); virtual void v22(); virtual void v23(); virtual void v24(); virtual void v25(); virtual void v26(); virtual void v27(); virtual void v28(); virtual void v29();
    virtual void v30(); virtual void v31(); virtual void v32(); virtual void v33(); virtual void v34(); virtual void v35(); virtual void v36(); virtual void v37(); virtual void v38(); virtual void v39();
    virtual void v40(); virtual void v41(); virtual void v42(); virtual void v43(); virtual void v44(); virtual void v45(); virtual void v46(); virtual void v47(); virtual void v48(); virtual void v49();
    virtual void v50(); virtual void v51(); virtual void v52(); virtual void v53(); virtual void v54(); virtual void v55(); virtual void v56(); virtual void v57(); virtual void v58(); virtual void v59();
    virtual void v60(); virtual void v61(); virtual void v62(); virtual void v63(); virtual void v64(); virtual void v65(); virtual void v66(); virtual void v67(); virtual void v68(); virtual void v69();
    virtual C3DVector GetFacing();              // slot 70 (+0x118)
};
struct CThing {
    unsigned char _pad00[0x20];
    unsigned long m_presentBits[9];             // +0x20
    unsigned char _pad44[0x5c - 0x44];
    CTCDriverBase* PTCDriver;                   // +0x5c
    CTCPhysicsBase* PhysicsTC;                  // +0x60
    CTCGraphicAppearance* PTCGraphicAppearance; // +0x64
    const C3DVector& GetPosition() const;       // 0x004C73D0
    __forceinline CTCPhysicsBase* GetPhysics() const { return PhysicsTC; }
    __forceinline CTCGraphicAppearance* GetGraphicAppearance() const { return PTCGraphicAppearance; }
    CTCDriverBase* GetDriver() const
    {
        CTCDriverBase* driver = 0;
        if (m_presentBits[0xed >> 5] & (1UL << (0xed & 31)))
            driver = PTCDriver;
        return driver;
    }
};
CThing* __fastcall CreatePhysicalBarrierThing(const C3DVector& start, const C3DVector& end, float radius);   // 0x007FD4B0

struct CCreatureAction_DragonRoar {
    virtual void v00();
    long _pad04;
    CBaseIntelligentPointer PCreature;          // +0x08
    virtual void PerformAction();
};

void CCreatureAction_DragonRoar::PerformAction()
{
    C3DVector position = PCreature.GetPItem()->GetPosition();
    CRightHandedSet orientation;
    {
        CCharString name("DRAGON_MOUTH_DUMMY");
        PCreature.GetPItem()->GetGraphicAppearance()->GetDummyObject(name, 0, 0, &position, &orientation, 1.0f);
    }
    position.Z = PCreature.GetPItem()->GetPosition().Z;
    position += PCreature.GetPItem()->GetPhysics()->GetFacing() * 2.0f;
    CThing* barrier = CreatePhysicalBarrierThing(position, PCreature.GetPItem()->GetPosition(), 2.0f);
    if (barrier) {
        CTCDriverBase* driver = barrier->GetDriver();
        driver->SetLifetime(3.0f);
        driver->Passengers.push_back(CIntelligentPointer<CThing>(PCreature.GetPItem()));
    }
}