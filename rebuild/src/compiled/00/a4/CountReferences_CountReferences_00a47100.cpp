// Leaf: ?GetSeed@CNavigatorBase@@IBEPAKXZ
// Flags:
#define FABLE_RETAIL_C2DVECTOR
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_STLP_LIST
#define FABLE_RETAIL_CNAVIGATORBASE
#define FABLE_RETAIL_CNAVIGATORBASE_ORIGINAL_ACCESS
#define FABLE_RETAIL_CNAVIGATORMANAGER
#define FABLE_RETAIL_NAVIGATOR_SEED_PASS30
#include "engine/C2DVector.h"
#include "engine/C3DVector.h"
#include "stlport/fable_containers.h"
#include "engine/CNavigatorManager.h"
#include "engine/CNavigatorBase.h"

unsigned long* CNavigatorBase::GetSeed() const
{
    return NavigationManager.GetSeed();
}
