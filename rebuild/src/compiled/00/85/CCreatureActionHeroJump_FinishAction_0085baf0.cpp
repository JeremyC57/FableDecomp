#include "rebuild_abi.h"
class CBaseObject;
class CBaseIntelligentPointer { public:
    CBaseObject* GetPItem() const;   // 0x00A01B50 (Ego_r.pdb ?GetPItem@CBaseIntelligentPointer@@QBEPAVCBaseObject@@XZ)
};
struct FableDestVal_0085baf0_t
{ char pad[0x8];  void FABLE_FASTCALL outer(); };
extern "C" void FABLE_FASTCALL FableDestVal_0085baf0(FableDestVal_0085baf0_t* action)
{
    FableDestVal_0085baf0_t* inner_this = (FableDestVal_0085baf0_t*)((char*)action + 0x8);
    ((CBaseIntelligentPointer*)inner_this)->GetPItem();
    action->outer();
}