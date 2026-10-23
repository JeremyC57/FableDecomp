#include <cstdio>
static int g=0; static char g_vtbl_012b5868[8]; static void __fastcall vb_helper_00c3f490(void* p){ (void)p; g=1; }
static void Dtor(void* self){ *reinterpret_cast<void**>(reinterpret_cast<char*>(self)+0x1f0) = (void*)g_vtbl_012b5868; vb_helper_00c3f490(self); }
int main(){ char blk[0x200]; Dtor(blk); if(*reinterpret_cast<void**>(blk+0x1f0)!=(void*)g_vtbl_012b5868||!g){ std::printf("00c3f630_TEST FAIL\n"); return 1;}
 std::printf("00c3f630_TEST PASS\n"); return 0;}