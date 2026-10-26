#include <stdio.h>

class CTCMapwho { public: int tag; };

static int g_member = 0;
static int g_helper = 0;
static CTCMapwho* g_last = 0;

extern "C" void __fastcall Mapwho_Member(CTCMapwho* self){ g_member++; g_last = self; }
void __cdecl operator delete(void* self)
{
    CTCMapwho* self_c = (CTCMapwho*)self; (void)self_c; g_helper++; g_last = self_c; return; }

void __fastcall OnDie(CTCMapwho* self);

int main(){
    OnDie(0);
    if (g_member != 0 || g_helper != 0) { printf("FAIL null\n"); return 1; }
    CTCMapwho obj; obj.tag = 7;
    OnDie(&obj);
    if (g_member != 1 || g_helper != 1) { printf("FAIL count\n"); return 1; }
    if (g_last != &obj) { printf("FAIL arg\n"); return 1; }
    printf("OK_007a4120\n");
    return 0;
}