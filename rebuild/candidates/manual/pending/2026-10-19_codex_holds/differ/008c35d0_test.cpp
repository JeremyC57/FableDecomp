#include <cstdio>
struct CModeBriarRoseTeleportInvulnerableModel {
 unsigned long filter_crc; void* item; void* other; unsigned long a,b,c; unsigned char flag; unsigned long e;
 CModeBriarRoseTeleportInvulnerableModel(void* p, unsigned char f) : filter_crc(0), item(p), other(0), a(0),b(0),c(0),flag(f),e(0) {}
};
int main(){ int x=7; CModeBriarRoseTeleportInvulnerableModel m(&x,1); if(m.item!=&x||m.other!=0||m.a||m.b||m.c||m.flag!=1||m.e){std::printf("008c35d0_TEST FAIL\n");return 1;} std::printf("008c35d0_TEST PASS\n"); return 0; }