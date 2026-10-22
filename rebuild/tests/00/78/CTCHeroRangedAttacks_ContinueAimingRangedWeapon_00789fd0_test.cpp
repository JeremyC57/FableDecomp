#include <stdio.h>
struct S { char pad[0x14]; unsigned char FireCycleInProgress; unsigned char Aiming; };
static bool gRet;
static bool check(S* s){ return gRet; }
void model(S* s){ if(check(s)){ s->FireCycleInProgress=1; s->Aiming=1; } }
int main(){
    S a; a.FireCycleInProgress=0; a.Aiming=0;
    gRet=false; model(&a);
    if(a.FireCycleInProgress!=0||a.Aiming!=0){ printf("FAIL false branch\n"); return 1; }
    gRet=true; model(&a);
    if(a.FireCycleInProgress!=1||a.Aiming!=1){ printf("FAIL true branch\n"); return 1; }
    printf("OK_00789fd0\n");
    return 0;
}