#include <stdio.h>
class C3DVector { public: float x, y, z; };
class CTCCreatureNavigation;
struct Callee {
    int marker;
    virtual bool s0(){return false;}
    virtual bool s1(){return false;}
    virtual bool s2(){return false;}
    virtual bool s3(){return false;}
    virtual bool s4(){return false;}
    virtual bool s5(){return false;}
    virtual bool Query(void* nav, C3DVector* v){ return v->x > 0.0f; }
};
class CTCCreatureNavigation { public:
    void* f0;
    Callee* ParentThing;
    bool IsNewDestinationGoingToResetNavigation(C3DVector* dest){ return ParentThing->Query(this, dest); }
};
int main(){
    Callee c; c.marker=1;
    CTCCreatureNavigation nav; nav.f0=0; nav.ParentThing=&c;
    C3DVector vpos = {1.0f,0,0};
    C3DVector vneg = {-1.0f,0,0};
    bool r1 = nav.IsNewDestinationGoingToResetNavigation(&vpos);
    bool r2 = nav.IsNewDestinationGoingToResetNavigation(&vneg);
    if (r1 && !r2) { printf("OK_00c238c0\n"); return 0; }
    printf("FAIL\n"); return 1;
}