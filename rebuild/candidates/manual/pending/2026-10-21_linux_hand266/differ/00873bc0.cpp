// Grade: DIFFER(478v481) -- register allocation only: retail keeps this in edi and &AxisZ in ebx (the cross product then loads AxisX's operand first), the lane swaps them (this in ebx, AxisZ in edi) and the fmul operand order follows
// CPCMouseLook::Recentre() -- start a camera recentre behind the player: rebuild the look axes from the player's facing (X = -facing, Z = up, Y = X x Z), take the signed XY angle between the old rotated X axis and the new one, and spread it over the def's recentre time in frames
// Leaf: Recentre
// Retail: Fable.exe (TLC, VC7.1) @ 0x00873BC0, 0x1E1 bytes (padded to 496), `ret`.
// Callees: the camera base's player accessor 0x0084F1B0 (this+0x40 -> +0x100 intelligent pointer GetPItem), the player
//   body's (+0x60) virtual slot 70 (+0x118, facing direction by hidden pointer), C3DVector::Rotate(const C3DVector& axis,
//   float angle) 0x00A13C60, _CIacos 0x00BFEBA2, __ftol2 0x00BFEA70. Globals: frames per second 0x01375550; -1.0f
//   0x0122DEE0, 1.0f 0x0122DED8, 0.0f 0x0122DEDC, 1/(2*pi) 0x01237F08.
// Layout: Ego_r.pdb CPCMouseLook (312) with the retail base 0x24 smaller and the STLport list 4 bytes: MouseInputs +0xa4,
//   AxisX +0xa8, AxisY +0xb4, AxisZ +0xc0, StateCurrent +0xcc {AngleXY, AngleUpDown, DollyLength, DeltaAngleXY +0xd8,
//   RecentreTimer +0xdc}; the mouse-look def at +0x44 holds the recentre time (seconds) at +0xc4.
extern "C" double __cdecl acos(double);
#pragma intrinsic(acos)
struct C3DVector {
    float X, Y, Z;
    C3DVector() { }
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) { }
    C3DVector(const C3DVector& o) : X(o.X), Y(o.Y), Z(o.Z) { }
    void Rotate(const C3DVector& axis, float angle);         // 0x00A13C60
    C3DVector operator*(float s) const { return C3DVector(X * s, Y * s, Z * s); }
    float DotProduct(const C3DVector& v) const { return X * v.X + Y * v.Y + Z * v.Z; }
};
inline C3DVector CrossProduct(const C3DVector& a, const C3DVector& b)
{
    return C3DVector(b.Z * a.Y - a.Z * b.Y, a.Z * b.X - b.Z * a.X, a.X * b.Y - b.X * a.Y);
}
struct CPhysicsBody {
    virtual void v000(); virtual void v001(); virtual void v002(); virtual void v003(); virtual void v004(); virtual void v005(); virtual void v006(); virtual void v007();
    virtual void v008(); virtual void v009(); virtual void v010(); virtual void v011(); virtual void v012(); virtual void v013(); virtual void v014(); virtual void v015();
    virtual void v016(); virtual void v017(); virtual void v018(); virtual void v019(); virtual void v020(); virtual void v021(); virtual void v022(); virtual void v023();
    virtual void v024(); virtual void v025(); virtual void v026(); virtual void v027(); virtual void v028(); virtual void v029(); virtual void v030(); virtual void v031();
    virtual void v032(); virtual void v033(); virtual void v034(); virtual void v035(); virtual void v036(); virtual void v037(); virtual void v038(); virtual void v039();
    virtual void v040(); virtual void v041(); virtual void v042(); virtual void v043(); virtual void v044(); virtual void v045(); virtual void v046(); virtual void v047();
    virtual void v048(); virtual void v049(); virtual void v050(); virtual void v051(); virtual void v052(); virtual void v053(); virtual void v054(); virtual void v055();
    virtual void v056(); virtual void v057(); virtual void v058(); virtual void v059(); virtual void v060(); virtual void v061(); virtual void v062(); virtual void v063();
    virtual void v064(); virtual void v065(); virtual void v066(); virtual void v067(); virtual void v068(); virtual void v069();
    virtual C3DVector GetFacingDirection() const;   // slot 70 (+0x118)
};
struct CThing {
    unsigned char _pad00[0x60];
    CPhysicsBody* Body;                                      // +0x60
    CPhysicsBody* GetBody() const { return Body; }
};
struct CMouseLookDef {
    unsigned char _pad00[0xc4];
    float         RecentreTime;                              // +0xc4
};
struct CRandomPacketOfVariablesDefiningTheCameraState {
    float     AngleXY;                                       // +0x00
    float     AngleUpDown;
    float     DollyLength;
    float     DeltaAngleXY;                                  // +0x0c
    long      RecentreTimer;                                 // +0x10
    C3DVector ViewPosOffset;
};
extern long g_framesPerSecond;                               // 0x01375550
struct CCameraModeBase {
    unsigned char  _pad00[0x44];
    CMouseLookDef* Def;                                      // +0x44
    unsigned char  _pad48[0xa4 - 0x48];
    CThing* GetPlayerCharacter();                            // 0x0084F1B0
};
struct CPCMouseLook : CCameraModeBase {
    void*     MouseInputs;                                   // +0xa4
    C3DVector AxisX;                                         // +0xa8
    C3DVector AxisY;                                         // +0xb4
    C3DVector AxisZ;                                         // +0xc0
    CRandomPacketOfVariablesDefiningTheCameraState StateCurrent;   // +0xcc
    void Recentre();
};
void CPCMouseLook::Recentre()
{
    C3DVector facing = GetPlayerCharacter()->GetBody()->GetFacingDirection();
    C3DVector oldX = AxisX;
    oldX.Rotate(AxisZ, StateCurrent.AngleXY);
    AxisZ = C3DVector(0.0f, 0.0f, 1.0f);
    AxisX = facing * -1.0f;
    AxisY = CrossProduct(AxisX, AxisZ);
    float dot = oldX.DotProduct(AxisX);
    if (dot < -1.0f)
        dot = -1.0f;
    else if (dot > 1.0f)
        dot = 1.0f;
    StateCurrent.AngleXY = (float)acos(dot) * 0.15915493667125702f;
    if (oldX.X * AxisX.Y - oldX.Y * AxisX.X < 0.0f)
        StateCurrent.AngleXY = StateCurrent.AngleXY * -1.0f;
    long frames = (long)((float)g_framesPerSecond * Def->RecentreTime);
    StateCurrent.RecentreTimer = frames;
    StateCurrent.DeltaAngleXY = StateCurrent.AngleXY * -1.0f / frames;
    StateCurrent.RecentreTimer = frames - 1;
}
