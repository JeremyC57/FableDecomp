
#include <cstdio>
class C3DVector { public: float x, y, z; };
class CTCCreatureNavigation;
static int g_calls=0; static void* g_h=0; static void* g_self=0; static void* g_v=0; static bool g_ret=false;
struct Helper {
    virtual bool slot7(CTCCreatureNavigation* nav, C3DVector* v){
        g_calls++; g_h=this; g_self=(void*)nav; g_v=(void*)v; return g_ret;
    }
};
class CTCCreatureNavigation { public: void* f0; Helper* ParentThing; };
bool model(CTCCreatureNavigation* self, C3DVector* v){ return self->ParentThing->slot7(self, v); }
int main(){
    Helper h; CTCCreatureNavigation nav; nav.f0=0; nav.ParentThing=&h; C3DVector v={1,2,3};
    g_ret=true;
    bool r=model(&nav,&v);
    if(!(r==true && g_calls==1 && g_h==(void*)&h && g_self==(void*)&nav && g_v==(void*)&v)){ printf("FAIL1\n"); return 1; }
    g_ret=false;
    bool r2=model(&nav,&v);
    if(!(r2==false && g_calls==2)){ printf("FAIL2\n"); return 1; }
    printf("OK_00c45140\n");
    return 0;
}