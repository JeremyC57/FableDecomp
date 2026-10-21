// CCreatureAction_DragonSwoop::FrameUpdate() -- build a cardinal spline through the three SwoopPoints, place the dragon at
// Grade: DIFFER(507v493) -- orientation build: retail keeps the three up = side x forward components on the x87 stack (forward copied into the set first, the up Z through a reused temp slot) where ours stores each component through a temp; spline/push_back/offset part matches
// the action's progress along it, face it along the normalised offset from its old position (up = side x forward with the
// side vector (f.Y, -f.X, 0)), then the base FrameUpdate
// Leaf: ?FrameUpdate@CCreatureAction_DragonSwoop
extern "C" void __cdecl free(void* block);           // 0x00BFEA14
inline void* operator new(unsigned int, void* where) { return where; }
struct __true_type { __true_type() {} };
struct C3DVector {
    float X, Y, Z;
    C3DVector() {}
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) {}
    void Normalise();                                 // 0x00A14440
};
inline C3DVector operator-(const C3DVector& a, const C3DVector& b)
{
    C3DVector result;
    result.X = a.X - b.X;
    result.Y = a.Y - b.Y;
    result.Z = a.Z - b.Z;
    return result;
}
inline C3DVector CrossProduct(const C3DVector& a, const C3DVector& b)
{
    return C3DVector(a.Y * b.Z - a.Z * b.Y, a.Z * b.X - a.X * b.Z, a.X * b.Y - a.Y * b.X);
}
struct CRightHandedSet {
    C3DVector Up;
    C3DVector Forward;
    CRightHandedSet(const C3DVector& up, const C3DVector& forward) : Up(up), Forward(forward) {}
};
struct C3DVectorArray {                               // STLport vector<C3DVector> (12 bytes)
    typedef unsigned int size_type;
    C3DVector* _M_start;
    C3DVector* _M_finish;
    C3DVector* _M_end_of_storage;
    C3DVectorArray() : _M_start(0), _M_finish(0), _M_end_of_storage(0) {}
    ~C3DVectorArray() { if (_M_start) free(_M_start); }
    void _M_insert_overflow(C3DVector* pos, const C3DVector& x, const __true_type&, size_type fill_len, bool atend);   // 0x0066E370
    void push_back(const C3DVector& x)
    {
        if (_M_finish != _M_end_of_storage) {
            new (_M_finish) C3DVector(x);
            ++_M_finish;
        } else
            _M_insert_overflow(_M_finish, x, __true_type(), 1, true);
    }
};
struct CCardinalSpline {                              // retail 0x14 (PDB 0x18 with the 16-byte CArray)
    C3DVectorArray ControlPoints;                     // +0
    float          Tension;                           // +0xc
    bool           EndPointsConnected;                // +0x10
    CCardinalSpline() : Tension(0.5f), EndPointsConnected(false) {}
    void GetSplinePosition(C3DVector* position, float progress);   // 0x00A567A0
};
struct CPhysicsBody {                                 // the thing's physics body at +0x60
#define VF(n) virtual void v##n();
#define VF10(p) VF(p##0) VF(p##1) VF(p##2) VF(p##3) VF(p##4) VF(p##5) VF(p##6) VF(p##7) VF(p##8) VF(p##9)
    VF10(0) VF10(1) VF10(2) VF(30)
    virtual void SetPosition(const C3DVector& position);   // slot 31 (+0x7c)
    VF(32) VF(33) VF(34) VF(35) VF(36) VF(37) VF(38) VF(39) VF10(4) VF10(5) VF10(6) VF(70) VF(71) VF(72) VF(73) VF(74)
    virtual void SetOrientation(const CRightHandedSet& set);   // slot 75 (+0x12c)
};
struct CThing {
    unsigned char _pad00[0x60];
    CPhysicsBody* PhysicsBody;                        // +0x60
    CPhysicsBody* GetPhysics() const { return PhysicsBody; }
    const C3DVector& GetPosition() const;             // 0x004C73D0
};
struct CBaseIntelligentPointer {
    void* __vftable;
    CThing* m_pItem;
    CThing* GetPItem() const;                         // 0x00A01B50
};
struct CCombatActionBase {                            // retail sizeof 0x114
    virtual void v00();
    unsigned char _pad04[4];
    CBaseIntelligentPointer PCreature;                // +0x08
    unsigned char _pad10[0x114 - 0x10];
    float GetProgress();                              // 0x006925E0
    void FrameUpdate();                               // 0x00859D80
};
struct CCreatureAction_DragonSwoop : CCombatActionBase {
    C3DVector SwoopPoints[3];                         // +0x114 (PDB +0x12C)
    void FrameUpdate();
};

void CCreatureAction_DragonSwoop::FrameUpdate()
{
    CCardinalSpline spline;
    spline.ControlPoints.push_back(SwoopPoints[0]);
    spline.ControlPoints.push_back(SwoopPoints[1]);
    spline.ControlPoints.push_back(SwoopPoints[2]);
    spline.Tension = 0.5f;
    C3DVector position;
    spline.GetSplinePosition(&position, GetProgress());
    C3DVector forward = position - PCreature.GetPItem()->GetPosition();
    forward.Normalise();
    PCreature.GetPItem()->GetPhysics()->SetPosition(position);
    PCreature.GetPItem()->GetPhysics()->SetOrientation(CRightHandedSet(CrossProduct(C3DVector(forward.Y, -forward.X, 0.0f), forward), forward));
    CCombatActionBase::FrameUpdate();
}
