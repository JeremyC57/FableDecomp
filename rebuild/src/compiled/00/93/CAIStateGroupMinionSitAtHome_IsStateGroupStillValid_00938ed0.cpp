// Leaf: ?IsStateGroupStillValid@CAIStateGroup_MinionSitAtHome@@QAEDXZ
#include "rebuild_abi.h"
class CWorld { public: static long __fastcall GetFrame(); };   // 0x0049D870 ?GetFrame@CWorld@@SIJXZ
class CAIStateGroupBase { protected:   // engine/CAIStateGroupBase.h flattens the vptr: the getter only
    CWorld& GetWorld() const;   // 0x008FCFC0 (Ego_r.pdb ?GetWorld@CAIStateGroupBase@@IBEAAVCWorld@@XZ)
};
struct CAIStateGroup_MinionSitAtHome : public CAIStateGroupBase { char p[0x3c]; int f3c; char IsStateGroupStillValid();  };
char CAIStateGroup_MinionSitAtHome::IsStateGroupStillValid(){ GetWorld(); int b=CWorld::GetFrame(); return (char)(b <= this->f3c); }
// Stand-in (owner rule 2026-11-05): the real method on a stand-in class named after the real class (retail ?IsStateGroupStillValid@CAIStateGroup_MinionSitAtHome@@MBE_NXZ); evidence: RTTI vtable slot + PDB name set.
