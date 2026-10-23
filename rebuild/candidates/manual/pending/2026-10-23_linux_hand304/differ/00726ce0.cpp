// Grade: DIFFER(1002v1011) -- 227 difflines: frame 0x40 and the snap-block compare (`<=` jnp needs !(a > b)) match; residue is register choice: ours pushes edi in the prologue (FastInvSqrt uses ebx/edi, retail edx/ecx with edi pushed only inside the snap block), the flags byte lands in cl/dl (retail al), and the oldPos copy order (retail Z,X,Y through eax/ecx/edx) differs. Element-wise oldPos init kills the lea edi CSE of &Position; reusing oldPos as the later pos matches retail slot 0x20.
// CTCPhysicsNavigator::Move() -- navigating: snap to PositionToNavigateTo (interacting with the things near it when the camera point is close) when the additional impulse is negligible; a non-physics forced move goes straight to ForcedMoveToPos; otherwise clamp the additional impulse, add the forced-move delta, run force updates, gather the things in the swept area, and do the impassability / forced / step-based movement before setting the new position
// Leaf: Move
// Retail: Fable.exe (TLC, VC7.1) @ 0x00726CE0, 1012 bytes, `ret`.
// Layout: local model -- PDB CTCPhysicsNavigator (0xb0, pack 1): Navigating +0x88, PositionToNavigateTo +0x89,
//   AdditionalImpulse +0x95, ForcedMoveToPos +0xa1, ForcedMoveToPosSet:1 / ForcedMoveToPosUsePhysics:1 at +0xad.
extern "C" double __cdecl sqrt(double);
#pragma intrinsic(sqrt)
extern "C" void __cdecl free(void*);
extern unsigned long g_InvSqrtMantissaTable[128];   // 0x013CE558
extern const float g_MinImpulse;                    // 0x0129BA3C
extern const float g_SnapRange;                     // 0x01381194
extern const float g_MaxImpulse;                    // 0x012305A0
extern bool g_DisableMoveElements;                  // 0x013B86EC
__forceinline float FastInvSqrt(float lengthSquared)
{
    unsigned long bits = *(unsigned long*)&lengthSquared;
    unsigned long estimate = (((0x17c - ((bits >> 23) & 0xff)) >> 1) << 23) |
                             (g_InvSqrtMantissaTable[(bits >> 17) & 0x7f] << 15);
    float r = *(float*)&estimate;
    return (3.0f - r * r * lengthSquared) * r * 0.5f;
}
struct C3DVector {
    float X, Y, Z;
    C3DVector() { }
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) { }
    float GetLengthSquared() const { return X * X + Y * Y + Z * Z; }
};
struct C2DBoxI { long x0, y0, x1, y1; };
struct CThing;
template <class T> struct vector {
    T* _M_start; T* _M_finish; T* _M_end_of_storage;
    vector() : _M_start(0), _M_finish(0), _M_end_of_storage(0) { }
    ~vector() { if (_M_start) free(_M_start); }
};
struct CWorld { virtual void v00(); virtual void v04(); virtual void v08(); virtual void v0c(); virtual void v10();
    virtual void v14(); virtual void v18(); virtual void v1c(); virtual void GetFocusPosition(C3DVector* out); };
#pragma pack(push, 1)
struct CTCPhysicsNavigator {
    virtual void v00();
    virtual void v004();
    virtual void v008();
    virtual void v00c();
    virtual void v010();
    virtual void v014();
    virtual void v018();
    virtual void v01c();
    virtual void v020();
    virtual void v024();
    virtual void v028();
    virtual void v02c();
    virtual void v030();
    virtual void v034();
    virtual void v038();
    virtual void v03c();
    virtual void v040();
    virtual void v044();
    virtual void v048();
    virtual void v04c();
    virtual void v050();
    virtual void v054();
    virtual void v058();
    virtual void v05c();
    virtual void v060();
    virtual void v064();
    virtual void v068();
    virtual void v06c();
    virtual void v070();
    virtual void v074();
    virtual void SetPositionDirect(const C3DVector& pos);   // vtable +0x78
    virtual void v07c();
    virtual void v080();
    virtual void v084();
    virtual void v088();
    virtual void v08c();
    virtual void v090();
    virtual void v094();
    virtual void v098();
    virtual void v09c();
    virtual void v0a0();
    virtual bool IsNavigating();                          // vtable +0xa4
    virtual void v0a8();
    virtual void v0ac();
    virtual void v0b0();
    virtual void v0b4();
    virtual void v0b8();
    virtual void v0bc();
    virtual void v0c0();
    virtual void v0c4();
    virtual void v0c8();
    virtual void v0cc();
    virtual void v0d0();
    virtual void v0d4();
    virtual void v0d8();
    virtual void v0dc();
    virtual void v0e0();
    virtual void v0e4();
    virtual void v0e8();
    virtual void v0ec();
    virtual void v0f0();
    virtual void v0f4();
    virtual void v0f8();
    virtual void v0fc();
    virtual void v100();
    virtual void v104();
    virtual void v108();
    virtual void v10c();
    virtual void v110();
    virtual void v114();
    virtual void v118();
    virtual void v11c();
    virtual void v120();
    virtual void v124();
    virtual void v128();
    virtual void v12c();
    virtual void v130();
    virtual void v134();
    virtual void v138();
    virtual void v13c();
    virtual void v140();
    virtual void v144();
    virtual void v148();
    virtual void v14c();
    virtual void v150();
    virtual void v154();
    virtual void v158();
    virtual void v15c();
    virtual void v160();
    virtual void v164();
    virtual void v168();
    virtual void v16c();
    virtual void v170();
    virtual void v174();
    virtual void v178();
    virtual void v17c();
    virtual void v180();
    virtual void UpdateInteractions(vector<CThing*>* list, const C3DVector* pos);  // vtable +0x184
    virtual void GetInteractionArea(C2DBoxI* out);          // vtable +0x188
    unsigned char _pad04[0x0c - 0x04];
    C3DVector Position;                              // +0x0c
    unsigned char _pad18[0x28 - 0x18];
    C3DVector Velocity;                              // +0x28
    unsigned char _pad34[0x88 - 0x34];
    bool      Navigating;                            // +0x88
    C3DVector PositionToNavigateTo;                  // +0x89
    C3DVector AdditionalImpulse;                     // +0x95
    C3DVector ForcedMoveToPos;                       // +0xa1
    bool ForcedMoveToPosSet : 1;                     // +0xad
    bool ForcedMoveToPosUsePhysics : 1;
    unsigned char _padae[2];

    void Move();
    CWorld* GetWorld();                                                   // 0x00686A70
    void GetPossibleInteractedWithList(C2DBoxI* box, vector<CThing*>* list, float range);  // 0x008E57D0
    void SetPosition(const C3DVector& pos);                               // 0x006B0C10
    void UpdateAdditionalImpulse();                                       // 0x00726BE0
    void PerformForceUpdates();                                           // 0x008E5670
    C2DBoxI GetPossibleCollidedWithArea(const C3DVector& delta);          // 0x007267C0
    void Position_MoveElements(vector<CThing*>* list);                    // 0x00724240
    void UpdateVelocityForImpassability(C3DVector* pos, C3DVector* delta);  // 0x007243C0
    void PerformForcedMovement(vector<CThing*>* list, C3DVector* delta, C3DVector* pos);  // 0x00726980
    void PerformStepBasedMovement(vector<CThing*>* list, C3DVector* pos);   // 0x008E5880
};
#pragma pack(pop)

void CTCPhysicsNavigator::Move()
{
    if (IsNavigating() && !ForcedMoveToPosSet && !(AdditionalImpulse.GetLengthSquared() > g_MinImpulse * g_MinImpulse)) {
        C3DVector focus;
        GetWorld()->GetFocusPosition(&focus);
        const C3DVector& target = PositionToNavigateTo;
        float dx = focus.X - target.X;
        float dy = focus.Y - target.Y;
        if (dx * dx + dy * dy < g_SnapRange * g_SnapRange) {
            C2DBoxI area;
            GetInteractionArea(&area);
            vector<CThing*> things;
            GetPossibleInteractedWithList(&area, &things, 0.0f);
            UpdateInteractions(&things, &target);
        }
        SetPosition(target);
        UpdateAdditionalImpulse();
        return;
    }
    if (ForcedMoveToPosSet && !ForcedMoveToPosUsePhysics) {
        SetPositionDirect(ForcedMoveToPos);
        ForcedMoveToPosSet = false;
        UpdateAdditionalImpulse();
        return;
    }
    {
    C3DVector oldPos(Position.X, Position.Y, Position.Z);
    if ((float)sqrt(AdditionalImpulse.GetLengthSquared()) > g_MaxImpulse) {
        float r = FastInvSqrt(AdditionalImpulse.Z * AdditionalImpulse.Z + AdditionalImpulse.Y * AdditionalImpulse.Y + AdditionalImpulse.X * AdditionalImpulse.X);
        AdditionalImpulse.X *= r; AdditionalImpulse.Y *= r; AdditionalImpulse.Z *= r;
        AdditionalImpulse.X *= g_MaxImpulse; AdditionalImpulse.Y *= g_MaxImpulse; AdditionalImpulse.Z *= g_MaxImpulse;
    }
    C3DVector delta = AdditionalImpulse;
    if (ForcedMoveToPosSet) {
        ForcedMoveToPosSet = false;
        delta.X += ForcedMoveToPos.X - oldPos.X;
        delta.Y += ForcedMoveToPos.Y - oldPos.Y;
        delta.Z += ForcedMoveToPos.Z - oldPos.Z;
    }
    PerformForceUpdates();
    C2DBoxI area = GetPossibleCollidedWithArea(delta);
    vector<CThing*> things;
    GetPossibleInteractedWithList(&area, &things, (float)sqrt(delta.Z * delta.Z + delta.Y * delta.Y + delta.X * delta.X));
    if (!g_DisableMoveElements) Position_MoveElements(&things);
    oldPos = Position;
    UpdateVelocityForImpassability(&oldPos, &delta);
    PerformForcedMovement(&things, &delta, &oldPos);
    UpdateVelocityForImpassability(&oldPos, &Velocity);
    PerformStepBasedMovement(&things, &oldPos);
    UpdateInteractions(&things, &oldPos);
    SetPositionDirect(oldPos);
    UpdateAdditionalImpulse();
    }
}
