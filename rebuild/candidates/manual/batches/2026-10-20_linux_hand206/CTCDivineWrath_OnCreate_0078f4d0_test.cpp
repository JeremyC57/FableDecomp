// Behaviour test for CTCDivineWrath::OnCreate (0x0078F4D0): a barrier is created at the caster's position with the physics
// radius; a live barrier's obstruction driver ignores the caster and the spell keeps the barrier; a dead/null one is ignored.
#include <cstdio>
struct C3DVector { float X, Y, Z; };
struct CTCPhysicsBase { float Radius; };
struct CTCDPhysicalObstruction;
struct CThing {
    unsigned long   m_presentBits[9];
    CTCDPhysicalObstruction* PTCDriver;
    CTCPhysicsBase* PhysicsTC;
    unsigned char   Flags91;
    C3DVector       Position;
    unsigned long HasTC(long type) const { return m_presentBits[type >> 5] & (1UL << (type & 31)); }
    CTCDPhysicalObstruction* GetObstructionDriver()
    {
        CTCDPhysicalObstruction* driver = 0;            // unset in retail when absent; the test keeps it present
        if (HasTC(0xed))
            driver = PTCDriver;
        return driver;
    }
    const C3DVector& GetPosition() const { return Position; }
};
static CThing* g_barrier = 0; static const C3DVector* g_start = 0; static const C3DVector* g_end = 0; static float g_radius = 0;
CThing* CreatePhysicalBarrierThing(const C3DVector& start, const C3DVector& end, float radius) { g_start = &start; g_end = &end; g_radius = radius; return g_barrier; }
static int g_temps = 0;
struct CBaseIntelligentPointer {
    virtual void v00() { }
    CThing* m_pItem;
    CBaseIntelligentPointer() : m_pItem(0) { ++g_temps; }
    ~CBaseIntelligentPointer() { }
    CThing* GetPItem() const { return m_pItem; }
    void SetPItem(CThing* item) { m_pItem = item; }
};
struct CIntelligentPointer : CBaseIntelligentPointer {
    virtual void v00() { }
    CIntelligentPointer(CThing* item) : CBaseIntelligentPointer() { SetPItem(item); }
    CIntelligentPointer& operator=(const CIntelligentPointer& other) { SetPItem(other.GetPItem()); return *this; }
};
struct CIntelligentPointerVector {
    CThing* Pushed[4]; int Count;
    void push_back(const CIntelligentPointer& value) { Pushed[Count++] = value.GetPItem(); }
};
struct CTCDPhysicalObstruction { CIntelligentPointerVector ThingsToIgnoreObstruction; };
struct CTCDivineWrath {
    virtual void v00() { }
    CThing* ParentThing;
    CIntelligentPointer PObstruction;
    CTCDivineWrath() : PObstruction(0) { }
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

int main()
{
    int bad = 0;
    CTCPhysicsBase phys; phys.Radius = 2.5f;
    CThing caster; caster.PhysicsTC = &phys; caster.Position.X = 1; caster.Position.Y = 2; caster.Position.Z = 3;
    CTCDPhysicalObstruction drv; drv.ThingsToIgnoreObstruction.Count = 0;
    CThing barrier; for (int i = 0; i < 9; ++i) barrier.m_presentBits[i] = 0; barrier.m_presentBits[7] = 1UL << 13;
    barrier.PTCDriver = &drv; barrier.Flags91 = 0;
    CTCDivineWrath w; w.ParentThing = &caster;
    w.OnCreate();                                    // no barrier created
    if (w.PObstruction.GetPItem() || g_start != &caster.Position || g_end != &caster.Position || g_radius != 2.5f) bad |= 1;
    g_barrier = &barrier;
    w.OnCreate();
    if (w.PObstruction.GetPItem() != &barrier || drv.ThingsToIgnoreObstruction.Count != 1 || drv.ThingsToIgnoreObstruction.Pushed[0] != &caster) bad |= 2;
    barrier.Flags91 = 1; w.PObstruction.SetPItem(0);
    w.OnCreate();                                    // dead barrier
    if (w.PObstruction.GetPItem() || drv.ThingsToIgnoreObstruction.Count != 1) bad |= 4;
    if (bad) { std::printf("0078f4d0_TEST FAIL\n"); return 1; }
    std::printf("0078f4d0_TEST PASS\n");
    return 0;
}
