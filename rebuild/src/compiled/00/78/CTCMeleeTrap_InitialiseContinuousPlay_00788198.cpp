#include "rebuild_abi.h"
// CTCMeleeTrap::InitialiseContinuousPlay @ 0x00788198
struct CEngine3 { char pad[0x60]; char m_flag; void InitialiseContinuousPlay(); };
extern "C" void FABLE_STDCALL FableCam_788198(void *p, int one);
void CEngine3::InitialiseContinuousPlay(){
    FableCam_788198((void*)0x13bae00, 1);
    m_flag = 1;
}