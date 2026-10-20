// CTCDivineWrath::OnCreate() -- raise a physical barrier around the caster (its physics radius, at its position); a live barrier ignores the caster and is kept as the spell's obstruction
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0078F4D0, 193 bytes (plain `ret`).
// Callees: CThing::GetPosition @ 0x004C73D0, CreatePhysicalBarrierThing(const C3DVector&, const C3DVector&, float) @ 0x007FD4B0
// (`__fastcall`; landed name), CBaseIntelligentPointer ctor @ 0x00A01B10 / SetPItem @ 0x00A01B90 / GetPItem @ 0x00A01B50 / dtor
// @ 0x00A01C10 (CIntelligentPointer<CThing> vtable 0x01238C6C), vector<CIntelligentPointer<CThing>>::push_back @ 0x004AD520 (out of
// line; manifest "Vector_PushBack_IntelligentPointer").
// Layout: retail CThing PhysicsTC +0x60, PTCDriver +0x5c, dead bit +0x91; the barrier's driver is a CTCDPhysicalObstruction
// (interface 0xed, bit 13 of the +0x3c word) with ThingsToIgnoreObstruction at +0x2c (Ego_r.pdb); the physics TC's radius
// +0x34 (named descriptively); Ego_r.pdb CTCDivineWrath PObstruction +0xcc sits at retail +0xc0 (two 12-byte containers and
// the 4-byte-lower packed pointer before it).
// Shape: the radius is read through an inline GetPhysics() accessor (ecx reused as the base, GOTCHAS 24k); the barrier's driver is a named local (read before the argument temporary is built) from an inline accessor
// returning an unset local when the thing has no 0xed driver; the
// caster is wrapped in a CIntelligentPointer temporary (the thing read before the base ctor) for the out-of-line push_back;
// the obstruction pointer is set through a second temporary and the inline operator= (`SetPItem(other.GetPItem())`).
struct C3DVector { float X, Y, Z; };
struct CTCPhysicsBase {
    unsigned char _pad00[0x34];
    float Radius;                                       // +0x34
};
struct CTCDPhysicalObstruction;
struct CThing {
    unsigned char   _pad00[0x20];
    unsigned long   m_presentBits[9];                   // +0x20 interface-present bitset
    unsigned char   _pad44[0x5c - 0x44];
    CTCDPhysicalObstruction* PTCDriver;                 // +0x5c
    CTCPhysicsBase* PhysicsTC;                          // +0x60
    unsigned char   _pad64[0x91 - 0x64];
    unsigned char   Flags91;                            // +0x91 bit 0 Dead
    __forceinline unsigned long HasTC(long type) const { return m_presentBits[type >> 5] & (1UL << (type & 31)); }
    __forceinline CTCDPhysicalObstruction* GetObstructionDriver()
    {
        CTCDPhysicalObstruction* driver;                // deliberately unset when absent (retail reads a dead slot)
        if (HasTC(0xed))
            driver = PTCDriver;
        return driver;
    }
    const C3DVector& GetPosition() const;               // 0x004C73D0
};
CThing* __fastcall CreatePhysicalBarrierThing(const C3DVector& start, const C3DVector& end, float radius);   // 0x007FD4B0
struct CBaseIntelligentPointer {
    virtual void v00();
    CThing* m_pItem;                                    // +0x04
    CBaseIntelligentPointer();                          // 0x00A01B10
    ~CBaseIntelligentPointer();                         // 0x00A01C10
    CThing* GetPItem() const;                           // 0x00A01B50
    void SetPItem(CThing* item);                        // 0x00A01B90
};
struct CIntelligentPointer : CBaseIntelligentPointer {  // CIntelligentPointer<CThing>, vtable 0x01238C6C
    virtual void v00();
    CIntelligentPointer(CThing* item) : CBaseIntelligentPointer() { SetPItem(item); }
    CIntelligentPointer& operator=(const CIntelligentPointer& other) { SetPItem(other.GetPItem()); return *this; }
};
struct CIntelligentPointerVector {                            // STLport vector<CIntelligentPointer<CThing>>
    CIntelligentPointer* _M_start;
    CIntelligentPointer* _M_finish;
    CIntelligentPointer* _M_end_of_storage;
    void push_back(const CIntelligentPointer& value);   // 0x004AD520
};
struct CTCDPhysicalObstruction {
    unsigned char _pad00[0x2c];
    CIntelligentPointerVector ThingsToIgnoreObstruction;      // +0x2c
};
struct CTCDivineWrath {
    virtual void v00();
    CThing* ParentThing;                                // +0x04 (CTCBase)
    unsigned char _pad08[0xc0 - 0x08];
    CIntelligentPointer PObstruction;                   // +0xc0 (PDB +0xcc)
    CTCPhysicsBase* GetPhysics() const { return ParentThing->PhysicsTC; }
    void OnCreate();
};

void CTCDivineWrath::OnCreate()
{
    const C3DVector& position = ParentThing->GetPosition();
    CThing* barrier = CreatePhysicalBarrierThing(position, position, GetPhysics()->Radius);
    if (barrier && !(barrier->Flags91 & 1)) {
        CTCDPhysicalObstruction* driver = barrier->GetObstructionDriver();
        driver->ThingsToIgnoreObstruction.push_back(CIntelligentPointer(ParentThing));
        PObstruction = CIntelligentPointer(barrier);
    }
}
