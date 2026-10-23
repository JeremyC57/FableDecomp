// CAIStateGroup_NymphFloatAround::Update() -- reset the nymph's attack flags and target, start flying around (or, back from limbo, play the teleport action first), poll CheckIfCanAttack every frame until ready, then stop flying and, if a target lies more than 1 unit away in the plane, face it; always Terminate()
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0091C3D0, length 624 bytes (+ int3 padding), plain `ret` (virtual).
// Callees: CVectorMap::LowerBound @ 0x0040F020 (GetInterface 0xF5 = CTCNymph, bit 21 of +0x3c, unchecked), CBaseIntelligentPointer
// ctor @ 0x00A01B10 / GetPItem @ 0x00A01B50 / SetPItem @ 0x00A01B90 / dtor @ 0x00A01C10, CCreatureAction_NymphTeleportIntoLimbo
// ctor @ 0x008CA800 / ~CCombatActionBase @ 0x0062E900, AIAction_PerformActionAndWait @ 0x0097DB70, CTCNymph::StartFlyingAround
// @ 0x0080F5E0 / CheckIfCanAttack @ 0x0080F210 / StopFlyingAround @ 0x0080F870, GetPWorld @ 0x008FCFC0, CWorld::GetWorldFrame
// @ 0x0049D870, IsStopping @ 0x008FD010, the brain's virtual slot 2 (yield), CThing::GetPosition @ 0x004C73D0, the physics
// body's virtual slot 74 (+0x128, set orientation), Terminate @ 0x008FCF40.
// Data: the reciprocal-square-root mantissa table @ 0x013CE558; 1.0f / 3.0f / 0.5f / 0.0f @ 0x0122DED8 / 0x0122DED4 /
// 0x0122F59C / 0x0122DEDC.
// Layout: local models. CTCNymph from Ego_r.pdb with the 12-byte retail spline vector (every member after FlightSpline is 4 lower):
// FlyingAround +0x40, CanAttack +0x58, ReadyToAttack +0x59, HasBeenInLimbo +0x5b, PTarget +0x68. Retail CThing physics body +0x60.
// Shape: the shared `push 0` serves both the StartFlyingAround argument and the action's wait flag; a planar direction with Z = 0
// normalised by the landed 0x007A6E20 inline Normalise, then a CRightHandedSet (Up 0,0,1; Forward) by reference.
extern unsigned long g_InvSqrtMantissaTable[128];   // 0x013CE558

struct C3DVector {
    float X, Y, Z;
    C3DVector() { }
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) { }
    C3DVector(const C3DVector& other) : X(other.X), Y(other.Y), Z(other.Z) { }
    float GetLengthSquared2D() const { return X * X + Y * Y; }
    __forceinline void Normalise()
    {
        float x = X;
        float y = Y;
        float z = Z;
        float lengthSquared = x * x + y * y + z * z;
        unsigned long bits = *(unsigned long*)&lengthSquared;
        unsigned long estimate = (((0x17c - ((bits >> 23) & 0xff)) >> 1) << 23) |
                                 (g_InvSqrtMantissaTable[(bits >> 17) & 0x7f] << 15);
        float r = *(float*)&estimate;
        r = (3.0f - r * r * lengthSquared) * r * 0.5f;
        X *= r;
        Y *= r;
        Z *= r;
    }
};
struct CRightHandedSet {
    C3DVector Up;
    C3DVector Forward;
    CRightHandedSet(const C3DVector& up, const C3DVector& forward) : Up(up), Forward(forward) { }
};
struct CPhysicsBody {
#define VF(n) virtual void v##n();
#define VF10(p) VF(p##0) VF(p##1) VF(p##2) VF(p##3) VF(p##4) VF(p##5) VF(p##6) VF(p##7) VF(p##8) VF(p##9)
    VF10(0) VF10(1) VF10(2) VF10(3) VF10(4) VF10(5) VF10(6) VF(70) VF(71) VF(72) VF(73)
    virtual void SetOrientation(const CRightHandedSet& orientation);   // slot 74 (+0x128)
};
struct CTCBase;
struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);       // 0x0040F020
};
struct CThing {
    unsigned char _pad0[0x20];
    unsigned long m_presentBits[9];             // +0x20
    CInterfaceMap m_interfaces;                 // +0x44
    unsigned char _pad4c[0x60 - 0x4c];
    CPhysicsBody* PPhysics;                     // +0x60
    const C3DVector& GetPosition() const;       // 0x004C73D0
    template<class T> __forceinline bool GetInterface(T** ppOut, long interfaceType)
    {
        if (m_presentBits[interfaceType >> 5] & (1u << (interfaceType & 31))) {
            long type = interfaceType;
            CInterfaceMap* map = &m_interfaces;
            CInterfaceMap::SEntry* it = map->LowerBound(&type);
            *ppOut = (T*)((it == map->last || interfaceType < it->type) ? map->last : it)->component;
            return true;
        }
        return false;
    }
};
struct CThingCreatureBase : CThing { };
struct CBaseIntelligentPointer {
    virtual void v00();
    CThing* m_pItem;                            // +0x04
    CBaseIntelligentPointer();                  // 0x00A01B10
    ~CBaseIntelligentPointer();                 // 0x00A01C10
    CThing* GetPItem() const;                   // 0x00A01B50
    void SetPItem(const CThing* item);          // 0x00A01B90
};
struct CIntelligentPointerCreature : CBaseIntelligentPointer {     // CIntelligentPointer<CThingCreatureBase>, vtable 0x0123E7D8
    virtual void v00();
    __forceinline CIntelligentPointerCreature(CThingCreatureBase* item) : CBaseIntelligentPointer() { SetPItem(item); }
    __forceinline CIntelligentPointerCreature& operator=(const CIntelligentPointerCreature& other) { CThing* item = other.GetPItem(); SetPItem(item); return *this; }
};
#pragma pack(push, 1)
struct CTCNymph {
    unsigned char _pad0[0x40];
    bool FlyingAround;                          // +0x40
    unsigned char _pad41[0x58 - 0x41];
    bool CanAttack;                             // +0x58
    bool ReadyToAttack;                         // +0x59
    bool ReadyToSummon;                         // +0x5a
    bool HasBeenInLimbo;                        // +0x5b
    unsigned char _pad5c[0x68 - 0x5c];
    CIntelligentPointerCreature PTarget;        // +0x68
    void StartFlyingAround(long frames);        // 0x0080F5E0
    void CheckIfCanAttack(long frame);          // 0x0080F210
    void StopFlyingAround(long mode);           // 0x0080F870
};
#pragma pack(pop)
struct CCreatureActionBase {
    virtual void v00();
};
struct CCombatActionBase : CCreatureActionBase {
    unsigned char _body[0x110];                 // retail sizeof 0x114
    ~CCombatActionBase();                       // 0x0062E900
};
struct CCreatureAction_NymphTeleportIntoLimbo : CCombatActionBase {
    CCreatureAction_NymphTeleportIntoLimbo(CThingCreatureBase* owner);    // 0x008CA800
};
struct CWorld {
    static long GetWorldFrame();                // 0x0049D870
};
struct CAIBrain {
    virtual void v00();
    virtual void v01();
    virtual void Yield();                       // slot 2 (+8)
    unsigned char _pad[0x14];
    CThingCreatureBase* PCreature;              // +0x18
};
struct CAIStateGroupBase {
    virtual ~CAIStateGroupBase();
    CAIBrain* PBrain;                           // +0x04
    CThingCreatureBase* GetCreature() const { return PBrain->PCreature; }
    CWorld* GetPWorld();                        // 0x008FCFC0
    bool IsStopping();                          // 0x008FD010
    void Terminate();                           // 0x008FCF40
    long AIAction_PerformActionAndWait(CCreatureActionBase* action, bool wait);   // 0x0097DB70
};
struct CAIStateGroup_NymphFloatAround : CAIStateGroupBase {
    virtual void Update();
};

extern "C" int StandInSink(int, int);
#pragma auto_inline(off)
CInterfaceMap::SEntry* CInterfaceMap::LowerBound(const long* type)
{
    SEntry* f = first;
    long count = last - first;
    while (count > 0) {
        long half = count >> 1;
        if (f[half].type < *type) { f += half + 1; count -= half + 1; } else count = half;
    }
    return f;
}
#pragma auto_inline(on)
void CAIStateGroup_NymphFloatAround::Update()
{
    CTCNymph* nymph;
    GetCreature()->GetInterface(&nymph, 0xf5);
    nymph->CanAttack = false;
    nymph->ReadyToAttack = false;
    nymph->PTarget = CIntelligentPointerCreature(0);
    if (!nymph->FlyingAround) {
        if (nymph->HasBeenInLimbo) {
            if (AIAction_PerformActionAndWait(&CCreatureAction_NymphTeleportIntoLimbo(GetCreature()), false) != 2) {
                Terminate();
                return;
            }
        } else
            nymph->StartFlyingAround(0);
    }
    long frame = GetPWorld()->GetWorldFrame();
    while (!nymph->ReadyToAttack) {
        nymph->CheckIfCanAttack(frame);
        if (IsStopping() || !nymph->FlyingAround) {
            Terminate();
            return;
        }
        PBrain->Yield();
    }
    nymph->StopFlyingAround(-1);
    if (nymph->PTarget.GetPItem()) {
        const CThing* creature = GetCreature();
        const CThing* target = nymph->PTarget.GetPItem();
        const C3DVector& from = creature->GetPosition();
        const C3DVector& to = target->GetPosition();
        C3DVector direction(to.X - from.X, to.Y - from.Y, 0.0f);
        if (direction.GetLengthSquared2D() > 1.0f) {
            direction.Normalise();
            GetCreature()->PPhysics->SetOrientation(CRightHandedSet(C3DVector(0.0f, 0.0f, 1.0f), direction));
        }
    }
    Terminate();
}