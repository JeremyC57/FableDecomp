#include <stdio.h>

class CTCMapwho { public: int dummy; };

static int g_pre=0, g_free=0;
static CTCMapwho* g_freed=0;

extern "C" void __fastcall Mapwho_PreDie(CTCMapwho* self){ g_pre++; }
// spy on operator delete (MSVCR71 thunk 0x00BFE9BC) the source calls; it never releases (test objects live on the stack)
void __cdecl operator delete(void* p_) { CTCMapwho* p = (CTCMapwho*)p_; g_free++; g_freed=p; }

void __fastcall OnDie(CTCMapwho* self);

int main(){
    CTCMapwho o; o.dummy=7;
    OnDie(&o);
    if(g_pre!=1||g_free!=1||g_freed!=&o) return 1;
    OnDie(0);
    if(g_pre!=1||g_free!=1) return 1;
    puts("OK_00676720");
    return 0;
}