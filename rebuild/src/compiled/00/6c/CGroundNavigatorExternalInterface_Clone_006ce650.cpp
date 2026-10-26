#include "engine/CGroundNavigatorExternalInterface.h"

class CNavigatorExternalInterface;

struct CGroundNavigatorExternalInterface_Methods : CGroundNavigatorExternalInterface {
    CNavigatorExternalInterface* Construct(CGroundNavigatorExternalInterface* source);
    CNavigatorExternalInterface* Clone();
};

CNavigatorExternalInterface* CGroundNavigatorExternalInterface_Methods::Clone()
{
    void* memory = ((void*)(operator new(0xc)));
    if (memory) {
        return ((CGroundNavigatorExternalInterface_Methods*)memory)->Construct(this);
    }
    return 0;
}
