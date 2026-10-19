#include <cstdio>
struct Vec { unsigned long first,last,end; };
struct Obj { unsigned char pad0[0xb8]; long sounds[7]; unsigned char gap[0x5c]; Vec filters[7]; };
int main(){ Obj o; for(int i=0;i<7;i++){o.sounds[i]=-1;o.filters[i].first=o.filters[i].last=o.filters[i].end=0;} for(int i=0;i<7;i++) if(o.sounds[i]!=-1||o.filters[i].first||o.filters[i].last||o.filters[i].end){std::printf("00457537_TEST FAIL\n");return 1;} std::printf("00457537_TEST PASS\n"); return 0; }