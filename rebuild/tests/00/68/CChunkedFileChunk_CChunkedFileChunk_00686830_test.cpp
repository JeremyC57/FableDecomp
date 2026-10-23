#include <cstdio>
struct C {};
static int g_vtbl_0125be0c; static int g_called=0; static void* g_set=0;
static void __fastcall base_dtor_0099a300(C* s){ g_called=1; g_set=*(void**)s; }
static void __fastcall Dtor(C* self){ *(void**)self = &g_vtbl_0125be0c; base_dtor_0099a300(self); }
int main(){ void* buf[2]; Dtor((C*)buf); if(!g_called||g_set!=&g_vtbl_0125be0c){ std::printf("00686830_TEST FAIL\n"); return 1;} std::printf("00686830_TEST PASS\n"); return 0;}