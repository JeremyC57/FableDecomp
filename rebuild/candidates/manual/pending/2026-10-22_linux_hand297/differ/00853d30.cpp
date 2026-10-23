// NCameraModes::CScriptModuleOpenGate::OnInitLeaf() -- set up the gate-opening camera move: start beside the gate (1.5 units along its horizontal facing towards the camera plus 1.5 units sideways away from the camera, 0.75 up) and step DeltaPos so that WFDuration frames later it is twice that offset from the gate
// Grade: DIFFER(627v625) -- the two dot products load toCamera before forward/side (retail `fld fwd.Y ; fmul tc.Y`, ours `fld tc.Y ; fmul fwd.Y` whatever the Dot operand order), forward is declared early so GetForward lands in a temp then copied, and the offset temps take 3 more frame words (frame 0x30 v 0x24: retail reuses toCamera's slots for side*1.5 and offset). Tried: Dot this/v swapped, Dot formula reversed, ctor-built toCamera, two named floats, toCamera reused as offset, forward declared first (fixes the esi/edi split).
// Leaf: OnInitLeaf
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00853D30, length 625 bytes, plain `ret`.
// Callees: CBaseIntelligentPointer::GetPItem 0x00A01B50, CThing::GetPosition 0x004C73D0 (out of line, `[this+60h]+0Ch`),
// the owner's focus-thing getter 0x0084F1B0 (`[this+40h]` + 0x100 -> GetPItem), the gate placement's slot 70 (+0x118,
// C3DVector by hidden pointer), C3DVector::Normalise 0x00A14440 (manifest label "GetScaled" is wrong: no argument, result
// unused). Constants 0.0 0x0122DEDC, -1.0 0x0122DEE0, 1.0 0x0122DED8, 1.5 0x0125AADC, 0.75 0x01238174.
// Layout: PDB NCameraModes::CScriptModuleOpenGate (0x74 bytes): PGate +0x54, Pos +0x5c, DeltaPos +0x68 over
// CScriptModuleSelfTerminateBase (POwner +0x38, WFDuration +0x4c). CThing: the dead flag +0x91 bit 0, placement +0x60.
struct CBaseObject;
struct C3DVector {
    float X, Y, Z;
    C3DVector() { }
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) { }
    C3DVector operator+(const C3DVector& v) const { return C3DVector(X + v.X, Y + v.Y, Z + v.Z); }
    C3DVector operator-(const C3DVector& v) const { return C3DVector(X - v.X, Y - v.Y, Z - v.Z); }
    C3DVector operator*(float s) const { return C3DVector(X * s, Y * s, Z * s); }
    C3DVector& operator*=(float s) { X *= s; Y *= s; Z *= s; return *this; }
    float Dot(const C3DVector& v) const { return X * v.X + Y * v.Y + Z * v.Z; }
    void Normalise();                                // 0x00A14440
};
struct CThingPlacement {
    virtual void p00(); virtual void p01(); virtual void p02(); virtual void p03(); virtual void p04(); virtual void p05(); virtual void p06(); virtual void p07();
    virtual void p08(); virtual void p09(); virtual void p10(); virtual void p11(); virtual void p12(); virtual void p13(); virtual void p14(); virtual void p15();
    virtual void p16(); virtual void p17(); virtual void p18(); virtual void p19(); virtual void p20(); virtual void p21(); virtual void p22(); virtual void p23();
    virtual void p24(); virtual void p25(); virtual void p26(); virtual void p27(); virtual void p28(); virtual void p29(); virtual void p30(); virtual void p31();
    virtual void p32(); virtual void p33(); virtual void p34(); virtual void p35(); virtual void p36(); virtual void p37(); virtual void p38(); virtual void p39();
    virtual void p40(); virtual void p41(); virtual void p42(); virtual void p43(); virtual void p44(); virtual void p45(); virtual void p46(); virtual void p47();
    virtual void p48(); virtual void p49(); virtual void p50(); virtual void p51(); virtual void p52(); virtual void p53(); virtual void p54(); virtual void p55();
    virtual void p56(); virtual void p57(); virtual void p58(); virtual void p59(); virtual void p60(); virtual void p61(); virtual void p62(); virtual void p63();
    virtual void p64(); virtual void p65(); virtual void p66(); virtual void p67(); virtual void p68(); virtual void p69();
    virtual C3DVector GetForward() const;            // slot 70 (+0x118)
};
struct CThing {
    unsigned char    _pad00[0x60];
    CThingPlacement* Placement;                      // +0x60
    unsigned char    _pad64[0x91 - 0x64];
    unsigned char    Dead : 1;                       // +0x91 bit 0
    const C3DVector& GetPosition() const;            // 0x004C73D0
};
struct CCameraState {
    const CThing* GetFocusThing() const;             // 0x0084F1B0
};
struct CBaseIntelligentPointer {
    virtual void v00();
    void* m_pItem;
    CBaseObject* GetPItem() const;                   // 0x00A01B50
};
struct CIntelligentPointer_CThingConst : CBaseIntelligentPointer {   // vtable 0x01237F1C
    const CThing* operator->() const { return (const CThing*)GetPItem(); }
    bool IsValid() const { return GetPItem() != 0 && !((const CThing*)GetPItem())->Dead; }
};

namespace NCameraModes {

struct CScriptModuleOpenGate {
    virtual void v00();
    unsigned char                   _pad04[0x34];
    CCameraState*                   POwner;          // +0x38
    unsigned char                   _pad3c[0x10];
    long                            WFDuration;      // +0x4c
    unsigned char                   _pad50[0x4];
    CIntelligentPointer_CThingConst PGate;           // +0x54
    C3DVector                       Pos;             // +0x5c
    C3DVector                       DeltaPos;        // +0x68
    void OnInitLeaf();
};

void CScriptModuleOpenGate::OnInitLeaf()
{
    if (!PGate.IsValid())
        return;
    CCameraState* owner = POwner;
    const C3DVector& gatePos = PGate->GetPosition();
    C3DVector forward;
    C3DVector toCamera = owner->GetFocusThing()->GetPosition() - gatePos;
    toCamera.Z = 0.0f;
    forward = PGate->Placement->GetForward();
    forward.Z = 0.0f;
    forward.Normalise();
    if (forward.Dot(toCamera) < 0.0f)
        forward *= -1.0f;
    C3DVector side(forward.Y, -forward.X, 0.0f);
    if (side.Dot(toCamera) > 0.0f)
        side *= -1.0f;
    C3DVector offset = forward * 1.5f + side * 1.5f;
    offset.Z += 0.75f;
    Pos = offset + PGate->GetPosition();
    DeltaPos = (offset * 2.0f + PGate->GetPosition() - Pos) * (1.0f / (float)WFDuration);
}

}
