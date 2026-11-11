// CAIStateGroup_CreatureUseCoverBase::SetFailedToFindCover() -- remember where and when cover search failed
// Leaf: ?SetFailedToFindCover@CAIStateGroup_CreatureUseCoverBase@@QAEXXZ
// Plan phase 3 (2026-11-04), C3DVector wave: the local C3DVector stand-in replaced by the shared engine/C3DVector.h (PDB C3DVector TPI 0x2783: public X/Y/Z at 0/4/8, size 12).
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00970590, length 54 bytes (+ int3 padding).
// Callees: CThingPhysical::GetPos @ 0x004C73D0 (reference into the creature's physics
// block, copied memberwise into FailedToFindCoverAt), CAIStateGroupBase::GetPWorld @ 0x008FCFC0
// (PBrain -> World, called for its result object only), CWorld::GetFrameCount @ 0x0049D870
// (a static reading the frame counter global; __fastcall with no arguments).
// Retail layout (the engine header is 4 bytes high here because Ego_r's CoverList vector is
// 16 bytes where retail's is 12): FailedToFindCover +0x34, FailedTime +0x38,
// FailedToFindCoverAt +0x3c. The creature comes from PBrain (+0x4) -> Creature (+0x18).
#include "engine/C3DVector.h"

#include "engine/CThing.h"
class CThingAICreature : public CThing { public:
};

class CWorld { public:
    static long __fastcall GetFrame(); // 0x0049D870
};

class CAIBrain { public:
    unsigned char _pad[0x18];
    CThingAICreature* Creature;             // +0x18
};

class CAIStateGroupBase { public:
    virtual ~CAIStateGroupBase();
    CAIBrain* PBrain;                       // +0x04
    unsigned char _pad[0xc];                // +0x08..+0x13
protected:
    CWorld& GetWorld() const;                    // 0x008FCFC0 ?GetWorld@CAIStateGroupBase@@IBEAAVCWorld@@XZ (Ego_r/FableWin, protected; agentNF16: was GetPWorld)
public:
};

struct CAIStateGroup_CreatureUseCoverBase : CAIStateGroupBase {
    unsigned char _pad2[0x20];              // +0x14..+0x33 (target, flags, cover pointer, cover list)
    bool      FailedToFindCover;            // +0x34
    long      FailedTime;                   // +0x38
    C3DVector FailedToFindCoverAt;          // +0x3c
    void SetFailedToFindCover();
};

void CAIStateGroup_CreatureUseCoverBase::SetFailedToFindCover()
{
    FailedToFindCover = true;
    FailedToFindCoverAt = PBrain->Creature->GetPos();
    FailedTime = GetWorld().GetFrame();
}