// CModeStandBase::OnDeactivate -- release six counted animation entries and clear the inherited Active flag.
// Retail 0x00861D00,252 bytes. Actual generated header supplies all six pointer pairs.
// PDB CModeBase::Active is +0x18, in flattened inherited padding. CCPPointerInfo is refs0/callback4/data8.
// Callback ABI is native fastcall: ecx=data, no stack argument. Scalar block delete is 0x00BFE9BC.
#include "engine/CModeStandBase.h"
#ifndef _MSC_VER
#define __fastcall
#define __forceinline inline
#endif
class CCPPointerInfo { public: unsigned long RefCount; void (__fastcall* DeleteFunc)(void*); void* Data; };
FABLE_STATIC_ASSERT(sizeof(CCPPointerInfo)==12);
FABLE_STATIC_ASSERT(offsetof(CCPPointerInfo,DeleteFunc)==4);
FABLE_STATIC_ASSERT(offsetof(CCPPointerInfo,Data)==8);
void operator delete(void*) throw(); // 0x00BFE9BC
struct CModeStandBaseNative : CModeStandBase {
    __forceinline void Reset(CAnimEntryLite*& data,CCPPointerInfo*& info) {
        if(info) {
            --info->RefCount;
            if(info->RefCount==0) {
                info->DeleteFunc(info->Data);
                operator delete(info);
            }
        }
        info=0;
        data=0;
    }
    void OnDeactivate();
};
void CModeStandBaseNative::OnDeactivate(){
    Reset(PStandAnimation_Data,PStandAnimation_Info);
    Reset(PSlowWalkAnimation_Data,PSlowWalkAnimation_Info);
    Reset(PWalkAnimation_Data,PWalkAnimation_Info);
    Reset(PJogAnimation_Data,PJogAnimation_Info);
    Reset(PRunAnimation_Data,PRunAnimation_Info);
    Reset(PSprintAnimation_Data,PSprintAnimation_Info);
    _pad_0x18[0]=0;
}