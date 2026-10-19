#include <cstdio>
struct Proc { int calls; Proc():calls(0){} void Shutdown(){++calls;} };
struct Player { Proc *p104,*p188,*p16c,*p170,*p174,*p150,*p158,*p15c,*p160,*p164,*p184,*p168,*p17c,*p1a0;
 void run(){ p104->Shutdown(); p188->Shutdown(); p16c->Shutdown(); p170->Shutdown(); p174->Shutdown(); p150->Shutdown(); p158->Shutdown(); p15c->Shutdown(); p160->Shutdown(); p164->Shutdown(); p184->Shutdown(); p168->Shutdown(); p17c->Shutdown(); p1a0->Shutdown(); }
};
int main(){ Proc p[14]; Player x={p,p+1,p+2,p+3,p+4,p+5,p+6,p+7,p+8,p+9,p+10,p+11,p+12,p+13}; x.run(); for(int i=0;i<14;++i) if(p[i].calls!=1){std::printf("006339b0_TEST FAIL\n");return 1;} std::printf("006339b0_TEST PASS\n"); return 0; }
