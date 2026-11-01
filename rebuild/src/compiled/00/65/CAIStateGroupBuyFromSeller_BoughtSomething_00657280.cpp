#include "rebuild_abi.h"
class CMainGameComponent { public:
    long GetCurrentWorldFrame() const;             // 0x004167C8 (Ego_r.pdb ?GetCurrentWorldFrame@CMainGameComponent@@QBEJXZ)
};
extern CMainGameComponent* g_pMainGameComponent;   // 0x013B86A0
struct C { char pad[0x2c]; int f2c; void Bought(); };
void C::Bought(){ this->f2c = g_pMainGameComponent->GetCurrentWorldFrame(); }