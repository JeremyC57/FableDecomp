#include "rebuild_abi.h"
#include <cstdio>
extern "C" int FABLE_STDCALL C3DMeshLODInfo_CanAbortResourceLoad(int a){ return a>1; }
int main(){ if(C3DMeshLODInfo_CanAbortResourceLoad(2)!=1){std::printf("B1");return 1;}
 if(C3DMeshLODInfo_CanAbortResourceLoad(1)!=0){std::printf("B2");return 1;}
 std::printf("RB_a23c20_OK\n"); return 0; }