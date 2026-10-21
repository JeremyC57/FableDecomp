// CModeStandBase::AnimUpdate(CAnimateThing& anim) -- per-frame animation update of the stand/walk mode: track how long the creature has idled, play the idle animation when it is not moving, otherwise the walking animation, and occasionally (one in three, once idled long enough at walking pace) add combination idles
// Leaf: AnimUpdate
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00862EC0, length 331 bytes (padded to 333), `ret 4`; CModeStandBase vtable 0x01276D8C slot 2.
// Ego_r.pdb: `?AnimUpdate@CModeStandBase@@UAEXAAVCAnimateThing@@@Z`.
// Callees: CModeBase::GetGlobalComponents() const @ 0x0073A680 (reads [0x013B8A1C]+0x24, as the landed CModeStrafe rows) and its
//   virtual slot 4 (+0x10, the game state: two "suspend idling" bytes +0x54/+0x55), CModeStandBase::UpdateIdleAnim @ 0x00862780,
//   UpdateWalkingAnim @ 0x00862AA0, the movement component's (+0x60) virtual slot 70 (+0x118, a C3DVector by value),
//   CThing::GetPosition @ 0x004C73D0, the creature's virtual slot 79 GetMaxJoggingSpeed(const C3DVector&) (+0x13c,
//   CThingAICreature 0x00830000), virtual GetModeType (slot 1), CModeBase::GetNoFramesBeforePlayingCombinationIdleAnims(EMode)
//   @ 0x0073A6F0, the random generator accessor @ 0x0049D810 and GFROR13 @ 0x00497890 (as the landed 0x008EE720),
//   AddCombinationIdles(CThingCreatureBase*, CAnimateThing&) @ 0x008619C0 (`__fastcall`).
// Globals: epsilon 0.0001f @ 0x0129BA3C.
// Layout: engine/CModeStandBase.h: IdleCounter +0x1c, FramesToKeepUpdatingMovement +0x20 (the header's vptr is a plain pointer, so
//   the class is restated with its virtual slots). The creature's planar velocity X/Y at +0x10c/+0x110.
// Shape: the planar speed is sqrt(X*X + Y*Y); the walk branch tests `GetMaxJoggingSpeed(GetPosition() + GetVelocity()) >= speed`
//   (member operator+ on the position: the argument call runs first and VC loads the velocity operand first); the random
//   step is the inlined generator `seed = GFROR13(seed * 0x24A1 + 0x24DF)` taken `% 3`.
#include <math.h>

long __fastcall GFROR13(unsigned long value);     // 0x00497890

struct CRandomGenerator {
    unsigned long Seed;
    __forceinline unsigned long Next() { Seed = Seed * 0x24a1 + 0x24df; Seed = GFROR13(Seed); return Seed; }
};

struct C3DVector {
    float X, Y, Z;
    C3DVector() {}
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) {}
    C3DVector operator+(const C3DVector& o) const { return C3DVector(X + o.X, Y + o.Y, Z + o.Z); }
};

struct CGameState {
    unsigned char _pad00[0x54];
    bool          SuspendIdling;                // +0x54
    bool          SuspendIdlingForCutscene;     // +0x55
};

struct CGlobalComponents {
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03();
    virtual CGameState* GetGameState();         // slot 4 (+0x10)
    CRandomGenerator* GetRandomGenerator();     // 0x0049D810
};

struct CMovementComponent {
    virtual void v000(); virtual void v001(); virtual void v002(); virtual void v003(); virtual void v004(); virtual void v005();
    virtual void v006(); virtual void v007(); virtual void v008(); virtual void v009(); virtual void v010(); virtual void v011();
    virtual void v012(); virtual void v013(); virtual void v014(); virtual void v015(); virtual void v016(); virtual void v017();
    virtual void v018(); virtual void v019(); virtual void v020(); virtual void v021(); virtual void v022(); virtual void v023();
    virtual void v024(); virtual void v025(); virtual void v026(); virtual void v027(); virtual void v028(); virtual void v029();
    virtual void v030(); virtual void v031(); virtual void v032(); virtual void v033(); virtual void v034(); virtual void v035();
    virtual void v036(); virtual void v037(); virtual void v038(); virtual void v039(); virtual void v040(); virtual void v041();
    virtual void v042(); virtual void v043(); virtual void v044(); virtual void v045(); virtual void v046(); virtual void v047();
    virtual void v048(); virtual void v049(); virtual void v050(); virtual void v051(); virtual void v052(); virtual void v053();
    virtual void v054(); virtual void v055(); virtual void v056(); virtual void v057(); virtual void v058(); virtual void v059();
    virtual void v060(); virtual void v061(); virtual void v062(); virtual void v063(); virtual void v064(); virtual void v065();
    virtual void v066(); virtual void v067(); virtual void v068(); virtual void v069();
    virtual C3DVector GetVelocity() const;      // slot 70 (+0x118)
};

class CThing {
public:
    virtual void v000(); virtual void v001(); virtual void v002(); virtual void v003(); virtual void v004(); virtual void v005();
    virtual void v006(); virtual void v007(); virtual void v008(); virtual void v009(); virtual void v010(); virtual void v011();
    virtual void v012(); virtual void v013(); virtual void v014(); virtual void v015(); virtual void v016(); virtual void v017();
    virtual void v018(); virtual void v019(); virtual void v020(); virtual void v021(); virtual void v022(); virtual void v023();
    virtual void v024(); virtual void v025(); virtual void v026(); virtual void v027(); virtual void v028(); virtual void v029();
    virtual void v030(); virtual void v031(); virtual void v032(); virtual void v033(); virtual void v034(); virtual void v035();
    virtual void v036(); virtual void v037(); virtual void v038(); virtual void v039(); virtual void v040(); virtual void v041();
    virtual void v042(); virtual void v043(); virtual void v044(); virtual void v045(); virtual void v046(); virtual void v047();
    virtual void v048(); virtual void v049(); virtual void v050(); virtual void v051(); virtual void v052(); virtual void v053();
    virtual void v054(); virtual void v055(); virtual void v056(); virtual void v057(); virtual void v058(); virtual void v059();
    virtual void v060(); virtual void v061(); virtual void v062(); virtual void v063(); virtual void v064(); virtual void v065();
    virtual void v066(); virtual void v067(); virtual void v068(); virtual void v069(); virtual void v070(); virtual void v071();
    virtual void v072(); virtual void v073(); virtual void v074(); virtual void v075(); virtual void v076(); virtual void v077();
    virtual void v078();
    virtual float GetMaxJoggingSpeed(const C3DVector& position);   // slot 79 (+0x13c)
    unsigned char       _pad04[0x5c];
    CMovementComponent* Movement;               // +0x60
    unsigned char       _pad64[0xa8];
    float               VelocityX;              // +0x10c
    float               VelocityY;              // +0x110
    const C3DVector& GetPosition() const;       // 0x004C73D0
};
class CThingCreatureBase : public CThing {
};
class CAnimateThing;

void __fastcall AddCombinationIdles(CThingCreatureBase* creature, CAnimateThing& anim);   // 0x008619C0

enum EMode { EMODE_NONE = 0 };

class CModeBase {
public:
    virtual ~CModeBase();
    virtual EMode GetModeType() const = 0;      // slot 1
    virtual void AnimUpdate(CAnimateThing& anim);   // slot 2
    CThingCreatureBase* ParentThing;            // +0x04
    unsigned char       _pad08[0x14];
    CGlobalComponents* GetGlobalComponents() const;   // 0x0073A680
    long GetNoFramesBeforePlayingCombinationIdleAnims(EMode mode);   // 0x0073A6F0
};

class CModeStandBase : public CModeBase {
public:
    long IdleCounter;                           // +0x1c
    long FramesToKeepUpdatingMovement;          // +0x20
    virtual void AnimUpdate(CAnimateThing& anim);
    void UpdateIdleAnim(CAnimateThing& anim);   // 0x00862780
    void UpdateWalkingAnim(CAnimateThing& anim);   // 0x00862AA0
};

void CModeStandBase::AnimUpdate(CAnimateThing& anim)
{
    CThingCreatureBase* creature = ParentThing;
    float speed = (float)sqrt(creature->VelocityX * creature->VelocityX + creature->VelocityY * creature->VelocityY);
    if (GetGlobalComponents()->GetGameState()->SuspendIdling || GetGlobalComponents()->GetGameState()->SuspendIdlingForCutscene)
        IdleCounter = 0;
    else
        ++IdleCounter;
    if (speed > 0.0001f || FramesToKeepUpdatingMovement > 0) {
        UpdateWalkingAnim(anim);
        if (creature->GetMaxJoggingSpeed(creature->GetPosition() + creature->Movement->GetVelocity()) >= speed) {
            if (IdleCounter > GetNoFramesBeforePlayingCombinationIdleAnims(GetModeType())) {
                if (GetGlobalComponents()->GetRandomGenerator()->Next() % 3 == 0) {
                    AddCombinationIdles(creature, anim);
                    IdleCounter = 0;
                }
            }
        }
    } else
        UpdateIdleAnim(anim);
}