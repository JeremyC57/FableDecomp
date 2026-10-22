// NCreatureMode::CModeJackOfBladesScream::FrameUpdate() -- while the sword is alive and falling, for the first 30 frames interpolate its orientation from InitialSwordRHS towards (forward 0,1,0 / up 0,0,-1) and its position from InitialSwordPos towards SwordFallPos by timer/30, then advance the timer
// Leaf: FrameUpdate
// Retail: Fable.exe (TLC, VC7.1) @ 0x008D77A0, 316 bytes (+ int3 padding).
// Callees: CBaseIntelligentPointer::GetPItem 0x00A01B50, CRightHandedSet::Interpolate 0x00A23190 (result by hidden pointer),
//   the sword thing's placement component (+0x60) slots 30 SetPosition (+0x78) and 75 SetRHSet (+0x12c).
// Layout = Ego_r.pdb CModeJackOfBladesScream (retail offsets equal the PDB's): PSword +0x74, InitialSwordRHS +0x7c,
//   SwordFalling +0x94, SwordFallTimer +0x98, PEmitter +0x9c, SwordFallPos +0xa4, InitialSwordPos +0xb0.
// Shape: the target set is built from six immediates (a float ctor; two C3DVector temporaries copy through the frame); the
//   interpolated set and the position are NAMED locals (GetPItem runs after them); the placement pointer comes through an
//   inline getter, which loads it straight into ecx (a direct member access loads eax and copies); the position is
//   `(SwordFallPos - InitialSwordPos) * t + InitialSwordPos` through inline C3DVector operators.
#define V4(n) virtual void n##0(); virtual void n##1(); virtual void n##2(); virtual void n##3();
#define V16(n) V4(n##a) V4(n##b) V4(n##c) V4(n##d)
struct C3DVector {
    float X, Y, Z;
    C3DVector() { }
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) { }
    C3DVector operator-(const C3DVector& o) const { return C3DVector(X - o.X, Y - o.Y, Z - o.Z); }
    C3DVector operator+(const C3DVector& o) const { return C3DVector(X + o.X, Y + o.Y, Z + o.Z); }
    C3DVector operator*(float f) const { return C3DVector(X * f, Y * f, Z * f); }
};
struct CRightHandedSet {
    C3DVector Forward;
    C3DVector Up;
    CRightHandedSet() { }
    CRightHandedSet(float fx, float fy, float fz, float ux, float uy, float uz) : Forward(fx, fy, fz), Up(ux, uy, uz) { }
    CRightHandedSet Interpolate(const CRightHandedSet& to, float t) const;   // 0x00A23190
};
struct CThingPlacement {                           // the thing's placement component (+0x60)
    V16(a) virtual void b0(); virtual void b1(); virtual void b2(); virtual void b3(); virtual void b4(); virtual void b5(); virtual void b6(); virtual void b7();
    virtual void b8(); virtual void b9(); virtual void b10(); virtual void b11(); virtual void b12(); virtual void b13();
    virtual void SetPosition(const C3DVector& pos);       // slot 30 (+0x78)
    V16(c) V16(d) virtual void e0(); virtual void e1(); virtual void e2(); virtual void e3(); virtual void e4(); virtual void e5(); virtual void e6(); virtual void e7(); virtual void e8(); virtual void e9(); virtual void e10(); virtual void e11();
    virtual void SetRHSet(const CRightHandedSet& rhs);    // slot 75 (+0x12c)
};
struct CThing {
    unsigned char    _pad00[0x60];
    CThingPlacement* m_pPlacement;                 // +0x60
    CThingPlacement* GetPlacement() const { return m_pPlacement; }
    unsigned char    _pad64[0x91 - 0x64];
    unsigned char    m_flags91;                    // +0x91 bit 0: dead
};
struct CBaseIntelligentPointer {
    void*   __vftable;
    CThing* m_pItem;
    CThing* GetPItem() const;                      // 0x00A01B50
};
inline bool IS_THING_ALIVE(const CBaseIntelligentPointer& p)
{
    if (p.GetPItem() != 0) {
        if (!(p.GetPItem()->m_flags91 & 1))
            return true;
    }
    return false;
}
struct CModeJackOfBladesScream {
    unsigned char           _pad00[0x74];
    CBaseIntelligentPointer PSword;                // +0x74
    CRightHandedSet         InitialSwordRHS;       // +0x7c
    bool                    SwordFalling;          // +0x94
    long                    SwordFallTimer;        // +0x98
    CBaseIntelligentPointer PEmitter;              // +0x9c
    C3DVector               SwordFallPos;          // +0xa4
    C3DVector               InitialSwordPos;       // +0xb0
    void FrameUpdate();
};

void CModeJackOfBladesScream::FrameUpdate()
{
    if (IS_THING_ALIVE(PSword) && SwordFalling) {
        CRightHandedSet target(0.0f, 1.0f, 0.0f, 0.0f, 0.0f, -1.0f);
        if (SwordFallTimer <= 30) {
            float t = SwordFallTimer * (1.0f / 30.0f);
            CRightHandedSet rhs = InitialSwordRHS.Interpolate(target, t);
            PSword.GetPItem()->GetPlacement()->SetRHSet(rhs);
            C3DVector pos = (SwordFallPos - InitialSwordPos) * t + InitialSwordPos;
            PSword.GetPItem()->GetPlacement()->SetPosition(pos);
            ++SwordFallTimer;
        }
    }
}