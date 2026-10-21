// CTCPhysicsRigidBody::ComputeForces(CRigidBodyConfig& config) -- clear the config's force and torque, (debug key) kick it upward and give it a random normalised torque, add gravity when the body is affected by it, then linear and angular drag
// Leaf: ComputeForces
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00759D30, 400 bytes (`ret 4`).
// Callees: CInputEvent ctor 0x00A04410 / dtor 0x00A03B30; CInputManager::IsKeyboardEventInQueue 0x009F50A0 (g_pInputManager
//          0x013B8388, its +0x28 word gates the key test); CTCBase::GetWorld() 0x00686A70; CWorld random-seed accessor 0x0049D810;
//          GFFloatRandom 0x004978A0 (__fastcall); C3DVector::Normalise 0x00A14440; virtual slot 48 (+0xc0, affected by gravity).
// Layout: retail Body.OneOverMass +0xa0 (PDB +0xac: the three STLport vectors at +0x40/+0x50/+0x60 are 12 bytes in release,
//          not 16, so engine/CTCPhysicsRigidBody.h does not hold); CRigidBodyConfig as the PDB (CMVelocity +0x3c, CMForce +0x54,
//          Torque +0x60, AngularVelocity +0x9c).
// Shape: inline C3DVector::Zero (`X = Y = Z = 0`, stored Z,Y,X) and member operator+= keep &Torque in edi; the Z kick,
//          gravity and drag are expressions of the inline ctor.
enum EInputEventType { EIET_NONE = 0 };
enum EInputKey { EIK_NONE = 0 };
struct CInputEvent {
    unsigned char _d[0x34];
    CInputEvent();                                 // 0x00A04410
    ~CInputEvent();                                // 0x00A03B30
};
struct CInputManager {
    unsigned char _pad00[0x28];
    long          m_enabled;                       // +0x28 (descriptive)
    bool IsKeyboardEventInQueue(EInputEventType type, EInputKey key, CInputEvent* event);   // 0x009F50A0
};
extern CInputManager* g_pInputManager;             // 0x013B8388
float __fastcall GFFloatRandom(float range, unsigned long* seed);   // 0x004978A0
struct C3DVector {
    float X, Y, Z;
    C3DVector() {}
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) {}
    void Zero() { X = Y = Z = 0.0f; }
    void operator+=(const C3DVector& v) { X += v.X; Y += v.Y; Z += v.Z; }
    void Normalise();                              // 0x00A14440
};
struct CRigidBodyConfig {
    C3DVector     CMPosition;                      // +0x00
    unsigned char Orientation[0x30];               // +0x0c
    C3DVector     CMVelocity;                      // +0x3c
    C3DVector     AngularMomentum;                 // +0x48
    C3DVector     CMForce;                         // +0x54
    C3DVector     Torque;                          // +0x60
    unsigned char InverseWorldInertiaTensor[0x30]; // +0x6c
    C3DVector     AngularVelocity;                 // +0x9c
};
struct CWorld {
    unsigned long* GetRandomSeed();                // 0x0049D810
};
class CTCPhysicsRigidBody {
public:
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04(); virtual void v05();
    virtual void v06(); virtual void v07(); virtual void v08(); virtual void v09(); virtual void v10(); virtual void v11();
    virtual void v12(); virtual void v13(); virtual void v14(); virtual void v15(); virtual void v16(); virtual void v17();
    virtual void v18(); virtual void v19(); virtual void v20(); virtual void v21(); virtual void v22(); virtual void v23();
    virtual void v24(); virtual void v25(); virtual void v26(); virtual void v27(); virtual void v28(); virtual void v29();
    virtual void v30(); virtual void v31(); virtual void v32(); virtual void v33(); virtual void v34(); virtual void v35();
    virtual void v36(); virtual void v37(); virtual void v38(); virtual void v39(); virtual void v40(); virtual void v41();
    virtual void v42(); virtual void v43(); virtual void v44(); virtual void v45(); virtual void v46(); virtual void v47();
    virtual bool IsAffectedByGravity();            // slot 48 (+0xc0)
    unsigned char _pad04[0xa0 - 0x04];
    float         OneOverMass;                     // +0xa0 (Body)
    CWorld* GetWorld();                            // 0x00686A70 (CTCBase)
    void ComputeForces(CRigidBodyConfig& config);
};

void CTCPhysicsRigidBody::ComputeForces(CRigidBodyConfig& config)
{
    config.Torque.Zero();
    config.CMForce.Zero();
    CInputEvent event;
    if (g_pInputManager->m_enabled && g_pInputManager->IsKeyboardEventInQueue((EInputEventType)2, (EInputKey)0x2e, &event)) {
        config.CMForce.Z += 25.0f / OneOverMass;
        config.Torque.X = GFFloatRandom(100.0f, GetWorld()->GetRandomSeed());
        config.Torque.Y = GFFloatRandom(100.0f, GetWorld()->GetRandomSeed());
        config.Torque.Z = GFFloatRandom(100.0f, GetWorld()->GetRandomSeed());
        config.Torque.Normalise();
    }
    if (IsAffectedByGravity()) {
        float mass = 1.0f / OneOverMass;
        config.CMForce += C3DVector(0.0f * mass, 0.0f * mass, mass * -10.0f);
    }
    config.CMForce += C3DVector(config.CMVelocity.X * -0.06f, config.CMVelocity.Y * -0.06f, config.CMVelocity.Z * -0.06f);
    config.Torque += C3DVector(config.AngularVelocity.X * -0.02f, config.AngularVelocity.Y * -0.02f, config.AngularVelocity.Z * -0.02f);
}
