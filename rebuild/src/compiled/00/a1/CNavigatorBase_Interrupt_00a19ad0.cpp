// Leaf: ?Interrupt@CNavigatorBase@@UAEXXZ
#define FABLE_RETAIL_CNAVIGATORBASE   // alias route: engine/CNavigatorBase.h declares Interrupt under this class-model switch
#include "engine/CNavigatorBase.h"  // retyped onto the PDB layout; byte parity re-verified
void CNavigatorBase::Interrupt(){ this->Interrupted = 1; }
// Was defined on a stand-in alias class (owner rule 2026-11-05); now the real member, through the class-model switch above (alias route, 2026-11-23).
