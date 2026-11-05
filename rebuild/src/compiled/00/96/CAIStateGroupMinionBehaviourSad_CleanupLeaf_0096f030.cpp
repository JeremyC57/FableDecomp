// Leaf: ?CleanupLeaf@CAIStateGroup_MinionBehaviourSad@@QAEXXZ
#include "rebuild_abi.h"
class CTCCreatureModeManager { public: public:
    void RemoveMode(unsigned long);   // 0x0073ABA0
};
struct CAIStateGroup_MinionBehaviourSad { char p24[0x24]; CTCCreatureModeManager* f24; char p28[0x3c-0x28]; char f3c; void CleanupLeaf(); };
void CAIStateGroup_MinionBehaviourSad::CleanupLeaf(){ this->f24->RemoveMode(0x14); this->f3c=0; }
// Stand-in (owner rule 2026-11-05): the real method on a stand-in class named after the real class (retail ?CleanupLeaf@CAIStateGroup_MinionBehaviourSad@@UAEXXZ); evidence: RTTI vtable slot + PDB name set.
