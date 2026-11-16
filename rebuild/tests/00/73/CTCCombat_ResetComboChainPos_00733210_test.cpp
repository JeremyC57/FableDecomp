#include "rebuild_abi.h"
#include <cstdio>
class CTCCombatFields { public: char pad00[0xc]; long f0c; long f10; void ResetComboChainPos(); };
void CTCCombatFields::ResetComboChainPos(){ f0c=(long)0xfffffcf7; f10=(long)0xfffffc19; }
int main(){
    CTCCombatFields w; w.f0c=0; w.f10=0; w.ResetComboChainPos();
    if(w.f0c!=(long)0xfffffcf7||w.f10!=(long)0xfffffc19){std::printf("BAD\n");return 1;}
    std::printf("WADCLOSE733_OK\n"); return 0;
}