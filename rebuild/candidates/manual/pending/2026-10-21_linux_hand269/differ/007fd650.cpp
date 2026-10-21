// Grade: DIFFER(434v448) -- frame only: retail gives the three line/vector/colour temporaries their own words (sub esp,0xd8, colour bytes stored after the pushes); ours shares them (0x60). Named primitive locals give 0xd0 but put the engine in a register and drop the xor ebx,ebx zero register
// CTCDPhysicalObstruction::FrameUpdate() -- when the thing manager's obstruction debug display is on, draw the obstruction as three blue persistent lines (a 2-unit post at each end and the bar joining their tops); then push creatures off the line and count down the life timer, killing the obstruction thing when it runs out
// Retail 0x007FD650, 448 bytes (plain `ret`); catalog "vfunc_9" (the CTCBase frame-update slot).
// Callees: CTCBase::GetThingManager @ 0x00686AE0 (byte +0x78 = debug display), CEnginePrimitiveLine ctor @ 0x004C3090
// (returns `this`), the engine's SetPersistentPrimitive (vtable slot 23, +0x5c; engine = global 0x013B86A0 -> +0x28 display
// engine -> +0x2c), CTCDPhysicalObstruction::UpdateAdditionalImpulse @ 0x007FD580, CThing::Kill(bool) @ 0x004C9B80.
// Constants: 2.0f @ 0x0122DCDC, 0.05f immediate.
// Layout: Ego_r.pdb CTCDPhysicalObstruction with the retail 12-byte vector (as the landed UpdateAdditionalImpulse
// 0x007FD580): StartPos +0x0c, EndPos +0x18, LifeTimer +0x24, Radius +0x28, ThingsToIgnoreObstruction +0x2c, line1 +0x38,
// line2 +0x40, line3 +0x48.
struct C3DVector {
    float X, Y, Z;
    C3DVector() { }
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) { }
};
struct CRGBColour {
    unsigned char B, G, R, A;
    void Set(unsigned char r, unsigned char g, unsigned char b, unsigned char a) { R = r; G = g; B = b; A = a; }
    CRGBColour(unsigned char r, unsigned char g, unsigned char b, unsigned char a) { Set(r, g, b, a); }
};
struct CEnginePrimitiveHandle { unsigned long m_data[2]; };
struct CEnginePrimitive { };
struct CEnginePrimitiveLine : CEnginePrimitive {
    unsigned char _body[0x3c];
    CEnginePrimitiveLine(unsigned __int64 id, const C3DVector& start, const C3DVector& end, const CRGBColour& colour,
                         float width, long blendMode, unsigned long flags, bool depthTest, long interpolation,
                         long blendOp);                                     // 0x004C3090
};
struct CIEngine {
#define VF(n) virtual void v##n();
    VF(00) VF(01) VF(02) VF(03) VF(04) VF(05) VF(06) VF(07) VF(08) VF(09) VF(10) VF(11) VF(12) VF(13) VF(14) VF(15) VF(16)
    VF(17) VF(18) VF(19) VF(20) VF(21) VF(22)
#undef VF
    virtual void SetPersistentPrimitive(CEnginePrimitiveHandle& handle, const CEnginePrimitive& primitive);   // slot 23 (+0x5c)
};
struct CDisplayEngine {
    unsigned char _pad00[0x2c];
    CIEngine* Engine;                                   // +0x2c
};
struct CEngineGlobals {
    unsigned char _pad00[0x28];
    CDisplayEngine* DisplayEngine;                      // +0x28
};
extern CEngineGlobals* g_pEngineGlobals_013b86a0;       // 0x013B86A0
inline CIEngine* GetEngine() { return g_pEngineGlobals_013b86a0->DisplayEngine->Engine; }
struct CThingManager {
    unsigned char _pad00[0x78];
    bool DebugDisplayObstructions;                      // +0x78 (descriptive)
};
struct CThing {
    void Kill(bool b);                                  // 0x004C9B80
};
struct CTCDPhysicalObstruction {
    virtual void v00();
    CThing* ParentThing;                                // +0x04 (CTCBase)
    bool    ToBeRemoved;                                // +0x08
    bool    IsInGlobalUpdateTCs;                        // +0x09
    C3DVector StartPos;                                 // +0x0c
    C3DVector EndPos;                                   // +0x18
    long    LifeTimer;                                  // +0x24
    float   Radius;                                     // +0x28
    void*   ThingsToIgnoreObstruction[3];               // +0x2c
    CEnginePrimitiveHandle line1;                       // +0x38
    CEnginePrimitiveHandle line2;                       // +0x40
    CEnginePrimitiveHandle line3;                       // +0x48
    CThingManager* GetThingManager();                   // 0x00686AE0 (CTCBase)
    void UpdateAdditionalImpulse();                     // 0x007FD580
    void FrameUpdate();
};

void CTCDPhysicalObstruction::FrameUpdate()
{
    if (GetThingManager()->DebugDisplayObstructions) {
        GetEngine()->SetPersistentPrimitive(line1, CEnginePrimitiveLine(0, StartPos,
            C3DVector(StartPos.X, StartPos.Y, StartPos.Z + 2.0f), CRGBColour(0, 0, 255, 255), 0.05f, 0, 0, true, 0, 0));
        GetEngine()->SetPersistentPrimitive(line2, CEnginePrimitiveLine(0, EndPos,
            C3DVector(EndPos.X, EndPos.Y, EndPos.Z + 2.0f), CRGBColour(0, 0, 255, 255), 0.05f, 0, 0, true, 0, 0));
        GetEngine()->SetPersistentPrimitive(line3, CEnginePrimitiveLine(0,
            C3DVector(StartPos.X, StartPos.Y, StartPos.Z + 2.0f), C3DVector(EndPos.X, EndPos.Y, EndPos.Z + 2.0f),
            CRGBColour(0, 0, 255, 255), 0.05f, 0, 0, true, 0, 0));
    }
    UpdateAdditionalImpulse();
    if (LifeTimer > 0) {
        if (--LifeTimer == 0)
            ParentThing->Kill(true);
    }
}
