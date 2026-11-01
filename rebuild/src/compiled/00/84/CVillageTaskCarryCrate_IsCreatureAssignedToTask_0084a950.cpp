#include "rebuild_abi.h"
class CBaseObject;
class CBaseIntelligentPointer { public:
    CBaseObject* GetPItem() const;   // 0x00A01B50 (Ego_r.pdb ?GetPItem@CBaseIntelligentPointer@@QBEPAVCBaseObject@@XZ)
};
extern "C" bool FABLE_FASTCALL Is_94b330(void* self, void* arg){ void* r=((CBaseIntelligentPointer*)arg)->GetPItem(); return self==r; }