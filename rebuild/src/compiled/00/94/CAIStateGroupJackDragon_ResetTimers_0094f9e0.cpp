// CAIStateGroup_JackDragon_Methods::ResetTimers  @ 0x0094f9e0
// Faithful VC7.1 reconstruction.

#include "engine/CAIStateGroup_JackDragon.h"  // retyped onto the PDB layout; byte parity re-verified
class CWorld {
public:
    static long __fastcall GetFrame();   // 0x0049D870
};        // 0x0049d870 : returns int in eax

class CAIStateGroupBase { protected:   // engine/CAIStateGroupBase.h flattens the vptr: the getter only
    CWorld& GetWorld() const;   // 0x008FCFC0 (Ego_r.pdb ?GetWorld@CAIStateGroupBase@@IBEAAVCWorld@@XZ)
};
struct CAIStateGroup_JackDragon_Methods : public CAIStateGroupBase, CAIStateGroup_JackDragon {
    // +0x58

    void ResetTimers();
};

void CAIStateGroup_JackDragon_Methods::ResetTimers()
{
    this->FramesInSnapRange = 0;
    GetWorld();
    int t = CWorld::GetFrame();
    this->FrameOfLastSlam = t;
    this->FrameWhenLastInRange = t;
}