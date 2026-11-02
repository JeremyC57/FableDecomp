// test: CNavigatorBase::~CNavigatorBase -- unregister from the navigation manager, delete the steering agent and the interface, null both
#include <stdio.h>
#include <string.h>
static char trace[32];
struct Manager { int count; };
struct Agent { ~Agent() { strcat(trace, "a"); } };
struct Interface { virtual ~Interface() { strcat(trace, "i"); } };
struct NavBase {
    Manager* NavigationManager; Agent* SteeringAgent; Interface* PInterface;
    void Unregister(Manager& m) { --m.count; strcat(trace, "U"); }
    virtual ~NavBase() { Unregister(*NavigationManager); delete SteeringAgent; delete PInterface; SteeringAgent = 0; PInterface = 0; }
};
int main() {
    Manager m; m.count = 2;
    NavBase* n = new NavBase; n->NavigationManager = &m; n->SteeringAgent = new Agent; n->PInterface = new Interface;
    delete n;
    NavBase* e = new NavBase; e->NavigationManager = &m; e->SteeringAgent = 0; e->PInterface = 0;
    delete e;
    if (strcmp(trace, "UaiU") != 0 || m.count != 0) { printf("00a47180_TEST FAIL %s\n", trace); return 1; }
    printf("00a47180_TEST PASS\n");
    return 0;
}