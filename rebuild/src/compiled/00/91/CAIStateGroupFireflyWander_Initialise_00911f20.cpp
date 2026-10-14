// CAIStateGroup_FireflyWander::Initialise(CAIBrain&, CThingAICreature&, long, EScriptAIPriority, const CCharString&)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00911F20, length 95 bytes.
// Callees: CThing::GetPosition @ 0x006A4D00 (manifest "CVertexBufferWin32::DoSizeof" is a propagated label; the
// callee returns a C3DVector* the caller copies three floats from), CAIStateGroupBase::Initialise @ 0x008FCEC0.
//
// Shape: the creature's position is copied into the wander centre at +0x24 (three dword moves through
// `lea ecx,[esi+24h]`, the compiler's own C3DVector copy), the frame counter at +0x30 is zeroed and three
// wander parameters are set (0.7f at +0x14, 2.5f at +0x1c, 6.0f at +0x20; +0x18 is left alone), then the
// base Initialise runs with the five arguments re-pushed (`ret 14h`). The scheduler sinks the four constant
// stores below the argument pushes. The CAIStateGroup_FlyingThingBase layout (+0x14..+0x20, see its
// constructor @ 0x009126C0) is modelled locally; no engine header exists for either class.
struct CAIBrain;
struct CCharString;
typedef long EScriptAIPriority;

struct C3DVector {
    float X, Y, Z;
};

struct CThingAICreature {
    C3DVector* GetPosition();                   // 0x006A4D00
};

struct CAIStateGroupBase {
    virtual void v00();
    CAIBrain* PBrain;                           // +0x04
    long ScriptAIPriority;                      // +0x08
    void* Name_PStringData;                     // +0x0c
    unsigned char Terminated;                   // +0x10
    unsigned char Priority;                     // +0x11
    unsigned char Enabled;                      // +0x12
    unsigned char _pad_0x13;
    void Initialise(CAIBrain& brain, CThingAICreature& creature, long priority, EScriptAIPriority scriptPriority, const CCharString& name);   // 0x008FCEC0
};

struct CAIStateGroup_FlyingThingBase : CAIStateGroupBase {
    float m_heightAboveGround;                  // +0x14
    float m_speed;                              // +0x18
    float m_minHeight;                          // +0x1c
    float m_maxHeight;                          // +0x20
};

struct CAIStateGroup_FireflyWander : CAIStateGroup_FlyingThingBase {
    C3DVector m_wanderCentre;                   // +0x24
    long m_frameLastChangedDirection;           // +0x30
    void Initialise(CAIBrain& brain, CThingAICreature& creature, long priority, EScriptAIPriority scriptPriority, const CCharString& name);
};

void CAIStateGroup_FireflyWander::Initialise(CAIBrain& brain, CThingAICreature& creature, long priority, EScriptAIPriority scriptPriority, const CCharString& name)
{
    m_wanderCentre = *creature.GetPosition();
    m_frameLastChangedDirection = 0;
    m_heightAboveGround = 0.7f;
    m_minHeight = 2.5f;
    m_maxHeight = 6.0f;
    CAIStateGroupBase::Initialise(brain, creature, priority, scriptPriority, name);
}