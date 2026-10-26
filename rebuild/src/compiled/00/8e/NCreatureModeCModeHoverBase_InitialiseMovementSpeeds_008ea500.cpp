// NCreatureMode::CModeHoverBase::InitialiseMovementSpeeds() -- measure the mode's FORWARD_SLOW/_MEDIUM/_FAST animations (prefixed
//   by the mode's animation prefix) and scale each hover state's speed by the creature def's slow/medium/fast hover speed over the
//   matching animation speed (1 when either is below 1e-4); a state's tier comes from its animation-name suffix (_SLOW, _MEDIUM,
//   _FAST, else 0) and the scaled speed is written to both MovementSpeed and Area.RadHigh
// Leaf: InitialiseMovementSpeeds
// Retail: Fable.exe (TLC, VC7.1) @ 0x008EA500, 923 bytes (`ret`).
// Callees: CModeBase::GetWorld 0x0073A680 (world +0x3c passed through), own virtual slot 12 (+0x30, CCharString by value),
//   operator+(const CCharString&, const char*) 0x0099F600 (__fastcall), the animation movement-speed helper 0x00861C90 (__fastcall
//   name, creature, one stack word), the creature def hover-speed getters 0x006620E0 / 0x006620F0 / 0x00662100 (def +0xe0 -> +0xa4/
//   +0xa8/+0xac; manifest "CTCPhysicsBase::GetAirResistance" is a propagated label), CCharString 0x0099EBF0 / ~ 0x0099EAE0.
// Data: "FORWARD_SLOW" 0x01287CFC, "FORWARD_MEDIUM" 0x01287CEC, "FORWARD_FAST" 0x01287CDC, "_SLOW" 0x01287CD4, "_MEDIUM" 0x01287CCC,
//   "_FAST" 0x01287CC4, epsilon 1e-4f 0x0129BA3C, the empty string 0x0129AAF4.
// Layout: Ego_r.pdb CModeHoverBase: HoverStates vector +0x1c (12-byte retail vector), CHoverState 0x18 (AnimName +0, Area +4
//   {ThetaLow, ThetaHigh, RadHigh}, MovementType +0x10, MovementSpeed +0x14).
// Shape: the inline CCharString EndsWith (the landed 0x005955AB model) with null-first Length()/c_str() ternaries (`== 0 ? ...`);
//   the speeds are written as one chained assignment (fst / fstp).
#include <string.h>
extern char g_emptyString[];                            // 0x0129AAF4
struct CStringData { char* Text; long Length; };
class CCharString { public:
    CStringData* PStringData;
    CCharString(const char* text, long length = -1);    // 0x0099EBF0
    ~CCharString();                                     // 0x0099EAE0
    long Length() const { return PStringData == 0 ? 0 : PStringData->Length; }
    const char* c_str() const { return PStringData == 0 ? g_emptyString : PStringData->Text; }
    __forceinline bool EndsWith(const CCharString& suffix) const
    {
        long offset = Length() - suffix.Length();
        if (offset >= 0)
            return strcmp(c_str() + offset, suffix.c_str()) == 0;
        return false;
    }
};
CCharString __fastcall operator+(const CCharString& left, const char* right);   // 0x0099F600
class CThingCreatureBase { public:
    float GetHoverSpeedSlow() const;                    // 0x006620E0
    float GetHoverSpeedMedium() const;                  // 0x006620F0
    float GetHoverSpeedFast() const;                    // 0x00662100
};
float __fastcall GetAnimMovementSpeed(const CCharString& animName, CThingCreatureBase* creature, long world3c);   // 0x00861C90
class CWorld { public:
    unsigned char _pad[0x3c];
    long          Value3c;                              // +0x3c
};
extern const float g_epsilon_0129ba3c;                  // 0x0129BA3C (1e-4f)
struct CModeBase {
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04(); virtual void v05();
    virtual void v06(); virtual void v07(); virtual void v08(); virtual void v09(); virtual void v10(); virtual void v11();
    virtual CCharString GetAnimPrefix();                // slot 12 (+0x30)
    CThingCreatureBase* ParentThing;                    // +0x04
    unsigned char _body[0x14];
    CWorld* GetWorld();                                 // 0x0073A680
};
namespace NCreatureMode {
struct CModeHoverBase : CModeBase {
    class CRadialArea { public: float ThetaLow, ThetaHigh, RadHigh; };
    class CHoverState { public:
        CCharString AnimName;                           // +0x00
        CRadialArea Area;                               // +0x04
        long        MovementType;                       // +0x10
        float       MovementSpeed;                      // +0x14
    };
    CHoverState* HoverStatesStart;                      // +0x1c
    CHoverState* HoverStatesFinish;                     // +0x20
    CHoverState* HoverStatesEnd;                        // +0x24
    void InitialiseMovementSpeeds();
};

void CModeHoverBase::InitialiseMovementSpeeds()
{
    CThingCreatureBase* creature = ParentThing;
    long world3c = GetWorld()->Value3c;
    CCharString prefix = GetAnimPrefix();
    float slowAnimSpeed = GetAnimMovementSpeed(prefix + "FORWARD_SLOW", creature, world3c);
    float mediumAnimSpeed = GetAnimMovementSpeed(prefix + "FORWARD_MEDIUM", creature, world3c);
    float fastAnimSpeed = GetAnimMovementSpeed(prefix + "FORWARD_FAST", creature, world3c);
    float slowSpeed = creature->GetHoverSpeedSlow();
    float mediumSpeed = creature->GetHoverSpeedMedium();
    float fastSpeed = creature->GetHoverSpeedFast();
    float slowScale, mediumScale, fastScale;
    if (slowAnimSpeed < g_epsilon_0129ba3c || slowSpeed < g_epsilon_0129ba3c)
        slowScale = 1.0f;
    else
        slowScale = slowSpeed / slowAnimSpeed;
    if (mediumAnimSpeed < g_epsilon_0129ba3c || mediumSpeed < g_epsilon_0129ba3c)
        mediumScale = 1.0f;
    else
        mediumScale = mediumSpeed / mediumAnimSpeed;
    if (fastAnimSpeed < g_epsilon_0129ba3c || fastSpeed < g_epsilon_0129ba3c)
        fastScale = 1.0f;
    else
        fastScale = fastSpeed / fastAnimSpeed;
    for (CHoverState* state = HoverStatesStart; state != HoverStatesFinish; ++state) {
        float scale = 0.0f;
        if (state->AnimName.EndsWith("_SLOW"))
            scale = slowScale;
        else if (state->AnimName.EndsWith("_MEDIUM"))
            scale = mediumScale;
        else if (state->AnimName.EndsWith("_FAST"))
            scale = fastScale;
        state->Area.RadHigh = state->MovementSpeed = GetAnimMovementSpeed(state->AnimName, creature, world3c) * scale;
    }
}
}