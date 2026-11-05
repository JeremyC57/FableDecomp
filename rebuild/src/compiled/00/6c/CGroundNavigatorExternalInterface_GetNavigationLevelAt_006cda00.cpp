// Forwarder to a member of the Creature-object pointer at this+0x4. VC7.1 tail-calls
// Leaf: ?GetNavigationLevelAt@CGroundNavigatorExternalInterface_Methods@@QAEXXZ
// a void member->void member forward, so retail is `mov ecx,[ecx+d]; jmp rel32`.
#include "engine/CGroundNavigatorExternalInterface.h"  // retyped onto the PDB layout; byte parity re-verified
#pragma pack(push,1)
class CThingCreatureBase { public: void Run(); };
struct CGroundNavigatorExternalInterface_Methods : CGroundNavigatorExternalInterface {
    void GetNavigationLevelAt();
};
#pragma pack(pop)
void CGroundNavigatorExternalInterface_Methods::GetNavigationLevelAt() { this->Creature->Run(); }
// Stand-in (owner rule 2026-11-05): the real method on a stand-in class named after the real class (retail ?GetNavigationLevelAt@CGroundNavigatorExternalInterface@@UBEJABVC3DVector@@@Z); evidence: RTTI vtable slot + PDB name set.
