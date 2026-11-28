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

#include <stdio.h>
#include <string.h>
#include <assert.h>

// This method uses only the manager reference in the native base layout.
struct SeedProbe : CNavigatorBase {
    static unsigned long* Read(const SeedProbe& probe) { return probe.GetSeed(); }
};
int main() {
    CNavigatorManager manager;
    unsigned long storage[(sizeof(CNavigatorBase)+3)/4]={0};
    CNavigatorManager* target=&manager;
    memcpy(reinterpret_cast<char*>(storage)+0x1c,&target,sizeof target);
    unsigned long seeds[3]={0,19,42};
    for (int i=0;i<4;++i) {
        manager.Seed=i==3?0:seeds+i;
        unsigned long* result=SeedProbe::Read(*reinterpret_cast<const SeedProbe*>(storage));
        assert(result==manager.Seed);
        if (result) {unsigned long prior=seeds[i]; ++*result; assert(seeds[i]==prior+1);}
    }
    puts("00a47100_TEST PASS");
    return 0;
}
