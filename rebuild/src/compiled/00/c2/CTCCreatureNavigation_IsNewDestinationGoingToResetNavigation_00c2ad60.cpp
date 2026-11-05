// Plan phase 4 (2026-11-05), C3DVector wave: the local C3DVector stand-in replaced by the shared engine/C3DVector.h (PDB C3DVector TPI 0x2783: public X/Y/Z at 0/4/8, size 12).

#include "engine/C3DVector.h"
class CTCCreatureNavigation;

struct INavHelper {
    virtual bool m00();
    virtual bool m04();
    virtual bool m08();
    virtual bool m0c();
    virtual bool m10();
    virtual bool m14();
    virtual bool slot6(CTCCreatureNavigation* nav, C3DVector* dest);
};

class CTCCreatureNavigation { public:
    void* pad0;
    INavHelper* helper; // +4
    bool IsNewDestinationGoingToResetNavigation(C3DVector* dest);
};

bool CTCCreatureNavigation::IsNewDestinationGoingToResetNavigation(C3DVector* dest)
{
    return this->helper->slot6(this, dest);
}