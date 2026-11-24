// CAIStateGroup_JackDragon::StayIdle(float seconds) -- idle frame by frame for `seconds`; terminate the group as soon as the enemy dies or the group stops
// Leaf: ?StayIdle@CAIStateGroup_JackDragon@@AAEXM@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0094FE30, length 126 bytes (+ int3 padding).
// Callees: the world accessor @ 0x008FCFC0 (PBrain->+0x1c->+0x24), CWorld::GetWorldFrame @ 0x0049D870 (a
// static returning the frame global @ 0x013B89BC; manifest "GFGetPreMainMemoryUsed" is propagated),
// CAIStateGroupBase::AIAction_DoNothing @ 0x0097E200, CBaseIntelligentPointer::GetPItem @ 0x00A01B50,
// CAIStateGroupBase::IsStopping @ 0x008FD010, CAIStateGroupBase::Terminate @ 0x008FCF40.
// Globals: the frames-per-second long @ 0x01375550 (`fidiv`).
//
// Shape: `GetWorld().GetWorldFrame()` still calls the accessor (static member through an object
// expression); the start frame lives in ebp, &PEnemy (+0x14) in edi (hoisted above the loop). A do/while:
// DoNothing, then the inlined IS_THING_ALIVE(PEnemy) / IsStopping exits sharing one Terminate tail, then
// `(float)(frame - start) / g_framesPerSecond < seconds` (`fild ; fidiv ; fcomp [arg] ; test ah,5 ; jnp`,
// the int difference parked in the `push ecx` slot). No engine header for the class: local model.
// NF166 (round 10, q11679 / q11691 / q11725): 0x01375550 is ?ConstantFPS@CMainGameComponent@@1JA (protected static long,
//   engine/CMainGameComponent.h), read through the inline public accessor GetConstantFPS() (Ego_r + FableWin
//   ?GetConstantFPS@CMainGameComponent@@SIJXZ) as retail code outside the class does; was the row-local stand-in g_framesPerSecond.
#include "engine/CMainGameComponent.h"

#include "engine/CThing.h"

class CBaseObject;
class CBaseObjectPointer;
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#include "engine/CIntelligentPointer.h"


inline bool IS_THING_ALIVE(const CIntelligentPointer<CThing>& p)
{
    if (p.Get() != 0) {
        if (!p.Get()->IsDead())
            return true;
    }
    return false;
}

class CWorld { public:
    static long __fastcall GetFrame();                // 0x0049D870
};

enum EAIActionReturn { };
class CAIStateGroupBase { public:
    virtual ~CAIStateGroupBase();
    void* PBrain;                               // +0x04
    unsigned char _pad[0x0c];                   // +0x08..+0x13
protected:
    CWorld& GetWorld() const;                        protected: // 0x008FCFC0 ?GetWorld@CAIStateGroupBase@@IBEAAVCWorld@@XZ (Ego_r/FableWin, protected; agentNF16: was GetPWorld)
protected:
    EAIActionReturn AIAction_DoNothing(); public:                  // 0x0097E200
protected:
    bool IsStopping() const;                          // 0x008FD010 (Ego_r.pdb protected const)
public:
    void Terminate();                           // 0x008FCF40
};

struct CAIStateGroup_JackDragon : CAIStateGroupBase {
    CIntelligentPointer<CThing> PEnemy;             // +0x14 CIntelligentPointer<CThingCreatureBase>
    private: void StayIdle(float seconds); public:
};

void CAIStateGroup_JackDragon::StayIdle(float seconds)
{
    long start = GetWorld().GetFrame();
    long frames;
    do {
        AIAction_DoNothing();
        if (!IS_THING_ALIVE(PEnemy) || IsStopping()) {
            Terminate();
            return;
        }
        frames = GetWorld().GetFrame() - start;
    } while ((float)frames / CMainGameComponent::GetConstantFPS() < seconds);
}