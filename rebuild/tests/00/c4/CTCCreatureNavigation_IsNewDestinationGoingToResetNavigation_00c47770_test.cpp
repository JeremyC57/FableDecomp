#include <cstdio>
class C3DVector { public: float x,y,z; };
class CTCCreatureNavigation;
struct IHelper {
    virtual bool v6(CTCCreatureNavigation* nav, C3DVector* dest) { return dest->x > 0.0f; }
};
class CTCCreatureNavigation { public:
    void* f0;
    IHelper* ParentThing;
    bool IsNewDestinationGoingToResetNavigation(C3DVector* dest) {
        return this->ParentThing->v6(this, dest);
    }
};
int main(){
    IHelper h;
    CTCCreatureNavigation n; n.f0=0; n.ParentThing=&h;
    C3DVector pos; pos.x=1.0f; pos.y=0; pos.z=0;
    C3DVector neg; neg.x=-1.0f; neg.y=0; neg.z=0;
    bool a = n.IsNewDestinationGoingToResetNavigation(&pos);
    bool b = n.IsNewDestinationGoingToResetNavigation(&neg);
    if (a && !b) { printf("OK_00c47770\n"); return 0; }
    printf("FAIL\n"); return 1;
}