#include <cstdio>
struct Flags { unsigned char bit0:1; unsigned char bit1:1; unsigned char bit2:1; unsigned char rest:5; };
struct Options { unsigned long a,b; Flags f; unsigned long word25; unsigned char name[16]; unsigned long words[12]; unsigned char tailbytes[8]; unsigned long tail; };
static void copy_options(Options& d, const Options& s) { d.a=s.a; d.b=s.b; d.f.bit0=s.f.bit0; d.f.bit1=s.f.bit1; d.f.bit2=s.f.bit2; d.word25=s.word25; for(int i=0;i<16;i++) d.name[i]=s.name[i]; for(int i=0;i<12;i++) d.words[i]=s.words[i]; for(int i=0;i<8;i++) d.tailbytes[i]=s.tailbytes[i]; d.tail=s.tail; }
int main() { Options a={}, b={}; a.f.rest=0x1f; b.f.bit0=1; b.f.bit2=1; b.word25=0x12345678; b.name[13]=7; b.words[5]=42; copy_options(a,b); if(!a.f.bit0||a.f.bit1||!a.f.bit2||a.f.rest!=0x1f||a.word25!=0x12345678||a.name[13]!=7||a.words[5]!=42) { std::printf("00434450_TEST FAIL\n"); return 1; } std::printf("00434450_TEST PASS\n"); return 0; }
