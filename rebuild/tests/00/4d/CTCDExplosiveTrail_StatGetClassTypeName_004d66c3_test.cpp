// Behaviour test for 0x004D66C3 (CTCDExplosiveTrail class-type name, stand-in self StatGetClassTypeName_9): links the row object
// (declared here, not defined) and asserts what it hands to the CCharString constructor: the retail text "CTCDExplosiveTrail",
// length -1, one call on the result buffer, which it returns.
#include <cstdio>
#include <string.h>
class CCharString { public: char* p; };
static int g_called=0; static const char* g_arg=0; static int g_n=0; static void* g_self=0;
struct Helper {
    void Do(const char* s, int n);   // the row's callee (0x0099EBF0), defined here
};
void Helper::Do(const char* s, int n){ g_called++; g_arg=s; g_n=n; g_self=this; }
extern "C" CCharString* __fastcall StatGetClassTypeName_9(Helper* result);   // definition linked from the row object
int main(){
    Helper obj;
    CCharString* r = StatGetClassTypeName_9(&obj);
    if((void*)r != (void*)&obj){ printf("FAIL ret\n"); return 1; }
    if(g_called != 1 || g_self != (void*)&obj){ printf("FAIL call\n"); return 1; }
    if(g_arg == 0 || strcmp(g_arg, "CTCDExplosiveTrail") != 0){ printf("FAIL arg %s\n", g_arg ? g_arg : "(null)"); return 1; }
    if(g_n != -1){ printf("FAIL n\n"); return 1; }
    printf("OK_004d66c3\n");
    return 0;
}
