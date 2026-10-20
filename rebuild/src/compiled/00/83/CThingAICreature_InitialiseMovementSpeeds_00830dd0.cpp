// CThingAICreature::InitialiseMovementSpeeds() -- the flying speed comes from the STANDARD_FLY animation when the creature has one in any group (else zero); when the creature def's speed variation is positive the walking, jogging, running and flying speeds are each randomised
// Leaf: InitialiseMovementSpeeds
// Retail: Fable.exe (TLC, VC7.1) @ 0x00830DD0, 212 bytes (+12 int3). `ret`.
// Callees: CCharString::CCharString(const char*, long) 0x0099EBF0 (x2, "STANDARD_FLY" @ 0x0125ABD4), CThingCreatureBase::HasAnim
//          (const CCharString&, const CAnimFilterBase&) 0x00662C70, ~CCharString 0x0099EAE0 (x2), CThingCreatureBase::
//          InitialiseMovementSpeed(float*, const CCharString&) 0x00663510, CThingAICreature::RandomiseMovementSpeed(float*)
//          0x00830260 (x4). Vtables: CAnimFilterAnyGroup 0x0127261C, CAnimFilterBase 0x0122FFCC. 1e-4f @ 0x0129BA3C.
// Shape: the filter temporary (vtable only) and the name temporary share the frame; the HasAnim result is kept in bl across
//        both destructors; the fly-speed address (edi) is shared by the InitialiseMovementSpeed call and the last randomise.
// Layout: MaxWalkingSpeed +0x1B8, MaxJoggingSpeed +0x1BC, MaxRunningSpeed +0x1C0, MaxFlyingSpeed +0x1C4 (Ego_r.pdb +0x1E8..
//         +0x1F4, 0x30 lower in retail); the +0xE0 pointer's +0xB0 float is the def's speed variation (descriptive).
class CCharString {
public:
    void* PStringData;
    CCharString(const char* text, long length);      // 0x0099EBF0
    ~CCharString();                                  // 0x0099EAE0
};
struct CAnimFilterBase {                             // vtable 0x0122FFCC
    virtual ~CAnimFilterBase() { }
    virtual void v01();
    virtual void v02();
};
struct CAnimFilterAnyGroup : CAnimFilterBase {       // vtable 0x0127261C (implicit constructor: the vtable store sinks to the call)
};
extern const float g_epsilon_0129ba3c;               // 0x0129BA3C (1e-4f)
struct CCreatureSpeedSource {                        // descriptive: the +0xE0 pointee
    unsigned char _pad00[0xb0];
    float         SpeedVariation;                    // +0xb0
};
struct CThingAICreature {
    unsigned char _pad00[0xe0];
    CCreatureSpeedSource* m_pSpeedSource;            // +0xe0 (descriptive)
    unsigned char _pade4[0x1b8 - 0xe4];
    float         MaxWalkingSpeed;                   // +0x1b8
    float         MaxJoggingSpeed;                   // +0x1bc
    float         MaxRunningSpeed;                   // +0x1c0
    float         MaxFlyingSpeed;                    // +0x1c4
    bool HasAnim(const CCharString& name, const CAnimFilterBase& filter);   // 0x00662C70 (CThingCreatureBase)
    void InitialiseMovementSpeed(float* speed, const CCharString& name);   // 0x00663510 (CThingCreatureBase)
    void RandomiseMovementSpeed(float* speed);       // 0x00830260
    void InitialiseMovementSpeeds();
};
void CThingAICreature::InitialiseMovementSpeeds()
{
    bool hasFlyAnim;
    {
        CCharString flyAnim("STANDARD_FLY", -1);
        CAnimFilterAnyGroup anyGroup;
        hasFlyAnim = HasAnim(flyAnim, anyGroup);
    }
    if (hasFlyAnim) {
        CCharString flyAnim("STANDARD_FLY", -1);
        InitialiseMovementSpeed(&MaxFlyingSpeed, flyAnim);
    } else {
        MaxFlyingSpeed = 0.0f;
    }
    if (m_pSpeedSource->SpeedVariation > g_epsilon_0129ba3c) {
        RandomiseMovementSpeed(&MaxWalkingSpeed);
        RandomiseMovementSpeed(&MaxJoggingSpeed);
        RandomiseMovementSpeed(&MaxRunningSpeed);
        RandomiseMovementSpeed(&MaxFlyingSpeed);
    }
}