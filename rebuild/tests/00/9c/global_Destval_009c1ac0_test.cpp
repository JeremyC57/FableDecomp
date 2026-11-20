#include <cstdio>
// Behaviour test for 0x009c1ac0 (links the row object): the implicit CQuickDrawTriInfo destructor destroys UploadedTexture (+0x18,
//   CTexture::Uninitialise) first, then String (+0x14, ~CWideString).
// NF142 (2026-11-20, q4296): rewritten on engine/CQuickDrawTriInfo.h, the header the row now uses (the old test was a standalone copy over
//   the FableMember / FableBase stand-ins); the two out-of-line callees are the test's fixtures. PassPattern unchanged.
#include "engine/CQuickDrawTriInfo.h"
static int g_ord[2]; static int g_n=0; static const void* g_this[2];
void CTexture::Uninitialise() { g_this[g_n]=this; g_ord[g_n++]=1; }
CWideString::~CWideString() { g_this[g_n]=this; g_ord[g_n++]=2; }
extern "C" void FABLE_FASTCALL FableDualDestval_009c1ac0(CQuickDrawTriInfo *self);
int main(){
    static double buf[(sizeof(CQuickDrawTriInfo)+7)/8];
    char* raw=(char*)buf;
    FableDualDestval_009c1ac0((CQuickDrawTriInfo*)raw);
    if(g_n==2 && g_ord[0]==1 && g_ord[1]==2 && g_this[0]==raw+0x18 && g_this[1]==raw+0x14){std::printf("DUALDV9C1A_OK\n");return 0;}
    std::printf("FAIL\n"); return 1;
}
